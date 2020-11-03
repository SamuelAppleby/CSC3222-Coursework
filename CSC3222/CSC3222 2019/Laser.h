/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class Laser : public SimObject {
		public:
			Laser(Vector2 direction);
			~Laser();
			int collisons = 4;
			void DrawObject(GameSimsRenderer &r) override;
			bool UpdateObject(float dt, BadFoodGame& bfg) override;
		};
	}
}

