/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include "GameMap.h"

namespace NCL {
	namespace CSC3222 {
		class Food : public SimObject
		{
		public:
			Food();
			~Food();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;
			void DrawObject(GameSimsRenderer& r) override;
			static std::vector<Food*> food;

		protected:
			int foodItem;
			float timeAlive = 0.0f;
		};
	}
}

