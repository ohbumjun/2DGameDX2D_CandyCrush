#pragma once

#include "GameInfo.h"

class CEngine
{
private :
	HINSTANCE m_hInst;;
	HWND m_hWnd;
	Resolution m_RS;

public :
	// 인자로 Client가 사용할 변수들 세팅
	// ex) 해상도, TitleBarName 등
	// ex) IconID의 경우, Client에서 Icon을 만들어주고, 해당 ID를 Engine으로 전달하는 형태를 띄게 할 것이다.
	bool Init(HINSTANCE hInst, const TCHAR* TitleBarName, unsigned int Width, unsigned int Height, 
		int IconID, bool WindowMode = true);
	int Run();

private :
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render(float DeltaTime);
private :
	ATOM Register(const TCHAR* Name, int IconID);
	BOOL Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine);
};

