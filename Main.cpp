#include "Game.h"

int main()
{
	auto game = std::make_unique<Game>("Debreky");
	game->Run();
	return 0;
}