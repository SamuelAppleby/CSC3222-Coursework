/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "AABB.h"
using namespace NCL;
using namespace CSC3222;

AABB::AABB(RigidBody* rb, std::string type, SimObject* obj, float hW, float hH) : CollisionVolume(rb, type, obj) {
	halfWidth = hW;
	halfHeight = hH;
}

float AABB::getHalfWidth() {
	return halfWidth;
}

float AABB::getHalfHeight() {
	return halfHeight;
}
