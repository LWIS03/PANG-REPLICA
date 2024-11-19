#pragma once
#include "Sprite.h"
#include "TileMap.h"
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
class item
{
public:
	item(int x, int y, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int tipus);
	~item() {};
	void render();
	void update(int deltaTime);
	int checkPosX();
	int checkPosY();
	int checkType();

	
private:
	int posX, posY;
	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 tileMapDispl;
	TileMap* map;
	int type;
	
};

