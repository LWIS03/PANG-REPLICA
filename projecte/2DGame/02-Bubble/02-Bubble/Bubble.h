#pragma once


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Bubble
{

public:
	Bubble() {};
	Bubble(glm::ivec2 position, int speed, int size, int color, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	Bubble(glm::ivec2 position, int size, int color,  ShaderProgram& shaderProgram);
	~Bubble();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	int update(int deltaTime, bool move, bool broken);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	float getPosX();
	float getPosY();
	float getRadius();
	void setSize(int s);
	int getSize();
	void changeAnimation(int animation);
	int getColor();
	
private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	float posX, posY;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	float velocitatX, velocitatY;
	int bubbleSize;
	int sizeNum;
	int breakingTime;
	int col;

};



