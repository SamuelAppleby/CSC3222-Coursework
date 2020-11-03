/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Sphere.h"
using namespace NCL;
using namespace CSC3222;

Sphere::Sphere(RigidBody* rb, std::string name, SimObject* obj, float r) : CollisionVolume(rb, name, obj) {
	radius = r;
}

float Sphere::getRadius() {
	return radius;
}
