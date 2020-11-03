/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#pragma once
#include "SimObject.h"
#include <vector>
namespace NCL {
	namespace CSC3222 {
		class MapNode
		{
		public:
			MapNode();
			~MapNode();
			MapNode* bestParent;
			int posX;
			int posY;
			float g;
			float h;
			float f;
			bool closed;
			std::vector<MapNode*> neighbours;
			bool operator<(const MapNode& n) const {
				return (f < n.f);		// Overriden comparator for f costs
			}
			MapNode* nodeData;
		};
	}
}


