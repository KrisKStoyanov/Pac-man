#ifndef PACMAN_H
#define PACMAN_H

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

struct PACMAN_DESC
{
	const char* avatarImage = "open_32.png";
	
	const char* ghostDefaultImage = "ghost_32.png";
	const char* ghostVulnerableImage = "ghost_32.png";
	const char* ghostDeadImage = "ghost_32.png";

	const char* playfieldImage = "playfield.png";
	const char* dotImage = "Small_Dot_32.png";
	const char* bigDotImage = "Big_Dot_32.png";
	const char* cherryImage = "dot.png";

	const char* uiFont = "freefont-ttf\\sfd\\FreeMono.ttf";
};

class Pacman
{
public:
	static Pacman* Create();
	~Pacman(void);

	bool Init(Core& core, const PACMAN_DESC& pacman_desc);
	int Run(Core& core);
	bool Update(Core& core, float aTime);
	bool Draw(Core& core);

	void Shutdown();

private:
	Pacman();
	bool CheckEndGameCondition();

	void UpdateAvatar(float deltaTime);

	void UpdateScore(int amount);
	void UpdateLives(int amount);
	void UpdateFPS(float deltaTime);

	void RedrawUI(SDL_Renderer* renderer);

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
	bool m_redrawUI;

	DrawTextEntity* m_pScoreText;
	DrawTextEntity* m_pLivesText;
	DrawTextEntity* m_pFpsText;

	DrawEntity* m_pAvatar;

	DrawEntity* m_pDefaultGhost;
	DrawEntity* m_pVulnerableGhost;
	DrawEntity* m_pDeadGhost;

	PACMAN_DESC m_desc;

	const float m_drawOffsetX;
	const float m_drawOffsetY;

	const int m_tileSize;
};

#endif // PACMAN_H