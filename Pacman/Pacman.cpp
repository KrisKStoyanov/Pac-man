#include "Pacman.h"

Pacman::Pacman(PACMAN_DESC pacman_desc) :
	myNextMovement(-1.f, 0.f),
	m_score(0), m_win(false), m_fps(0),
	m_tileSize(22), m_drawOffsetX(220.0f), m_drawOffsetY(60.0f),
	m_lives(pacman_desc.lives),
	m_ghostGhostCounter(0.0f),
	m_ghostCounterDefault(pacman_desc.ghostCounterDuration), 
	m_ghostCounterDuration(pacman_desc.ghostCounterDuration),
	m_ghostCounterFlag(false), m_ghostCounterReducer(1.0f),
	m_avatarOpen(true), 
	m_toggleAvatarDrawDefault(pacman_desc.avatarToggleDrawCooldown), 
	m_toggleAvatarDrawCooldown(pacman_desc.avatarToggleDrawCooldown), 
	m_toggleAvatarDrawReducer(1.0f)
{
	myAvatar = new Avatar(Vector2f(13 * m_tileSize, 22 * m_tileSize), 120.0f);

	redGhost = new Ghost(Vector2f(13 * m_tileSize, 13 * m_tileSize), 60.0f);
	tealGhost = new Ghost(Vector2f(13 * m_tileSize - 26, 13 * m_tileSize), 60.0f);
	pinkGhost = new Ghost(Vector2f(13 * m_tileSize - 52, 13 * m_tileSize), 60.0f);
	orangeGhost = new Ghost(Vector2f(13 * m_tileSize + 26, 13 * m_tileSize), 60.0f);

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
				myNextMovement = Vector2f(0.f, -1.f);
			else if (keystate[SDL_SCANCODE_DOWN])
				myNextMovement = Vector2f(0.f, 1.f);
			else if (keystate[SDL_SCANCODE_RIGHT])
				myNextMovement = Vector2f(1.f, 0.f);
			else if (keystate[SDL_SCANCODE_LEFT])
				myNextMovement = Vector2f(-1.f, 0.f);

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

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition(), m_win))
		PickupDot();

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition(), m_win))
		PickupBigDot();

	if (m_ghostCounterFlag)
		ReduceGhostCounterDuration(aTime * m_ghostCounterReducer);

	AvatarGhostIntersection(*redGhost, 10.0f);
	AvatarGhostIntersection(*tealGhost, 10.0f);
	AvatarGhostIntersection(*pinkGhost, 10.0f);
	AvatarGhostIntersection(*orangeGhost, 10.0f);
	
	if (aTime > 0)
		UpdateFPS(aTime);
}

void Pacman::UpdateAvatar(float deltaTime)
{
	if (myAvatar->IsAtDestination())
	{
		Vector2f nextTile = Vector2f(myAvatar->GetCurrentTile() + myNextMovement);
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

	Vector2f destination(myAvatar->GetNextTile() * m_tileSize);
	Vector2f direction = destination - myAvatar->GetPosition();

	float distanceToMove = deltaTime * myAvatar->GetMovementSpeed();

	if (distanceToMove > direction.Length())
	{
		myAvatar->SetPosition(destination);
		myAvatar->SetCurrentTile(myAvatar->GetNextTile());
	}
	else
	{
		myAvatar->SetDirection(direction.Normalize());
		myAvatar->SetPosition(myAvatar->GetPosition() + direction * distanceToMove);
	}

	myAvatar->SetDrawPos(Vector2f(myAvatar->GetPosition().myX + m_drawOffsetX, myAvatar->GetPosition().myY + m_drawOffsetY));
}

void Pacman::UpdateGhost(Ghost& ghost, float deltaTime)
{	
	Vector2f nextTile = ghost.GetCurrentTile() + ghost.GetDesiredMovement();

	if (ghost.IsAtDestination())
	{
		if (!ghost.GetPath().empty())
		{
			PathmapTile* nextTile = ghost.GetPath().front();
			delete ghost.GetPath().front();
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

	Vector2f destination = ghost.GetNextTile() * m_tileSize;
	Vector2f direction = destination - ghost.GetPosition();

	float distanceToMove = deltaTime * ghost.GetMovementSpeed();

	if (distanceToMove > direction.Length())
	{
		ghost.SetPosition(destination);
		ghost.SetCurrentTile(ghost.GetNextTile());
	}
	else
	{
		ghost.SetDirection(direction.Normalize());
		ghost.SetPosition(ghost.GetPosition() + direction * distanceToMove);
	}

	ghost.SetDrawPos(Vector2f(ghost.GetPosition().myX + m_drawOffsetX, ghost.GetPosition().myY + m_drawOffsetY));
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

bool Pacman::AvatarGhostIntersection(Ghost& ghost, float dist)
{
	if ((ghost.GetPosition() - myAvatar->GetPosition()).Length() < dist)
	{
		IntersectGhost(ghost);
		return true;
	}
	return false;
}

void Pacman::ResetGhost(Ghost& ghost)
{
	ghost.SetPosition(ghost.GetSpawnPos());
	ghost.GetPath().clear();
	ghost.SetDesiredMovement(Vector2f(0, -1));
	ghost.SetCurrentTile(ghost.GetPosition() /= (float)m_tileSize);
	ghost.SetNextTile(ghost.GetCurrentTile());
}

void Pacman::ResetAvatar()
{
	myAvatar->SetPosition(myAvatar->GetSpawnPos());
	myAvatar->SetCurrentTile(myAvatar->GetPosition() /= m_tileSize);
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
	delete m_pScoreText;
	delete m_pLivesText;
	delete m_pFpsText;

	myWorld->Shutdown();
	delete myAvatar;
	delete redGhost;
	delete tealGhost;
	delete pinkGhost;
	delete orangeGhost;
	delete myWorld;

	delete m_pAvatarOpenLeft;
	delete m_pAvatarOpenRight;
	delete m_pAvatarOpenUp;
	delete m_pAvatarOpenDown;

	delete m_pAvatarClosedLeft;
	delete m_pAvatarClosedRight;
	delete m_pAvatarClosedUp;
	delete m_pAvatarClosedDown;

	delete m_pRedGhost;
	delete m_pTealGhost;
	delete m_pPinkGhost;
	delete m_pOrangeGhost;
	delete m_pVulnerableGhost;
	delete m_pDeadGhost;
}
