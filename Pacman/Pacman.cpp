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
	m_tileSize(22), m_win(false)
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

		m_isRunning = Update(core, elapsedTime);
		core.OnStartFrameRender();
		Draw(core);
		if (m_redrawUI)
		{
			RedrawUI(core.GetRenderer());
		}
		core.OnEndFrameRender();
	
		lastFrame = currentFrame;
	}

	Shutdown();
	core.Shutdown();
	return EXIT_SUCCESS;
}

bool Pacman::Update(Core& core, float aTime)
{
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
			return false;
		}
	}

	UpdateAvatar(aTime);

	UpdateGhost(*redGhost, aTime);
	UpdateGhost(*tealGhost, aTime);
	UpdateGhost(*pinkGhost, aTime);
	UpdateGhost(*orangeGhost, aTime);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition(), m_win))
	{
		UpdateScore(10);
	}

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition(), m_win))
	{
		UpdateScore(20);
		myGhostGhostCounter = 20.f;
		redGhost->myIsClaimableFlag = true;
		tealGhost->myIsClaimableFlag = true;
		pinkGhost->myIsClaimableFlag = true;
		orangeGhost->myIsClaimableFlag = true;
	}

	myGhostGhostCounter -= aTime;

	if (myGhostGhostCounter < 0.1f)
	{
		redGhost->myIsClaimableFlag = false;
		tealGhost->myIsClaimableFlag = false;
		pinkGhost->myIsClaimableFlag = false;
		orangeGhost->myIsClaimableFlag = false;
	}

	if ((redGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter < 0.1f)
		{
			UpdateLives(-1);
			
			myAvatar->SetPosition(Vector2f(13*22,22*22));
			myAvatar->Reset();
			redGhost->SetPosition(Vector2f(13*22,13*22));
			redGhost->Reset();
			tealGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			tealGhost->Reset();
			pinkGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			pinkGhost->Reset();
			orangeGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			orangeGhost->Reset();
		}
		else if (redGhost->myIsClaimableFlag && !redGhost->myIsDeadFlag)
		{
			redGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			redGhost->Reset();
			myScore += 50;
			redGhost->myIsDeadFlag = true;
			redGhost->Die(myWorld);
		}
	}

	if ((tealGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter < 0.1f)
		{
			UpdateLives(-1);

			myAvatar->SetPosition(Vector2f(13 * 22, 22 * 22));
			myAvatar->Reset();
			redGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			redGhost->Reset();
			tealGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			tealGhost->Reset();
			pinkGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			pinkGhost->Reset();
			orangeGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			orangeGhost->Reset();
		}
		else if (tealGhost->myIsClaimableFlag && !tealGhost->myIsDeadFlag)
		{
			tealGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			tealGhost->Reset();
			myScore += 50;
			tealGhost->myIsDeadFlag = true;
			tealGhost->Die(myWorld);
		}
	}

	if ((pinkGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter < 0.1f)
		{
			UpdateLives(-1);

			myAvatar->SetPosition(Vector2f(13 * 22, 22 * 22));
			myAvatar->Reset();
			redGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			redGhost->Reset();
			tealGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			tealGhost->Reset();
			pinkGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			pinkGhost->Reset();
			orangeGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			orangeGhost->Reset();
		}
		else if (pinkGhost->myIsClaimableFlag && !pinkGhost->myIsDeadFlag)
		{
			pinkGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			pinkGhost->Reset();
			myScore += 50;
			pinkGhost->myIsDeadFlag = true;
			pinkGhost->Die(myWorld);
		}
	}

	if ((orangeGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter < 0.1f)
		{
			UpdateLives(-1);

			myAvatar->SetPosition(Vector2f(13 * 22, 22 * 22));
			myAvatar->Reset();
			redGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			redGhost->Reset();
			tealGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			tealGhost->Reset();
			pinkGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			pinkGhost->Reset();
			orangeGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			orangeGhost->Reset();
		}
		else if (orangeGhost->myIsClaimableFlag && !orangeGhost->myIsDeadFlag)
		{
			orangeGhost->SetPosition(Vector2f(13 * 22, 13 * 22));
			orangeGhost->Reset();
			myScore += 50;
			orangeGhost->myIsDeadFlag = true;
			orangeGhost->Die(myWorld);
		}
	}
	
	if (aTime > 0)
		UpdateFPS(aTime);

	return true;
}

bool Pacman::CheckEndGameCondition()
{
	return false;
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

	//if (myIsDeadFlag)
	//	speed = 120.f;

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

			ghost.myIsDeadFlag = false;
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
	m_redrawUI = true;
}

void Pacman::UpdateLives(int amount)
{
	myLives += amount;
	if (myLives < 1)
	{
		myLives = 0;
	}
	m_redrawUI = true;
}

void Pacman::UpdateFPS(float deltaTime)
{
	myFps = (int)(1 / deltaTime);
	m_redrawUI = true;
}

void Pacman::RedrawUI(SDL_Renderer* renderer)
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
	m_pScoreText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream.flush();

	std::stringstream drawTextStream1;
	drawTextStream1 << "Lives: " << myLives;
	drawTextString = drawTextStream1.str();
	m_pLivesText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream1.flush();

	std::stringstream drawTextStream2;
	drawTextStream2 << "FPS: " << myFps << "   ";
	drawTextString = drawTextStream2.str();
	m_pFpsText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream2.flush();

	m_redrawUI = false;
}

bool Pacman::Draw(Core& core)
{
	myWorld->Draw(core);

	if (myAvatar->GetDirection().myX > 0.0f)
	{
		core.DrawObject(*m_pAvatarClosedRight, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
	}
	else if (myAvatar->GetDirection().myX < 0.0f)
	{
		core.DrawObject(*m_pAvatarClosedLeft, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
	}
	else if (myAvatar->GetDirection().myY > 0.0f)
	{
		core.DrawObject(*m_pAvatarClosedDown, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
	}
	else if (myAvatar->GetDirection().myY < 0.0f)
	{
		core.DrawObject(*m_pAvatarClosedUp, myAvatar->GetDrawPos().myX, myAvatar->GetDrawPos().myY);
	}

	if (redGhost->myIsDeadFlag)
	{
		core.DrawObject(*m_pDeadGhost, redGhost->GetDrawPos().myX, redGhost->GetDrawPos().myY);
	}
	else if (redGhost->myIsClaimableFlag)
	{
		core.DrawObject(*m_pVulnerableGhost, redGhost->GetDrawPos().myX, redGhost->GetDrawPos().myY);
	}
	else 
	{
		core.DrawObject(*m_pRedGhost, redGhost->GetDrawPos().myX, redGhost->GetDrawPos().myY);
	}
	
	if (tealGhost->myIsDeadFlag)
	{
		core.DrawObject(*m_pDeadGhost, tealGhost->GetDrawPos().myX, tealGhost->GetDrawPos().myY);
	}
	else if (tealGhost->myIsClaimableFlag)
	{
		core.DrawObject(*m_pVulnerableGhost, tealGhost->GetDrawPos().myX, tealGhost->GetDrawPos().myY);
	}
	else
	{
		core.DrawObject(*m_pTealGhost, tealGhost->GetDrawPos().myX, tealGhost->GetDrawPos().myY);
	}

	if (pinkGhost->myIsDeadFlag)
	{
		core.DrawObject(*m_pDeadGhost, pinkGhost->GetDrawPos().myX, pinkGhost->GetDrawPos().myY);
	}
	else if (pinkGhost->myIsClaimableFlag)
	{
		core.DrawObject(*m_pVulnerableGhost, pinkGhost->GetDrawPos().myX, pinkGhost->GetDrawPos().myY);
	}
	else
	{
		core.DrawObject(*m_pPinkGhost, pinkGhost->GetDrawPos().myX, pinkGhost->GetDrawPos().myY);
	}

	if (orangeGhost->myIsDeadFlag)
	{
		core.DrawObject(*m_pDeadGhost, orangeGhost->GetDrawPos().myX, orangeGhost->GetDrawPos().myY);
	}
	else if (orangeGhost->myIsClaimableFlag)
	{
		core.DrawObject(*m_pVulnerableGhost, orangeGhost->GetDrawPos().myX, orangeGhost->GetDrawPos().myY);
	}
	else
	{
		core.DrawObject(*m_pOrangeGhost, orangeGhost->GetDrawPos().myX, orangeGhost->GetDrawPos().myY);
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
