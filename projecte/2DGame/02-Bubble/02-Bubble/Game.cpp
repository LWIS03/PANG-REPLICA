#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include <iostream>
#include <windows.h>


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	start = true;
	level = 0;
	scene.init(level);
	escena = 0;
	instruccionsTime = -1;
}

bool Game::update(int deltaTime)
{
	if (Game::instance().getKey(GLFW_KEY_S) && (escena == 0 || (escena == 1 && instruccionsTime <= 540))) {	//Press S to start
		if(escena == 0) instruccionsTime = 600;
		escena++;
	}
	if (Game::instance().getKey(GLFW_KEY_S) && escena == 3 && instruccionsTime <= 0) {	//Press S to start
		escena = 0;
		level = 0;
		scene.init(level);
	}
	
	if (instruccionsTime > 0) {	//Update instruccionsTime
		instruccionsTime--;
	}
	else if(instruccionsTime == 0 && escena == 1) escena = 2;	//Change 
	int valor = scene.update(deltaTime, escena);
	if (valor == -1) {
		level++;
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		if (level == 3) {
			level = 0;
			instruccionsTime = 90;
			escena = 3;
		}
		scene.init(level);
	}
	else if (valor == 1) {
		level = 0;
		escena = 3;
		instruccionsTime = 90;
	}
	if (Game::instance().getKey(GLFW_KEY_0)) {
		instruccionsTime = -1;
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		escena = 0;
		level = 0;
		scene.init(0);
	}
	if (Game::instance().getKey(GLFW_KEY_1)) {
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		escena = 2;
		level = 0;
		scene.init(level);
	}
	if (Game::instance().getKey(GLFW_KEY_2)) {
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		escena = 2;
		level = 1;
		scene.init(level);
	}
	if (Game::instance().getKey(GLFW_KEY_3)) {
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		escena = 2;
		level = 2;
		scene.init(level);
	}
	if (Game::instance().getKey(GLFW_KEY_4)) {
		scene.canviaBoolSoroll();
		mciSendStringA("stop nivell", NULL, 0, NULL);
		mciSendStringA("stop corre", NULL, 0, NULL);
		mciSendStringA("stop corre2", NULL, 0, NULL);
		escena = 3;
		level = 0;
		scene.init(level);
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render(escena);
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}



