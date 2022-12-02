#include "GameController.h"
#include <windows.h>

int main() {
	GameController game;

	while (true) {
		///press enter to change to current song
		if (GetAsyncKeyState(VK_RETURN)) {
			game.ChangeSong();
		}
		///press 'Z' to start
		if (GetAsyncKeyState(0x5A)) {
			game.StartSong();
		}
	}
	return 0;
}