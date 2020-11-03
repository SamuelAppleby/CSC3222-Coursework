/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "Balloon.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

Balloon::Balloon() : SimObject() {
	texture = texManager->GetTexture("TL_Creatures.png");
	inverseMass = 5.0f;
}

Balloon::~Balloon() {

}

bool Balloon::UpdateObject(float dt, BadFoodGame& bfg) {
	timeAlive += dt;
	if (position.y < -10.0f) {
		return false;		// Destroy balloon if floated to top of screen
	}
	if (timeAlive < 5.0f) {
		/* Spring Calculations */
		float k = -10;
		float damping = 1;
		float gravity = 10;
		float springFY = k * (position.y - bfg.player->GetPosition().y);
		float dampFY = damping * velocity.y;
		float springFX = k * (position.x - bfg.player->GetPosition().x);
		float dampFX = damping * velocity.x;
		SetForce(Vector2{ springFX + inverseMass * gravity - dampFX,
			springFY + inverseMass * 10 - dampFY });
	}
	else {
		bfg.noBalloon = true;		// Can activate another balloon
		AddForce(Vector2(0, -0.5));		// Float balloon to top of the screen
	}
	return true;
}

void Balloon::DrawObject(GameSimsRenderer& r) {
	Vector2 texPos = Vector2(73, 99);
	Vector2 texSize = Vector2(14, 28);
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position);
}