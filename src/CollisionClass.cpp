#include <list>
#include <iostream>
#include <cmath>

#include "Geometrie.hpp"
#include "CollisionClass.hpp"

using namespace Physics;

//CircleStatic
CircleStatic::~CircleStatic() {
	disableCollision();
}

void CircleStatic::enableCollision() {
	collisionDetector->CircleStaticElements.push_back(this);
}

void CircleStatic::disableCollision() {
	collisionDetector->CircleStaticElements.remove(this);
}

//CircleDynamic
CircleDynamic::~CircleDynamic() {
	disableCollision();
}

void CircleDynamic::enableCollision() {
	collisionDetector->CircleDynamicElements.push_back(this);
}

void CircleDynamic::disableCollision() {
	collisionDetector->CircleDynamicElements.remove(this);
}

bool CircleDynamic::CheckCollision(Geometrie::vec2f *ActualVector) {
	return collisionDetector->checkCollision(this, ActualVector);
}

bool CircleDynamic::HitCircle(CircleStatic *circleStatic) {

}

//LineStatic
LineStatic::LineStatic(CollisionDetector *c) {
	c->LineStaticElements.push_back(this);
}


LineDynamic::LineDynamic(CollisionDetector *c) {
	c->LineDynamicElements.push_back(this);
}

//CollisionDetector

bool CollisionDetector::collision(CircleStatic circle1, CircleStatic circle2) {
	return Geometrie::vec2f(circle1.mainCircle.Position, circle2.mainCircle.Position).Length() <= (circle1.mainCircle.Rayon + circle2.mainCircle.Rayon);
}

bool CollisionDetector::checkCollision(CircleDynamic *Object, Geometrie::vec2f *speed) {
	bool hit = false;
	for (CircleDynamic *target : CircleDynamicElements) {
		if (target != Object) {
			if(computeCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed))
				hit = true;
		}
	}

	for (CircleStatic *target : CircleStaticElements) {
		if(computeCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed))
			hit = true;
	}

	for (LineStatic *target : LineStaticElements) {
		for (Geometrie::Line line : target->lines) {
			if(computeCollision(Object->mainCircle, line, speed, &Object->speed))
				hit = true;
		}
	}

	return hit;
}

bool CollisionDetector::computeCollision(Geometrie::Circle Object, Geometrie::Circle Target, Geometrie::vec2f *Speed,
										 Geometrie::vec2f *LongSpeed) {
	float RayonAB = Object.Rayon + Target.Rayon;

	if (Speed->Length() > Geometrie::Distance(Object.Position, Target.Position) -
						  RayonAB) {//si la distance qui les sépare est plus courte que le vecteur vitesse
		Geometrie::vec2f vecAC = *Speed;
		Geometrie::vec2f vecAB(Object.Position, Target.Position);

		if (Geometrie::ProduitScalaire(vecAC, vecAB) > 0) {// si il ne sont pas de dirrection opposé

			float AE = Geometrie::ProduitScalaire(Geometrie::vec2f(vecAC).Normalize(),
												  vecAB);//distance avant le point le plus proche

			float BE2 = vecAB.Length2() - AE * AE;

			if (BE2 < RayonAB * RayonAB) {

				float EF2 = RayonAB * RayonAB - BE2;
				float AF = AE - std::sqrt(EF2);

				if (AF < vecAC.Length()) {
					//ils se touche forcément
					//maintenant on calcul le vecteur tangente

					//Calcul de F

					Geometrie::point2f A = Object.Position;

					Geometrie::vec2f vecAF = Geometrie::vec2f(vecAC).setLength(AF);

					Geometrie::point2f F = A + vecAF;

					Geometrie::vec2f vecFB(F, Target.Position);

					Geometrie::vec2f u = Geometrie::vec2f(-vecFB.y, vecFB.x).Normalize();

					Geometrie::vec2f vecFH = u * Geometrie::ProduitScalaire(u, Geometrie::vec2f(F, A + vecAC));

					Speed->x = vecAF.x + vecFH.x;
					Speed->y = vecAF.y + vecFH.y;

					Geometrie::vec2f vecGI = u * ProduitScalaire(u, *LongSpeed);

					LongSpeed->x = vecGI.x;
					LongSpeed->y = vecGI.y;

					//std::cout <<
					//	"A = (" << A.x << ", " << A.y << ") => " <<
					//	"C = (" << vecAC.x + A.x << ", " << vecAC.y + A.y << ") --> " <<
					//	"vecAH = (" << speed->x << ", " << speed->y << ")" << std::endl;

					return true;
				}
			}
		}
	} else
		return false;
}

bool CollisionDetector::computeCollision(Geometrie::Circle Object, Geometrie::Line Target, Geometrie::vec2f *Speed,
										 Geometrie::vec2f *LongSpeed) {
	using namespace Geometrie;

	point2f F = Geometrie::ClosestPointOnLine(Target, Object.Position);
	vec2f vecFA(F, Object.Position);

	if (Object.Rayon + Speed->Length() > vecFA.Length()) {
		if (ProduitScalaire(vecFA, *Speed) < 0) {

			point2f E, I;
			vec2f vecAD = *Speed;
			point2f D = Object.Position + vecAD;

			//std::cout <<
			//	"A = (" << Object.Position.x << ", " << Object.Position.y << ") => " <<
			//	"B = (" << Target.PointA.x << ", " << Target.PointA.y  << ") --> " <<
			//	"C = (" << Target.PointB.x << ", " << Target.PointB.y << ") --> " <<
			//	"D = (" << D.x << ", " << D.y << ")" << std::endl;

			if (Geometrie::Intersection(Target, Line(D, Object.Position +
														vec2f(Object.Position, F).setLength(Object.Rayon)), &I)) {

				Geometrie::Intersection(Target, Line(D, Object.Position), &E);
				//cercle de milieu de du segment à demi seg + rayon cercle

				point2f M = (Target.PointA + Target.PointB) / 2;

				if (vec2f(M, I).Length() <= (Target.Length() / 2)) {

					point2f G = E - (vecAD * Object.Rayon * vec2f(E, Object.Position).Length()) /
									(vecFA.Length() * vecAD.Length());

					vec2f vecAG(Object.Position, G);

					vec2f vecEA(E, Object.Position);

					if (vecAG.Length2() < vecAD.Length2() || vecEA.Length2() < Object.Rayon * Object.Rayon) {

						//Rebond : vecGK

						//point2f J = Object.Position - (F - Geometrie::ClosestPointOnLine(Target, G)) * 2;
						//vec2f vecGK(G, J);
						//vecGK.setLength(vec2f(G, D).Length());

						vec2f vecGI = Target.getVector().Normalize() *
									  ProduitScalaire(Target.getVector().Normalize(), vec2f(G, D));

						Speed->x = vecAG.x + vecGI.x;
						Speed->y = vecAG.y + vecGI.y;

						vec2f vecGJ = Target.getVector().Normalize() *
									  ProduitScalaire(Target.getVector().Normalize(), *LongSpeed);

						LongSpeed->x = vecGJ.x;
						LongSpeed->y = vecGJ.y;

						return true;
					}
				} else if (vec2f(M, I).Length() <= (Target.Length() / 2 + Object.Rayon)) {
					point2f B;

					if (vec2f(Target.PointA, E).Length2() < vec2f(Target.PointB, E).Length2()) {
						B = Target.PointA;
					} else {
						B = Target.PointB;
					}

					return computeCollision(Object, Circle(B, 0), Speed, LongSpeed);
				}
			}
		}
	}
	return false;
}
