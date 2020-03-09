#ifndef PACMAN_H
#define PACMAN_H

#include "Avatar.h"
#include "Ghost.h"

struct PACMAN_DESC
{
	//Simulation data
	int tileSize = 22;
	float lives = 3;
	float avatarMovementSpeed = 160.0f;
	float ghostChaseSpeed = 140.0f;
	float ghostScatterSpeed = 100.0f;
	float ghostFrightenedSpeed = 60.0f;
	float ghostChaseDuration = 8.0f;
	float ghostFrightenedDuration = 10.0f;
	float ghostScatterDuration = 6.0f;
	float avatarToggleDrawCooldown = 0.05f;
	float cherrySpawnCooldown = 20.0f;

	//Graphics data
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
	const char* cherryImage = "cherry_32.png";

	const char* uiFont = "freefont-ttf\\sfd\\FreeMono.ttf";
};

class Pacman
{
public:
	Pacman(PACMAN_DESC pacman_desc);
	~Pacman(void);

	bool Init(Core& core, const PACMAN_DESC& pacman_desc);
	int Run(Core& core);
	void Shutdown(void);

private:

	//Simulation Logic:
	void OnUpdate(float aTime);
	void PickupDot(void);
	void PickupBigDot(void);
	void IntersectGhost(Ghost& ghost);
	void ResetGhost(Ghost& ghost);
	void ResetAvatar(void);
	void PickupCherry(void);
	void UpdateScore(int amount);
	void UpdateLives(int amount);

	void UpdateFPS(float deltaTime);

	//Graphics Logic:
	bool OnDraw(Renderer& renderer);
	void RedrawUI(Renderer& core);

	//Local data
	float m_ghostIntersectionDist;

	int m_lives;
	int m_score;
	int m_fps;

	int m_avgFps;
	int m_frameCounter;
	const int m_fpsCaptureThreshold;

	float m_bonusLiveScoreThreshold;

	Avatar* m_pAvatar;
	
	Ghost* m_pBlinky;
	Ghost* m_pPinky;
	Ghost* m_pInky;
	Ghost* m_pClyde;

	World* m_pWorld;

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
	
	Vector2f m_drawOffset;

	bool m_win;
};

#endif // PACMAN_H