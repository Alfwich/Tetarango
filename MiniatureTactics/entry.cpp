#include "src/game/GameApplication.h"

int main(int argc, char* args[]) {
	std::make_shared<MTGame::GameApplication>()->run(argc, args);
	return 0;
}