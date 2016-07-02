#pragma once

struct input {
	short x;
	short y;
	short padX;
	short padY;
	short buttons;
};

void bt_server();
void getInput(input* in);
