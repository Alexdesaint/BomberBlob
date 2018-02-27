#include "Geometrie.hpp"

#include <cmath>

using namespace Geometrie;

Geometrie::vec2f::vec2f()
{
	y = 0;
	x = 0;
}

vec2f::vec2f(const vec2f &v) {
	x = v.x;
	y = v.y;
}

Geometrie::vec2f::vec2f(float ix, float iy)
{
	x = ix;
	y = iy;
}

Geometrie::vec2f::vec2f(point2f PointA, point2f PointB)
{
	x = PointB.x - PointA.x;
	y = PointB.y - PointA.y;
}

float Geometrie::vec2f::Length()
{
	return std::sqrt(x * x + y * y);
}

float Geometrie::vec2f::Length2()
{
	return x * x + y * y;
}

Geometrie::vec2f Geometrie::vec2f::setLength(float newLength)
{
	float oldLength = Length();
	float Rapport;
	if (oldLength != 0) {
		Rapport = newLength / oldLength;
		x *= Rapport;
		y *= Rapport;
	}

	return *this;
}

Geometrie::vec2f Geometrie::vec2f::Normalize()
{
	float length = Length();
	x = x / length;
	y = y / length;

	return *this;
}

Geometrie::point2i::point2i()
{
	y = 0;
	x = 0;
}

Geometrie::point2i::point2i(int ix, int iy)
{
	x = ix;
	y = iy;
}

Geometrie::point2f::point2f()
{
	y = 0;
	x = 0;
}

Geometrie::point2f::point2f(float ix, float iy)
{
	x = ix;
	y = iy;
}

point2f Geometrie::point2f::operator+(vec2f a)
{
	return point2f(a.x + x, a.y + y);
}

point2f Geometrie::point2f::operator-(vec2f a)
{
	return point2f(x - a.x, y - a.y);
}

Geometrie::Circle::Circle()
{
}

Geometrie::Circle::Circle(point2f p, float r)
{
	Position = p;
	Rayon = r;
}

Geometrie::Line::Line(point2f a, point2f b)
{
	PointA = a;
	PointB = b;
}

Geometrie::vec2f Geometrie::Line::getVector()
{
	return vec2f(PointB.x - PointA.x, PointB.y - PointA.y);
}

float Geometrie::Line::Length()
{
	return std::sqrt((PointB.x - PointA.x) * (PointB.x - PointA.x) + (PointB.y - PointA.y) * (PointB.y - PointA.y));
}

float Geometrie::Line::Length2()
{
	return (PointB.x - PointA.x) * (PointB.x - PointA.x) + (PointB.y - PointA.y) * (PointB.y - PointA.y);
}

float Geometrie::ProduitScalaire(vec2f A, vec2f B)
{
	return A.x * B.x + A.y * B.y;
}

float Geometrie::Distance(point2f PointA, point2f PointB)
{
	return std::sqrt((PointB.x - PointA.x) * (PointB.x - PointA.x) + (PointB.y - PointA.y) * (PointB.y - PointA.y));
}

float Geometrie::Distance2(point2f PointA, point2f PointB)
{
	return (PointB.x - PointA.x) * (PointB.x - PointA.x) + (PointB.y - PointA.y) * (PointB.y - PointA.y);
}

bool Geometrie::Intersection(Line A, Line B, point2f *inter)
{
	//1 : A, A --> A.PointA
	//2 : A, B --> A.PointB
	//3 : B, A --> B.PointA
	//4 : B, B --> B.PointB
	
	float A1 = A.PointA.x - A.PointB.x;

	float B1 = B.PointA.x - B.PointB.x;

	float A2 = A.PointA.y - A.PointB.y;

	float B2 = B.PointA.y - B.PointB.y;

	float det = A1*B2 - A2*B1;

	if (det == 0)
		return false;

	float C1 = A.PointB.y * A.PointA.x - A.PointB.x * A.PointA.y;

	float C2 = B.PointB.y * B.PointA.x - B.PointB.x * B.PointA.y;

	inter->x = (C1*B1 - A1*C2) / det;
	inter->y = (C1*B2 - A2*C2) / det;

	return true;
}

Geometrie::point2f Geometrie::ClosestPointOnLine(Line line, point2f point) {

	float A1 = line.PointB.y - line.PointA.y;
	float B1 = line.PointA.x - line.PointB.x;;
	float C1 = A1*line.PointA.x + B1*line.PointA.y;
	float C2 = -B1*point.x + A1*point.y;

	float det = A1*A1 - -B1*B1;

	if (det != 0) {
		return Geometrie::point2f(((A1*C1 - B1*C2) / det), ((A1*C2 - -B1*C1) / det));
	}
	else {
		return point;
	}
}

Geometrie::Recti::Recti(unsigned int iOffsetX, unsigned int iOffsetY, unsigned int iX, unsigned int iY)
{
	OffsetX = iOffsetX;
	OffsetY = iOffsetY;
	X = iX;
	Y = iY;
}

Geometrie::Rectf::Rectf(float iOffsetX, float iOffsetY, float iX, float iY)
{
	OffsetX = iOffsetX;
	OffsetY = iOffsetY;
	X = iX;
	Y = iY;
}
