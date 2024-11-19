#pragma once
#include "Sprite.h"
#include "TileMap.h"
class Arma
{

public:
	Arma(glm::ivec2 pos, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool s);
	~Arma() {}
	int update(int deltaTime);
	void render();
	void setTileMap(TileMap *tm);
	int checkCollision();
	float getPosX();
	float getPosY();
	void stick();
	void stick2();

private:
	int pos;
	Texture spritesheet;
	Sprite* sprite;
	glm::ivec2 tileMapDispl, posArma;
	TileMap *map;
	bool sticky, sticked;
	int stickedFrames;
};

