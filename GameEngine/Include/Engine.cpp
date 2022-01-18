#include "Engine.h"
#include "Timer.h"

DEFINITION_SINGLE(CEngine);

bool CEngine::m_Loop = true;

CEngine::CEngine() :
m_Play(true),
m_Start(false),
m_ClearColor {},
m_Timer(nullptr),
m_Space(Engine_Space::Space2D)
{
	// 	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

CEngine::~CEngine()
{
	SAFE_DELETE(m_Timer);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* Name, unsigned Width, unsigned Height, int IconID, bool WindowMode)
{
	m_hInst = hInst;

	m_RS.Width = Width;
	m_RS.Height = Height;

	Register(Name, IconID);
	Create(Name);

	return Init(hInst, m_hWnd, Width, Height, WindowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned Width, unsigned Height, int IconID, bool WindowMode)
{
	m_hInst = hInst;
	m_hWnd = hWnd;
	m_RS.Width = Width;
	m_RS.Height = Height;

	m_Timer = new CTimer;

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	while (m_Loop)
	{
		// GetMessage는 메세지가 없을 경우 다른일을 할 수 없다.
		// 메세지가 올때까지 대기하고 있는 시간을 윈도우의 데드타임이라고 한다.
		// 실제로 메세지가 있는 시간보다 없는 시간이 훨씬 길다.
		// 그래서 게임은 윈도우의 데드타임동안 게임이 동작될 수 있게 제작한다.
		// PeekMessage는 메세지큐에서 메세지를 얻어온다.
		// 그런데 만약 메세지가 없다면 false를 리턴하면서 바로 빠져나오고
		// 메세지가 있다면 true를 리턴하면서 해당 메세지를 꺼내온다.

		// wMsgFilterMin, wMsgFilterMax : 전체 메시지 중에서 검사할 처음, 마지막 범위
		// 둘다 0이면 메시지 하나의 모든 부분을 받아온다.
		// wRemoveMsg : 메시지 처리 방식 -> PeekMessage 를 통해서, 메세지 큐에 있는 모든 것을 비운다.
		if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지
			// 이다. TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를
			// 판단해주고 눌려진 키가 문자 키인지를 판단해서 일반 무자 키라면 WM_CHAR라는
			// 메시지를 만들어서 메세지 큐에 추가해주게 된다.
			TranslateMessage(&msg);

			// WndProc 함수에 인자 전달하면서, 시스템 내부적으로 실행해준다.
			DispatchMessage(&msg);
		}
		// 윈도우 데드 타임일 경우, 여기로 들어오게 된다. ( 메시지 없을 때 여기 실행 )
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
	}

	float DeltaTime = m_Timer->Update();
	if (!m_Play)
		DeltaTime = 0.f;

	if (!Update(DeltaTime))
		return;

	if (!PostUpdate(DeltaTime))
		return;

	Render(DeltaTime);
}

bool CEngine::Update(float DeltaTime)
{
	return true;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	return true;
}

bool CEngine::Render(float DeltaTime)
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

	// LoadIcon : 실행 파일로부터 특정 Icon 자원을 만들어내는 함수
	// MAKEINTRESOURCE : 반드시 사용해야 하는 일종의 매크로
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wcex.hCursor = LoadCursor(m_hInst, IDC_ARROW);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = Name;
	wcex.hIconSm = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* Name)
{
	m_hWnd = CreateWindowW(Name, Name, WS_OVERLAPPEDWINDOW, 
		0,0,m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
		return false;

	// 클라이언트 영역을 rw, widht로 만들어준다
	RECT rc = { 0,0, (LONG)m_RS.Width, (LONG)m_RS.Height };

	// 윈도우 영역의 크기를 전달한다.
	// 3번째 인자 : 해당 Window가 menu를 들고 있는가 
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우 크기를 지정한다.
	// HWND_TOPMOST : Window의 ZOrder 과 관련된 요소 --> 제일 위에 보이도록 ?
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER); // SWP_NOZORDER = 현재 Z Order 유지 --> hWndInsertAfter  세팅 무시

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
