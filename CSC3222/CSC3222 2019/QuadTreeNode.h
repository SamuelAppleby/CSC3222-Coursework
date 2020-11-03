/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include <vector>
#include "CollisionVolume.h"
#include "AABB.h"
namespace NCL {
	namespace CSC3222 {
		class QuadTreeNode
		{
		public:
			QuadTreeNode();
			void Insert(CollisionVolume* v);
			void Split();
			QuadTreeNode* topLeft;
			QuadTreeNode* topRight;
			QuadTreeNode* bottomLeft;
			QuadTreeNode* bottomRight;
			AABB* shape;
			Vector2 position;
			std::vector < CollisionVolume* > contents;
			int maxDepth;
		};
	}
}

