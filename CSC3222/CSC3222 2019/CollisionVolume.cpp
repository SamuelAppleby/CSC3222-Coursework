/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume(RigidBody* rb, std::string s, SimObject* o) {
	this->body = rb;
	this->type = s;
	this->obj = o;
	/* By default, no volumes are static or asleep */
	staticObj = false;
	sleeping = false;
}

CollisionVolume::CollisionVolume() : CollisionVolume{nullptr, "", nullptr} {
}

CollisionVolume::~CollisionVolume() {
}

Vector2 CollisionVolume::getPosition() {
	return (body) ? body->GetPosition() : Vector2{};
}


