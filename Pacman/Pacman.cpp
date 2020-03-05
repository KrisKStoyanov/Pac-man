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

bool Pacman::Init(Core& core)
{
	m_isRunning = core.Init();
	myWorld->Init();

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

		Update(elapsedTime);

		core.OnStartFrameRender();

		Draw(core);

		core.OnEndFrameRender();
	
		lastFrame = currentFrame;
	}

	Shutdown();
	core.Shutdown();
	return EXIT_SUCCESS;
}

bool Pacman::Update(float aTime)
{
	//if (!UpdateInput())
	//	return false;

	//if (CheckEndGameCondition())
	//{
	//	myDrawer->DrawText("You win!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70);
	//	return true;
	//}
	//else if (myLives <= 0)
	//{
	//	myDrawer->DrawText("You lose!", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 70);	
	//	return true;
	//}

	MoveAvatar();
	myAvatar->Update(aTime);
	myGhost->Update(aTime, myWorld);

	if (myWorld->HasIntersectedDot(myAvatar->GetPosition()))
		myScore += 10;

	myGhostGhostCounter -= aTime;

	if (myWorld->HasIntersectedBigDot(myAvatar->GetPosition()))
	{
		myScore += 20;
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
		myFps = (int) (1 / aTime);

	return true;
}

bool Pacman::UpdateInput()
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP])
		myNextMovement = Vector2f(0.f, -1.f);
	else if (keystate[SDL_SCANCODE_DOWN])
		myNextMovement = Vector2f(0.f, 1.f);
	else if (keystate[SDL_SCANCODE_RIGHT])
		myNextMovement = Vector2f(1.f, 0.f);
	else if (keystate[SDL_SCANCODE_LEFT])
		myNextMovement = Vector2f(-1.f, 0.f);

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

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

bool Pacman::Draw(Core& core)
{
	myWorld->Draw(&core);
	myAvatar->Draw(&core);
	myGhost->Draw(&core);

	std::string scoreString;
	std::stringstream scoreStream;
	scoreStream << myScore;
	scoreString = scoreStream.str();
	core.DrawText("Score", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 50);
	core.DrawText(scoreString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 50);

	std::string livesString;
	std::stringstream liveStream;
	liveStream << myLives;
	livesString = liveStream.str();
	core.DrawText("Lives", "freefont-ttf\\sfd\\FreeMono.ttf", 20, 80);
	core.DrawText(livesString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 90, 80);

	core.DrawText("FPS", "freefont-ttf\\sfd\\FreeMono.ttf", 880, 50);
	std::string fpsString;
	std::stringstream fpsStream;
	fpsStream << myFps;
	fpsString = fpsStream.str();
	core.DrawText(fpsString.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 930, 50);

	return true;
}

void Pacman::Shutdown()
{
}
