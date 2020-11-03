/* Created by Samuel Buzz Appleby
			03/05/20
			170348069			*/
#include "MapNode.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"

using namespace NCL;
using namespace CSC3222;

MapNode::MapNode()
{
	bestParent = nullptr;
	posX = 0;
	posY = 0;
	g = 0;
	h = 0;
	f = 0;
	closed = false;
	neighbours = {};
	nodeData = nullptr;
}

MapNode::~MapNode()
{
}




