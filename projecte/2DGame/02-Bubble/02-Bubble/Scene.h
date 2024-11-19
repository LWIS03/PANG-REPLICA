#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"
#include "bubbleManager.h"
#include <vector>
#include "item.h"

class Player;
class Bubble;

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level);
	void revive();
	int update(int deltaTime, int escena);
	void render(int escena);
	void addScore(int i, int x, int y);
	void canviaBoolSoroll();

private:
	void initShaders();

private:
	//pantalla start
	Texture spritesheetStart;
	Sprite* spriteStart;

	//pantalla instruccions
	Texture spritesheetInstruccions;
	Sprite* spriteInstruccions;

	//game
	TileMap *map;
	Player* player;
	Bubble* bombolla;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	Sprite *spriteFondo, * spriteHud, *spriteArmasHud, *spriteGodmode;
	vector<Sprite*> spriteVides, spriteEscaleras;
	vector<pair<int, Sprite*>> spriteScore;
	Texture spritesheetFondo, spritesheetVides, spritesheetHud, spritesheetArmasHud, spritesheetEscaleras, spritesheetNumbers, spritesheetScore; //per la textura del fons
	vector<pair<pair<int,int>,Sprite*>> breakBlocks;
	vector<pair<int, Sprite*>> breakingBlocks;
	Texture spritesheetBreakBlocks;
	int vides;
	bubbleManager *manager;
	bool dead, justDead;
	int waitDeadFrames;
	bool stopTime;
	vector<item*> items;
	int waitFramesBeforeRound, deadtime;
	int stopTimeFrames;
	int animation;
	vector<glm::vec2> escaleras;
	int score;
	glm::vec2 lastBubble;
	int roundTime;
	int lvl;

	bool asonat1 = false;
	bool asonat2 = false;
	bool asonat3 = false;


	//pantalla credits
	Texture spritesheetCredits;
	Sprite* spriteCredits;

	const char* command = "open \"..\\02-Bubble\\Audio\\02 Stage 01-03 (Mt. Fuji).mp3\" type mpegvideo alias nivell";
	const char* play = "play nivell from 0 notify";

	const char* command1 = "open \"..\\02-Bubble\\Audio\\14 Hurry Up ! (Level 1).mp3\" type mpegvideo alias corre";
	const char* play1 = "play corre from 0 notify";

	const char* command2 = "open \"..\\02-Bubble\\Audio\\15 Hurry Up !! (Level 2).mp3\" type mpegvideo alias corre2";
	const char* play2 = "play corre2 from 0 notify";

};


#endif // _SCENE_INCLUDE

