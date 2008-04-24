// Includes
#include "demo.h"

int main(int argc, char* argv[]) {

	// Create the demo application
	Demo demo;
	demo.main(argc, argv);

#if USING_SDL

	// Shut down SDL systems
	SDL_Quit();

#endif

	return 0;
}
