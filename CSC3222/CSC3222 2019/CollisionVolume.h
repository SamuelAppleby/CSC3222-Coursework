/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "../../Common/MeshGeometry.h"
#include "RigidBody.h"
#include <string>
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class CollisionVolume {
		public:
			CollisionVolume(RigidBody* rb, std::string type, SimObject* o);
			CollisionVolume();
			~CollisionVolume();
			virtual  Vector2 getPosition();
			RigidBody* body;
			std::string type;
			SimObject* obj;	
			bool staticObj;
			bool sleeping;
		};
	}
}

