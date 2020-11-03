/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "GameMap.h"
#include "GameSimsRenderer.h"
#include "TextureManager.h"
#include "../../Common/Maths.h"
#include "../../Common/Assets.h"
#include "../../Common/TextureLoader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace NCL;
using namespace CSC3222;
using namespace Rendering;

Vector4 buildingTypes[4] = {
	Vector4(320,16,64,80),	//tree
	Vector4(16,16,96,80), //blue building
	Vector4(128,32,64,64), //red building
	Vector4(208,32,96,64)  //green building
};

GameMap::GameMap(const std::string& filename, std::vector<SimObject*>& objects, TextureManager& texManager) {
	tileTexture = texManager.GetTexture("badfoodTiles.png");

	std::ifstream mapFile(Assets::DATADIR + filename);

	if (!mapFile) {
		std::cout << "GameMap can't be loaded in!" << std::endl;
		return;
	}

	mapFile >> mapWidth;
	mapFile >> mapHeight;

	mapData = new char[mapWidth * mapHeight];
	mapCosts = new int[mapWidth * mapHeight];
	nodes = new MapNode[mapWidth * mapHeight];

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			mapFile >> mapData[tileIndex];
		}
	}

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			char c;
			mapFile >> c;
			mapCosts[tileIndex] = c - '0';
			nodes[tileIndex].posX = x;
			nodes[tileIndex].posY = y;
			nodes[tileIndex].g = mapCosts[tileIndex];
			if (mapCosts[tileIndex] == 9) {
				nodes[tileIndex].closed == true;		// If we reach a 9 in the data file, it is unreachable
			}
		}
	}
	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileIndex = (y * mapWidth) + x;
			nodes[tileIndex].nodeData = nodes;
		}
	}
	BuildMapMesh();

	mapFile >> structureCount;

	structureData = new StructureData[structureCount];

	for (int i = 0; i < structureCount; ++i) {
		int type = 0;
		int xTile = 0;
		int yTile = 0;

		mapFile >> type;
		mapFile >> xTile;
		mapFile >> yTile;

		structureData[i].structureType = (MapStructureType)type;
		structureData[i].startPos.x = xTile * 16.0f;
		structureData[i].startPos.y = yTile * 16.0f;
	}
}

GameMap::~GameMap() {
	delete[] mapData;
	delete[] mapCosts;
	delete[] mapTexCoords;

	delete mapMesh;
}

void GameMap::DrawMap(GameSimsRenderer& r) {
	r.DrawMesh((OGLMesh*)mapMesh, (OGLTexture*)tileTexture);
	//structures then go on top
	for (int i = 0; i < structureCount; ++i) {
		Vector4 buildingData = buildingTypes[structureData[i].structureType];

		Vector2 texPos = Vector2(buildingData.x, buildingData.y);
		Vector2 texSize = Vector2(buildingData.z, buildingData.w);

		Vector2 screenPos = structureData[i].startPos;
		buildings.emplace_back(screenPos);
		r.DrawTextureArea((OGLTexture*)tileTexture, texPos, texSize, screenPos, false);
	}
}

void GameMap::BuildMapMesh() {
	vector<Vector2> texCoords;
	vector<Vector3> positions;
	Vector2 flatGrassTile = Vector2(288, 144);
	Vector2 primaryTile;
	Vector2 secondaryTile = flatGrassTile; //some sit on top of another tile type, usually grass!

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			int tileType = mapData[(y * mapWidth) + x];
			bool doSecondary = false;
			bool object = false;
			switch (tileType) {
				//the normal tile types first
			case 'A':primaryTile = Vector2(272, 112); break; //top left grass
			case 'B':primaryTile = Vector2(288, 112); break;//top grass
			case 'C':primaryTile = Vector2(320, 112); break;//top right grass
			case 'D':primaryTile = Vector2(272, 128); break;//left grass
			case 'E':primaryTile = Vector2(320, 128); break;//right grass
			case 'F':primaryTile = Vector2(272, 160); break;//bottom left
			case 'G':primaryTile = Vector2(320, 160); break;//bottom right
			case 'H':primaryTile = Vector2(288, 160); break;//bottom grass

			case 'I':primaryTile = Vector2(336, 112); break; //top left grass
			case 'J':primaryTile = Vector2(352, 112); break; //top right grass
			case 'K':primaryTile = Vector2(336, 128); break; //bottom left grass
			case 'L':primaryTile = Vector2(352, 128); break; //bottom right grass

			case 'M':primaryTile = Vector2(208, 112); doSecondary = true; object = true; break; //top left fence
			case 'N':primaryTile = Vector2(224, 112); doSecondary = true; object = true; break; //top fence
			case 'O':primaryTile = Vector2(240, 112); doSecondary = true; object = true; break; //top right fence
			case 'P':primaryTile = Vector2(208, 128); doSecondary = true; object = true; break; //left fence
			case 'Q':primaryTile = Vector2(208, 144); doSecondary = true; object = true; break; //bottom left fence
			case 'R':primaryTile = Vector2(240, 144); doSecondary = true; object = true; break; //bottom right fence
//Stone wall bits

			case 'a':primaryTile = Vector2(16, 112); doSecondary = true; object = true; break; //top left grass
			case 'b':primaryTile = Vector2(32, 112); object = true; break;//Horizontal A
			case 'c':primaryTile = Vector2(32, 128); break;//Horizontal B
			case 'd':primaryTile = Vector2(144, 112); doSecondary = true; object = true; break;//top right grass
			case 'e':primaryTile = Vector2(144, 128); object = true; break;//Side
			case 'f':primaryTile = Vector2(16, 160); object = true; break;//bottom left A
			case 'g':primaryTile = Vector2(16, 176); doSecondary = true; break;//bottom left B

			case 'h':primaryTile = Vector2(64, 160); doSecondary = true; object = true; break;//right hand corner A
			case 'i':primaryTile = Vector2(64, 176); doSecondary = true; break;//right hand corner B

			case 'j':primaryTile = Vector2(96, 160); doSecondary = true; object = true; break;//Left hand corner A
			case 'k':primaryTile = Vector2(96, 176); doSecondary = true; break;//Left hand corner B

			case 'l':primaryTile = Vector2(144, 160); object = true; break;//Bottom right A
			case 'm':primaryTile = Vector2(144, 176); doSecondary = true; break;//Bottom right B
			case 'n':primaryTile = Vector2(32, 176); doSecondary = true; break;//bottom horizontal

			case 'o':primaryTile = Vector2(80, 176); doSecondary = true; break;//Stone to grass transition
			case 'p':primaryTile = Vector2(80, 176); doSecondary = true; break;//stone to road transition

			case '1':primaryTile = Vector2(352, 144); break; //Road Tile
			case '2':primaryTile = Vector2(64, 144);  break;//Stone Tile
			case '3':primaryTile = Vector2(16, 192); doSecondary = true; break; //Flower A
			case '4':primaryTile = Vector2(32, 192); doSecondary = true; break;//Flower B
			case '5':primaryTile = Vector2(48, 192); doSecondary = true; break;//Flower C
			case '6':primaryTile = Vector2(64, 192); doSecondary = true; break;//Flower D
			case '7':primaryTile = Vector2(176, 176); doSecondary = true; break;//Sign A
			case '8':primaryTile = Vector2(192, 176); doSecondary = true; break;//Sign B
			case '9':primaryTile = Vector2(208, 176); doSecondary = true; break;//Sign C	
			case '0': {
				static Vector2 grassTiles[] = {
				Vector2(288,128), Vector2(304,144),	Vector2(304,128), Vector2(288,144) };
				primaryTile = grassTiles[Maths::Clamp(rand() % 20, 0, 3)];
			}break;//Random grass tiles
			}
			if (object) {
				tiles.emplace_back(Vector2{ (x + 0) * 16.0f, (y + 0) * 16.0f });
			}
			if (doSecondary) {
				AddNewTilePos(x, y, positions);
				AddNewTileTex((int)secondaryTile.x, (int)secondaryTile.y, texCoords);
				secondaryTile = flatGrassTile;
			}
			AddNewTilePos(x, y, positions);
			AddNewTileTex((int)primaryTile.x, (int)primaryTile.y, texCoords);
		}
	}
	mapMesh = new OGLMesh();
	mapMesh->SetVertexPositions(positions);
	mapMesh->SetVertexTextureCoords(texCoords);
	mapMesh->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
	mapMesh->UploadToGPU();
}

void GameMap::AddNewTilePos(int x, int y, std::vector<Vector3>& pos) {
	Vector3 topLeft = Vector3((x + 0) * 16.0f, (y + 0) * 16.0f, 0);
	Vector3 topRight = Vector3((x + 1) * 16.0f, (y + 0) * 16.0f, 0);
	Vector3 bottomLeft = Vector3((x + 0) * 16.0f, (y + 1) * 16.0f, 0);
	Vector3 bottomRight = Vector3((x + 1) * 16.0f, (y + 1) * 16.0f, 0);

	pos.emplace_back(topLeft);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(topRight);

	pos.emplace_back(topRight);
	pos.emplace_back(bottomLeft);
	pos.emplace_back(bottomRight);
}

void GameMap::AddNewTileTex(int x, int y, std::vector<Vector2>& tex) {
	Vector2 topLeft = Vector2((x + 0.0f), (y + 0.0f));
	Vector2 topRight = Vector2((x + 16.0f), (y + 0.0f));
	Vector2 bottomLeft = Vector2((x + 0.0f), (y + 16.0f));
	Vector2 bottomRight = Vector2((x + 16.0f), (y + 16.0f));

	tex.emplace_back(topLeft);
	tex.emplace_back(bottomLeft);
	tex.emplace_back(topRight);

	tex.emplace_back(topRight);
	tex.emplace_back(bottomLeft);
	tex.emplace_back(bottomRight);
}

Vector2 GameMap::GetRandomPosition() const
{
	for (int i = 0; i < 10; i++) {		// Ensure we have a valid spawn
		int randWidth = rand() % mapWidth + 0;
		int randHeight = rand() % mapHeight + 0;
		int tileIndex = (randHeight * mapWidth) + (randWidth);
		int x = tileIndex % 30;
		int y = tileIndex / 30;
		if (mapCosts[tileIndex] != 9 && (x > 8 || x < 17) && (y > 3 || y < 1)) {		// If not a wall/building and not within the robots spawn
			return TilePosToWorldPos(Vector2(x, y));
			break;
		}
	}
}

Vector2 GameMap::GetRandomPositionWithinRadius(Vector2 pos, float r) const
{
	Vector2 location = WorldPosToTilePos(pos);
	int xLoc = location.x;
	int yLoc = location.y;
	int newR = r / 16;
	int maxX = xLoc + newR;
	int minX = xLoc - newR;
	int maxY = yLoc + newR;
	int minY = yLoc - newR;
	for (int i = 0; i < 10; i++) {
		int randWidth = minX + (std::rand() % (maxX - minX + 1));
		int randHeight = minY + (std::rand() % (maxY - minY + 1));
		int tileIndex = (randHeight * mapWidth) + (randWidth);
		int x = tileIndex % 30;
		int y = tileIndex / 30;
		if (mapCosts[tileIndex] != 9) {
			return TilePosToWorldPos(Vector2(x, y));
		}
	}
}

Vector2 GameMap::WorldPosToTilePos(Vector2 worldPos) const
{
	int xRoundPos = worldPos.x / 16;		// Cutting off remainders
	xRoundPos *= 16;
	int yRoundPos = worldPos.y / 16;
	yRoundPos *= 16;
	return Vector2(xRoundPos / 16, yRoundPos / 16);
}

Vector2 GameMap::TilePosToWorldPos(Vector2 tilePos) const
{
	return Vector2(tilePos.x * 16, tilePos.y * 16);
}

std::vector<Vector2> GameMap::GeneratePath(Vector2 from, Vector2 to)
{
	std::vector<Vector2> path;
	MapNode* current = &nodes[(int)(from.y * mapWidth + from.x)];       // Will not have a parent
	std::vector <MapNode*> neighbours;
	openList.clear();
	closedList.clear();
	openList.push_back(current);      // Add first node to open list
	bool contains = false;
	while (openList.size() != 0 && !contains)      // While our open list is not empty and the end position does not exit in the closed list
	{
		current = openList[0];      // Top element of open list is considered (this will be the next lowest cost)
		openList.erase(openList.begin() + 0);
		closedList.push_back(current);
		if (Vector2(current->posX, current->posY) == to) {
			contains = true;
		}
		neighbours = getNeighbours(current);
		for (auto& neighbour : neighbours)
		{
			if (!std::count(closedList.begin(), closedList.end(), neighbour)) 
			{
				if (!std::count(openList.begin(), openList.end(), neighbour))      // If not contained in the open list
				{
					neighbour->bestParent = current;     // Set its parent
					neighbour->h = abs(to.x - neighbour->posX) + abs(to.y - neighbour->posY);     // Manhattan distance 
					neighbour->g += neighbour->bestParent->g;		// Update g cost
					neighbour->f = neighbour->h + neighbour->g;
					openList.push_back(neighbour);    // Then add this neighbout to the open list
					std::sort(openList.begin(), openList.end(),			
						[](const MapNode* x, const MapNode* y) {
							return x->f < y->f;		// Orders by f values, that on top will be lowest f score
						});
				}
			}
		}
	}
	int check = 0;
	for(auto& x : closedList)
	{
		check++;
		if (Vector2(x->posX, x->posY) == to) {
			break;		// Closed list contains the destination
		}
		else if (check == closedList.size()) {
			return {};			// If the destination is not contained, we haven't found a path
		}
	}

	/* We have reached the end of our loop, now create the path */
	ptrdiff_t pos = std::distance(closedList.begin(), find(closedList.begin(), closedList.end(), current));		// Check to see if end is in the closed list
	if (pos <= closedList.size()) {		// Position of destination
		MapNode* temp = closedList[pos];
		while (temp != nullptr)
		{
			if (Vector2(temp->posX, temp->posY) != from) {
				path.push_back(Vector2(temp->posX, temp->posY));
				temp = temp->bestParent;     // Keep pushing onto the path the elements of the closed list, until start node is reached
			}
			else {
				break;		// Break if we reach the start position
			}
		}
	}
	return path;
}

std::vector<MapNode*> GameMap::getNeighbours(MapNode* node)
{
	node->neighbours.clear();
	/* Neighbour to Left */
	if (node->posX > 0) {
		int tileType = mapCosts[(node->posY * mapWidth) + (node->posX - 1)];
		if (tileType != 9) {		// Inaccessible neighbour
			MapNode* neighbour = &node->nodeData[(node->posY * mapWidth) + (node->posX - 1)];
			node->neighbours.emplace_back(neighbour);
		}
	}
	/* Neightbour to Right */
	if (node->posX < mapWidth - 2) {
		int tileType = mapCosts[((node->posY)*mapWidth) + (node->posX + 1)];
		if (tileType != 9) {
			MapNode* neighbour = &node->nodeData[(node->posY * mapWidth) + (node->posX + 1)];
			node->neighbours.emplace_back(neighbour);
		}
	}
	/* Upwards Neighbour */
	if (node->posY > 0) {
		int tileType = mapCosts[((node->posY - 1) * mapWidth) + node->posX];
		if (tileType != 9) {
			MapNode* neighbour = &node->nodeData[((node->posY - 1) * mapWidth) + node->posX];
			node->neighbours.emplace_back(neighbour);
		}
	}
	/* Downwards Neighbour */
	if (node->posY < mapHeight - 2) {
		int tileType = mapCosts[((node->posY + 1) * mapWidth) + node->posX];
		if (tileType != 9) {
			MapNode* neighbour = &node->nodeData[((node->posY + 1) * mapWidth) + node->posX];
			node->neighbours.emplace_back(neighbour);
		}
	}
	return node->neighbours;
}

