/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "BadRobot.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "Laser.h"
using namespace NCL;
using namespace CSC3222;

std::vector<BadRobot*> BadRobot::robots;

Vector4 animFramesGreen[] = {
	Vector4(71,142,18,18),
	Vector4(102,142,20,18),
	Vector4(135,142,19,18),
	Vector4(166,142,19,18),
	Vector4(198,140,20,18)
};

Vector4 animFramesWhite[] = {
	Vector4(71,266,17,22),
	Vector4(104,265,19,22),
	Vector4(137,266,17,22),
	Vector4(168,266,17,22),
	Vector4(200,264,17,23)
};

BadRobot::BadRobot() : SimObject() {
	robots.push_back(this);		// Add this robot to that static list
	texture = texManager->GetTexture("TL_Creatures.png");
	animFrameCount = 4;
	int r = rand() % 2;
	if (r == 0) {
		type = RobotType::Green;
	}
	else {
		type = RobotType::White;
	}
	inverseMass = 0.05f;
	elasticity = 1.0f;
}

BadRobot::~BadRobot() {

}

bool BadRobot::UpdateObject(float dt, BadFoodGame& bfg) {
	UpdateAnimFrame(dt);
	if (bfg.hit == true) {
		robots.clear();		// Delete all robots if player is hit
		collided = true;
	}
	if (collided == true) {
		return false;		// If the player dies
	}
	shoot -= dt;
	hitTimer -= dt;
	/* Setting robot states */
	float distanceToPlayer = (position - bfg.player->GetPosition()).Length();
	if (distanceToPlayer > 80) {
		if (bfg.foodCount >= 5) {
			patrol = false;
			attack = false;
			protect = true;
		}
		else {
			patrol = true;
			attack = false;
			protect = false;
		}
	}
	if (distanceToPlayer <= 80) {
		if (bfg.foodCount >= 5) {
			patrol = false;
			attack = true;
			protect = true;		// Can be in both protect and attack mode at once
		}
		else {
			patrol = false;
			attack = true;
			protect = false;
		}
	}

	/* State Behaviour */
	if (patrol) {
		if (!picked) {
			start = bfg.currentMap->WorldPosToTilePos(position);		
			destination = bfg.currentMap->GetRandomPosition();
			destination = bfg.currentMap->WorldPosToTilePos(destination);		// Get destination in tile coords
			std::cout << "Robot A* Destination: " << destination;
			path = bfg.currentMap->GeneratePath(start, destination);
			std::reverse(path.begin(), path.end());		// Path in intially in reverse order, swap it 
			currentWaypoint = 0;
			std::cout << "ROBOT PATROLLING" << std::endl;
			picked = true;		// We have chosen our path
		}
		if (picked){
			direction = path.at(currentWaypoint) - bfg.currentMap->WorldPosToTilePos(position);
			direction = bfg.currentMap->TilePosToWorldPos(direction);		// Direction of next node
		}
		if (direction.Length() >= 8) {
			SetForce(direction * 100);		// Multiply by 20 Newtons of force
		}
		else {
			if (currentWaypoint != path.size() - 1) {
				currentWaypoint++;		// Go to the next node
			}
			else {
				picked = false;		// Destination is reached, choose another path
			}
		}
		Vector2 line = bfg.currentMap->TilePosToWorldPos(destination);
		bfg.renderer->DrawLine(position, Vector2(line.x + 8, line.y + 8), Vector4(1, 0, 0, 1));		// +8 for centre of the tile
	}
	if (attack) {
		direction = bfg.player->GetPosition() - position;
		SetForce(direction * 20);
		if (shoot <= 0.0f && hitTimer <= 0.0f) {		// Cannot shoot if hit by a laser
			std::cout << "ROBOT ATTACKING" << std::endl;
			Laser* testLaser = new Laser(Vector2{ 0,0 });
			testLaser->SetPosition(Vector2(GetPosition().x - 10, GetPosition().y + 20));
			testLaser->SetVelocity(direction);
			bfg.AddNewObject(testLaser);
			shoot = 2.0f;		// 2 seconds between lasers
		}
		bfg.renderer->DrawLine(position, bfg.player->GetPosition(), Vector4(1, 0, 0, 1));		// +8 for centre of the tile
	}
	if (protect) {
		if (!picked) {
			std::cout << "ROBOT PROTECTING" << std::endl;
			start = bfg.currentMap->WorldPosToTilePos(position);		
			destination = bfg.currentMap->GetRandomPositionWithinRadius(Vector2(400, 112), 48);		// Random positon 3 tiles within entrance to food shop
			destination = bfg.currentMap->WorldPosToTilePos(destination);		// Get destination in tile coords
			std::cout << "Robot Destination: " << destination;
			path = bfg.currentMap->GeneratePath(start, destination);
			std::reverse(path.begin(), path.end());		// Path in intially in reverse order, swap it 
			currentWaypoint = 0;
			picked = true;
		}
		if (picked) {
			direction = path.at(currentWaypoint) - bfg.currentMap->WorldPosToTilePos(position);
			direction = bfg.currentMap->TilePosToWorldPos(direction);
		}
		if (direction.Length() >= 8) {
			/* Clamping the direction speed */
			if (direction.x > 10) {
				direction.x = 10;
			}
			if (direction.y > 10) {
				direction.y = 10;

			}
			SetForce(direction * 100);		// Multiply by 20 Newtons of force
		}
		else {
			if (currentWaypoint != path.size() - 1) {
				currentWaypoint++;
			}
			else {
				picked = false;
			}
		}
		Vector2 line = bfg.currentMap->TilePosToWorldPos(destination);
		bfg.renderer->DrawLine(position, Vector2(line.x + 8, line.y + 8), Vector4(1, 0, 0, 1));		// +8 for centre of the tile
	}
	
	Flocking(robots, dt);		// Perform flocking on group of boids
	Vector2 tilePos = bfg.currentMap->WorldPosToTilePos(Vector2((int)position.x / 16, (int)position.y / 16) * 16);
	int test = tilePos.y * bfg.currentMap->GetMapWidth() + tilePos.x;
	int tileCost = bfg.currentMap->mapCosts[test];
	if (tileCost == 5) {
		SetForce(force * 0.4);		// Robots take a 60% speed reduction from grass (both in A* and physically)
	}
	if (hitTimer >= 0.0f) {
		SetForce(Vector2(0, 0));		// If robot is hit, cannot move
	}
	return true;
}

void BadRobot::DrawObject(GameSimsRenderer& r) {
	Vector2	texPos;
	Vector2 texSize;

	switch (type) {
	case RobotType::Green: {
		texPos = Vector2(animFramesGreen[currentanimFrame].x, animFramesGreen[currentanimFrame].y);
		texSize = Vector2(animFramesGreen[currentanimFrame].z, animFramesGreen[currentanimFrame].w);
	}break;
	case RobotType::White: {
		texPos = Vector2(animFramesWhite[currentanimFrame].x, animFramesWhite[currentanimFrame].y);
		texSize = Vector2(animFramesWhite[currentanimFrame].z, animFramesWhite[currentanimFrame].w);
	}break;
	}
	r.DrawTextureArea((OGLTexture*)texture, texPos, texSize, position, false);
}

Vector2 BadRobot::Allignment(std::vector<BadRobot*> bots)
{
	Vector2 direction = this->velocity;
	for (auto& bot : bots) {
		if (bot != this) {
			float distance = (this->position - bot->position).Length();
			if (distance <= directionThreshold) {
				direction += bot->velocity;
			}
		}
	}
	return direction.Normalised();
}

Vector2 BadRobot::Separation(std::vector<BadRobot*> bots)
{
	Vector2 direction;
	for (auto& bot : bots) {
		if (bot != this) {
			float distance = (this->position - bot->position).Length();
			if (distance <= separationThreshold) {
				float strength = 1.0f - (distance / separationThreshold);
				direction += (this->position - bot->position).Normalised() * strength;
			}
		}
	}
	return direction.Normalised();
}

Vector2 BadRobot::Cohesion(std::vector<BadRobot*> bots)
{
	Vector2 avgPos = this->position;
	float count = 1;
	for (auto& bot : bots) {
		if (bot != this) {
			float distance = Vector2(this->position - bot->position).Length();
			if (distance <= cohesionThreshold) {
				avgPos += bot->position;
				count++;
			}
		}
	}
	avgPos /= count;
	Vector2 direction = avgPos - this->position;
	return direction.Normalised();
}

Vector2 BadRobot::Avoidance(std::vector<BadRobot*> bots)
{
	Vector2 direction;
	for (auto& bot : bots) {
		if (bot != this && bot->type != this->type) {
			float distance = Vector2(this->position - bot->position).Length();
			if (distance <= avoidanceThreshold) {
				direction += (this->position - bot->position).Normalised();
			}
		}
	}
	return direction.Normalised();
}

void BadRobot::Flocking(std::vector<BadRobot*> bots, float dt)
{
	for (auto& bot : bots) {
		Vector2 direction;
		direction += Allignment(bots);
		direction += Separation(bots);
		direction += Cohesion(bots);
		direction += Avoidance(bots);
		bot->position += direction * dt;
	}
}


