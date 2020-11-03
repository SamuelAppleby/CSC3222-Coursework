/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include "GameMap.h"
namespace NCL {
	namespace CSC3222 {
		class Balloon : public SimObject
		{
		public:
			Balloon();
			~Balloon();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;
			void DrawObject(GameSimsRenderer& r) override;
			float getTimeAlive() {
				return timeAlive;
			}
		protected:
			float timeAlive = 0.0f;
		};
	}
}

