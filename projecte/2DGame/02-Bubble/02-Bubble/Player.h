#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Arma.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void renderArma();
	int checkCollisionBubble(int cx, int cy, int radius, bool timeStoped);
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void die(int deltaTime, bool firstTime, int waitframes);
	void activateBoostDouble();
	bool checkBoostStopTime();
	int checkCollisionObject(int xmin, int xmax, int ymin, int ymax, int boost);
	int checkKeyObject();
	void checkCollisionStairs(vector<glm::vec2>);
	bool checkGodmode();

private:
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Arma* arma, *arma2;
	bool arma1Exists, arma2Exists;
	ShaderProgram shaderProgram1;
	int dispars;
	int shootingFramesLeft;
	float posPlayerXDead, posPlayerYDead;
	bool boostDouble, boostSticky, boostStopTime;
	int framesStoped;
	bool inStairs, stairs, aboveStairs, falling;
	bool godMode;
	int changeGodMode;
	bool haSonat;


};


#endif // _PLAYER_INCLUDE


