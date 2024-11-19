#include "item.h"

enum objectesAnims
{
	BOOST
};

item::item(int x, int y, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int tipus) {
	posX = x;
	posY = y;
	type = tipus;

	spritesheet.loadFromFile("images/objectes/objectes_256x512.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.03125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(BOOST, 8);
	sprite->addKeyframe(BOOST, glm::vec2((tipus-1)*0.0625, 0.5));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));
	map = tileMap;
}

void item::render() {
	sprite->render();
}

void item::update(int deltaTime) {
	if(!map->collisionMoveDownBubble(glm::ivec2(int(posX), int(posY)), glm::ivec2(16, 16), &posY)) posY += 1;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posX), float(tileMapDispl.y + posY)));
}

int item::checkPosX() {
	return posX;
}

int item::checkPosY() {
	return posY;
}

int item::checkType() {
	return type;
}