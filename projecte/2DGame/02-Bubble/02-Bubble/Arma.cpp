#include <iostream>
#include "Arma.h"
#include <windows.h>
#pragma comment(lib, "winmm.lib")

enum ArmaAnims
{
	RISE, STICKYRISE, STICKED1, STICKED2, STICKED3
};


Arma::Arma(glm::ivec2 pos, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, bool s)
{
	stickedFrames = 0;
	sticky = s;
	sticked = false;
	posArma.y = pos.y;
	posArma.x = pos.x;
	spritesheet.loadFromFile("images/objectes/objectes_256x512.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 256), glm::vec2(0.0625, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);



	sprite->setAnimationSpeed(RISE, 8);
	sprite->addKeyframe(RISE, glm::vec2(0.375f, 0.f));
	sprite->addKeyframe(RISE, glm::vec2(0.3125f, 0.f));

	sprite->setAnimationSpeed(STICKYRISE, 8);
	sprite->addKeyframe(STICKYRISE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STICKYRISE, glm::vec2(0.0625f, 0.f));

	sprite->setAnimationSpeed(STICKED1, 8);
	sprite->addKeyframe(STICKED1, glm::vec2(0.125f, 0.f));

	sprite->setAnimationSpeed(STICKED2, 8);
	sprite->addKeyframe(STICKED2, glm::vec2(0.1875f, 0.f));

	sprite->setAnimationSpeed(STICKED3, 8);
	sprite->addKeyframe(STICKED3, glm::vec2(0.250f, 0.f));

	if (!sticky) sprite->changeAnimation(RISE);
	if (sticky) sprite->changeAnimation(STICKYRISE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(posArma.x), float(posArma.y)));
}

int Arma::update(int deltaTime)
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posArma.x), float(tileMapDispl.y + posArma.y)));
	sprite->update(deltaTime);
	if(!sticked) posArma.y-=3;
	else {
		if (stickedFrames < 108) sprite->changeAnimation(STICKED1);
		else if (stickedFrames < 168) sprite->changeAnimation(STICKED2);
		else if (stickedFrames < 228) sprite->changeAnimation(STICKED3);
		else return -1;
		stickedFrames++;
	}
	return 0;
}

void Arma::render() {
	sprite->render();
}

void Arma::setTileMap(TileMap *tm) {
	map = tm;
}

int Arma::checkCollision(){
	return map->collisionMoveUp(posArma, glm::ivec2(16, -1), &posArma.y);
}

float Arma::getPosX() {
	return float(posArma.x);
}

float Arma::getPosY() {
	return float(posArma.y);
}

void Arma::stick() {
	sticked = true;
}