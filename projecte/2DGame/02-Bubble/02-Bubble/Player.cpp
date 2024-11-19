#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 3
float velocitatX = 0.2;
float velocitatY = -5;
float gravetat = 0.1;


enum PlayerAnims
{
	STAND, MOVE_LEFT, MOVE_RIGHT, SHOOT, DEAD, STAIRSUP, STAIRSDOWN, INSTAIRS
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	changeGodMode = 0;
	aboveStairs = false;
	godMode = false;
	stairs = false;
	inStairs = false;
	arma1Exists = false;
	arma1Exists = false;
	arma2Exists = false;
	boostDouble = false;
	boostSticky = false;
	boostStopTime = false;
	shootingFramesLeft = 0;
	dispars = 0;
	bJumping = false;
	shaderProgram1 = shaderProgram;
		spritesheet.loadFromFile("images/characters/Sprites_Personatge_256x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
		sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
		sprite->setNumberAnimations(8);

		sprite->setAnimationSpeed(STAND, 8);
		sprite->addKeyframe(STAND, glm::vec2(0.f, 0.375f));

		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.125f, 0.75f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.75f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.375f, 0.75f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.75f));

		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.125f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.375f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(SHOOT, 8);
		sprite->addKeyframe(SHOOT, glm::vec2(0.125f, 0.375f));


		sprite->setAnimationSpeed(DEAD, 8);
		sprite->addKeyframe(DEAD, glm::vec2(0.25f, 0.375f));

		sprite->setAnimationSpeed(STAIRSUP, 8);
		sprite->addKeyframe(STAIRSUP, glm::vec2(0.f, 0.125f));
		sprite->addKeyframe(STAIRSUP, glm::vec2(0.125f, 0.125f));
		sprite->addKeyframe(STAIRSUP, glm::vec2(0.25f, 0.125f));
		sprite->addKeyframe(STAIRSUP, glm::vec2(0.375f, 0.125f));

		sprite->setAnimationSpeed(STAIRSDOWN, 8);
		sprite->addKeyframe(STAIRSDOWN, glm::vec2(0.375f, 0.125f));
		sprite->addKeyframe(STAIRSDOWN, glm::vec2(0.25f, 0.125f));
		sprite->addKeyframe(STAIRSDOWN, glm::vec2(0.125f, 0.125f));
		sprite->addKeyframe(STAIRSDOWN, glm::vec2(0.f, 0.125f));

		sprite->setAnimationSpeed(INSTAIRS, 8);
		sprite->addKeyframe(STAIRSUP, glm::vec2(0.125f, 0.125f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + float(posPlayer.y)*0.1)));



	//const char* command = "open \"..\\02-Bubble\\Audio\\02 Stage 01-03 (Mt. Fuji).mp3\" type mpegvideo alias nivell";
	//const char* play = "play nivell from 0 notify";
	//mciSendStringA(command, NULL, 0, NULL);
	//mciSendStringA(play, NULL, 0, NULL);
}

void Player::update(int deltaTime)
{
	if (changeGodMode == 0) {
		if (Game::instance().getKey(GLFW_KEY_G)) {
			if (!godMode) godMode = true;
			else godMode = false;
			changeGodMode = 15;
		}
	}
	else changeGodMode--;
	if (boostStopTime) {
		if (framesStoped > 0) framesStoped--;
		if (framesStoped == 0) boostStopTime = false;
	}
	const char* filePath = "..\\02-Bubble\\Audio\\Dispar.wav";
	if (shootingFramesLeft > 0) shootingFramesLeft--;
	else if (!arma1Exists && Game::instance().getKey(GLFW_KEY_S) && !falling) {

		PlaySoundA(filePath, NULL, SND_FILENAME | SND_ASYNC);

		shootingFramesLeft = 10;
		sprite->changeAnimation(SHOOT);
		glm::ivec2 posArma = posPlayer;
		posArma.x += 2;
		arma = new Arma(posArma, tileMapDispl, shaderProgram1, boostSticky);
		arma->setTileMap(map);
		dispars++;
		arma1Exists = true;
		haSonat = false;
	}
	else if (arma1Exists && !arma2Exists && boostDouble && Game::instance().getKey(GLFW_KEY_S) && !falling) {
		PlaySoundA(filePath, NULL, SND_FILENAME | SND_ASYNC);
		shootingFramesLeft = 10;
		sprite->changeAnimation(SHOOT);
		glm::ivec2 posArma2 = posPlayer+glm::ivec2(2,0);
		posArma2.x += 1;
		arma2 = new Arma(posArma2, tileMapDispl, shaderProgram1, false);
		arma2->setTileMap(map);
		dispars++;
		arma2Exists = true;
	}
	else {
		if (Game::instance().getKey(GLFW_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 1;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x += 1;
				sprite->changeAnimation(STAND);
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_RIGHT))
		{
			if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 1;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
			{
				posPlayer.x -= 1;
				sprite->changeAnimation(STAND);
			}
		}
		else if (Game::instance().getKey(GLFW_KEY_UP) && stairs) {
			if (sprite->animation() != STAIRSUP) sprite->changeAnimation(STAIRSUP);
			posPlayer.y -= 1;
			inStairs = true;
		}
		else if (Game::instance().getKey(GLFW_KEY_DOWN) && (inStairs || aboveStairs)) {
			if (sprite->animation() != STAIRSDOWN) sprite->changeAnimation(STAIRSDOWN);
			posPlayer.y += 1;
			inStairs = true;
		}
		else if (inStairs) sprite->changeAnimation(STAIRSDOWN);
		else
		{
			sprite->changeAnimation(STAND);
		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
			}
		}
		else
		{
			if(!inStairs && !aboveStairs) posPlayer.y += FALL_STEP;
			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{
				//if(Game::instance().getKey(GLFW_KEY_UP))
				//{
				//	bJumping = true;
				//	jumpAngle = 0;
				//	startY = posPlayer.y;
				//}
			}
		}
	}
	if (arma1Exists) {
		if (arma->update(deltaTime) == -1) {
			arma->~Arma();
			dispars--;
			arma1Exists = false;
		}
		else{
			int colisioArma = arma->checkCollision();
			if (colisioArma != 0) {
				if (colisioArma == 2){
					arma->~Arma();
					dispars--;
					arma1Exists = false;
				}
				else {
					if (!boostSticky) {
						arma->~Arma();
						dispars--;
						arma1Exists = false;
					}
					else {
						if (!haSonat) {
							haSonat = true;
							const char* command = "open \"..\\02-Bubble\\Audio\\LapaSengancha.wav\" type waveaudio alias lapa";
							const char* play = "play lapa from 0 notify";
							mciSendStringA(command, NULL, 0, NULL);
							mciSendStringA(play, NULL, 0, NULL);
						}
						arma->stick();
					}
				}
			}
		}
	}
	if (arma2Exists) {
		arma2->update(deltaTime);
		if (arma2->checkCollision()) {
			arma2->~Arma();
			dispars--;
			arma2Exists = false;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite->update(deltaTime);
}


void Player::render()
{
	sprite->render();
}

void Player::renderArma()
{
	if (arma1Exists) arma->render();
	if (arma2Exists) arma2->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool collision(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {

	// temporary variables to set edges for testing
	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rx)         testX = rx;      // test left edge
	else if (cx > rx + rw) testX = rx + rw;   // right edge
	if (cy < ry)         testY = ry;      // top edge
	else if (cy > ry + rh) testY = ry + rh;   // bottom edge

	// get distance from closest edges
	float distX = cx - testX;
	float distY = cy - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		return true;
	}
	return false;
}

int Player::checkCollisionBubble(int cx, int cy, int radius, bool timeStoped) {
	
	if (arma2Exists){
		if (collision(float(cx), float(cy), float(radius), arma2->getPosX() + 8, arma2->getPosY(), 5.0, 256.0)) {
			arma2->~Arma();
			dispars--;
			arma2Exists = false;
			return 2;
		}
	}
	if (arma1Exists) {
		if (collision(float(cx), float(cy), float(radius), arma->getPosX() + 8, arma->getPosY(), 5.0, 256.0)) {
			arma->~Arma();
			dispars--;
			arma1Exists = false;
			return 2;
		}
	}
	if (collision(float(cx), float(cy), float(radius), float(posPlayer.x + 8), float(posPlayer.y + 8), 8.0, 8.0) && !timeStoped && !godMode) {

		return 1;
	}
	return 0;
}

int Player::checkCollisionObject(int xmin, int xmax, int ymin, int ymax, int boost) {
	//if (((posPlayer.x < xmax && posPlayer.x > xmax) || (posPlayer.x + 32 < xmax && posPlayer.x + 32 > xmax) || (posPlayer.x < xmin && posPlayer.x + 32 > xmax)) && ((posPlayer.y < ymax && posPlayer.y > ymax) || (posPlayer.y + 32 < ymax && posPlayer.y + 32 > ymax) || (posPlayer.y < ymin && posPlayer.y + 32 > ymax))) {
	if(posPlayer.x < xmax && posPlayer.x+32 > xmin && posPlayer.y < ymax && posPlayer.y+32 > ymin){

		const char* command = "open \"..\\02-Bubble\\Audio\\PillarItem.wav\" type waveaudio alias tengui";
		const char* play = "play tengui from 0 notify";
		mciSendStringA(command, NULL, 0, NULL);
		mciSendStringA(play, NULL, 0, NULL);

		if (boost == 1) {
			boostSticky = true;
			boostDouble = false;
			return 1;
		}
		if (boost == 2) {
			boostSticky = false;
			boostDouble = true;
			return 2;
		}
		if (boost == 3) {
			framesStoped = 240;
			boostStopTime = true;
			return 3;
		}
		else return 4;
	}
	return 0;
}

int Player::checkKeyObject() {
	if (Game::instance().getKey(GLFW_KEY_T)) {
		boostSticky = true;
		boostDouble = false;
		return 1;
	}
	if (Game::instance().getKey(GLFW_KEY_Y)) {
		boostSticky = false;
		boostDouble = true;
		return 2;
	}
	if (Game::instance().getKey(GLFW_KEY_U)) {
		if (!boostStopTime) {
			boostStopTime = true;
			framesStoped = 240;
		}
		return 3;
	}
	else return 0;
}

void Player::die(int deltaTime, bool firstTime, int waitFrames) {



	if (firstTime) {
		//spritesheet.loadFromFile("images/characters/Sprites_Personatge_256x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
		//sprite = NULL;
		sprite = Sprite::createSprite(glm::ivec2(64, 32), glm::vec2(0.25, 0.125), &spritesheet, &shaderProgram1);
		sprite->setNumberAnimations(5);
		sprite->setAnimationSpeed(DEAD, 8);
		sprite->addKeyframe(DEAD, glm::vec2(0.25f, 0.375f));
		posPlayerYDead = float(posPlayer.y);
		posPlayerXDead = float(posPlayer.x)-8;
		velocitatY = -5;
		sprite->changeAnimation(DEAD);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayerXDead), float(tileMapDispl.y + posPlayerYDead)));
	}
	if (waitFrames > 0) return;
	else {
		velocitatY += gravetat;
		posPlayerYDead += velocitatY;
		posPlayerXDead += velocitatX;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayerXDead), float(tileMapDispl.y + posPlayerYDead)));
	}
}

void Player::activateBoostDouble() {
	boostDouble = true;
}

bool Player::checkBoostStopTime() {
	return boostStopTime;
}

void Player::checkCollisionStairs(vector<glm::vec2> escaleras) {
	stairs = false;
	for (int i = 0; i < escaleras.size() && !stairs; ++i) {
		if ((posPlayer.x >= escaleras[i].x * 8) && ((posPlayer.x + 16) <= ((escaleras[i].x * 8) + 32))){
			if (((posPlayer.y + 16 <= escaleras[i].y * 8 + 16) && (posPlayer.y + 16 >= escaleras[i].y * 8 - 16))) {
				stairs = true;
			}
			else {
				aboveStairs = true;
			}
		}
		else {
			stairs = false;
			inStairs = false;
			aboveStairs = false;
		}
	}
}

bool Player::checkGodmode() {
	return godMode;
}