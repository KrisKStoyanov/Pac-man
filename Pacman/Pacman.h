#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"
#include "Core.h"
#include "SDL.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

struct SDL_Surface;
class Core;
class Avatar;
class World;
class Ghost;

class Pacman
{
public:
	static Pacman* Create();
	~Pacman(void);

	bool Init(Core& core);
	int Run(Core& core);
	bool Update(float aTime);
	bool Draw(Core& core);

	void Shutdown();

private:
	Pacman();
	bool UpdateInput();
	void MoveAvatar();
	bool CheckEndGameCondition();

	float myTimeToNextUpdate;
	float myGhostGhostCounter;

	int myLives;
	int myScore;
	int myFps;

	Vector2f myNextMovement;

	Avatar* myAvatar;
	Ghost* myGhost;
	World* myWorld;

	bool m_isRunning;
};

#endif // PACMAN_H