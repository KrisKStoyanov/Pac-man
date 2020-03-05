#ifndef PACMAN_H
#define PACMAN_H

#include "Vector2f.h"

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

	bool Update(float aTime);
	bool Draw(Core* core);

private:
	Pacman();
	bool Init();
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
};

#endif // PACMAN_H