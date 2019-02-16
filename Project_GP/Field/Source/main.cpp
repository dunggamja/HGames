#include "PCH.h"

int main()
{
	auto	newGame = std::make_shared<CSceneGame>();
	if (!newGame)
		throw std::bad_alloc();

	newGame->InitInstance();
	
	auto	previous = GetCurrentTime();
	auto	lag = 0.;
	auto	MS_PER_UPDATE = 1000 / 10;

	while (!newGame->IsExit())
	{
		auto	current = GetCurrentTime();
		auto	elapsed = current - previous;
		previous = current;
		lag += elapsed;

		if (MS_PER_UPDATE > lag)
			continue;

		newGame->Update(lag);
		newGame->Render();

		lag = 0;
	}

	int nExit = 0;
	std::cout << "Game Over." << std::endl;
	std::cin >> nExit;

	return 0;
}