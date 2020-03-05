#include "Pacman.h"

int main(int argc, char **argv)
{
	bool status = EXIT_FAILURE;
	Core* core = new Core();
	Pacman* pacman = Pacman::Create();
	if (pacman->Init(*core))
	{
		status = pacman->Run(*core);
	}

	delete core;
	delete pacman;

	return status;
}

