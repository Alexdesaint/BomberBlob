#pragma once

#include <list>
#include "Geometrie.hpp"

namespace Physics {
	class CollisionDetector;

	class CircleDynamic;

	class PhysicalObject{
	public:
		unsigned int objectType;

		explicit PhysicalObject(unsigned int objectType) : objectType(objectType) { };

		virtual void hit(const PhysicalObject& from) { }
	};

	class CircleStatic : PhysicalObject {
		friend class CollisionDetector;
		friend class CircleDynamic;
	private:
		CollisionDetector* collisionDetector;

	protected:
		Geometrie::Circle mainCircle;
		std::list<Geometrie::Circle> circles;

		virtual void enableCollision();

		virtual void disableCollision();

	public:
		explicit CircleStatic(CollisionDetector* c, unsigned int objectType) : PhysicalObject(objectType), collisionDetector(c) {
			enableCollision();
		}

		virtual ~CircleStatic() {
			disableCollision();
		}
	};

	class CircleDynamic : CircleStatic{
		friend class CollisionDetector;

	protected:
		Geometrie::Vec2f speed;
		Geometrie::Circle mainCircle;

		bool CheckCollision(Geometrie::Vec2f *ActualVector);
		bool HitCircle(CircleStatic *circleStatic);

		void enableCollision() override;
		void disableCollision() override;

	public:
		explicit CircleDynamic(CollisionDetector* c, unsigned int objectType) : CircleStatic(c, objectType){
			enableCollision();
		}
		~CircleDynamic() override;
	};

	class LineStatic  : PhysicalObject {
		friend class CollisionDetector;
	private:
		CollisionDetector* collisionDetector;

	protected:
		std::list<Geometrie::Line> lines;

		virtual void enableCollision();

		virtual void disableCollision();

	public:
		explicit LineStatic(CollisionDetector* c, unsigned int objectType) : PhysicalObject(objectType), collisionDetector(c) {
			enableCollision();
		}

		~LineStatic() {
			disableCollision();
		}
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

		static bool collision(CircleStatic circle1, CircleStatic circle2);

		bool checkCollision(CircleDynamic *Object, Geometrie::Vec2f *speed);

		bool computeCollision(Geometrie::Circle Object, Geometrie::Circle Target, Geometrie::Vec2f *Speed,
							  Geometrie::Vec2f *LongSpeed);

		bool computeCollision(Geometrie::Circle Object, Geometrie::Line Target, Geometrie::Vec2f *Speed,
							  Geometrie::Vec2f *LongSpeed);
	};
}