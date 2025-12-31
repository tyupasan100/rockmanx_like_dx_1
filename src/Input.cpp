#include "Common.h"

char Input::now[256];
char Input::prev[256];

void Input::Update()
{
    memcpy(prev, now, sizeof(now));
    GetHitKeyStateAll(now);
}

bool Input::Press(int key)
{
    return now[key] != 0;
}

bool Input::Trigger(int key)
{
    return now[key] && !prev[key];
}

bool Input::Release(int key)
{
    return !now[key] && prev[key];
}
