#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Player.h"
#include "Bubble.h"
#include "time.h"
#include "item.h"
#include "windows.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 21


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

enum mapAnims {
	FONDO
};

enum vidaAnims {
	VIDA
};
enum hud {
	HUD
};

enum arma {
	STICKY, DOBLE, NOTHING
};

enum start {
	START
};

enum instr {
	INSTRUCCIONS
};

enum esc {
	ESCALERAS
};

enum credits {
	CREDITS
};

enum godmode {
	CROWN, NOTHING2
};

enum breakingBlocks {
	BLOCK, BREAK, NOTHING3
};




void Scene::init(int level)
{
	initShaders();
	if(level == 0)
		score = 0;
	lastBubble = { -1,-1 };
	lvl = level;
	items = {};
	roundTime = 6000;
	deadtime = -1;
	waitFramesBeforeRound = 60;
	srand(time(NULL));
	stopTime = false;
	dead = false;
	vector<string> levels = { "levels/level01.txt" , "levels/level02.txt" , "levels/level03.txt" };
	map = TileMap::createTileMap(levels[level], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	if(level == 0) vides = 2;

	//start scene
	spritesheetStart.loadFromFile("images/start/start.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteStart = Sprite::createSprite(glm::ivec2(384, 240), glm::vec2(0.5, 1), &spritesheetStart, &texProgram);
	spriteStart->setNumberAnimations(1);
	spriteStart->setAnimationSpeed(START, 2);

	// Primer fotograma
	spriteStart->addKeyframe(START, glm::vec2(0.5f, 0.f)); // Posición del primer fotograma

	// Segundo fotograma
	spriteStart->addKeyframe(START, glm::vec2(0.f, 0.f)); // Posición del segundo fotograma

	spriteStart->setPosition(glm::vec2(0, 0));
	spriteStart->changeAnimation(START);



	//instruccions scene
	spritesheetInstruccions.loadFromFile("images/instruccions/instruccions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteInstruccions = Sprite::createSprite(glm::ivec2(384, 240), glm::vec2(1, 1), &spritesheetInstruccions, &texProgram);
	spriteInstruccions->setNumberAnimations(1);
	spriteInstruccions->setAnimationSpeed(INSTRUCCIONS, 2);
	spriteInstruccions->addKeyframe(INSTRUCCIONS, glm::vec2(0.f, 0.f)); // Posición del segundo fotograma
	spriteInstruccions->setPosition(glm::vec2(0, 0));
	spriteInstruccions->changeAnimation(INSTRUCCIONS);

	//spriteStart->update(0);
	spriteEscaleras = {};
	escaleras = map->escaleras();
	spritesheetEscaleras.loadFromFile("images/objectes/objectes_256x512.png", TEXTURE_PIXEL_FORMAT_RGBA);
	for (int i = 0; i < escaleras.size(); ++i) {
		int escaleraX = escaleras[i].x;
		int escaleraY = escaleras[i].y;
		Sprite* spriteEscalera = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(0.125, 0.03125), &spritesheetArmasHud, &texProgram);
		spriteEscalera->setNumberAnimations(3);
		spriteEscalera->setAnimationSpeed(ESCALERAS, 8);
		spriteEscalera->addKeyframe(ESCALERAS, glm::vec2(0, 0.5 + 0.03125));
		spriteEscalera->setPosition(glm::vec2(8 * escaleraX, 8 * escaleraY));
		spriteEscalera->changeAnimation(ESCALERAS);
		spriteEscaleras.push_back(spriteEscalera);
	}

	if (level == 0) {
		spritesheetVides.loadFromFile("images/characters/Sprites_Personatge_256x256.png", TEXTURE_PIXEL_FORMAT_RGBA);
		Sprite* spriteV1 = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheetVides, &texProgram);
		Sprite* spriteV2 = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.0625), &spritesheetVides, &texProgram);
		spriteVides = { spriteV1, spriteV2 };
		spriteVides[0]->setNumberAnimations(1);
		spriteVides[1]->setNumberAnimations(1);
		spriteVides[0]->setAnimationSpeed(VIDA, 8);
		spriteVides[1]->setAnimationSpeed(VIDA, 8);
		spriteVides[0]->addKeyframe(VIDA, glm::vec2(0.5f, 0.125f));
		spriteVides[1]->addKeyframe(VIDA, glm::vec2(0.5f, 0.125f));
		spriteVides[0]->changeAnimation(VIDA);
		spriteVides[1]->changeAnimation(VIDA);
		spriteVides[0]->setPosition(glm::vec2(16, 224));
		spriteVides[1]->setPosition(glm::vec2(32, 224));
	}
	if (level == 0) manager = new bubbleManager({ glm::vec2(32,32) }, { 1 }, { 0 }, {0}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	if (level == 1) manager = new bubbleManager({ glm::vec2(32,32), glm::vec2(160,16) }, { -1, 1 }, { 0, 1 }, {1,1}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	if (level == 2) manager = new bubbleManager({ glm::vec2(16,80), glm::vec2(160,16), glm::vec2(160,80) }, { -1, 1, -1 }, { 1, 1, 1 }, {2,2,2}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	spritesheetFondo.loadFromFile("images/fondos/fondos_384x624.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteFondo = Sprite::createSprite(glm::ivec2(384, 208), glm::vec2(1,0.3333), &spritesheetFondo, &texProgram);
	spriteFondo->setNumberAnimations(1);
	spriteFondo->setAnimationSpeed(FONDO, 8);
	spriteFondo->addKeyframe(FONDO, glm::vec2(0.f, level*0.3333f));
	spriteFondo->changeAnimation(FONDO);
	spriteFondo->setPosition(glm::vec2(0, 0));

	spritesheetHud.loadFromFile("images/hud/hud_32x1152.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteHud = Sprite::createSprite(glm::ivec2(384, 32), glm::vec2(1.0/3.0, 1), &spritesheetHud, &texProgram);
	spriteHud->setNumberAnimations(1);
	spriteHud->setAnimationSpeed(HUD, 8);
	spriteHud->addKeyframe(HUD, glm::vec2(1.0/3.0*level, 0.f));
	spriteHud->changeAnimation(HUD);
	spriteHud->setPosition(glm::vec2(0, 207));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;

	spritesheetArmasHud.loadFromFile("images/objectes/objectes_256x512.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteArmasHud = Sprite::createSprite(glm::ivec2(16,16), glm::vec2(0.0625, 0.03125), &spritesheetArmasHud, &texProgram);
	spriteArmasHud->setNumberAnimations(3);
	spriteArmasHud->setAnimationSpeed(STICKY, 8);
	spriteArmasHud->addKeyframe(STICKY, glm::vec2(0, 0.5));
	spriteArmasHud->setAnimationSpeed(DOBLE, 8);
	spriteArmasHud->addKeyframe(DOBLE, glm::vec2(0.0625, 0.5));
	spriteArmasHud->setAnimationSpeed(NOTHING, 8);
	spriteArmasHud->addKeyframe(NOTHING, glm::vec2(10*0.0625, 0.5));
	spriteArmasHud->changeAnimation(NOTHING);
	spriteArmasHud->setPosition(glm::vec2(112, 224));
	spriteGodmode = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.0625, 0.03125), &spritesheetArmasHud, &texProgram);
	spriteGodmode->setNumberAnimations(2);
	spriteGodmode->setAnimationSpeed(CROWN, 8);
	spriteGodmode->addKeyframe(CROWN, glm::vec2(0.0625 *9, 0.5));
	spriteGodmode->setAnimationSpeed(NOTHING2, 8);
	spriteGodmode->addKeyframe(NOTHING2, glm::vec2(0.0625*10, 0.5));
	spriteGodmode->setPosition(glm::vec2(360, 216));
	spriteGodmode->changeAnimation(NOTHING2);


	//instruccions scene
	spritesheetCredits.loadFromFile("images/start/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spriteCredits = Sprite::createSprite(glm::ivec2(382, 240), glm::vec2(0.5, 1), &spritesheetCredits, &texProgram);
	spriteCredits->setNumberAnimations(1);
	spriteCredits->setAnimationSpeed(CREDITS, 2);
	spriteCredits->addKeyframe(CREDITS, glm::vec2(0.f, 0.f)); // Posición del segundo fotograma
	spriteCredits->addKeyframe(CREDITS, glm::vec2(0.5f, 0.f)); // Posición del segundo fotograma
	spriteCredits->setPosition(glm::vec2(0, 0));
	spriteCredits->changeAnimation(CREDITS);

	spritesheetNumbers.loadFromFile("images/numbers/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheetScore.loadFromFile("images/numbers/score_160x8.png", TEXTURE_PIXEL_FORMAT_RGBA);


	if(level == 2) spritesheetBreakBlocks.loadFromFile("images/blocks/breakBlock.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else  spritesheetBreakBlocks.loadFromFile("images/blocks/breakBlock2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	breakBlocks = {};
	vector<pair<int, int>> bb = map->checkBreakBlocks();
	for (pair<int, int> b : bb) {
		Sprite* breakBlock;
		if (level == 1) breakBlock = Sprite::createSprite(glm::ivec2(32, 8), glm::vec2(1.0 / 6.0, 1), &spritesheetBreakBlocks, &texProgram);
		else breakBlock = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1.0 / 6.0, 1), &spritesheetBreakBlocks, &texProgram);
		breakBlock->setNumberAnimations(3);
		breakBlock->setAnimationSpeed(BLOCK, 2);
		breakBlock->addKeyframe(BLOCK, glm::vec2(0.f, 0.f)); // Posición del segundo fotograma
		breakBlock->setAnimationSpeed(NOTHING3, 2);
		breakBlock->addKeyframe(NOTHING3, glm::vec2(5.0 / 6.0f, 0.f)); // Posición del segundo fotograma
		breakBlock->setAnimationSpeed(BREAK, 8);
		breakBlock->addKeyframe(BREAK, glm::vec2(1.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(2.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(3.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(4.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->setPosition(glm::vec2(8*b.first, 8*b.second));
		breakBlock->changeAnimation(BLOCK);
		breakBlocks.push_back({ {b.first, b.second},breakBlock });
	}
}

void Scene::revive() {
	vector<string> levels = { "levels/level01.txt" , "levels/level02.txt" , "levels/level03.txt" };
	map = TileMap::createTileMap(levels[lvl], glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	breakBlocks = {};
	vector<pair<int, int>> bb = map->checkBreakBlocks();
	for (pair<int, int> b : bb) {
		Sprite* breakBlock;
		if (lvl == 1) breakBlock = Sprite::createSprite(glm::ivec2(32, 8), glm::vec2(1.0 / 6.0, 1), &spritesheetBreakBlocks, &texProgram);
		else breakBlock = Sprite::createSprite(glm::ivec2(24, 8), glm::vec2(1.0 / 6.0, 1), &spritesheetBreakBlocks, &texProgram);
		breakBlock->setNumberAnimations(3);
		breakBlock->setAnimationSpeed(BLOCK, 2);
		breakBlock->addKeyframe(BLOCK, glm::vec2(0.f, 0.f)); // Posición del segundo fotograma
		breakBlock->setAnimationSpeed(NOTHING3, 2);
		breakBlock->addKeyframe(NOTHING3, glm::vec2(5.0 / 6.0f, 0.f)); // Posición del segundo fotograma
		breakBlock->setAnimationSpeed(BREAK, 8);
		breakBlock->addKeyframe(BREAK, glm::vec2(1.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(2.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(3.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->addKeyframe(BREAK, glm::vec2(4.0 / 6.0, 0.f)); // Posición del segundo fotograma
		breakBlock->setPosition(glm::vec2(8 * b.first, 8 * b.second));
		breakBlock->changeAnimation(BLOCK);
		breakBlocks.push_back({ {b.first, b.second},breakBlock });
	}
	lastBubble = { -1,-1 };
	items = {};
	roundTime = 6000;
	spriteArmasHud->changeAnimation(NOTHING);
	deadtime = -1;
	waitFramesBeforeRound = 60;
	srand(time(NULL));
	stopTime = false;
	dead = false;
	initShaders();
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	if (lvl == 0) manager = new bubbleManager({ glm::vec2(32,32) }, { 1 }, { 0 }, {0}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	if (lvl == 1) manager = new bubbleManager({ glm::vec2(32,32), glm::vec2(160,16) }, { -1, 1 }, { 0, 1 }, {1,1}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	if (lvl == 2) manager = new bubbleManager({ glm::vec2(16,80), glm::vec2(160,16), glm::vec2(160,80) }, { -1, 1, -1 }, { 1, 1, 1 }, {2,2,2}, glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
}

int Scene::update(int deltaTime, int escena)
{
	spriteCredits->update(deltaTime);
	if (escena == 0) {
		spriteStart->update(deltaTime);
	}
	else if (escena == 1)
		spriteInstruccions->update(deltaTime);
	else if(escena != 3){
		if (manager->size() == 0) {
			return -1;
		}
		if (waitFramesBeforeRound != 0) {
			waitFramesBeforeRound--;
			return 0;
		}
		for (int i = 0; i < spriteEscaleras.size(); ++i) {
			spriteEscaleras[i]->update(deltaTime);
		}
		player->checkCollisionStairs(escaleras);
		if (deadtime != 0) deadtime--;
		else if (vides == -1) {

			return 1;
		}
		else revive();
		stopTime = player->checkBoostStopTime();
		if(Game::instance().getKey(GLFW_KEY_U)) {
			stopTime = true;
			stopTimeFrames = 240;
		}
		currentTime += deltaTime;
		for (int i = 0; i < items.size(); ++i) {
			int posObjX = items[i]->checkPosX();
			int posObjY = items[i]->checkPosY();
			if (player->checkCollisionObject(posObjX, posObjX + 16, posObjY, posObjY + 16, items[i]->checkType()) > 0) {
				vector<item*>::iterator it = items.begin();
				for (int j = 0; j < i; ++j) ++it;
				int changeAnimation = items[i]->checkType();
				if (changeAnimation == 1) spriteArmasHud->changeAnimation(STICKY);
				if (changeAnimation == 2) spriteArmasHud->changeAnimation(DOBLE);
				if (changeAnimation > 3 && changeAnimation < 6) {
					addScore(6, posObjX, posObjY);
					score += 1000;
				}
				if (changeAnimation > 5) {
					addScore(9, posObjX, posObjY);
					score += 2000;
				}
				items[i]->~item();
				items.erase(it);
				if(changeAnimation == 3) stopTimeFrames = 240;
			}
		}
		if (player->checkKeyObject() == 1) spriteArmasHud->changeAnimation(STICKY);
		if (player->checkKeyObject() == 2) spriteArmasHud->changeAnimation(DOBLE);

		if (dead) {		//quan s'ha mort
			player->die(deltaTime, justDead, waitDeadFrames);
			justDead = false;
			waitDeadFrames--;
		}
		else{			//quan no s'ha mort	
			if (!stopTime) {	//Quan no s'ha parat el temps
				manager->changeAnimation(0);
				manager->update(deltaTime, true);
				for (int i = 0; i < items.size(); ++i) items[i]->update(deltaTime);
				for (Sprite* v : spriteVides) {
					v->update(deltaTime);
				}
			}
			else manager->update(deltaTime, false);//quan s'ha parat el temps
			vector<pair<int, int>> bb = map->checkBreakBlocks();
			int i = 0;
			while(i < breakBlocks.size()) {
				bool found = false;
				for (int j = 0; j < bb.size() && !found; ++j) {
					if (bb[j].first == breakBlocks[i].first.first && bb[j].second == breakBlocks[i].first.second) {
						found = true;
					}
				}
				if (!found) {
					vector<pair<pair<int, int>, Sprite*>>::iterator it = breakBlocks.begin();
					for (int k = 0; k < i; ++k) ++it;
					breakBlocks[i].second->changeAnimation(BREAK);
					breakingBlocks.push_back({ 30, breakBlocks[i].second });
					breakBlocks.erase(it);
				}
				else i++;
			}
			if (roundTime == 0) {
				
				justDead = true;
				dead = true;
				waitDeadFrames = 60;
				vides--;
				deadtime = 180;
				if (vides > 0) {
					asonat1 = false;
					asonat2 = false;
					asonat3 = false;
					mciSendStringA("stop corre", NULL, 0, NULL);
					mciSendStringA("stop corre2", NULL, 0, NULL);
					mciSendStringA("stop nivell", NULL, 0, NULL);
					const char* command = "open \"..\\02-Bubble\\Audio\\16 [SFX] Sense temp.mp3\" type mpegvideo alias mor";
					const char* play = "play mor from 0 notify";
					mciSendStringA(command, NULL, 0, NULL);
					mciSendStringA(play, NULL, 0, NULL);
					spriteVides.pop_back();
				}
				else {
					asonat1 = false;
					asonat2 = false;
					asonat3 = false;
					mciSendStringA("stop corre", NULL, 0, NULL);
					mciSendStringA("stop corre2", NULL, 0, NULL);
					mciSendStringA("stop nivell", NULL, 0, NULL);
					const char* command = "open \"..\\02-Bubble\\Audio\\19 Game Over.mp3\" type mpegvideo alias rip";
					const char* play = "play rip from 0 notify";
					mciSendStringA(command, NULL, 0, NULL);
					mciSendStringA(play, NULL, 0, NULL);
				}
			}
			roundTime--;
			if (stopTimeFrames == 90) manager->changeAnimation(1);
			else if (stopTimeFrames > 90) manager->changeAnimation(0);
			stopTimeFrames--;
			spriteFondo->update(deltaTime);
			for (int i = 0; i < items.size(); ++i) items[i]->update(deltaTime);
			player->update(deltaTime);
			for(int i = 0; i < breakBlocks.size(); ++i) breakBlocks[i].second->update(deltaTime);
			int bBlock = 0;
			while(bBlock < breakingBlocks.size()) {
				breakingBlocks[bBlock].second->update(deltaTime);
				if (breakingBlocks[bBlock].first == 0) {
					vector<pair<int, Sprite*>>::iterator it = breakingBlocks.begin();
					for (int k = 0; k < bBlock; ++k) ++it;
					breakingBlocks.erase(it);
				}
				else {
					breakingBlocks[bBlock].first--;
					bBlock++;
				}
			}
			int size = manager->size();
			bool hasCollided = false;
			for (int i = 0; i < size && !hasCollided; ++i) {
				int colisio = player->checkCollisionBubble(manager->getPosXBubbleI(i), manager->getPosYBubbleI(i), manager->getRadiusBubbleI(i), false);
				if (colisio == 2) {
					hasCollided = true;

					//generacio de objectes
					int random = rand() % 10 + 1;
					int randomObject = rand() % 9 + 1;
					if (random < 3) {
						item* item1 = new item(manager->getPosXBubbleI(i), manager->getPosYBubbleI(i), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map, randomObject);
						items.push_back(item1);
					}

					//puntuacio
					vector<int> bubbleScore = { 50, 100, 150, 200 };
					vector<vector<int>> scorePos = { {0,0,1,3}, {0,1, 3, 5}, {0,2, 4, 7}, {0,3, 5, 8} };
					int bSize = manager->getSizeI(i);
					if (lastBubble.x != -1 && lastBubble.x == bSize) {
						if (lastBubble.y < 3) lastBubble.y++;
						if (lastBubble.y != -1) addScore(scorePos[bSize][lastBubble.y], manager->getPosXBubbleI(i), manager->getPosYBubbleI(i));
					}
					else lastBubble = { bSize , 0};
					manager->breakBubbleI(i, (stopTime && (stopTimeFrames <= 90)));
					score += bubbleScore[lastBubble.x] * pow(2, lastBubble.y);
					return 0;
				}
				else if (colisio == 1 && !stopTime) {
					justDead = true;
					dead = true;
					waitDeadFrames = 60;
					vides--;
					if (vides >= 0) {
						asonat1 = false;
						asonat2 = false;
						asonat3 = false;
						mciSendStringA("stop nivell", NULL, 0, NULL);
						mciSendStringA("stop corre", NULL, 0, NULL);
						mciSendStringA("stop corre2", NULL, 0, NULL);
						const char* command = "open \"..\\02-Bubble\\Audio\\16 [SFX] Sense temp.mp3\" type mpegvideo alias mor";
						const char* play = "play mor from 0 notify";
						mciSendStringA(command, NULL, 0, NULL);
						mciSendStringA(play, NULL, 0, NULL);
						deadtime = 180;
						spriteVides.pop_back();
					}
					else {
						asonat1 = false;
						asonat2 = false;
						asonat3 = false;
						mciSendStringA("stop nivell", NULL, 0, NULL);
						mciSendStringA("stop corre", NULL, 0, NULL);
						mciSendStringA("stop corre2", NULL, 0, NULL);
						const char* command = "open \"..\\02-Bubble\\Audio\\19 Game Over.mp3\" type mpegvideo alias rip";
						const char* play = "play rip from 0 notify";
						mciSendStringA(command, NULL, 0, NULL);
						mciSendStringA(play, NULL, 0, NULL);
						deadtime = 180;
					}
				}
			}
		}
	}
	return 0;
	
}

void Scene::render(int escena)
{
	if(escena == 0)spriteStart->render();
	if (escena == 1)spriteInstruccions->render();
	if (escena == 3)spriteCredits->render();
	if (escena == 2) {
		spriteFondo->render();
		player->renderArma();
		for (int i = 0; i < breakBlocks.size(); ++i) breakBlocks[i].second->render();
		for (int i = 0; i < breakingBlocks.size(); ++i) breakingBlocks[i].second->render();
		for (int i = 0; i < items.size(); ++i) items[i]->render();
		spriteHud->setPosition(glm::vec2(0, 210));
		spriteHud->render();
		spriteHud->setPosition(glm::vec2(0, 207));
		spriteHud->render();
		if (player->checkGodmode()) spriteGodmode->changeAnimation(CROWN);
		else spriteGodmode->changeAnimation(NOTHING2);
		spriteGodmode->render();

		int newScore = score;
		int numberPos = 0;
		while (newScore != 0) {
			float newNumber = newScore % 10;
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(newNumber * 0.1, 0));
			ss->setPosition(glm::ivec2(187 -numberPos, 224));
			ss->changeAnimation(0);
			ss->render();
			numberPos += 8;
			newScore /= 10;
		}
		if (score == 0) {
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(0 * 0.1, 0));
			ss->setPosition(glm::ivec2(187, 224));
			ss->changeAnimation(0);
			ss->render();
		}

		int newTime = roundTime / 60;

		if (newTime == 98 && !asonat1) {
			asonat3 = false;
			asonat1 = true;
			mciSendStringA("stop corre2", NULL, 0, NULL);
			mciSendStringA(command, NULL, 0, NULL);
			mciSendStringA(play, NULL, 0, NULL);

		}


		else if (newTime == 50 && !asonat2) {
			asonat1 = false;
			asonat2 = true;
			mciSendStringA("stop nivell", NULL, 0, NULL);
			mciSendStringA(command1, NULL, 0, NULL);
			mciSendStringA(play1, NULL, 0, NULL);
		}

		else if (newTime == 20 && !asonat3) {
			asonat2 = false;
			asonat3 = true;
			mciSendStringA("stop corre", NULL, 0, NULL);
			mciSendStringA(command2, NULL, 0, NULL);
			mciSendStringA(play2, NULL, 0, NULL);

		}

		if (newTime < 10) {
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(0, 0));
			ss->setPosition(glm::ivec2(219, 224));
			ss->changeAnimation(0);
			ss->render();
		}
		if (newTime < 100) {
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(0, 0));
			ss->setPosition(glm::ivec2(211, 224));
			ss->changeAnimation(0);
			ss->render();
		}
		if (newTime <= 0) {
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(0, 0));
			ss->setPosition(glm::ivec2(227, 224));
			ss->changeAnimation(0);
			ss->render();
		}
		numberPos = 0;
		while (newTime != 0) {
			float newNumber = newTime % 10;
			Sprite* ss = Sprite::createSprite(glm::ivec2(8, 16), glm::vec2(0.1, 1), &spritesheetNumbers, &texProgram);
			ss->setNumberAnimations(1);
			ss->addKeyframe(0, glm::vec2(newNumber * 0.1, 0));
			ss->setPosition(glm::ivec2(227 - numberPos, 224));
			ss->changeAnimation(0);
			ss->render();
			numberPos += 8;
			newTime /= 10;
		}
	}

	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);


	if (escena == 2) {
		map->render();
		for (int i = 0; i < spriteEscaleras.size(); ++i) spriteEscaleras[i]->render();
		player->render();

		int scoreNum = 0;
		while (scoreNum < spriteScore.size()) {
			spriteScore[scoreNum].second->render();
			spriteScore[scoreNum].first--;
			if (spriteScore[scoreNum].first == 0) {
				vector<pair<int, Sprite*>>::iterator it = spriteScore.begin();
				for (int j = 0; j < scoreNum; ++j) ++it;
				spriteScore.erase(it);
			}
			else scoreNum++;
		}

		//bombolla->render();

		manager->render();
		for (Sprite* v : spriteVides) {
			v->render();
		}
		spriteArmasHud->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


void Scene::addScore(int i, int x, int y) {
	Sprite* ss = Sprite::createSprite(glm::ivec2(16, 8), glm::vec2(0.1, 1), &spritesheetScore, &texProgram);
	ss->setNumberAnimations(1);
	ss->addKeyframe(0, glm::vec2(i * 0.1, 0));
	ss->setPosition(glm::ivec2(x, y));
	ss->changeAnimation(0);
	ss->render();
	spriteScore.push_back({120,ss});
}

void Scene::canviaBoolSoroll() {
	asonat1 = false;
	asonat2 = false;
	asonat3 = false;
}
