#ifndef PACMAN_H
#define PACMAN_H

#include "Avatar.h"
#include "World.h"
#include "Ghost.h"

struct PACMAN_DESC
{
	const char* avatarOpenLeftImage = "open_left_32.png";
	const char* avatarOpenRightImage = "open_right_32.png";
	const char* avatarOpenUpImage = "open_up_32.png";
	const char* avatarOpenDownImage = "open_down_32.png";

	const char* avatarClosedLeftImage = "closed_left_32.png";
	const char* avatarClosedRightImage = "closed_right_32.png";
	const char* avatarClosedUpImage = "closed_up_32.png";
	const char* avatarClosedDownImage = "closed_down_32.png";

	const char* redGhostImage = "ghost_32_red.png";
	const char* tealGhostImage = "ghost_32_cyan.png";
	const char* pinkGhostImage = "ghost_32_pink.png";
	const char* orangeGhostImage = "ghost_32_orange.png";

	const char* ghostVulnerableImage = "Ghost_Vulnerable_32.png";
	const char* ghostDeadImage = "Ghost_Dead_32.png";

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
	void UpdateGhost(float deltaTime);

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
	
	Ghost* orangeGhost;
	Ghost* redGhost;
	Ghost* tealGhost;
	Ghost* pinkGhost;

	World* myWorld;

	bool m_isRunning;
	bool m_redrawUI;

	DrawTextEntity* m_pScoreText;
	DrawTextEntity* m_pLivesText;
	DrawTextEntity* m_pFpsText;

	DrawEntity* m_pAvatarOpenLeft;
	DrawEntity* m_pAvatarOpenRight;
	DrawEntity* m_pAvatarOpenUp;
	DrawEntity* m_pAvatarOpenDown;

	DrawEntity* m_pAvatarClosedLeft;
	DrawEntity* m_pAvatarClosedRight;
	DrawEntity* m_pAvatarClosedUp;
	DrawEntity* m_pAvatarClosedDown;

	DrawEntity* m_pRedGhost;
	DrawEntity* m_pTealGhost;
	DrawEntity* m_pPinkGhost;
	DrawEntity* m_pOrangeGhost;

	DrawEntity* m_pVulnerableGhost;
	DrawEntity* m_pDeadGhost;

	PACMAN_DESC m_desc;

	const float m_drawOffsetX;
	const float m_drawOffsetY;

	const int m_tileSize;

	bool m_win;
};

#endif // PACMAN_H