/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "CollisionVolume.h"
#include "RigidBody.h"

namespace NCL {
	namespace CSC3222 {
		class CollisionPair
		{
		public:
			CollisionPair();
			CollisionPair(CollisionVolume* a, CollisionVolume* b, float penetration, Vector2 normal);
			float getPenetration() {
				return penetration;
			}
			Vector2 getNormal() {
				return normal;
			}
			CollisionVolume* getFirst() {
				return a;
			}
			void setFirst(CollisionVolume* x) {
				a = x;
			}
			CollisionVolume* getSecond() {
				return b;
			}
			void setSecond(CollisionVolume* y) {
				b = y;
			}
			void setPenetration(float p) {
				penetration = p;
			}
			void setNormal(Vector2 v) {
				normal = v;
			}
		private:
			CollisionVolume* a;
			CollisionVolume* b;
			float penetration = 0;
			Vector2 normal;
		};
	}
}


