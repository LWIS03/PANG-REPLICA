#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Bubble.h"
#include "Game.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")




#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GRAVETAT 0.1



enum BubbleAnims
{
	BUBBLE, FLICKER, EXPLODE
};

Bubble::Bubble(glm::ivec2 position, int speed, int size, int color, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
	breakingTime = -1;
	velocitatY = -1;
	velocitatX = speed;
	bJumping = false;
	posX = position.x;
	posY = position.y;
	col = color,

	spritesheet.loadFromFile("images/bubbles/Sprites_Pompas_600x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
	vector<pair<float,float>> division = { {0.1066, 0.25}, {0.05333,0.125},{ 0.02666,0.0625},{0.0133,0.03125} };
	vector<float> pos_estatiques = { 0, 0.10666, 0.16, 0.18666 };
	vector<vector<float>> pos_animacio = { {0.213,0.32, 0.426, 0.533}, {0.64, 0.693, 0.74666, 0.8},{0.853,0.88,0.9066,0.93},{0.96, 0.973333, 0.986666,  0.986666} };
	vector<float> colors = { 0.f, 0.25f, 0.5f };
	vector<int> sizes = { 64, 32, 16, 8 };
	bubbleSize = sizes[size];
	sprite = Sprite::createSprite(glm::ivec2(sizes[size], sizes[size]), glm::vec2(division[size].first, division[size].second), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(BUBBLE, 8);
	sprite->addKeyframe(BUBBLE, glm::vec2(pos_estatiques[size], colors[color]));

	sprite->setAnimationSpeed(FLICKER, 8);
	sprite->addKeyframe(FLICKER, glm::vec2(pos_estatiques[size], colors[color]));
	sprite->addKeyframe(FLICKER, glm::vec2(0.5, 0.75));

	sprite->setAnimationSpeed(EXPLODE, 1);
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][0], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][1], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][2], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][3], colors[color]));


	sprite->changeAnimation(BUBBLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));


}

Bubble::Bubble(glm::ivec2 position, int size, int color, ShaderProgram& shaderProgram) {
	posX = position.x;
	posY = position.y;
	breakingTime = 15;

	spritesheet.loadFromFile("images/bubbles/Sprites_Pompas_600x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
	vector<pair<float, float>> division = { {0.1066, 0.25}, {0.05333,0.125},{ 0.02666,0.0625},{0.0133,0.03125} };
	vector<float> pos_estatiques = { 0, 0.10666, 0.16, 0.18666 };
	vector<vector<float>> pos_animacio = { {0.213,0.32, 0.426, 0.533}, {0.64, 0.693, 0.74666, 0.8},{0.853,0.88,0.9066,0.93},{0.96, 0.973333, 0.986666,  0.986666} };
	vector<float> colors = { 0.f, 0.25f, 0.5f };
	vector<int> sizes = { 64, 32, 16, 8 };
	bubbleSize = sizes[size];
	sprite = Sprite::createSprite(glm::ivec2(sizes[size], sizes[size]), glm::vec2(division[size].first, division[size].second), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(EXPLODE, 8);
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][0], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][1], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][2], colors[color]));
	sprite->addKeyframe(EXPLODE, glm::vec2(pos_animacio[size][3], colors[color]));

	sprite->changeAnimation(EXPLODE);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));
}

Bubble::~Bubble() {
	if (breakingTime == -1) {
		const char* command = "open \"..\\02-Bubble\\Audio\\Bombolla.wav\" type waveaudio alias explota";
		const char* play = "play explota from 0 notify";

		mciSendStringA(command, NULL, 0, NULL);
		mciSendStringA(play, NULL, 0, NULL);
	}
}

void Bubble::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	velocitatX = 1;
	velocitatY = 0;
	bJumping = false;
	spritesheet.loadFromFile("images/bubbles/Sprites_Pompas_256x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(BUBBLE, 8);
		sprite->addKeyframe(BUBBLE, glm::vec2(0.25f, 0.f));
		
	//sprite->changeAnimation(BUBBLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));
	//sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

int Bubble::update(int deltaTime, bool move, bool broken)
{
	if (broken) {
		sprite->update(deltaTime);
		breakingTime--;
		if (breakingTime == 0) return true;
		else return false;
	}
	else {
		if (move) {
			posY += velocitatY;
			posX += velocitatX;
			velocitatY += GRAVETAT;
		}
		posPlayer.y = int(posY);
		posPlayer.x = int(posX);
		//sprite->changeAnimation(FLICKER);
		sprite->update(deltaTime);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));

		/*if (bubbleSize == 64) {
			if (map->collisionMoveRightBubble(glm::ivec2(posX, posY), glm::ivec2(bubbleSize - 8, bubbleSize - 8)) || map->collisionMoveLeftBubble(posPlayer, glm::ivec2(bubbleSize + 32, bubbleSize + 32)))
			{
				velocitatX *= -1;
			}
			if (map->collisionMoveDownBubble(glm::ivec2(int(posX), int(posY)), glm::ivec2(bubbleSize - 8, bubbleSize - 8), &posPlayer.y))
			{
				if (sizeNum == 0) velocitatY = -5;
				if (sizeNum == 1) velocitatY = -4.33;
				if (sizeNum == 2) velocitatY = -3.66;
				if (sizeNum == 3) velocitatY = -3;
			}
			else if (map->collisionMoveUp(posPlayer, glm::ivec2(bubbleSize, bubbleSize), &posPlayer.y)) {
				velocitatY *= -1;
			}
		}
		else {
			if (map->collisionMoveRightBubble(glm::ivec2(posX, posY), glm::ivec2(bubbleSize, bubbleSize)) || map->collisionMoveLeftBubble(posPlayer, glm::ivec2(bubbleSize, bubbleSize)))
			{
				velocitatX *= -1;
			}
			if (map->collisionMoveDownBubble(glm::ivec2(int(posX), int(posY)), glm::ivec2(bubbleSize, bubbleSize), &posPlayer.y))
			{
				if (sizeNum == 0) velocitatY = -5;
				if (sizeNum == 1) velocitatY = -4.33;
				if (sizeNum == 2) velocitatY = -3.66;
				if (sizeNum == 3) velocitatY = -3;
			}
			else if (map->collisionMoveUp(posPlayer, glm::ivec2(bubbleSize, bubbleSize), &posPlayer.y)) {
				velocitatY *= -1;
			}
		}*/
		int radius;
		if (sizeNum == 0) radius = 28;
		if (sizeNum == 1) radius = 17;
		if (sizeNum == 2) radius = 9;
		if (sizeNum == 3) radius = 5;
		int collision = map->collisionBubble(glm::ivec2(int(posX+radius), int(posY+radius)), radius, &posPlayer.y);
		if (collision == 1) velocitatX *= -1;
		if (collision == 2) {
			if (posY+radius > 19 * 8) {
				if (sizeNum == 0) velocitatY = -5;
				if (sizeNum == 1) velocitatY = -4.33;
				if (sizeNum == 2) velocitatY = -3.66;
				if (sizeNum == 3) velocitatY = -3;
			}
			else velocitatY = -1;
		}
		if(collision == 3) velocitatY = 1;
	}
	return false;
}

void Bubble::render()
{
	sprite->render();
}

void Bubble::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Bubble::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	velocitatY = 0;
	posX = int(pos.x);
	posY = int(pos.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

float Bubble::getPosX() {
	return (posX + (float(bubbleSize) / 2.0));
}

float Bubble::getPosY() {
	return (posY + (float(bubbleSize) / 2.0));
}

float Bubble::getRadius() {
	return float(bubbleSize)/2.0;
}

void Bubble::setSize(int s){
	sizeNum = s;
}

int Bubble::getSize() {
	return sizeNum;
}

void Bubble::changeAnimation(int animation) {
	sprite->changeAnimation(animation);
}

int Bubble::getColor() {
	return col;
}