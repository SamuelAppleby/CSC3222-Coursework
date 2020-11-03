/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Wall.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Wall::Wall() {
	inverseMass = 0.0f;		// Infinite mass
	elasticity = 0.0f;		// Perfectly inelastic
}

Wall::~Wall() {

}

bool Wall::UpdateObject(float dt, BadFoodGame& bfg) {
	return true;
}

void Wall::DrawObject(GameSimsRenderer& r) {
}


