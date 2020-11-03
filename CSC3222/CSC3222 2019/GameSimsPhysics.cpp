/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"
#include "IntervalArithmetics.h"
#include <list>
#include "Sphere.h"
#include "AABB.h"
#include "BadFoodGame.h"
#include "CollisionPair.h"
#include "Laser.h"
#include <algorithm>
#include "QuadTreeNode.h"
using namespace NCL;
using namespace CSC3222;

GameSimsPhysics::GameSimsPhysics(BadFoodGame* game) {
	bfg = game;
}

GameSimsPhysics::~GameSimsPhysics() {

}

void GameSimsPhysics::Update(float dt) {
	bounceTimer += dt;		// Prevent multiple bounces per collision
	pickUpTimer += dt;		// Prevent multiple increments per collision
	IntegrateAcceleration(dt);
	CollisionDetection(dt);
	IntegrateVelocity(dt);
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);
	if (at != allBodies.end()) {
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);
	if (at != allColliders.end()) {
		at = allColliders.erase(at);
	}
}

std::vector<CollisionVolume*> GameSimsPhysics::getColliders() {
	return allColliders;
}

void GameSimsPhysics::IntegrateAcceleration(float dt) {
	for (const auto& it : allBodies) {
		Vector2 acceleration = it->force * it->inverseMass;
		it->velocity += acceleration * dt;
	}
}

void GameSimsPhysics::CollisionDetection(float dt) {
	QuadTreeNode* root = new QuadTreeNode{};		// Create root tree node
	for (int i = 0; i < allColliders.size(); ++i) {
		root->Insert(allColliders.at(i));		// Insert all colliders into this AABB
		if (allColliders.at(i)->body->velocity == Vector2(0, 0)) {
			allColliders.at(i)->sleeping = true;		// If we find 2 colliders that aren't moving, set them to sleeping 
		}
	}
	CollisionAcceleration(root, dt);
}

void GameSimsPhysics::CollisionAcceleration(QuadTreeNode* n, float dt) {
	if (n->topLeft != NULL && n->topRight != NULL && n->bottomLeft != NULL && n->bottomRight != NULL) {		
		/* If we have not reached the max depth, recursively call on each quad */
		CollisionAcceleration(n->topLeft, dt);
		CollisionAcceleration(n->topRight, dt);
		CollisionAcceleration(n->bottomLeft, dt);
		CollisionAcceleration(n->bottomRight, dt);
	}
	else {
		for (int i = 0; i < n->contents.size(); ++i) {
			for (int j = i + 1; j < n->contents.size(); ++j) {
				if (!(n->contents.at(i)->staticObj && n->contents.at(j)->staticObj) ||
					!(n->contents.at(i)->sleeping && n->contents.at(j)->sleeping)) {		// Only look at objects which aren't both static or both sleeping
					if (Sphere* x = dynamic_cast<Sphere*>(n->contents.at(i))) {
						if (Sphere* y = dynamic_cast<Sphere*>(n->contents.at(j))) {
							if (!(x->type == "Balloon" || y->type == "Balloon")) {		// Don't test balloon collision
								SphereSphere(x, y);
							}
						}
						else if (AABB* y = dynamic_cast<AABB*>(n->contents.at(j))) {
							if (x->type != "Ballon"){
								sphereAABB(x, y);
							}
						}
					}
					else {
						if (AABB* x = dynamic_cast<AABB*>(n->contents.at(i))) {
							if (AABB* y = dynamic_cast<AABB*>(n->contents.at(j))) {
								(AABBAABB(x, y));
							}
							else if (Sphere* y = dynamic_cast<Sphere*>(n->contents.at(j))) {
								if (y->type != "Balloon") {
									sphereAABB(y, x);
								}
							}
						}
					}
				}		
			}
		}
	}
}

bool GameSimsPhysics::SphereSphere(Sphere* s, Sphere* s1) {
	if (s->getRadius() + s1->getRadius() >
		(s1->getPosition() - s->getPosition()).Length()) {
		std::cout << "SPHERE SPHERE COLLISION DETECTED" << std::endl;
		if (s->type == "Player" && s1->type == "Laser") {
			std::cout << "Laser Hit" << std::endl;
			if (bfg->noBalloon) {
				bfg->playerHit();		// If not invincible, they are hit
			}
			s1->obj->collided = true;
			return true;		// If player hits a laser, do not create collision pair
		}
		if (s->type == "Laser" || s1->type == "Laser" && bounceTimer > 0.1f) {
			if (s->type == "Laser") {
				Laser* x = dynamic_cast<Laser*>(s->obj);
				x->collisons--;		// Lasers can collide only 4 times
				if (x->collisons <= 0) {
					s->obj->collided = true;		// Delete if collided 4 times already
				}
			}
			if (s1->type == "Laser") {
				Laser* x = dynamic_cast<Laser*>(s1->obj);
				x->collisons--;
				if (x->collisons <= 0) {
					s1->obj->collided = true;
				}
			}
			bounceTimer = 0.0f;
		}
		if ((s->type == "Laser" && s1->type == "Robot") || (s1->type == "Laser" && s->type == "Robot")) {
			std::cout << "Robot Hit!";
			if (s->type == "Laser") {
				BadRobot* x = dynamic_cast<BadRobot*>(s1->obj);
				s->obj->collided = true;
				x->hitTimer = 3.0f;		// Set robots hit timer, during which it cannot move or attack
			}
			if (s1->type == "Laser") {
				BadRobot* x = dynamic_cast<BadRobot*>(s->obj);
				s1->obj->collided = true;
				x->hitTimer = 3.0f;
			}
			bounceTimer = 0.0f;
		}
		CollisionPair* p = new CollisionPair;		// We create our collision pair
		p->setFirst(s);
		p->setSecond(s1);
		p->setPenetration((s->getRadius() + s1->getRadius()) - (s1->getPosition() - s->getPosition()).Length());
		Vector2 norm = Vector2(s1->getPosition().x - s->getPosition().x, s1->getPosition().y - s->getPosition().y);
		norm.Normalise();
		p->setNormal(norm);
		std::cout << "Penetration: " << p->getPenetration() << std::endl;
		std::cout << "Normal: " << p->getNormal() << std::endl;
		if (s->type == "Player" && s1->type == "Coin" && pickUpTimer > 0.1f) {
			std::cout << "Coin Collected" << std::endl;
			s1->obj->collided = true;
			bfg->coins++;
			pickUpTimer = 0.0f;
		}
		CollisionResolution(p);		// Resolve the collision
		return true;
	}
	return false;
}

bool GameSimsPhysics::AABBAABB(AABB* a, AABB* a1) {
	if (abs(a->getPosition().x - a1->getPosition().x) < (a->getHalfWidth() + a1->getHalfWidth())
		&& (abs(a->getPosition().y - a1->getPosition().y) < (a->getHalfHeight() + a1->getHalfHeight()))) {
		std::cout << "AABB AABB COLLISION DETECTED" << std::endl;
		CollisionPair* p = new CollisionPair;
		p->setFirst(a);
		p->setSecond(a1);
		float xpen = (a->getHalfWidth() + a1->getHalfWidth()) -
			abs(a->getPosition().x - a1->getPosition().x);
		float ypen = (a->getHalfHeight() + a1->getHalfHeight()) -
			abs(a->getPosition().y - a1->getPosition().y);
		if (xpen <= ypen) {
			p->setPenetration(xpen);
			Vector2 norm = Vector2(a1->getPosition().x - a->getPosition().x, 0);
			norm.Normalise();
			p->setNormal(norm);
		}
		else {
			p->setPenetration(ypen);
			Vector2 norm = Vector2{ 0, a1->getPosition().y - a->getPosition().y };
			norm.Normalise();
			p->setNormal(norm);
		}
		std::cout << "Penetration: " << p->getPenetration() << std::endl;
		std::cout << "Normal: " << p->getNormal() << std::endl;
		CollisionResolution(p);
		return true;
	}
	return false;
}

bool GameSimsPhysics::sphereAABB(CollisionVolume* a, CollisionVolume* b) {
	if ((dynamic_cast<AABB*>(a)) && (dynamic_cast<Sphere*>(b))) {
		sphereAABB(b, a);
	}
	if (Sphere* x = dynamic_cast<Sphere*>(a)) {
		if (AABB* y = dynamic_cast<AABB*>(b)) {
			auto dist = x->getPosition() - y->getPosition();
			auto center = x->getPosition();
			float minX = y->getPosition().x - y->getHalfWidth();
			float maxX = y->getPosition().x + y->getHalfWidth();
			float minY = y->getPosition().y - y->getHalfHeight();
			float maxY = y->getPosition().y + y->getHalfHeight();
			IntervalArithmetic cx{ center.x }, x2{ minX, maxX };
			IntervalArithmetic cy{ center.y }, y2{ minY , maxY };
			IntervalArithmetic r{ x->getRadius() };
			IntervalArithmetic equation = (x2 - cx).pow(2) + (y2 - cy).pow(2) - r.pow(2);
			if (equation.x <= 0) {
				if (x->type == "Laser" && bounceTimer > 0.1f) {
					Laser* l = dynamic_cast<Laser*>(x->obj);
					l->collisons--;
					if (l->collisons <= 0) {
						x->obj->collided = true;		// After 4 bounces, destroy the laser
					}
					bounceTimer = 0.0f;		// Reset the timer for laser collisions
				}
				std::cout << "SPHERE AABB COLLISION DETECTED" << std::endl;
				Vector2 closestp = closestPoint(x->getPosition(), y);
				CollisionPair* p = new CollisionPair;
				p->setFirst(x);
				p->setSecond(y);
				p->setPenetration(std::abs((closestp - x->getPosition()).Length() - x->getRadius()));
				Vector2 norm = Vector2(closestp - x->getPosition());
				norm.Normalise();
				p->setNormal(norm);
				std::cout << "Penetration: " << p->getPenetration() << std::endl;
				std::cout << "Normal: " << p->getNormal() << std::endl;
				if (x->type == "Player" && y->type == "Food" && pickUpTimer > 0.1f) {
					std::cout << "Food Collected" << std::endl;
					y->obj->collided = true;
					bfg->foodCount++;
					pickUpTimer = 0.0f;		// Used to prevent extra pick ups per frame
				}
				CollisionResolution(p);
				return true;
			}
			return false;
		}
	}
}

/* Method for finding the closest point on an AABB and a sphere */
Vector2 GameSimsPhysics::closestPoint(Vector2 pos, AABB* b) {
	Vector2 point;
	float minX = b->getPosition().x - b->getHalfWidth();
	float maxX = b->getPosition().x + b->getHalfWidth();
	float minY = b->getPosition().y - b->getHalfHeight();
	float maxY = b->getPosition().y + b->getHalfHeight();
	float v = pos.x;
	if (v < minX) {
		point.x = minX;
	}
	if (v > maxX) {
		point.x = maxX;
	}
	if (v > minX && v < maxX) {
		point.x = pos.x;
	}
	v = pos.y;
	if (v < minY) {
		point.y = minY;
	}
	if (v > maxY) {
		point.y = maxY;
	}
	if (v > minY && v < maxY) {
		point.y = pos.y;
	}
	return point;
}

void GameSimsPhysics::CollisionResolution(CollisionPair* pair) {
	/* Projection */
	float totalMass = pair->getFirst()->body->inverseMass + pair->getSecond()->body->inverseMass;
	pair->getFirst()->body->position -= pair->getNormal() * pair->getPenetration() * (pair->getFirst()->body->inverseMass / totalMass);
	pair->getSecond()->body->position += pair->getNormal() * pair->getPenetration() * (pair->getSecond()->body->inverseMass / totalMass);

	/* Impulse */
	Vector2 relativeVelocity = pair->getSecond()->body->GetVelocity() - pair->getFirst()->body->GetVelocity();
	float dotproduct = Vector2::Dot(relativeVelocity, pair->getNormal());
	float maxelasticity = std::max(pair->getFirst()->body->getElasticity(), pair->getSecond()->body->getElasticity());		// Take the maximum elasticity from both objects, lasers will not slow down
	float impulse = (-(1 + maxelasticity) * dotproduct) / totalMass;
	pair->getFirst()->body->velocity -= pair->getNormal() * pair->getFirst()->body->inverseMass * impulse;
	pair->getSecond()->body->velocity += pair->getNormal() * pair->getSecond()->body->inverseMass * impulse;
}

void GameSimsPhysics::IntegrateVelocity(float dt) {
	for (const auto& it : allBodies) {
		it->position += it->velocity * dt;
		if (it->inverseMass != 15.0f) {		// Lasers don't lose velocity
			it->velocity *= 0.975f;		// Damping velocity
		}
	}
}




