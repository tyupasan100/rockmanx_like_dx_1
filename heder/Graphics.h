#pragma once

class Graphics
{
public:
	static int internalScreen;
	
	static void Init();		//初期化
	static void Begin();	//ループの最初
	static void End();		//ループの最後
};