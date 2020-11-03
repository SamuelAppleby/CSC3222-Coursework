/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Food.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

std::vector<Food*> Food::food;

Vector4 foodFrames[] = {
	Vector4(0,0,15,16), //Apple
	Vector4(17,0,14,16),//Can
	Vector4(31,0,18,16),//Pizza
	Vector4(49,0,15,16),//Sundae
	Vector4(64,0,16,16) //Bread
};

Food::Food() : SimObject() {
	food.push_back(this);		// Add this food to the list of food
	texture = texManager->GetTexture("food_items16x16.png");
	foodItem = rand() % 5;
	currentanimFrame = foodItem;
	inverseMass = 1.0f;
	elasticity = 1.0f;
}

Food::~Food() {

}

bool Food::UpdateObject(float dt, BadFoodGame& bfg) {
	timeAlive += dt;
	if (timeAlive > 12.0f || collided == true) {		// If living for more than 12 seconds
		return false;
	}
	if (bfg.hit == true) {
		food.clear();		// Delete all food if the player is hit
		collided = true;
	}
	if (collided == true) {
		return false;		// If the player dies
	}
	return true;
}

void Food::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	= Vector2(foodFrames[currentanimFrame].x, foodFrames[currentanimFrame].y);
	Vector2 texSize = Vector2(foodFrames[currentanimFrame].z, foodFrames[currentanimFrame].w);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}
