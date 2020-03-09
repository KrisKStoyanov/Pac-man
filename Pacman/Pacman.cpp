#include "Pacman.h"

Pacman::Pacman(PACMAN_DESC pacman_desc) :
	m_desc(pacman_desc),
	m_score(0), m_win(false), m_fps(0),
	m_drawOffset(220.0f, 60.0f),
	m_lives(pacman_desc.lives),
	m_ghostIntersectionDist(10.0f),
	m_bonusLiveScoreThreshold(1000),
	m_frameCounter(0), m_fpsCaptureThreshold(60),
	m_avgFps(0)
{
	m_pAvatar = new Avatar(Vector2f(13, 22) * pacman_desc.tileSize, pacman_desc.avatarMovementSpeed);

	m_pBlinky = new Ghost(Vector2f(13, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed, 
		pacman_desc.ghostScatterSpeed, 
		pacman_desc.ghostFrightenedSpeed, 
		pacman_desc.ghostChaseDuration + pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration,
		Vector2f(1, 0));

	m_pPinky = new Ghost(Vector2f(11, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed,
		pacman_desc.ghostChaseDuration + pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration,
		Vector2f(-1, 0));

	m_pInky = new Ghost(Vector2f(12, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed, 
		pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration + pacman_desc.ghostScatterDuration,
		Vector2f(-1, 0));

	m_pClyde = new Ghost(Vector2f(14, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed,
		pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration + pacman_desc.ghostScatterDuration,
		Vector2f(1, 0));

	m_pWorld = new World();
}

Pacman::~Pacman(void)
{
}

bool Pacman::Init(Core& core, const PACMAN_DESC& pacman_desc)
{
	m_isRunning = core.Init();

	std::stringstream scoreStream;
	scoreStream << "Score: " << m_score << "   "; //intentionally left blank
	m_pScoreText = core.GetRenderer()->CreateDrawTextEntity(scoreStream.str().c_str(), pacman_desc.uiFont, 20, 50);
	scoreStream.flush();

	std::stringstream liveStream;
	liveStream << "Lives: " << m_lives;
	m_pLivesText = core.GetRenderer()->CreateDrawTextEntity(liveStream.str().c_str(), pacman_desc.uiFont, 20, 80);
	liveStream.flush();

	std::stringstream fpsStream;
	fpsStream << "FPS: " << m_fps << "   "; //intentionally left blank
	m_pFpsText = core.GetRenderer()->CreateDrawTextEntity(fpsStream.str().c_str(), pacman_desc.uiFont, 880, 50);
	fpsStream.flush();

	m_pAvatarOpenLeft = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarOpenLeftImage);
	m_pAvatarOpenRight = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarOpenRightImage);
	m_pAvatarOpenUp = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarOpenUpImage);
	m_pAvatarOpenDown = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarOpenDownImage);

	m_pAvatarClosedLeft = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarClosedLeftImage);
	m_pAvatarClosedRight = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarClosedRightImage);
	m_pAvatarClosedUp = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarClosedUpImage);
	m_pAvatarClosedDown = core.GetRenderer()->CreateDrawEntity(pacman_desc.avatarClosedDownImage);
	
	m_pRedGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.redGhostImage);
	m_pTealGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.tealGhostImage);
	m_pPinkGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.pinkGhostImage);
	m_pOrangeGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.orangeGhostImage);

	m_pVulnerableGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.ghostVulnerableImage);
	m_pDeadGhost = core.GetRenderer()->CreateDrawEntity(pacman_desc.ghostDeadImage);

	WORLD_DESC world_desc;
	world_desc.tileSize = pacman_desc.tileSize;;
	world_desc.spawnCherryCooldown = pacman_desc.cherrySpawnCooldown;
	world_desc.playfieldDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.playfieldImage);
	world_desc.dotDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.dotImage);
	world_desc.bigDotDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.bigDotImage);
	world_desc.cherryDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.cherryImage);

	m_pWorld->Init(world_desc);

	return m_isRunning;
}

int Pacman::Run(Core& core)
{
	float lastFrame = (float)SDL_GetTicks() * 0.001f;
	m_frameCounter = 0;
	while (m_isRunning) 
	{
		m_frameCounter++;
		float currentFrame = (float)SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		SDL_Event curEvent;
		while (SDL_PollEvent(&curEvent) > 0)
		{
			const Uint8* keystate = SDL_GetKeyboardState(NULL);

			if (keystate[SDL_SCANCODE_UP])
				m_pAvatar->SetNextDir(Vector2f(0.f, -1.f));
			else if (keystate[SDL_SCANCODE_DOWN])
				m_pAvatar->SetNextDir(Vector2f(0.f, 1.f));
			else if (keystate[SDL_SCANCODE_RIGHT])
				m_pAvatar->SetNextDir(Vector2f(1.f, 0.f));
			else if (keystate[SDL_SCANCODE_LEFT])
				m_pAvatar->SetNextDir(Vector2f(-1.f, 0.f));

			if (keystate[SDL_SCANCODE_ESCAPE])
			{
				m_isRunning = false;
			}
		}
		if (!m_win && m_lives > 0)
		{
			OnUpdate(elapsedTime);
		}
		core.GetRenderer()->OnStartFrameRender();
		OnDraw(*core.GetRenderer());
		core.GetRenderer()->OnEndFrameRender();
	
		lastFrame = currentFrame;

		if (m_frameCounter > m_fpsCaptureThreshold)
		{
			UpdateFPS(elapsedTime);
		}
	}

	Shutdown();
	core.Shutdown();
	return EXIT_SUCCESS;
}

void Pacman::OnUpdate(float aTime)
{
	m_pWorld->Update(aTime);

	m_pAvatar->Update(*m_pWorld, aTime, m_desc.tileSize, m_drawOffset);

	m_pBlinky->Update(*m_pWorld, m_pAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	m_pPinky->Update(*m_pWorld, m_pAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	m_pInky->Update(*m_pWorld, m_pAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	m_pClyde->Update(*m_pWorld, m_pAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);

	if (m_pWorld->HasIntersectedDot(*m_pAvatar, m_win))
	{
		PickupDot();
	}	
	if (m_pWorld->HasIntersectedBigDot(*m_pAvatar, m_win))
	{
		PickupBigDot();
	}
	if (m_pWorld->HasIntersectedCherry(*m_pAvatar))
	{
		PickupCherry();
	}
	
	if (m_pAvatar->Intersect(m_pBlinky->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*m_pBlinky);
	}
	if (m_pAvatar->Intersect(m_pPinky->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*m_pPinky);
	}
	if (m_pAvatar->Intersect(m_pInky->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*m_pInky);
	}
	if (m_pAvatar->Intersect(m_pClyde->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*m_pClyde);
	}
}

void Pacman::UpdateScore(int amount)
{
	m_score += amount;
	if (m_score > m_bonusLiveScoreThreshold)
	{
		UpdateLives(1);
	}
	m_updateUI = true;
}

void Pacman::UpdateLives(int amount)
{
	m_lives += amount;
	if (m_lives < 1)
	{
		m_lives = 0;
	}
	m_updateUI = true;
}

void Pacman::UpdateFPS(float deltaTime)
{
	m_frameCounter = 0;
	m_fps = (int)(1 / deltaTime);
	m_updateUI = true;
}

void Pacman::RedrawUI(Renderer& renderer)
{
	std::string drawTextString;
	std::stringstream scoreStream;
	if (m_win)
	{
		scoreStream << "You win!   ";
	}
	else if (m_lives < 1)
	{
		scoreStream << "You lose!   ";
	}
	else
	{
		scoreStream << "Score: " << m_score << "   ";
	}
	drawTextString = scoreStream.str();
	renderer.SetDrawEntityText(*m_pScoreText, drawTextString.c_str(), m_desc.uiFont);
	scoreStream.flush();

	std::stringstream liveStream;
	liveStream << "Lives: " << m_lives;
	drawTextString = liveStream.str();
	renderer.SetDrawEntityText(*m_pLivesText, drawTextString.c_str(), m_desc.uiFont);
	liveStream.flush();

	std::stringstream fpsStream;
	fpsStream << "FPS: " << m_fps << "   ";
	drawTextString = fpsStream.str();
	renderer.SetDrawEntityText(*m_pFpsText, drawTextString.c_str(), m_desc.uiFont);
	fpsStream.flush();

	m_updateUI = false;
}

void Pacman::PickupDot()
{
	UpdateScore(10);
}

void Pacman::PickupBigDot()
{
	UpdateScore(20);
	m_pBlinky->SetState(GhostState::FRIGHTENED);
	m_pPinky->SetState(GhostState::FRIGHTENED);
	m_pInky->SetState(GhostState::FRIGHTENED);
	m_pClyde->SetState(GhostState::FRIGHTENED);
}

void Pacman::ResetGhost(Ghost& ghost)
{
	ghost.SetNextDir(Vector2f(0, -1));
	ghost.SetPosition(ghost.GetSpawnPos());
	ghost.SetCurrentTile(ghost.GetPosition());
	ghost.SetNextTile(ghost.GetCurrentTile());
}

void Pacman::ResetAvatar()
{
	m_pAvatar->SetNextDir(Vector2f(-1.f, 0.f));
	m_pAvatar->SetPosition(m_pAvatar->GetSpawnPos());
	m_pAvatar->SetCurrentTile(m_pAvatar->GetPosition());
	m_pAvatar->SetNextTile(m_pAvatar->GetCurrentTile());
}

void Pacman::IntersectGhost(Ghost& ghost)
{
	if (!ghost.GetDeadFlag())
	{
		if (!(ghost.GetState() == GhostState::FRIGHTENED))
		{
			UpdateLives(-1);
			m_pWorld->Reset();
			ResetAvatar();
			ResetGhost(*m_pBlinky);
			ResetGhost(*m_pPinky);
			ResetGhost(*m_pInky);
			ResetGhost(*m_pClyde);
		}
		else
		{
			UpdateScore(50);
			ResetGhost(ghost);
			ghost.SetDeadFlag(true);
		}
	}
}

void Pacman::PickupCherry()
{
	UpdateScore(1000);
}

bool Pacman::OnDraw(Renderer& renderer)
{
	m_pWorld->Draw(renderer);

	m_pAvatar->Draw(renderer,
		*m_pAvatarOpenLeft, *m_pAvatarOpenRight, *m_pAvatarOpenUp, *m_pAvatarOpenDown,
		*m_pAvatarClosedLeft, *m_pAvatarClosedRight, *m_pAvatarClosedUp, *m_pAvatarClosedDown);

	m_pBlinky->Draw(renderer, *m_pRedGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	m_pPinky->Draw(renderer, *m_pPinkGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	m_pInky->Draw(renderer, *m_pTealGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	m_pClyde->Draw(renderer, *m_pOrangeGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	
	if (m_updateUI)
	{
		RedrawUI(renderer);
	}

	renderer.DrawObject(*m_pScoreText, Vector2f(m_pScoreText->GetPosRect().x, m_pScoreText->GetPosRect().y));
	renderer.DrawObject(*m_pLivesText, Vector2f(m_pLivesText->GetPosRect().x, m_pLivesText->GetPosRect().y));
	renderer.DrawObject(*m_pFpsText, Vector2f(m_pFpsText->GetPosRect().x, m_pFpsText->GetPosRect().y));

	return true;
}

void Pacman::Shutdown()
{
	m_pWorld->Shutdown();
	SAFE_DELETE(m_pScoreText);
	SAFE_DELETE(m_pLivesText);
	SAFE_DELETE(m_pFpsText);

	SAFE_DELETE(m_pWorld);
	SAFE_DELETE(m_pAvatar);
	SAFE_DELETE(m_pBlinky);
	SAFE_DELETE(m_pPinky);
	SAFE_DELETE(m_pInky);
	SAFE_DELETE(m_pClyde);

	SAFE_DELETE(m_pAvatarOpenLeft);
	SAFE_DELETE(m_pAvatarOpenRight);
	SAFE_DELETE(m_pAvatarOpenUp);
	SAFE_DELETE(m_pAvatarOpenDown);

	SAFE_DELETE(m_pAvatarClosedLeft);
	SAFE_DELETE(m_pAvatarClosedRight);
	SAFE_DELETE(m_pAvatarClosedUp);
	SAFE_DELETE(m_pAvatarClosedDown);
	
	SAFE_DELETE(m_pRedGhost);
	SAFE_DELETE(m_pTealGhost);
	SAFE_DELETE(m_pPinkGhost);
	SAFE_DELETE(m_pOrangeGhost);
	SAFE_DELETE(m_pVulnerableGhost);
	SAFE_DELETE(m_pDeadGhost);
}
