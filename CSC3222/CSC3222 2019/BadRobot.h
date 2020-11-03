/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include "GameMap.h"
namespace NCL {
	namespace CSC3222 {
		class BadRobot : public SimObject
		{
		public:
			BadRobot();
			~BadRobot();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;
			void DrawObject(GameSimsRenderer& r) override;
			bool patrol = false;
			bool attack = false;
			bool protect = false;
			bool picked = false;
			Vector2 destination;
			Vector2 start;
			Vector2 direction;
			int currentWaypoint = 0;
			std::vector<Vector2> path;
			float shoot = 0.0f;
			float hitTimer = 0.0f;		// If robot is hit with laser
			static std::vector<BadRobot*> robots;
			Vector2 Allignment(std::vector<BadRobot*> bots);
			float directionThreshold = 48.0f;		// 3 tiles for allignment influence
			Vector2 Separation(std::vector<BadRobot*> bots);
			float separationThreshold = 16.0f;		// 1 tile apart for separation
			Vector2 Cohesion(std::vector<BadRobot*> bots);
			float cohesionThreshold = 32.0f;		// 2 tiles for cohesion
			Vector2 Avoidance(std::vector<BadRobot*> bots);
			float avoidanceThreshold = 48.0f;		// 3 tiles for avoidance
			void Flocking(std::vector<BadRobot*> bots, float dt);

		protected:
			enum class RobotType {
				Green,
				White,
			};
			RobotType type;
			
		};
	}
}

