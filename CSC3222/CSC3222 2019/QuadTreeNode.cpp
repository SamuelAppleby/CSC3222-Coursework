/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "QuadTreeNode.h"
#include "AABB.h"
#include "CollisionPair.h"
#include "Sphere.h"
using namespace NCL;
using namespace CSC3222;


QuadTreeNode::QuadTreeNode()
{
	position = Vector2(240, 160);		// Position at centre of the world
	shape = new AABB(NULL, "Node", NULL, 250.0f, 160.0f);		// Half width and height of the game world
	maxDepth = 2;		// Creates at most 4 ^ 2 = 16 nodes, ~16* fewer comparisons
	topLeft = NULL;
	topRight = NULL;
	bottomLeft = NULL;
	bottomRight = NULL;
}

void QuadTreeNode::Insert(CollisionVolume* v)
{
	if (Sphere* x = dynamic_cast<Sphere*>(v)) {
		AABB* y = new AABB(x->body, x->type, x->obj, x->getRadius(), x->getRadius());		// All spheres become AABB with half width and height, r
		if (abs(y->getPosition().x - position.x) < (y->getHalfWidth() + shape->getHalfWidth())
			&& (abs(y->getPosition().y - position.y) < (y->getHalfHeight() + shape->getHalfHeight()))) {
			if (topLeft != NULL && topRight != NULL && bottomLeft != NULL && bottomRight != NULL) {
				/* If we have not reahed the max depth */
				topLeft->Insert(v);
				topRight->Insert(v);
				bottomLeft->Insert(v);
				bottomRight->Insert(v);
			}
			else {
				if (contents.size() >= 5 && maxDepth != 0) {		// If our contents is too large and not the maxDepth
					Split();
				}
				else {
					contents.push_back(v);		// Otherwise push the object into the vector
				}
			}
		}
	}
	else {
		/* Repeat for actual AABB */
		AABB* y = dynamic_cast<AABB*>(v);
		if (abs(y->getPosition().x - position.x) < (y->getHalfWidth() + shape->getHalfWidth())
			&& (abs(y->getPosition().y - position.y) < (y->getHalfHeight() + shape->getHalfHeight()))) {
			if (topLeft != NULL && topRight != NULL && bottomLeft != NULL && bottomRight != NULL) {
				topLeft->Insert(v);
				topRight->Insert(v);
				bottomLeft->Insert(v);
				bottomRight->Insert(v);
			}
			else {
				if (contents.size() >= 5 && maxDepth != 0) {
					Split();
				}
				else {
					contents.push_back(v);
				}
			}
		}
	}
}

void QuadTreeNode::Split()
{
	topLeft = new QuadTreeNode();
	topLeft->maxDepth = maxDepth - 1;		// Set the max depth of this new node
	topLeft->position = Vector2(position.x - (shape->getHalfWidth() / 2), position.y - (shape->getHalfHeight() / 2));
	topLeft->shape = new AABB(NULL, "Node", NULL, shape->getHalfWidth() / 2, shape->getHalfWidth() / 2);

	topRight = new QuadTreeNode();
	topRight->maxDepth = maxDepth - 1;
	topRight->position = Vector2(position.x + (shape->getHalfWidth() / 2), position.y - (shape->getHalfHeight() / 2));
	topRight->shape = new AABB(NULL, "Node", NULL, shape->getHalfWidth() / 2, shape->getHalfWidth() / 2);

	bottomLeft = new QuadTreeNode();
	bottomLeft->maxDepth = maxDepth - 1;
	bottomLeft->position = Vector2(position.x - (shape->getHalfWidth() / 2), position.y + (shape->getHalfHeight() / 2));
	bottomLeft->shape = new AABB(NULL, "Node", NULL, shape->getHalfWidth() / 2, shape->getHalfWidth() / 2);

	bottomRight = new QuadTreeNode();
	bottomRight->maxDepth = maxDepth - 1;
	bottomRight->position = Vector2(position.x + (shape->getHalfWidth() / 2), position.y + (shape->getHalfHeight() / 2));
	bottomRight->shape = new AABB(NULL, "Node", NULL, shape->getHalfWidth() / 2, shape->getHalfWidth() / 2);

	for (int i = 0; i < contents.size(); ++i) {
		/* Try to insert all current contents into these 4 new nodes */
		topLeft->Insert(contents.at(i));
		topRight->Insert(contents.at(i));
		bottomLeft->Insert(contents.at(i));
		bottomRight->Insert(contents.at(i));
	}
	contents.clear();		// After passing contents to children remove them all
}