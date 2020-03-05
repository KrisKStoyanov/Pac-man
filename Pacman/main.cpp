#include "Core.h"

int main(int argc, char **argv)
{
	bool status = EXIT_FAILURE;
	Core* core = new Core();
	if (core->Init())
	{
		status = core->Run();
	}
	return status;
}

