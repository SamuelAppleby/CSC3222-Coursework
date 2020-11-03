/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "CollisionVolume.h"
#include "RigidBody.h"

namespace NCL {
	namespace CSC3222 {
		class Sphere : public CollisionVolume
		{
		public:
			Sphere(RigidBody* rb, std::string type, SimObject* obj, float radius);
			float getRadius();
		private:
			float radius;
		};
	}
}


