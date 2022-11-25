#include "GameController.h"
#include <windows.h>

// TODO LIST
// - Try to fix bezier red dots
// - Try to fix bezier speeds
// - Check why it does not fully reset correctly when restarting
// - Try to make the bot hit the first object instead of the user
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