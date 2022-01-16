#pragma once

#include "GameInfo.h"

class CEngine
{
private :
	HINSTANCE m_hInst;;
	HWND m_hWnd;
	Resolution m_RS;

public :
	// ���ڷ� Client�� ����� ������ ����
	// ex) �ػ�, TitleBarName ��
	// ex) IconID�� ���, Client���� Icon�� ������ְ�, �ش� ID�� Engine���� �����ϴ� ���¸� ��� �� ���̴�.
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

