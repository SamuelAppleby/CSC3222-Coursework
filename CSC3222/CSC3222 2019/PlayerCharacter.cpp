/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "PlayerCharacter.h"
#include "TextureManager.h"
#include "../../Common/Window.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

int  charARunY = 128;
int  charARunX[] = { 32,64,96,128,160,192,224,256 };
bool charAFlips[] = { false, true, true, false, false };

Vector2 charAIdle(64, 64);
Vector2 charBIdle(64, 64);

int  charBRunY = 96;
int  charBRunX[] = { 128,160,192,224,256,288 };
bool charBFlips[] = { false, true, true, false, false };

PlayerCharacter::PlayerCharacter() : SimObject() {
	currentAnimDir = MovementDir::Left;
	SetCharacterType(CharacterType::TYPE_A);
	inverseMass = 0.05f;
	elasticity = 1.0f;
}

PlayerCharacter::~PlayerCharacter() {

}

bool PlayerCharacter::UpdateObject(float dt, BadFoodGame& bfg) {
	currentAnimDir = MovementDir::Idle;
	Vector2 force;
	if ((Window::GetKeyboard()->KeyDown(KeyboardKeys::W) || Window::GetKeyboard()->KeyDown(KeyboardKeys::S) || Window::GetKeyboard()->KeyDown(KeyboardKeys::A) || Window::GetKeyboard()->KeyDown(KeyboardKeys::D))) {
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
			currentAnimDir = MovementDir::Up;
			UpdateAnimFrame(dt);
			force.y += -50;		// Add force on the negative y axis
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
			currentAnimDir = MovementDir::Down;
			UpdateAnimFrame(dt);
			force.y += 50;		// Add force on the positive y axis
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
			currentAnimDir = MovementDir::Left;
			UpdateAnimFrame(dt);
			force.x += -50;		// Add force on the negative x axis
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
			currentAnimDir = MovementDir::Right;
			UpdateAnimFrame(dt);
			force.x += 50;		// Add force on the positive x axis
		}
		Vector2 tilePos = bfg.currentMap->WorldPosToTilePos(position);
		int tileCoord = tilePos.y * bfg.currentMap->GetMapWidth() + tilePos.x;
		int tileCost = bfg.currentMap->mapCosts[tileCoord];		// Get cost of this coord
		if (tileCost == 5) {
			AddForce(force * 0.7);		// Player takes 30% speed reduction from grass
		}
		else {
			AddForce(force);		// Otherwise treat normally
		}
	}
	else {
		SetForce({ 0,0 });		// If no keys are pressed, set force to 0
	}
	return true;
}

void PlayerCharacter::DrawObject(GameSimsRenderer& r) {
	switch (charType) {
	case CharacterType::TYPE_A:DrawCharacterA(r); break;
	case CharacterType::TYPE_B:DrawCharacterB(r); break;
	}
}

void PlayerCharacter::DrawCharacterA(GameSimsRenderer& r) {
	Vector2 screenPos = position;
	Vector2 texSize = Vector2(32, 32);

	Vector2	texPos;
	texPos.x = (float)charARunX[currentanimFrame];
	texPos.y = (float)charARunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charAIdle;
	}
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, charAFlips[(int)currentAnimDir]);
}

void PlayerCharacter::DrawCharacterB(GameSimsRenderer& r) {
	Vector2 screenPos = position;
	Vector2 texSize = Vector2(32, 32);

	Vector2	texPos;
	texPos.x = (float)charBRunX[currentanimFrame];
	texPos.y = (float)charBRunY;

	if (currentAnimDir == MovementDir::Idle) {
		texPos = charBIdle;
	}

	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, screenPos, charBFlips[(int)currentAnimDir]);
}

void PlayerCharacter::SetCharacterType(CharacterType t) {
	charType = t;
	switch (charType) {
	case CharacterType::TYPE_A:
		texture = texManager->GetTexture("TL_oldcharacters.png");
		animFrameCount = 8;
		break;
	case CharacterType::TYPE_B:
		texture = texManager->GetTexture("Twiggy_spritesheet.png");
		animFrameCount = 6;
		break;
	}
}