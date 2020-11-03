/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody()	{
	inverseMass = 1.0f;
	elasticity  = 0.5f;
}


RigidBody::~RigidBody()	{

}
