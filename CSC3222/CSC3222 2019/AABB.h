/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "CollisionVolume.h"
#include "RigidBody.h"

namespace NCL {
	namespace CSC3222 {
		class AABB : public CollisionVolume
		{
		public:
			AABB(RigidBody* rb, std::string type, SimObject* obj, float hW, float hH);
			float getHalfWidth();
			float getHalfHeight();
		private:
			float halfWidth;
			float halfHeight;
		};
	}
}


