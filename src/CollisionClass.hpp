#pragma once

#include <list>
#include "Geometrie.hpp"

namespace Physics {
	class CollisionDetector;

	class CircleStatic {
		friend class CollisionDetector;
	public:
		explicit CircleStatic(CollisionDetector* c);
	protected:
		Geometrie::Circle mainCircle;
		std::list<Geometrie::Circle> circles;
	};

	class CircleDynamic {
		friend class CollisionDetector;
	public:
		explicit CircleDynamic(CollisionDetector* c);
		~CircleDynamic();
	protected:
		Geometrie::Circle mainCircle;
		std::list<Geometrie::Circle> circles;
		Geometrie::vec2f speed;

		void Checkcollision(Geometrie::vec2f *ActualVector);
	private:
		CollisionDetector* collisionDetector;
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

		void CheckCollision(CircleDynamic *Object, Geometrie::vec2f *speed);

		bool ComputeCirclesToCirclesCollision(Geometrie::Circle Object, Geometrie::Circle Target,
											  Geometrie::vec2f *Speed, Geometrie::vec2f *LongSpeed);

		bool ComputeCirclesToLineCollision(Geometrie::Circle Object, Geometrie::Line Target, Geometrie::vec2f *Speed, Geometrie::vec2f * LongSpeed);
	};
}