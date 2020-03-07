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
	void OnUpdate(float aTime);
	bool Draw(Core& core);

	void Shutdown();

private:
	Pacman();

	//Simulation Logic:
	void UpdateAvatar(float deltaTime);
	void UpdateGhost(Ghost& ghost, float deltaTime);
	void PickupDot();
	void PickupBigDot();
	void SetGhostCounter(float value);
	void ReduceGhostCounterDuration(float amount);
	bool AvatarGhostIntersection(Ghost& ghost, float dist);
	void IntersectGhost(Ghost& ghost);
	void ResetGhost(Ghost& ghost);
	void ResetAvatar();
	void PickupCherry();
	void UpdateScore(int amount);
	void UpdateLives(int amount);
	void UpdateFPS(float deltaTime);

	//Graphics Logic:
	void DrawAvatar(Core& core, Vector2f& direction, const bool& open);
	void DrawGhost(
		Core& core, Ghost& ghost,
		DrawEntity& deadGhost,
		DrawEntity& vulnerableGhost,
		DrawEntity& defaultGhost);
	void RedrawUI(Core& core);

	float myTimeToNextUpdate;
	float myGhostGhostCounter;

	bool m_ghostCounterFlag;
	float m_ghostCounterDefault;
	float m_ghostCounterDuration;
	float m_ghostCounterReducer;

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
	bool m_updateUI;

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

	float m_toggleAvatarDrawDefault;
	float m_toggleAvatarDrawCooldown;
	float m_toggleAvatarDrawReducer;
	bool m_avatarOpen;
};

#endif // PACMAN_H