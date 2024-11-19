#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <algorithm>


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i = 0; i < mapSize.x; ++i)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else if (tile == '.')
				map[j * mapSize.x + i] = -1;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	getline(fin, line);
	if (line == "Escaleras") {
		getline(fin, line);
		while (line != "Fin") {
			int escaleraX, escaleraY;
			escaleraY = stoi(line);
			getline(fin, line);
			escaleraX = stoi(line);
			getline(fin, line);
			vectorEscaleras.push_back(glm::vec2(escaleraX, escaleraY));
		}
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0 && tile != -1 && tile != -2 && tile != -3)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if (map[y * mapSize.x + x] != 0) {
			return true;
		}
	}
	
	return false;
}

bool TileMap::collisionMoveLeftBubble(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y;

	x = pos.x / tileSize;
	y = (pos.y + (size.y/2)) / tileSize;
	if (map[y * mapSize.x + x] != 0)
		return true;


	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	//if (map[y * mapSize.x + x] != 0) return true;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRightBubble(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y;

	x = (pos.x + size.x) / tileSize;
	y = (pos.y + (size.y / 2)) / tileSize;
	if (map[y * mapSize.x + x] != 0)
		return true;


	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveDownBubble(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x, y;

	x = ((pos.x + pos.x + size.x) / 2) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	if (map[y * mapSize.x + x] != 0)return true;
	return false;
}

int TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{

	int x, y;

	x = ((pos.x + pos.x + size.x) / 2) / tileSize;
	y = (pos.y) / tileSize;

	if (map[y * mapSize.x + x] != 0 && map[y * mapSize.x + x] != -1) {
		return 1;
	}
	if (map[y * mapSize.x + x] == -1){
		map[y * mapSize.x + x] = 0;
		return 2;
	}
	if (map[y * mapSize.x + x - 1] == -1) {
		map[y * mapSize.x + x - 1] = 0;
		return 2;
	}
	if (map[y * mapSize.x + x - 2] == -1) {
		map[y * mapSize.x + x - 2] = 0;
		return 2;
	}
	if (map[y * mapSize.x + x - 3] == -1) {
		map[y * mapSize.x + x - 3] = 0;
		return 2;
	}
	return 0;
}

bool TileMap::collisionMoveUpBubble(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x, y;
	x = ((pos.x+ size.x) / 2) / tileSize;
	y = (pos.y) / tileSize;
	if (map[y * mapSize.x + x] != 0) {
		return true;
	}
	return false;
}


int TileMap::collisionBubble(glm::ivec2 &pos, int radius, int* posY) {
	int cy = pos.y;
	int cx = pos.x;
	int returnNum = 0;
	int minDistance = -1;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (map[j * mapSize.x + i] != 0 || map[j * mapSize.x + i - 1] == -1 || map[j * mapSize.x + i - 2] == -1) {
				// calcule bloc positions
				int rx = i* tileSize;
				int ry = j* tileSize;
				int rw = 8;
				int rh = 8;
				// temporary variables to set edges for testing
				float testX = cx;
				float testY = cy;

				// which edge is closest?
				if (cx < rx)			testX = rx;       // test left edge
				else if (cx > rx + rw) testX = rx + rw;   // right edge
				if (cy < ry)			testY = ry;       // top edge
				else if (cy > ry + rh) testY = ry + rh;   // bottom edge
				// get distance from closest edges
				float distX = cx - testX;
				float distY = cy - testY;
				if (distX < 0) {
					distX = -distX;
				}
				float distance = sqrt((distX * distX) + (distY * distY));
				if (distance <= radius) {
					if (minDistance == -1 || distance <= minDistance) {
						minDistance = distance;
						if (distX < abs(distY)) {
							if (distY > 0) {
								returnNum = 3;
							}
							else returnNum = 2;
						}
						else {
							returnNum = 1;
						}
					}
				}
			}
			
		}
	}
	return returnNum;
}

vector<pair<int, int>> TileMap::checkBreakBlocks() {
	vector<pair<int, int>> bb;
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (map[j * mapSize.x + i] == -1) bb.push_back({ i,j });
		}
	}
	return bb;
}