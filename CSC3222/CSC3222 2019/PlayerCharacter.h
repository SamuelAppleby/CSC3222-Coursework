/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include "BadFoodGame.h"
namespace NCL {
	namespace CSC3222 {
		class PlayerCharacter : public SimObject
		{
		public:
			enum class CharacterType {
				TYPE_A,
				TYPE_B
			};
			PlayerCharacter();
			~PlayerCharacter();

			bool UpdateObject(float dt, BadFoodGame& bfg) override;

			void DrawObject(GameSimsRenderer& r) override;

			void SetCharacterType(CharacterType t);



		protected:
			enum class MovementDir {
				Up,
				Down,
				Left,
				Right,
				Idle
			};
			void DrawCharacterA(GameSimsRenderer& r);
			void DrawCharacterB(GameSimsRenderer& r);

			MovementDir		currentAnimDir;
			CharacterType	charType;
		};
	}
}

