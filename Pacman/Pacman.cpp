#include "Pacman.h"

Pacman* Pacman::Create()
{
	return new Pacman();
}

Pacman::Pacman() :
	myTimeToNextUpdate(0.f),
	myNextMovement(-1.f, 0.f),
	myScore(0), myFps(0), myLives(3),
	myGhostGhostCounter(0.f),
	m_drawOffsetX(220.0f), m_drawOffsetY(60.0f),
	m_tileSize(22), m_win(false),
	m_ghostCounterDefault(20.0f), m_ghostCounterDuration(20.0f), 
	m_ghostCounterFlag(false), m_ghostCounterReducer(1.0f),
	m_avatarOpen(true), m_toggleAvatarDrawDefault(0.1f), 
	m_toggleAvatarDrawCooldown(0.1f), m_toggleAvatarDrawReducer(1.0f)
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

	std::string drawTextString;
	std::stringstream scoreStream;
	scoreStream << "Score: " << myScore << "   "; //intentionally left blank
	drawTextString = scoreStream.str();
	m_pScoreText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 20, 50);
	scoreStream.flush();

	std::stringstream liveStream;
	liveStream << "Lives: " << myLives;
	drawTextString = liveStream.str();
	m_pLivesText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 20, 80);
	liveStream.flush();

	std::stringstream fpsStream;
	fpsStream << "FPS: " << myFps << "   "; //intentionally left blank
	drawTextString = fpsStream.str();
	m_pFpsText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 880, 50);
	fpsStream.flush();

	m_pAvatarOpenLeft = new DrawEntity(core.GetRenderer(), pacman_desc.avatarOpenLeftImage, 0, 0);
	m_pAvatarOpenRight = new DrawEntity(core.GetRenderer(), pacman_desc.avatarOpenRightImage, 0, 0);
	m_pAvatarOpenUp = new DrawEntity(core.GetRenderer(), pacman_desc.avatarOpenUpImage, 0, 0);
	m_pAvatarOpenDown = new DrawEntity(core.GetRenderer(), pacman_desc.avatarOpenDownImage, 0, 0);

	m_pAvatarClosedLeft = new DrawEntity(core.GetRenderer(), pacman_desc.avatarClosedLeftImage, 0, 0);
	m_pAvatarClosedRight = new DrawEntity(core.GetRenderer(), pacman_desc.avatarClosedRightImage, 0, 0);
	m_pAvatarClosedUp = new DrawEntity(core.GetRenderer(), pacman_desc.avatarClosedUpImage, 0, 0);
	m_pAvatarClosedDown = new DrawEntity(core.GetRenderer(), pacman_desc.avatarClosedDownImage, 0, 0);
	
	m_pRedGhost = new DrawEntity(core.GetRenderer(), pacman_desc.redGhostImage, 0, 0);
	m_pTealGhost = new DrawEntity(core.GetRenderer(), pacman_desc.tealGhostImage, 0, 0);
	m_pPinkGhost = new DrawEntity(core.GetRenderer(), pacman_desc.pinkGhostImage, 0, 0);
	m_pOrangeGhost = new DrawEntity(core.GetRenderer(), pacman_desc.orangeGhostImage, 0, 0);

	m_pVulnerableGhost = new DrawEntity(core.GetRenderer(), pacman_desc.ghostVulnerableImage, 0, 0);
	m_pDeadGhost = new DrawEntity(core.GetRenderer(), pacman_desc.ghostDeadImage, 0, 0);

	WORLD_DESC world_desc;
	world_desc.playfieldImage = pacman_desc.playfieldImage;
	world_desc.dotImage = pacman_desc.dotImage;
	world_desc.bigDotImage = pacman_desc.bigDotImage;
	world_desc.cherryImage = pacman_desc.cherryImage;

	myWorld->Init(core, world_desc);

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
		if (!m_win && myLives > 0)
		{
			OnUpdate(elapsedTime);
		}
		core.OnStartFrameRender();
		Draw(core);
		core.OnEndFrameRender();
	
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

	int tileSize = 22;
	Vector2f destination = ghost.GetNextTile() * tileSize;
	Vector2f direction = destination - ghost.GetPosition();

	float distanceToMove = deltaTime * ghost.GetMovementSpeed();

	if (distanceToMove > direction.Length())
	{
		ghost.SetPosition(destination);
		ghost.SetCurrentTile(ghost.GetNextTile());
	}
	else
	{
		direction.Normalize();
		ghost.SetPosition(ghost.GetPosition() + direction * distanceToMove);
	}

	ghost.SetDrawPos(Vector2f(ghost.GetPosition().myX + m_drawOffsetX, ghost.GetPosition().myY + m_drawOffsetY));
}

void Pacman::UpdateScore(int amount)
{
	myScore += amount;
	m_updateUI = true;
}

void Pacman::UpdateLives(int amount)
{
	myLives += amount;
	if (myLives < 1)
	{
		myLives = 0;
	}
	m_updateUI = true;
}

void Pacman::UpdateFPS(float deltaTime)
{
	myFps = (int)(1 / deltaTime);
	m_updateUI = true;
}

void Pacman::DrawAvatar(Core& core)
{
	if (myAvatar->GetDirection().myX > 0.0f)
	{
		if (m_avatarOpen)
		{
			core.DrawObject(*m_pAvatarClosedRight, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else 
		{
			core.DrawObject(*m_pAvatarOpenRight, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}	
	}
	else if (myAvatar->GetDirection().myX < 0.0f)
	{
		if (m_avatarOpen)
		{
			core.DrawObject(*m_pAvatarClosedLeft, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			core.DrawObject(*m_pAvatarOpenLeft, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		
	}
	else if (myAvatar->GetDirection().myY > 0.0f)
	{
		if (m_avatarOpen)
		{
			core.DrawObject(*m_pAvatarClosedDown, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);	
		}
		else
		{
			core.DrawObject(*m_pAvatarOpenDown, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		
	}
	else if (myAvatar->GetDirection().myY < 0.0f)
	{
		if (m_avatarOpen)
		{
			core.DrawObject(*m_pAvatarClosedUp, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		else
		{
			core.DrawObject(*m_pAvatarOpenUp, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
		}
		
	}
}

void Pacman::DrawGhost(
	Core& core, Ghost& ghost, 
	DrawEntity& deadGhost, 
	DrawEntity& vulnerableGhost, 
	DrawEntity& defaultGhost)
{
	if (ghost.GetDeadFlag())
	{
		core.DrawObject(deadGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
	else if (m_ghostCounterFlag)
	{
		core.DrawObject(vulnerableGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
	else
	{
		core.DrawObject(defaultGhost, ghost.GetDrawPos().myX, ghost.GetDrawPos().myY);
	}
}

void Pacman::RedrawUI(Core& core)
{
	std::string drawTextString;
	std::stringstream drawTextStream;
	if (m_win)
	{
		drawTextStream << "You win!   ";
	}
	else if (myLives < 1)
	{
		drawTextStream << "You lose!   ";
	}
	else
	{
		drawTextStream << "Score: " << myScore << "   ";
	}
	drawTextString = drawTextStream.str();
	m_pScoreText->SetText(core.GetRenderer(), drawTextString.c_str(), m_desc.uiFont);
	drawTextStream.flush();

	std::stringstream drawTextStream1;
	drawTextStream1 << "Lives: " << myLives;
	drawTextString = drawTextStream1.str();
	m_pLivesText->SetText(core.GetRenderer(), drawTextString.c_str(), m_desc.uiFont);
	drawTextStream1.flush();

	std::stringstream drawTextStream2;
	drawTextStream2 << "FPS: " << myFps << "   ";
	drawTextString = drawTextStream2.str();
	m_pFpsText->SetText(core.GetRenderer(), drawTextString.c_str(), m_desc.uiFont);
	drawTextStream2.flush();

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
	myGhostGhostCounter = value;
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
	ghost.SetPosition(Vector2f(13 * m_tileSize, 13 * m_tileSize));
	ghost.GetPath().clear();
	ghost.SetDesiredMovement(Vector2f(0, -1));
	ghost.SetCurrentTile(ghost.GetPosition() /= (float)m_tileSize);
	ghost.SetNextTile(ghost.GetCurrentTile());
}

void Pacman::ResetAvatar()
{
	myAvatar->SetPosition(Vector2f(13 * m_tileSize, 22 * m_tileSize));
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

bool Pacman::Draw(Core& core)
{
	myWorld->Draw(core);

	DrawAvatar(core);

	DrawGhost(core, *redGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pRedGhost);
	DrawGhost(core, *tealGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pTealGhost);
	DrawGhost(core, *pinkGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pPinkGhost);
	DrawGhost(core, *orangeGhost, *m_pDeadGhost, *m_pVulnerableGhost, *m_pOrangeGhost);
	
	if (m_updateUI)
	{
		RedrawUI(core);
	}

	core.DrawObject(*m_pScoreText);
	core.DrawObject(*m_pLivesText);
	core.DrawObject(*m_pFpsText);

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
