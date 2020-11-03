/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"

namespace NCL {
	namespace CSC3222 {
		class Coin :	public SimObject
		{
		public:
			Coin();
			~Coin();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;
			void DrawObject(GameSimsRenderer& r) override;

		protected:
		};
	}
}