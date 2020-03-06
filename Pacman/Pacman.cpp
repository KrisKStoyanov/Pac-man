#include "Pacman.h"

Pacman* Pacman::Create()
{
	return new Pacman();
}

Pacman::Pacman()
: myTimeToNextUpdate(0.f)
, myNextMovement(-1.f,0.f)
, myScore(0)
, myFps(0)
, myLives(3)
, myGhostGhostCounter(0.f)
{
	myAvatar = new Avatar(Vector2f(13*22,22*22));
	myGhost = new Ghost(Vector2f(13*22,13*22));
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
	scoreStream << myScore;
	drawTextString = "Score: " + scoreStream.str();
	m_pScoreText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 20, 50);
	scoreStream.flush();

	std::stringstream liveStream;
	liveStream << myLives;
	drawTextString = "Lives: " + liveStream.str();
	m_pLivesText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 20, 80);
	liveStream.flush();

	std::stringstream fpsStream;
	fpsStream << myFps;
	drawTextString = "FPS: " + fpsStream.str();
	m_pFpsText = new DrawTextEntity(core.GetRenderer(), drawTextString.c_str(), pacman_desc.uiFont, 880, 50);
	fpsStream.flush();

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

	MoveAvatar();
	myAvatar->Update(aTime);
	myGhost->Update(aTime, myWorld);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		UpdateScore(10);
	
	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		UpdateScore(20);
		myGhostGhostCounter = 20.f;
		myGhost->myIsClaimableFlag = true;
	}

	if (myGhostGhostCounter <= 0)
	{
		myGhost->myIsClaimableFlag = false;
	}

	if ((myGhost->GetPosition() - myAvatar->GetPosition()).Length() < 10.f)
	{
		if (myGhostGhostCounter < 0.1f)
		{
			myLives--;

			myAvatar->SetPosition(Vector2f(13*22,22*22));
			myGhost->SetPosition(Vector2f(13*22,13*22));
		}
		else if (myGhost->myIsClaimableFlag && !myGhost->myIsDeadFlag)
		{
			myScore += 50;
			myGhost->myIsDeadFlag = true;
			myGhost->Die(myWorld);
		}
	}
	
	if (aTime > 0)
		UpdateFPS(aTime);

	return true;
}

void Pacman::MoveAvatar()
{
	if (myAvatar->IsAtDestination())
	{
		int nextTileX = myAvatar->GetCurrentTileX() + myNextMovement.myX;
		int nextTileY = myAvatar->GetCurrentTileY() + myNextMovement.myY;
		if (myWorld->TileIsValid(nextTileX, nextTileY))
		{
			myAvatar->SetNextTile(nextTileX, nextTileY);
		}
	}
}

bool Pacman::CheckEndGameCondition()
{
	return false;
}

void Pacman::UpdateScore(int amount)
{
	myScore += amount;
	m_redrawUI = true;
}

void Pacman::UpdateLives(int amount)
{
	myLives = +amount;
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
	drawTextStream << myScore;
	drawTextString = "Score: " + drawTextStream.str();
	m_pScoreText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream.flush();

	std::stringstream drawTextStream1;
	drawTextStream1 << myLives;
	drawTextString = "Lives: " + drawTextStream1.str();
	m_pLivesText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream1.flush();

	std::stringstream drawTextStream2;
	drawTextStream2 << myFps;
	drawTextString = "FPS: " + drawTextStream2.str();
	m_pFpsText->SetText(renderer, drawTextString.c_str(), m_desc.uiFont);
	drawTextStream2.flush();

	m_redrawUI = false;
}

bool Pacman::Draw(Core& core)
{
	myWorld->Draw(core);
	myAvatar->Draw(&core);
	myGhost->Draw(&core);

	core.DrawObject(*m_pScoreText);
	core.DrawObject(*m_pLivesText);
	core.DrawObject(*m_pFpsText);

	return true;
}

void Pacman::Shutdown()
{
}
