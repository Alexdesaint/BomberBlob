#include <list>
#include <iostream>
#include <cmath>

#include "Geometrie.hpp"
#include "CollisionClass.hpp"

Physics::CircleStatic::CircleStatic(CollisionDetector * c)
{
	c->CircleStaticElements.push_back(this);
}

Physics::CircleDynamic::CircleDynamic(CollisionDetector * c)
{
	collisionDetector = c;
	c->CircleDynamicElements.push_back(this);
}

Physics::CircleDynamic::~CircleDynamic() {
	collisionDetector->CircleDynamicElements.remove(this);
}

void Physics::CircleDynamic::Checkcollision(Geometrie::vec2f *ActualVector){
	collisionDetector->CheckCollision(this, ActualVector);
}

Physics::LineStatic::LineStatic(CollisionDetector * c)
{
	c->LineStaticElements.push_back(this);
}

Physics::LineDynamic::LineDynamic(CollisionDetector * c)
{
	c->LineDynamicElements.push_back(this);
}

void Physics::CollisionDetector::CheckCollision(CircleDynamic *Object, Geometrie::vec2f *speed)
{
	for (CircleDynamic* target : CircleDynamicElements) {
		if (target != Object)
			ComputeCirclesToCirclesCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed);
	}

	for (CircleStatic* target : CircleStaticElements) {
		ComputeCirclesToCirclesCollision(Object->mainCircle, target->mainCircle, speed, &Object->speed);
	}

	for (LineStatic* target : LineStaticElements) {
		for (Geometrie::Line line : target->lines) {
			ComputeCirclesToLineCollision(Object->mainCircle, line, speed, &Object->speed);
		}
	}
}

bool Physics::CollisionDetector::ComputeCirclesToCirclesCollision(Geometrie::Circle Object, Geometrie::Circle Target,
																  Geometrie::vec2f *Speed, Geometrie::vec2f *LongSpeed)
{
	float RayonAB = Object.Rayon + Target.Rayon;

	if (Speed->Length() > Geometrie::Distance(Object.Position, Target.Position) - RayonAB) {//si la distance qui les sépare est plus courte que le vecteur vitesse
		Geometrie::vec2f vecAC = *Speed;
		Geometrie::vec2f vecAB(Object.Position, Target.Position);

		if (Geometrie::ProduitScalaire(vecAC, vecAB) > 0) {// si il ne sont pas de dirrection opposé

			float AE = Geometrie::ProduitScalaire(Geometrie::vec2f(vecAC).Normalize(), vecAB);//distance avant le point le plus proche

			float BE2 = vecAB.Length2() - AE * AE;

			if (BE2 < RayonAB * RayonAB) {

				float EF2 = RayonAB * RayonAB - BE2;
				float AF = AE - std::sqrt(EF2);

				if (AF < vecAC.Length()) {
					//ils se touche forc�ment
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
	}
	else
		return false;
}

bool Physics::CollisionDetector::ComputeCirclesToLineCollision(Geometrie::Circle Object, Geometrie::Line Target, Geometrie::vec2f * Speed, Geometrie::vec2f * LongSpeed)
{
	using namespace Geometrie;
	
	point2f F = Geometrie::ClosestPointOnLine(Target, Object.Position);
	vec2f vecFA(F, Object.Position);

	if (Object.Rayon + Speed->Length() > vecFA.Length()){
		if (ProduitScalaire(vecFA, *Speed) < 0) {

			point2f E, I;
			vec2f vecAD = *Speed;
			point2f D = Object.Position + vecAD;

			//std::cout << 
			//	"A = (" << Object.Position.x << ", " << Object.Position.y << ") => " <<
			//	"B = (" << Target.PointA.x << ", " << Target.PointA.y  << ") --> " <<
			//	"C = (" << Target.PointB.x << ", " << Target.PointB.y << ") --> " <<
			//	"D = (" << D.x << ", " << D.y << ")" << std::endl;

			if (Geometrie::Intersection(Target, Line(D, Object.Position + vec2f(Object.Position, F).setLength(Object.Rayon)), &I)) {

				Geometrie::Intersection(Target, Line(D, Object.Position), &E);
				//cercle de milieu de du segment à demi seg + rayon cercle

				point2f M = (Target.PointA + Target.PointB) / 2;

				if(vec2f(M, I).Length() <= (Target.Length() / 2)) {

					point2f G = E - (vecAD * Object.Rayon * vec2f(E, Object.Position).Length()) /
									(vecFA.Length() * vecAD.Length());

					vec2f vecAG(Object.Position, G);

					vec2f vecEA(E, Object.Position);

					if (vecAG.Length2() < vecAD.Length2() || vecEA.Length2() < Object.Rayon * Object.Rayon) {

						//Rebond : vecGK

						//point2f J = Object.Position - (F - Geometrie::ClosestPointOnLine(Target, G)) * 2;
						//vec2f vecGK(G, J);
						//vecGK.setLength(vec2f(G, D).Length());

						vec2f vecGI = Target.getVector().Normalize() * ProduitScalaire(Target.getVector().Normalize(), vec2f(G, D));

						Speed->x = vecAG.x + vecGI.x;
						Speed->y = vecAG.y + vecGI.y;

						vec2f vecGJ = Target.getVector().Normalize() *
									  ProduitScalaire(Target.getVector().Normalize(), *LongSpeed);

						LongSpeed->x = vecGJ.x;
						LongSpeed->y = vecGJ.y;

						return true;
					}
				}
				else if(vec2f(M, I).Length() <= (Target.Length() / 2 + Object.Rayon)) {
					point2f B;

					if(vec2f(Target.PointA,E).Length2() < vec2f(Target.PointB, E).Length2()){
						B = Target.PointA;
					} else{
						B = Target.PointB;
					}

					ComputeCirclesToCirclesCollision(Object, Circle(B,0), Speed, LongSpeed);
				}
			}
		}
	}

	return false;
}
