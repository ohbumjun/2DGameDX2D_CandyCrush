#include "Engine.h"
#include "Device.h"
#include "Timer.h"
#include "Input.h"

CEngine::CEngine() :
	m_Timer(nullptr),
m_Start(false),
m_Play(true),
m_Space(Engine_Space::Space2D),
m_ClearColor { }
{}

CEngine::~CEngine()
{
	SAFE_DELETE(m_Timer);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* Name, 
	unsigned Width, unsigned Height, int IconID, bool WindowMode)
{
	m_hInst = hInst;

	m_RS.Width = Width;
	m_RS.Height = Height;

	Register(Name, IconID);
	Create(Name);

	return Init(m_hInst, m_hWnd, Width, Height, WindowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned Width, unsigned Height, bool WindowMode)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

	m_RS.Width = Width;
	m_RS.Height = Height;

	if (!CDevice::GetInst()->Init(hWnd, Width, Height, WindowMode))
		return false;

	if (!CInput::GetInst()->Init(hInst, hWnd))
		return false;

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	else
	{
		Logic();
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	if (!m_Start)
	{
		m_Start = true;

	}

	float DeltaTime = m_Timer->Update();

	if (Update(DeltaTime))
		return;

	if (PostUpdate(DeltaTime))
		return;

	Render();
}

bool CEngine::Update(float DeltaTime)
{

	CInput::GetInst()->Update(DeltaTime);

	return true;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	return true;
}

bool CEngine::Render()
{
	return true;
}

ATOM CEngine::Register(const TCHAR* Name, int IconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = Name;
	wcex.hIconSm = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* Name)
{
	m_hWnd = CreateWindowW(Name, Name, WS_OVERLAPPEDWINDOW, 
		0, 0, m_RS.Width, m_RS.Height, nullptr, nullptr,
		m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT rc = { 0, 0, (LONG)m_RS.Width, (LONG)m_RS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.top - rc.bottom,
		SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		// HDC : 화면에 출력하기 위한 그리기 도구이다.
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
