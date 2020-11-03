/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "AABB.h"
#include "CollisionPair.h"
using namespace NCL;
using namespace CSC3222;

CollisionPair::CollisionPair() {
	a = nullptr;
	b = nullptr;
	penetration = 0;
	normal = Vector2{ 0,0 };
}

CollisionPair::CollisionPair(CollisionVolume* first, CollisionVolume* second, float p, Vector2 n) {
	a = first;
	b = second;
	penetration = p;
	normal = n;
}
