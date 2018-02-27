#pragma once

#define PI 3.14159265

namespace  Geometrie
{
	struct point2i {
		int x, y;

		point2i() {
			x = 0;
			y = 0;
		}
		point2i(int ix, int iy);
	};

	struct vec2f;

	struct point2f {
		float x, y;

		point2f();
		point2f(float ix, float iy);

		point2f operator+(point2f a) {
			return {a.x + x, a.y + y};
		}

		point2f operator-(point2f a) {
			return {x - a.x, y - a.y};
		}

		point2f operator+(vec2f a); 

		point2f operator-(vec2f a);

		point2f operator*(point2f a) {
			return {a.x * x, a.y * y};
		}

		point2f operator*(float a) {
			return {a * x, a * y};
		}

		point2f operator/(float a) {
			return {x / a, y / a};
		}

		point2f operator*(int a) {
			return {a * x, a * y};
		}

		point2f operator/(int a) {
			return {x / a, y / a};
		}
	};

	struct vec2i {
		int x, y;

		vec2i() {
			x = 0;
			y = 0;
		}
		vec2i(int ix, int iy) {
			x = ix;
			y = iy;
		}
	};

	class vec2f {
	public:
		float x, y;

		vec2f();
		vec2f(const vec2f& v);
		vec2f(float ix, float iy);
		vec2f(point2f ix, point2f iy);

		vec2f operator+(vec2f a) {
			return vec2f(a.x + x, a.y + y);
		}

		vec2f operator-(vec2f a) {
			return vec2f(x - a.x, y - a.y);
		}

		vec2f operator*(vec2f a) {
			return vec2f(a.x * x, a.y * y);
		}

		vec2f operator*(float a) {
			return vec2f(a * x, a * y);
		}

		vec2f operator/(float a) {
			return vec2f(x / a, y / a);
		}

		float Length();
		float Length2();
		vec2f setLength(float newLength);
		vec2f Normalize();
	};

	class Circle {
	public:
		Circle();
		Circle(point2f p, float r);
		float Rayon;
		point2f Position;
	};

	struct Line	{
		Line(point2f a, point2f b);
		vec2f getVector();

		point2f PointA;
		point2f PointB;

		float Length();
		float Length2();
	};

	struct Recti {
		unsigned int OffsetX, OffsetY, X, Y;

		Recti(unsigned int iOffsetX, unsigned int iOffsetY, unsigned int iX, unsigned int iY);
	};

	struct Rectf {
		float OffsetX, OffsetY, X, Y;

		Rectf(float iOffsetX, float iOffsetY, float iX, float iY);
	};

	float ProduitScalaire(vec2f A, vec2f B);
	float Distance(point2f PointA, point2f PointB);
	float Distance2(point2f PointA, point2f PointB);
	bool Intersection(Line A, Line B, point2f *inter);
	point2f ClosestPointOnLine(Line line, point2f point);
};