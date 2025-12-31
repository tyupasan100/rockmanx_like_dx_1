#include "Common.h"

int Graphics::internalScreen = -1;

void Graphics::Init()
{
	internalScreen = MakeScreen(SCREEN_WIDTH, SCREEN_HEIGHT, true);
}

void Graphics::Begin()
{
	SetDrawScreen(internalScreen);
	ClearDrawScreen();
}

void Graphics::End()
{
	SetDrawScreen(DX_SCREEN_BACK);
	DrawExtendGraph(0, 0, SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, internalScreen, FALSE);
}