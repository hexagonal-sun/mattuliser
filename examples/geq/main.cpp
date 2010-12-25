#include "../../src/visualiserWin.h"
#include <SDL.h>

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	visualiserWin win(0, true, 300, 400, 0);
}
