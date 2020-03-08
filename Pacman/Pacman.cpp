#include "Pacman.h"

Pacman::Pacman(PACMAN_DESC pacman_desc) :
	m_desc(pacman_desc),
	avatarDirection(-1.f, 0.f),
	m_score(0), m_win(false), m_fps(0),
	m_tileSize(22), m_drawOffset(220.0f, 60.0f),
	m_lives(pacman_desc.lives),
	m_ghostGhostCounter(0.0f), m_ghostIntersectionDist(10.0f),
	m_ghostCounterDefault(pacman_desc.ghostCounterDuration), 
	m_ghostCounterDuration(pacman_desc.ghostCounterDuration),
	m_ghostCounterFlag(false), m_ghostCounterReducer(1.0f),
	m_avatarOpen(true), 
	m_toggleAvatarDrawDefault(pacman_desc.avatarToggleDrawCooldown), 
	m_toggleAvatarDrawCooldown(pacman_desc.avatarToggleDrawCooldown), 
	m_toggleAvatarDrawReducer(1.0f), 
	m_avatarMovementSpeed(pacman_desc.avatarMovementSpeed), 
	m_ghostMovementSpeed(pacman_desc.ghostMovementSpeed)
{
	myAvatar = new Avatar(Vector2f(13, 22) * m_tileSize, m_avatarMovementSpeed);

	redGhost = new Ghost(Vector2f(13, 13) * m_tileSize, m_ghostMovementSpeed);
	tealGhost = new Ghost(Vector2f(13, 13) * m_tileSize - Vector2f(26, 0), m_ghostMovementSpeed);
	pinkGhost = new Ghost(Vector2f(13, 13) * m_tileSize - Vector2f(52, 0), m_ghostMovementSpeed);
	orangeGhost = new Ghost(Vector2f(13, 13) * m_tileSize + Vector2f(26, 0), m_ghostMovementSpeed);

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
	world_desc.playfieldDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.playfieldImage);
	world_desc.dotDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.dotImage);
	world_desc.bigDotDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.bigDotImage);
	world_desc.cherryDrawEntity = core.GetRenderer()->CreateDrawEntity(pacman_desc.cherryImage);

	myWorld->Init(world_desc, m_tileSize);

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
				avatarDirection = Vector2f(0.f, -1.f);
			else if (keystate[SDL_SCANCODE_DOWN])
				avatarDirection = Vector2f(0.f, 1.f);
			else if (keystate[SDL_SCANCODE_RIGHT])
				avatarDirection = Vector2f(1.f, 0.f);
			else if (keystate[SDL_SCANCODE_LEFT])
				avatarDirection = Vector2f(-1.f, 0.f);

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
	UpdateAvatar(aTime);

	UpdateGhost(*redGhost, aTime);
	UpdateGhost(*tealGhost, aTime);
	UpdateGhost(*pinkGhost, aTime);
	UpdateGhost(*orangeGhost, aTime);

	if (myWorld->HasIntersectedDot(*myAvatar, m_win))
	{
		PickupDot();
	}
	
	if (myWorld->HasIntersectedBigDot(*myAvatar, m_win))
	{
		PickupBigDot();
	}
	
	if (myAvatar->Intersect(*redGhost, m_ghostIntersectionDist))
	{
		IntersectGhost(*redGhost);
	}
	if (myAvatar->Intersect(*tealGhost, m_ghostIntersectionDist))
	{
		IntersectGhost(*tealGhost);
	}
	if (myAvatar->Intersect(*pinkGhost, m_ghostIntersectionDist))
	{
		IntersectGhost(*pinkGhost);
	}
	if (myAvatar->Intersect(*orangeGhost, m_ghostIntersectionDist))
	{
		IntersectGhost(*orangeGhost);
	}
	
	if (m_ghostCounterFlag)
	{
		ReduceGhostCounterDuration(aTime * m_ghostCounterReducer);
	}

	if (aTime > 0)
	{
		UpdateFPS(aTime);
	}
}

void Pacman::UpdateAvatar(float deltaTime)
{
	if (myAvatar->IsAtDestination())
	{
		Vector2f nextTile = myAvatar->GetCurrentTile() + avatarDirection * m_tileSize;
		if (myWorld->TileIsValid(nextTile))
		{
			myAvatar->SetNextTile(nextTile);
		}
		m_toggleAvatarDrawCooldown = m_toggleAvatarDrawDefault;
	}
	else
	{
		m_toggleAvatarDrawCooldown -= deltaTime * m_toggleAvatarDrawReducer;
		if (m_toggleAvatarDrawCooldown < 0.0f)
		{
			m_avatarOpen = !m_avatarOpen;
			m_toggleAvatarDrawCooldown = m_toggleAvatarDrawDefault;
		}
	}

	Vector2f direction = myAvatar->GetNextTile() - myAvatar->GetCurrentTile();

	float distanceToMove = deltaTime * myAvatar->GetMovementSpeed();

	if (distanceToMove > direction.Length())
	{
		myAvatar->SetCurrentTile(myAvatar->GetNextTile());
	}
	else
	{
		myAvatar->SetDirection(direction.Normalize());
		myAvatar->SetCurrentTile(myAvatar->GetCurrentTile() + direction * distanceToMove);
	}

	myAvatar->SetPosition(myAvatar->GetCurrentTile());
	myAvatar->SetDrawPos(myAvatar->GetCurrentTile() + m_drawOffset);
}

void Pacman::UpdateGhost(Ghost& ghost, float deltaTime)
{	
	Vector2f nextTile = ghost.GetCurrentTile() + ghost.GetDesiredMovement() * m_tileSize;

	if (ghost.IsAtDestination())
	{
		if (!ghost.GetPath().empty())
		{
			PathmapTile* nextTile = ghost.GetPath().front();
			SAFE_DELETE(ghost.GetPath().front());
			ghost.GetPath().erase(ghost.GetPath().begin());
			ghost.SetNextTile(nextTile->m_pos);
		}
		else if (myWorld->TileIsValid(nextTile))
		{
			ghost.SetNextTile(nextTile);
		}
		else
		{
			if (ghost.GetDesiredMovement().myX == 1)
			{
				ghost.SetDesiredMovement(Vector2f(0, 1));
			}
			else if (ghost.GetDesiredMovement().myY == 1)
			{
				ghost.SetDesiredMovement(Vector2f(-1, 0));
			}
			else if (ghost.GetDesiredMovement().myX == -1)
			{
				ghost.SetDesiredMovement(Vector2f(0, -1));
			}
			else
			{
				ghost.SetDesiredMovement(Vector2f(1, 0));
			}

			ghost.SetDeadFlag(false);
		}
	}

	Vector2f direction = ghost.GetNextTile() - ghost.GetPosition();

	float distanceToMove = deltaTime * ghost.GetMovementSpeed();

	if (distanceToMove > direction.Length())
	{
		ghost.SetPosition(ghost.GetNextTile());
		ghost.SetCurrentTile(ghost.GetNextTile());
	}
	else
	{
		ghost.SetDirection(direction.Normalize());
		ghost.SetPosition(ghost.GetPosition() + direction * distanceToMove);
	}

	ghost.SetDrawPos(ghost.GetPosition() + m_drawOffset);
}

void Pacman::UpdateScore(int amount)
{
	m_score += amount;
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

void Pacman::DrawAvatar(Renderer& renderer, Vector2f& direction, const bool& open)
{
	if (direction == Vector2f(1, 0))
	{
		if (open)
		{
			renderer.DrawObject(*m_pAvatarOpenRight, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			renderer.DrawObject(*m_pAvatarClosedRight, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
	}
	else if (direction == Vector2f(-1, 0))
	{
		if (open)
		{
			renderer.DrawObject(*m_pAvatarOpenLeft, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			renderer.DrawObject(*m_pAvatarClosedLeft, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
	}
	else if (direction == Vector2f(0, 1))
	{
		if (open)
		{
			renderer.DrawObject(*m_pAvatarOpenDown, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			renderer.DrawObject(*m_pAvatarClosedDown, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
	}
	else if (direction == Vector2f(0, -1))
	{
		if (open)
		{
			renderer.DrawObject(*m_pAvatarOpenUp, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			renderer.DrawObject(*m_pAvatarClosedUp, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
	}
}

void Pacman::DrawGhost(
	Renderer& renderer, Ghost& ghost, 
	DrawEntity& deadGhost, 
	DrawEntity& vulnerableGhost, 
	DrawEntity& defaultGhost)
{
	if (ghost.GetDeadFlag())
	{
		renderer.DrawObject(deadGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
	else if (m_ghostCounterFlag)
	{
		renderer.DrawObject(vulnerableGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
	else
	{
		renderer.DrawObject(defaultGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
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
	SetGhostCounter(m_ghostCounterDuration);
}

void Pacman::SetGhostCounter(float value)
{
	m_ghostGhostCounter = value;
	m_ghostCounterFlag = true;
}

void Pacman::ReduceGhostCounterDuration(float amount)
{
	m_ghostCounterDuration -= amount;
	if (m_ghostCounterDuration < 0.0f)
	{
		m_ghostCounterFlag = false;
		m_ghostCounterDuration = m_ghostCounterDefault;
	}
}

void Pacman::ResetGhost(Ghost& ghost)
{
	ghost.GetPath().clear();
	ghost.SetDesiredMovement(Vector2f(0, -1));
	ghost.SetPosition(ghost.GetSpawnPos());
	ghost.SetCurrentTile(ghost.GetPosition());
	ghost.SetNextTile(ghost.GetCurrentTile());
}

void Pacman::ResetAvatar()
{
	avatarDirection = Vector2f(-1.f, 0.f);
	myAvatar->SetPosition(myAvatar->GetSpawnPos());
	myAvatar->SetCurrentTile(myAvatar->GetPosition());
	myAvatar->SetNextTile(myAvatar->GetCurrentTile());
}

void Pacman::IntersectGhost(Ghost& ghost)
{
	if (!ghost.GetDeadFlag())
	{
		if (!m_ghostCounterFlag)
		{
			UpdateLives(-1);
			ResetAvatar();
			ResetGhost(*redGhost);
			ResetGhost(*tealGhost);
			ResetGhost(*pinkGhost);
			ResetGhost(*orangeGhost);
		}
		else if (m_ghostCounterFlag)
		{
			UpdateScore(50);
			ResetGhost(ghost);
			ghost.SetDeadFlag(true);
			ghost.SetPath(myWorld->GetPath(ghost.GetCurrentTile(), Vector2f(13, 13)));
		}
	}
}

void Pacman::PickupCherry()
{
}

bool Pacman::OnDraw(Renderer& renderer)
{
	myWorld->Draw(renderer);

	DrawAvatar(renderer, myAvatar->GetDirection(), m_avatarOpen);

	DrawGhost(renderer, *redGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pRedGhost);
	DrawGhost(renderer, *tealGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pTealGhost);
	DrawGhost(renderer, *pinkGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pPinkGhost);
	DrawGhost(renderer, *orangeGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pOrangeGhost);
	
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
