/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Building.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Building::Building(int t) {
	type = t;
	inverseMass = 0.0f;		// Infinite mass
	elasticity = 0.0f;		// Perfectly inelastic
}

Building::~Building() {

}

bool Building::UpdateObject(float dt, BadFoodGame& bfg) {
	return true;
}

void Building::DrawObject(GameSimsRenderer& r) {
}


