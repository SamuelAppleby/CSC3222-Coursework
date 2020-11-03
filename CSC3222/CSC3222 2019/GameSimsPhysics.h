/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "Sphere.h"
#include "AABB.h"
#include "BadFoodGame.h"
#include "CollisionPair.h"
#include "QuadTreeNode.h"
namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics(BadFoodGame* game);
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

			std::vector<CollisionVolume*> getColliders();
			float bounceTimer = 0.0f;		// Used for a short delay after a laser bounces (prevents multiple collisons at once)
			float pickUpTimer = 0.0f;		// Used to prevent multiple incremenets per collision
			BadFoodGame* bfg;


		protected:
			void IntegrateAcceleration(float dt);
			void CollisionDetection(float dt);
			void CollisionAcceleration(QuadTreeNode* n, float dt);
			bool SphereSphere(Sphere* s, Sphere* s1);
			bool AABBAABB(AABB* a, AABB* a1);
			bool sphereAABB(CollisionVolume* a, CollisionVolume* b);
			Vector2 closestPoint(Vector2 pos, AABB* b);
			void CollisionResolution(CollisionPair* pair);
			void IntegrateVelocity(float dt);

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
		};
	}
}

