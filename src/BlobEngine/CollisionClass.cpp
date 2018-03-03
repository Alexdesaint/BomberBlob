#include <list>
#include <iostream>
#include <cmath>

#include "Geometrie.hpp"
#include "CollisionClass.hpp"

using namespace Physics;
using namespace Geometrie;

//CircleStatic

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

bool CircleDynamic::CheckCollision(Vec2f *ActualVector) {
	return collisionDetector->checkCollision(this, ActualVector);
}

bool CircleDynamic::HitCircle(CircleStatic *circleStatic) {

}

//LineStatic
void LineStatic::enableCollision() {
	collisionDetector->LineStaticElements.push_back(this);
}

void LineStatic::disableCollision() {
	collisionDetector->LineStaticElements.remove(this);
}

//CollisionDetector

bool CollisionDetector::collision(CircleStatic circle1, CircleStatic circle2) {
	return Line(circle1.mainCircle.position, circle2.mainCircle.position).Length() <= (circle1.mainCircle.rayon + circle2.mainCircle.rayon);
}

bool CollisionDetector::checkCollision(CircleDynamic *Object, Vec2f *speed) {
	bool hit = false;
	for (CircleDynamic *target : CircleDynamicElements) {
		if (target != Object) {
			if(computeCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed)) {
				hit = true;
				target->hit(*Object);
			}

		}
	}

	for (CircleStatic *target : CircleStaticElements) {
		if(computeCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed)){
			hit = true;
			target->hit(*Object);
		}
	}

	for (LineStatic *target : LineStaticElements) {
		for (Line line : target->lines) {
			if(computeCollision(Object->mainCircle, line, speed, &Object->speed)){
				hit = true;
				target->hit(*Object);
			}
		}
	}

	return hit;
}

bool CollisionDetector::computeCollision(Circle Object, Circle Target, Vec2f *Speed, Vec2f *LongSpeed) {
	float RayonAB = Object.rayon + Target.rayon;

	if (Speed->Length() > Distance(Object.position, Target.position) - RayonAB) {//si la distance qui les sépare est plus courte que le vecteur vitesse
		Vec2f vecAC = *Speed;
		Vec2f vecAB(Object.position, Target.position);

		if (ProduitScalaire(vecAC, vecAB) > 0) {// si il ne sont pas de dirrection opposé

			float AE = ProduitScalaire(Vec2f(vecAC).Normalize(),
												  vecAB);//distance avant le point le plus proche

			float BE2 = vecAB.Length2() - AE * AE;

			if (BE2 < RayonAB * RayonAB) {

				float EF2 = RayonAB * RayonAB - BE2;
				float AF = AE - std::sqrt(EF2);

				if (AF < vecAC.Length()) {
					//ils se touche forcément
					//maintenant on calcul le vecteur tangente

					//Calcul de F

					Point2f A = Object.position;

					Vec2f vecAF = Vec2f(vecAC).setLength(AF);

					Point2f F = A + vecAF;

					Vec2f vecFB(F, Target.position);

					Vec2f u = Vec2f(-vecFB.y, vecFB.x).Normalize();

					Vec2f vecFH = u * ProduitScalaire(u, Vec2f(F, A + vecAC));

					Speed->x = vecAF.x + vecFH.x;
					Speed->y = vecAF.y + vecFH.y;

					Vec2f vecGI = u * ProduitScalaire(u, *LongSpeed);

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

bool CollisionDetector::computeCollision(Circle Object, Line Target, Vec2f *Speed, Vec2f *LongSpeed) {

	Point2f F = ClosestPointOnLine(Target, Object.position);
	Vec2f vecFA(F, Object.position);

	if (Object.rayon + Speed->Length() > vecFA.Length()) {
		if (ProduitScalaire(vecFA, *Speed) < 0) {

			Point2f E, I;
			Vec2f vecAD = *Speed;
			Point2f D = Object.position + vecAD;

			//std::cout <<
			//	"A = (" << Object.Position.x << ", " << Object.Position.y << ") => " <<
			//	"B = (" << Target.PointA.x << ", " << Target.PointA.y  << ") --> " <<
			//	"C = (" << Target.PointB.x << ", " << Target.PointB.y << ") --> " <<
			//	"D = (" << D.x << ", " << D.y << ")" << std::endl;

			if (Intersection(Target, Line(D, Object.position +
														Vec2f(Object.position, F).setLength(Object.rayon)), &I)) {

				Intersection(Target, Line(D, Object.position), &E);
				//cercle de milieu de du segment à demi seg + rayon cercle

				Point2f M = (Target.pointA + Target.pointB) / 2;

				if (Vec2f(M, I).Length() <= (Target.Length() / 2)) {

					Point2f G = E - (vecAD * Object.rayon * Vec2f(E, Object.position).Length()) /
									(vecFA.Length() * vecAD.Length());

					Vec2f vecAG(Object.position, G);

					Vec2f vecEA(E, Object.position);

					if (vecAG.Length2() < vecAD.Length2() || vecEA.Length2() < Object.rayon * Object.rayon) {

						//Rebond : vecGK

						//point2f J = Object.Position - (F - ClosestPointOnLine(Target, G)) * 2;
						//Vec2f vecGK(G, J);
						//vecGK.setLength(Vec2f(G, D).Length());

						Vec2f vecGI = Target.getVector().Normalize() *
									  ProduitScalaire(Target.getVector().Normalize(), Vec2f(G, D));

						Speed->x = vecAG.x + vecGI.x;
						Speed->y = vecAG.y + vecGI.y;

						Vec2f vecGJ = Target.getVector().Normalize() *
									  ProduitScalaire(Target.getVector().Normalize(), *LongSpeed);

						LongSpeed->x = vecGJ.x;
						LongSpeed->y = vecGJ.y;


						return true;
					}
				} else if (Vec2f(M, I).Length() <= (Target.Length() / 2 + Object.rayon)) {
					Point2f B;

					if (Vec2f(Target.pointA, E).Length2() < Vec2f(Target.pointB, E).Length2()) {
						B = Target.pointA;
					} else {
						B = Target.pointB;
					}

					return computeCollision(Object, Circle(B, 0), Speed, LongSpeed);
				}
			}
		}
	}
	return false;
}
