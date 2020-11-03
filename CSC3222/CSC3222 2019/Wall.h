/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include "BadFoodGame.h"

namespace NCL {
	namespace CSC3222 {
		class Wall : public SimObject
		{
		public:
			Wall();
			~Wall();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;
			void DrawObject(GameSimsRenderer& r) override;

		protected:
		};
	}
}

