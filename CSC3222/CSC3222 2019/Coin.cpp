/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Coin.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Vector2 animFrames[] = {
	Vector2(96 , 80),
	Vector2(112, 80),
	Vector2(128, 80),
	Vector2(144, 80),
};

Coin::Coin() : SimObject() {
	animFrameCount = 4;
	texture = texManager->GetTexture("Items.png");
	inverseMass = 1.0f;
	elasticity = 1.0f;
}

Coin::~Coin() {

}

bool Coin::UpdateObject(float dt, BadFoodGame& bfg) {
	UpdateAnimFrame(dt);
	if (collided == true) {
		return false;
	}
	return true;
}

void Coin::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos	 = animFrames[currentanimFrame];
	Vector2 texSize	 = Vector2(16,16);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}