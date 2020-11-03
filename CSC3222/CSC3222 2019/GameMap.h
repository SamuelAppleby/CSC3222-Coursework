/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include <string>
#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"
#include "../../Common/MeshGeometry.h"
#include "BadFoodGame.h"
#include "MapNode.h"

namespace NCL {
	namespace Rendering {
		class TextureBase;
	}
	using namespace Maths;
	namespace CSC3222 {
		class GameSimsRenderer;
		class SimObject;
		class TextureManager;

		enum MapStructureType {
			Tree,
			BlueBuilding,
			RedBuilding,
			GreenBuilding
		};

		struct StructureData {
			Vector2				startPos;
			MapStructureType	structureType;
		};

		class GameMap	{
		public:
			GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager);
			~GameMap();

			void DrawMap(GameSimsRenderer & r);

			int GetMapWidth() const {
				return mapWidth;
			}

			int GetMapHeight() const {
				return mapHeight;
			}
			Vector2 GetRandomPosition() const;
			Vector2 GetRandomPositionWithinRadius(Vector2 pos, float r) const;
			Vector2 WorldPosToTilePos(Vector2 worldPos) const;
			Vector2 TilePosToWorldPos(Vector2 tilePos) const;
			std::vector<Vector2> tiles;
			std::vector<Vector2> buildings;
			std::vector<Vector2> GeneratePath(Vector2 from, Vector2 to);
			int* mapCosts;
			std::vector<MapNode*> getNeighbours(MapNode* node);


		protected:
			void BuildMapMesh();
			void AddNewTilePos(int x, int y, std::vector<Vector3>& pos);
			void AddNewTileTex(int x, int y, std::vector<Vector2>& tex);
			

			int mapWidth;
			int mapHeight;
			int structureCount;

			Rendering::TextureBase*	tileTexture;
			char*			mapData;
			MapNode*		nodes;
			std::vector<MapNode*> openList;
			std::vector<MapNode*> closedList;
			Vector2*		mapTexCoords;
			StructureData*	structureData;		
			MeshGeometry* mapMesh;
		};
		
	}
}

