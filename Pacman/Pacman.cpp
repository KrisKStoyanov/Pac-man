#include "Pacman.h"

Pacman::Pacman(PACMAN_DESC pacman_desc) :
	m_desc(pacman_desc),
	m_score(0), m_win(false), m_fps(0),
	m_drawOffset(220.0f, 60.0f),
	m_lives(pacman_desc.lives),
	m_ghostIntersectionDist(10.0f),
	m_bonusLiveScoreThreshold(1000)
{
	myAvatar = new Avatar(Vector2f(13, 22) * pacman_desc.tileSize, pacman_desc.avatarMovementSpeed);

	redGhost = new Ghost(Vector2f(13, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed, 
		pacman_desc.ghostScatterSpeed, 
		pacman_desc.ghostFrightenedSpeed, 
		pacman_desc.ghostChaseDuration + pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration,
		Vector2f(1, 0));

	tealGhost = new Ghost(Vector2f(12, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed, 
		pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration + pacman_desc.ghostScatterDuration,
		Vector2f(-1, 0));

	pinkGhost = new Ghost(Vector2f(11, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed, 
		pacman_desc.ghostChaseDuration + pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration,
		Vector2f(-1, 0));

	orangeGhost = new Ghost(Vector2f(14, 10) * pacman_desc.tileSize,
		pacman_desc.ghostChaseSpeed,
		pacman_desc.ghostScatterSpeed,
		pacman_desc.ghostFrightenedSpeed,
		pacman_desc.ghostChaseDuration,
		pacman_desc.ghostFrightenedDuration,
		pacman_desc.ghostScatterDuration + pacman_desc.ghostScatterDuration,
		Vector2f(1, 0));

	myWorld = new World();
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

	myWorld->Init(world_desc);

	return m_isRunning;
}

int Pacman::Run(Core& core)
{
	float lastFrame = (float)SDL_GetTicks() * 0.001f;
	while (m_isRunning) 
	{
		float currentFrame = (float)SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		SDL_Event curEvent;
		while (SDL_PollEvent(&curEvent) > 0)
		{
			const Uint8* keystate = SDL_GetKeyboardState(NULL);

			if (keystate[SDL_SCANCODE_UP])
				myAvatar->SetNextDir(Vector2f(0.f, -1.f));
			else if (keystate[SDL_SCANCODE_DOWN])
				myAvatar->SetNextDir(Vector2f(0.f, 1.f));
			else if (keystate[SDL_SCANCODE_RIGHT])
				myAvatar->SetNextDir(Vector2f(1.f, 0.f));
			else if (keystate[SDL_SCANCODE_LEFT])
				myAvatar->SetNextDir(Vector2f(-1.f, 0.f));

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
	}

	Shutdown();
	core.Shutdown();
	return EXIT_SUCCESS;
}

void Pacman::OnUpdate(float aTime)
{
	myWorld->Update(aTime);

	myAvatar->Update(*myWorld, aTime, m_desc.tileSize, m_drawOffset);

	redGhost->Update(*myWorld, myAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	tealGhost->Update(*myWorld, myAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	pinkGhost->Update(*myWorld, myAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);
	orangeGhost->Update(*myWorld, myAvatar->GetCurrentTile(), aTime, m_desc.tileSize, m_drawOffset);

	if (myWorld->HasIntersectedDot(*myAvatar, m_win))
	{
		PickupDot();
	}	
	if (myWorld->HasIntersectedBigDot(*myAvatar, m_win))
	{
		PickupBigDot();
	}
	if (myWorld->HasIntersectedCherry(*myAvatar))
	{
		PickupCherry();
	}
	
	if (myAvatar->Intersect(redGhost->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*redGhost);
	}
	if (myAvatar->Intersect(tealGhost->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*tealGhost);
	}
	if (myAvatar->Intersect(pinkGhost->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*pinkGhost);
	}
	if (myAvatar->Intersect(orangeGhost->GetPosition(), m_ghostIntersectionDist))
	{
		IntersectGhost(*orangeGhost);
	}
	
	if (aTime > 0)
	{
		UpdateFPS(aTime);
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
	redGhost->SetState(GhostState::FRIGHTENED);
	tealGhost->SetState(GhostState::FRIGHTENED);
	pinkGhost->SetState(GhostState::FRIGHTENED);
	orangeGhost->SetState(GhostState::FRIGHTENED);
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
	myAvatar->SetNextDir(Vector2f(-1.f, 0.f));
	myAvatar->SetPosition(myAvatar->GetSpawnPos());
	myAvatar->SetCurrentTile(myAvatar->GetPosition());
	myAvatar->SetNextTile(myAvatar->GetCurrentTile());
}

void Pacman::IntersectGhost(Ghost& ghost)
{
	if (!ghost.GetDeadFlag())
	{
		if (!(ghost.GetState() == GhostState::FRIGHTENED))
		{
			UpdateLives(-1);
			myWorld->Reset();
			ResetAvatar();
			ResetGhost(*redGhost);
			ResetGhost(*tealGhost);
			ResetGhost(*pinkGhost);
			ResetGhost(*orangeGhost);
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
	myWorld->Draw(renderer);

	myAvatar->Draw(renderer, 
		*m_pAvatarOpenLeft, *m_pAvatarOpenRight, *m_pAvatarOpenUp, *m_pAvatarOpenDown,
		*m_pAvatarClosedLeft, *m_pAvatarClosedRight, *m_pAvatarClosedUp, *m_pAvatarClosedDown);

	redGhost->Draw(renderer, *m_pRedGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	tealGhost->Draw(renderer, *m_pTealGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	pinkGhost->Draw(renderer, *m_pPinkGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	orangeGhost->Draw(renderer, *m_pOrangeGhost, *m_pVulnerableGhost, *m_pDeadGhost);
	
	if (m_updateUI)
	{
		RedrawUI(renderer);
	}

	renderer.DrawObject(*m_pScoreText);
	renderer.DrawObject(*m_pLivesText);
	renderer.DrawObject(*m_pFpsText);

	return true;
}

void Pacman::Shutdown()
{
	myWorld->Shutdown();
	SAFE_DELETE(m_pScoreText);
	SAFE_DELETE(m_pLivesText);
	SAFE_DELETE(m_pFpsText);

	SAFE_DELETE(myWorld);
	SAFE_DELETE(myAvatar);
	SAFE_DELETE(redGhost);
	SAFE_DELETE(tealGhost);
	SAFE_DELETE(pinkGhost);
	SAFE_DELETE(orangeGhost);

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
