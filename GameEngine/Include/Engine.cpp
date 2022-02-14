#include "Engine.h"
#include "Device.h"
#include "Timer.h"
#include "Input.h"
#include "PathManager.h"
#include "Render/DepthStencilState.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Render/RenderManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine() :
	m_Timer(nullptr),
m_Start(false),
m_Play(true),
m_Space(Engine_Space::Space2D),
m_ClearColor {0,0,1,1}
// m_ClearColor{}
{}

CEngine::~CEngine()
{
	SAFE_DELETE(m_Timer);
	CInput::DestroyInst();
	CDevice::DestroyInst();
	CSceneManager::DestroyInst();
	CPathManager::DestroyInst();
	CRenderManager::DestroyInst();
	CResourceManager::DestroyInst(); // 가장 마지막에 지워주자 ..!
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

	m_Timer = new CTimer;

	// 장치
	if (!CDevice::GetInst()->Init(hWnd, Width, Height, WindowMode))
		return false;

	// PathManager --> ResourceManager보다 앞에 
	if (!CPathManager::GetInst()->Init())
		return false;


	// ResourceManager --> SceneManager 보다 앞에 세팅하기
	if (!CResourceManager::GetInst()->Init())
		return false;

	// Render 
	if (!CRenderManager::GetInst()->Init())
		return false;

	// 입력 
	if (!CInput::GetInst()->Init(hInst, hWnd))
		return false;

	// Scene Manager
	if (!CSceneManager::GetInst()->Init())
		return false;


	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	while (m_Loop)
	{
		if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	if (!m_Start)
	{
		m_Start = true;
		CSceneManager::GetInst()->Start();
	}

	// Timer
	float DeltaTime = m_Timer->Update();

	// Input
	CInput::GetInst()->Update(DeltaTime);

	if (Update(DeltaTime))
		return;

	if (PostUpdate(DeltaTime))
		return;

	Render();
}

bool CEngine::Update(float DeltaTime)
{
	// SceneManager
	if (CSceneManager::GetInst()->Update(DeltaTime))
		return true;

	return false;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	if (CSceneManager::GetInst()->PostUpdate(DeltaTime))
		return true;

	return false;
}

bool CEngine::Render()
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	CRenderManager::GetInst()->Render();

	CDevice::GetInst()->FlipAndRender();

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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = Name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IconID));

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

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
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
