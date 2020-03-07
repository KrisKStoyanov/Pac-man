#include "Pacman.h"

int main(int argc, char **argv)
{
	bool status = EXIT_FAILURE;

	CORE_DESC core_desc = {};
	Core* core = new Core(core_desc);

	PACMAN_DESC pacman_desc = {};
	Pacman* pacman = new Pacman(pacman_desc);
	if (pacman->Init(*core, pacman_desc))
	{
		status = pacman->Run(*core);
	}

	SAFE_DELETE(pacman);
	SAFE_DELETE(core);

	return status;
}

