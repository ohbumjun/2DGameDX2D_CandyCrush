#pragma once

#include "GameInfo.h"

class CEngine {

private :
	HINSTANCE m_hInst;
	HWND m_hWnd;
	Resolution m_RS;
	class CTimer* m_Timer;
	static bool m_Loop;
	bool m_Start;
	Engine_Space m_Space;
	float m_ClearColor[4];
	bool m_Play;

public :
	bool Init(HINSTANCE hInst, const TCHAR* Name, unsigned int Width, unsigned int Height,
		int IconID, bool WindowMode = true);
	bool Init(HINSTANCE hInst, HWND hWnd, unsigned int Width, unsigned int Height,
		bool WindowMode = true);
	int Run();
	void Logic();
public :
	Engine_Space GetEngineSpace() const
{
		return m_Space;
}
private :
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Render();
private :
	ATOM Register(const TCHAR* Name, int IconID);
	BOOL Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	DECLARE_SINGLE(CEngine);
};