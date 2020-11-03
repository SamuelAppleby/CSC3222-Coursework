/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include <vector>
#include "PlayerCharacter.h"
#include "Food.h"
#include "BadRobot.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class TextureManager;
		class PlayerCharacter;

		class BadFoodGame {
		public:
			BadFoodGame();
			~BadFoodGame();
			void Update(float dt);
			void AddNewObject(SimObject* object);
			int fibonacci(int n);
			void playerHit();
			std::vector<SimObject*> getGameObjects();
			float foodSpawn = 5.0f;		// Food spawns every 5 seconds
			float coinSpawn = 15.0f;		// Coins spawn every 15 seconds
			float robotSpawn = 15.0f;		// Robots spawn every 15 seconds
			bool noBalloon = true;		
			bool deleteBots = false;
			PlayerCharacter* player;
			GameMap* currentMap;
			int foodCount;
			int lives;
			int coins;
			bool hit = false;
			bool spent = false;
			GameSimsRenderer* renderer;


		protected:
			void InitialiseGame();

			TextureManager*		texManager;
			GameSimsPhysics*	physics;
			float gameTime;
			int currentScore;
			int balloons;
			bool drawn = false;
			std::vector<SimObject*> newObjects;
			std::vector<SimObject*> gameObjects;	
		};
	}
}

