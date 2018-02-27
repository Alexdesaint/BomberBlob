#pragma once

#include <list>
#include "Geometrie.hpp"

namespace Physics {
	class CollisionDetector;

	class CircleDynamic;

	class PhysicalObject{
	protected:
		unsigned int objectType;
	public:
		explicit PhysicalObject(unsigned int objectType) : objectType(objectType) { };

		virtual void hit(const PhysicalObject& from) { }
	};

	class CircleStatic : PhysicalObject {
		friend class CollisionDetector;
		friend class CircleDynamic;
	private:
		CollisionDetector* collisionDetector;

	public:
		explicit CircleStatic(CollisionDetector* c, unsigned int objectType) : PhysicalObject(objectType), collisionDetector(c) { }

		virtual ~CircleStatic();
	protected:
		Geometrie::Circle mainCircle;
		std::list<Geometrie::Circle> circles;

		virtual void enableCollision();

		virtual void disableCollision();
	};

	class CircleDynamic : CircleStatic{
		friend class CollisionDetector;
	public:
		explicit CircleDynamic(CollisionDetector* c, unsigned int objectType) : CircleStatic(c, objectType){ }
		~CircleDynamic() override;
	protected:
		Geometrie::vec2f speed;
		Geometrie::Circle mainCircle;

		bool CheckCollision(Geometrie::vec2f *ActualVector);
		bool HitCircle(CircleStatic *circleStatic);

		void enableCollision() override;
		void disableCollision() override;
	};

	class LineStatic {
		friend class CollisionDetector;
	public:
		explicit LineStatic(CollisionDetector* c);
	protected:
		std::list<Geometrie::Line> lines;
	};

	class LineDynamic {
		friend class CollisionDetector;
	public:
		explicit LineDynamic(CollisionDetector* c);
	protected:
		std::list<Geometrie::Line> Lines;
		Geometrie::vec2f Speed;
	};

	class CollisionDetector {
		friend class CircleStatic;
		friend class CircleDynamic;
		friend class LineStatic;
		friend class LineDynamic;

	private:
		//for debug
		//std

		std::list<CircleStatic*>	CircleStaticElements;
		std::list<CircleDynamic*>	CircleDynamicElements;
		std::list<LineStatic*>		LineStaticElements;
		std::list<LineDynamic*>		LineDynamicElements;

		static bool collision(CircleStatic circle1, CircleStatic circle2);

		bool checkCollision(CircleDynamic *Object, Geometrie::vec2f *speed);

		bool computeCollision(Geometrie::Circle Object, Geometrie::Circle Target, Geometrie::vec2f *Speed,
							  Geometrie::vec2f *LongSpeed);

		bool computeCollision(Geometrie::Circle Object, Geometrie::Line Target, Geometrie::vec2f *Speed,
							  Geometrie::vec2f *LongSpeed);
	};
}