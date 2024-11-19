#include "bubbleManager.h"
#include <iostream>



bubbleManager::bubbleManager(vector<glm::ivec2> positions, vector<int> speeds, vector<int> sizes, vector<int> colors, const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map) {
	tileMapDispl = tileMapPos;
	sp = shaderProgram;
	mapa = map;
	int size = positions.size();
	for (int i = 0; i < size; ++i) {
		Bubble* novaBombolla = new Bubble(positions[i], speeds[i], sizes[i], colors[i], tileMapDispl, sp);
		novaBombolla->setTileMap(map);
		novaBombolla->setSize(sizes[i]);
		bubbles.push_back(novaBombolla);
	}
}

void bubbleManager::breakBubbleI(int i, bool animation) {
	// Reproducir el archivo de audio
	int a;
	if (animation) a = 1;
	else a = 0;
	int size = bubbles[i]->getSize();
	if (size < 3){
		Bubble *novaBombolla1 = new Bubble(glm::ivec2(bubbles[i]->getPosX(), bubbles[i]->getPosY()- bubbles[i]->getRadius()/2), 1, size + 1, bubbles[i]->getColor(), tileMapDispl, sp);
		novaBombolla1->setTileMap(mapa);
		novaBombolla1->setSize(size + 1);
		novaBombolla1->changeAnimation(a);
		bubbles.push_back(novaBombolla1);
		Bubble *novaBombolla2 = new Bubble(glm::ivec2(bubbles[i]->getPosX()-bubbles[i]->getRadius(), bubbles[i]->getPosY()-bubbles[i]->getRadius()/2), -1, size + 1, bubbles[i]->getColor(), tileMapDispl, sp);
		novaBombolla2->setTileMap(mapa);
		novaBombolla2->setSize(size + 1);
		novaBombolla2->changeAnimation(a);
		bubbles.push_back(novaBombolla2);
	}

	Bubble* novaBombolla3 = new Bubble(glm::ivec2(bubbles[i]->getPosX() - bubbles[i]->getRadius(), bubbles[i]->getPosY() - bubbles[i]->getRadius()), size, bubbles[i]->getColor(), sp);
	brokenBubbles.push_back(novaBombolla3);

	vector<Bubble*>::iterator it = bubbles.begin();
	for (int j = 0; j < i; ++j) ++it;
	bubbles[i]->~Bubble();
	bubbles.erase(it);


}

float bubbleManager::getPosXBubbleI(int i) {
	return bubbles[i]->getPosX();
}
float bubbleManager::getPosYBubbleI(int i) {
	return bubbles[i]->getPosY();
}
float bubbleManager::getRadiusBubbleI(int i) {
	return bubbles[i]->getRadius();
}



void bubbleManager::update(int deltaTime, bool move) {
	int size = bubbles.size();
	for (int i = 0; i < size; ++i) {
		bubbles[i]->update(deltaTime, move, false);
	}
	int  i = 0;
	while (i < brokenBubbles.size()) {
		if (brokenBubbles[i]->update(deltaTime, move, true)) {
			vector<Bubble*>::iterator it = brokenBubbles.begin();
			for (int j = 0; j < i; ++j) ++it;
			brokenBubbles[i]->~Bubble();
			brokenBubbles.erase(it);
		}
		 ++i;
	}
}

void bubbleManager::render() {
	for (int i = 0; i < brokenBubbles.size(); ++i) {
		brokenBubbles[i]->render();
	}
	int size = bubbles.size();
	for (int i = 0; i < size; ++i) {
		bubbles[i]->render();
	}
}

int bubbleManager::size() {
	return bubbles.size();
}

void bubbleManager::changeAnimation(int animation) {
	for(int i = 0; i < bubbles.size(); ++i) bubbles[i]->changeAnimation(animation);
}

int bubbleManager::getSizeI(int i) {
	return bubbles[i]->getSize();
}