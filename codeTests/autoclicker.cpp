#include <windows.h>


void leftClick() {
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}
void rightClick() {
	INPUT    Input = { 0 };
	// right down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}
void moveMouse(int x, int y) {
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	double fx = x * (65535.0f / fScreenWidth);
	double fy = y * (65535.0f / fScreenHeight);
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}

int main() {
	bool clicking = false;
	int speed = 1;
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		//activate clicker
		if (GetAsyncKeyState(VK_RETURN)) {
			clicking = true;
			while (clicking) {
				leftClick();
				Sleep(speed);
				if (GetAsyncKeyState(0x31)) {
					speed = 1;
				}
				if (GetAsyncKeyState(0x32)) {
					speed = 5;
				}
				if (GetAsyncKeyState(0x33)) {
					speed = 20;
				}
				if (GetAsyncKeyState(0x34)) {
					speed = 50;
				}
				if (GetAsyncKeyState(0x35)) {
					speed = 100;
				}
				if (GetAsyncKeyState(0x36)) {
					speed = 200;
				}
				if (GetAsyncKeyState(0x37)) {
					speed = 400;
				}
				if (GetAsyncKeyState(0x38)) {
					speed = 800;
				}
				if (GetAsyncKeyState(0x39)) {
					speed = 1600;
				}
				//deactivate clicker
				if (GetAsyncKeyState(VK_SPACE)) {
					clicking = false;
				}
			}
		}
	}
	return 0;
}