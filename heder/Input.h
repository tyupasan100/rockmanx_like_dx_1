#pragma once

class Input 
{
public:
    static void Update();

    static bool Press(int key);   // ‰Ÿ‚³‚ê‚Ä‚¢‚é
    static bool Trigger(int key); // ‰Ÿ‚µ‚½uŠÔ
    static bool Release(int key); // —£‚µ‚½uŠÔ

private:
    static char now[256];   //‰Ÿ‚µ‚½key
    static char prev[256];  
};