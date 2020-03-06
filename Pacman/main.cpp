#include "Pacman.h"

int main(int argc, char **argv)
{
	bool status = EXIT_FAILURE;
	Core* core = new Core();

	PACMAN_DESC pacman_desc = {0};
	Pacman* pacman = Pacman::Create();
	if (pacman->Init(*core, pacman_desc))
	{
		status = pacman->Run(*core);
	}

	delete core;
	delete pacman;

	return status;
}

