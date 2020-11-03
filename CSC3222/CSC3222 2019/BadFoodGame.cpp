/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "BadFoodGame.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "Laser.h"
#include "Coin.h"
#include "Balloon.h"
#include "CollisionVolume.h"
#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"
#include <sstream>
#include <iomanip>
#include "Sphere.h"
#include "AABB.h"
#include "Wall.h"
#include "Building.h"
#include "QuadTreeNode.h"
#include "BadRobot.h"


using namespace NCL;
using namespace CSC3222;

BadFoodGame::BadFoodGame() {
	renderer = new GameSimsRenderer();
	texManager = new TextureManager();
	physics = new GameSimsPhysics(this);
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

BadFoodGame::~BadFoodGame() {
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}

void BadFoodGame::Update(float dt) {
	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();
	gameTime += dt;
	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);
	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end();) {
		if (!(*i)->UpdateObject(dt, *this)) {		//object has said its finished with
			SimObject* x = dynamic_cast<SimObject*>(*i);
			physics->RemoveCollider(x->GetCollider());
			physics->RemoveRigidBody(x);
			delete (*i);
			i = gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}
	if ((player->GetPosition().x > 304 && player->GetPosition().x < 400) &&
		(player->GetPosition().y > 273) && coins > 0 && !spent) {			// If the player is in front of the balloon shop and hasn't spent a coin since last here
		balloons++;
		coins--;
		spent = true;		// Can spend another coin upon returning
	}
	if (!((player->GetPosition().x > 304 && player->GetPosition().x < 400) &&
		(player->GetPosition().y > 273) && coins > 0)) {
		spent = false;		// Player can spend only one coin at a time
	}
	if ((player->GetPosition().x > 352 && player->GetPosition().x < 448) &&
		(player->GetPosition().y > 104 && player->GetPosition().y < 124)) {
		if (foodCount != 0) {
			currentScore += 100 * fibonacci(foodCount + 3);			// If the player is infront of the food shop
		}
		foodCount = 0;		// Reset the food count
	}
	std::ostringstream os;
	os << std::fixed << std::setprecision(2) << gameTime;
	renderer->DrawString("B4DF00D", Vector2(420, 700));
	renderer->DrawString("Time:" + os.str(), Vector2(10, 700));
	renderer->DrawString("Score:" + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives left:" + std::to_string(lives), Vector2(10, 30));
	renderer->DrawString("Food:" + std::to_string(foodCount), Vector2(10, 50));
	renderer->DrawString("Coins:" + std::to_string(coins), Vector2(10, 70));
	renderer->DrawString("Balloons:" + std::to_string(balloons), Vector2(10, 90));
	auto i = gameObjects.begin(); i != gameObjects.end();
	for (int i = 0; i < physics->getColliders().size(); ++i) {
		/* Draw bounding boxes for all colliders */
		if (Sphere* x = dynamic_cast<Sphere*>(physics->getColliders().at(i))) {
			renderer->DrawCircle(x->getPosition(), x->getRadius(), Vector4(1, 0, 1, 1));
		}
		if (AABB* x = dynamic_cast<AABB*>(physics->getColliders().at(i))) {
			renderer->DrawBox(x->getPosition(), Vector2{ x->getHalfWidth(), x->getHalfHeight() }, Vector4(1, 0, 1, 1));
		}
	}
	if (!drawn) {
		for (int i = 0; i < currentMap->tiles.size(); ++i) {
			Wall* wall = new Wall;
			wall->SetPosition(Vector2{ currentMap->tiles.at(i).x + 8, currentMap->tiles.at(i).y + 8 });
			AddNewObject(wall);
		}
		for (int i = 0; i < currentMap->buildings.size(); ++i) {
			Building* building = new Building(i);
			building->SetPosition(Vector2{ currentMap->buildings.at(i).x, currentMap->buildings.at(i).y });
			AddNewObject(building);
		}
	}
	drawn = true;		// Do not try to redraw all buildings and walls
	robotSpawn -= dt;
		
	if (BadRobot::robots.size() < 20 && robotSpawn <= 0.0f) {		// No more than 20 robots and spawn 15 seconds apart
		hit = false;
		BadRobot* b = new BadRobot;
		b->SetPosition(currentMap->GetRandomPositionWithinRadius(Vector2(208, 48), 8));			// Robots spawn randomly in walled off area
		AddNewObject(b);
		robotSpawn = 15.0f;
	}
	foodSpawn -= dt;
	if (foodSpawn <= 0.0f) {
		Food* testFood = new Food;
		testFood->SetPosition(currentMap->GetRandomPosition());
		AddNewObject(testFood);
		foodSpawn = 5.0f;
	}
	coinSpawn -= dt;
	if (coinSpawn <= 0.0f) {
		Coin* testCoin = new Coin;
		testCoin->SetPosition(currentMap->GetRandomPosition());
		AddNewObject(testCoin);
		coinSpawn = 15.0f;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && balloons > 0 && noBalloon) {		// If space is pressed and are not currently invincible
		balloons--;
		Balloon* testBalloon = new Balloon();
		testBalloon->SetPosition(Vector2{ player->GetPosition().x, player->GetPosition().y });
		AddNewObject(testBalloon);
		noBalloon = false;		// Cannot spawn another balloon until the current one ends
	}	
	renderer->Render();
}

void BadFoodGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();
	currentMap = new GameMap("BadFoodMap.txt", gameObjects, *texManager);
	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());
	player = new PlayerCharacter;
	player->SetCharacterType(PlayerCharacter::CharacterType::TYPE_A);
	player->SetPosition(Vector2(50, 150));
	AddNewObject(player);
	gameTime = 0;
	currentScore = 0;
	foodCount = 0;
	coins = 0;
	balloons = 0;
	lives = 3;
}

void BadFoodGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		CollisionVolume* vol = new CollisionVolume;
		if (Coin* x = dynamic_cast<Coin*>(object)) {
			vol = new Sphere(object, "Coin", x, 8.0f);
		}
		if (Balloon* x = dynamic_cast<Balloon*>(object)) {
			vol = new Sphere(object, "Balloon", x, 8.0f);
		}
		if (BadRobot* x = dynamic_cast<BadRobot*>(object)) {
			vol = new Sphere(object, "Robot", x, 6.0f);
		}
		if (Laser* x = dynamic_cast<Laser*>(object)) {
			vol = new Sphere(object, "Laser", x, 4.0f);
		}
		if (PlayerCharacter* x = dynamic_cast<PlayerCharacter*>(object)) {
			vol = new Sphere(object, "Player", x, 12.0f);
		}
		if (Food* x = dynamic_cast<Food*>(object)) {
			vol = new AABB(object, "Food", x, 8.0f, 8.0f);
		}
		if (Wall* x = dynamic_cast<Wall*>(object)) {
			vol = new AABB(object, "Wall", x, 8.0f, 8.0f);
			vol->staticObj = true;		// Walls are static, do not test against other static objects
		}
		if (Building* x = dynamic_cast<Building*>(object)) {
			switch (x->type) {
			case 0:
				vol = new AABB(object, "Building", x, 30.0f, 30.0f);
				vol->staticObj = true;		// Building are also static
				break;
			case 1:
				vol = new AABB(object, "Building", x, 32.0f, 32.0f);
				vol->staticObj = true;
				break;
			case 2:
				vol = new AABB(object, "Building", x, 48.0f, 40.0f);
				vol->staticObj = true;
				break;
			case 3:
				vol = new AABB(object, "Building", x, 48.0f, 33.0f);
				vol->staticObj = true;
				break;
			}
		}
		object->SetCollider(vol);
	}
	physics->AddCollider(object->GetCollider());
}

std::vector<SimObject*> BadFoodGame::getGameObjects() {
	return gameObjects;
}

void BadFoodGame::playerHit() {
	lives--;
	foodCount = 0;
	hit = true;
	player->SetPosition(Vector2(50, 150));
}

int BadFoodGame::fibonacci(int n) {
	if (n <= 1) {
		return n;
	}
	return fibonacci(n - 1) + fibonacci(n - 2);
}