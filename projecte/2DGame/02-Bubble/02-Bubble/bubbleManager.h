#pragma once
#include <vector>
#include "Bubble.h"
class bubbleManager
{
public:
	bubbleManager() {};
	bubbleManager(vector<glm::ivec2> positions, vector<int> speeds, vector<int> size, vector<int> colors, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map);
	float getPosXBubbleI(int i);
	float getPosYBubbleI(int i);
	float getRadiusBubbleI(int i);
	void breakBubbleI(int i, bool animation);
	void update(int deltaTime, bool move);
	void render();
	int size();
	void changeAnimation(int animation);
	int getSizeI(int i);

private:
	vector<Bubble*> bubbles, brokenBubbles;
	ShaderProgram sp;
	glm::ivec2 tileMapDispl;
	TileMap *mapa;
};

