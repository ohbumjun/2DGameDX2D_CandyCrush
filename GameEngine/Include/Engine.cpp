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
		// GetMessage�� �޼����� ���� ��� �ٸ����� �� �� ����.
		// �޼����� �ö����� ����ϰ� �ִ� �ð��� �������� ����Ÿ���̶�� �Ѵ�.
		// ������ �޼����� �ִ� �ð����� ���� �ð��� �ξ� ���.
		// �׷��� ������ �������� ����Ÿ�ӵ��� ������ ���۵� �� �ְ� �����Ѵ�.
		// PeekMessage�� �޼���ť���� �޼����� ���´�.
		// �׷��� ���� �޼����� ���ٸ� false�� �����ϸ鼭 �ٷ� ����������
		// �޼����� �ִٸ� true�� �����ϸ鼭 �ش� �޼����� �����´�.

		// wMsgFilterMin, wMsgFilterMax : ��ü �޽��� �߿��� �˻��� ó��, ������ ����
		// �Ѵ� 0�̸� �޽��� �ϳ��� ��� �κ��� �޾ƿ´�.
		// wRemoveMsg : �޽��� ó�� ��� -> PeekMessage �� ���ؼ�, �޼��� ť�� �ִ� ��� ���� ����.
		if (PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN �̶�� �޼����� �ִ�. �̴� Ű���� Ű�� �������� �߻��Ǵ� �޼���
			// �̴�. TranslateMessage �Լ������� �޼����� �������ָ� WM_KEYDOWN ������
			// �Ǵ����ְ� ������ Ű�� ���� Ű������ �Ǵ��ؼ� �Ϲ� ���� Ű��� WM_CHAR���
			// �޽����� ���� �޼��� ť�� �߰����ְ� �ȴ�.
			TranslateMessage(&msg);

			// WndProc �Լ��� ���� �����ϸ鼭, �ý��� ���������� �������ش�.
			DispatchMessage(&msg);
		}
		// ������ ���� Ÿ���� ���, ����� ������ �ȴ�. ( �޽��� ���� �� ���� ���� )
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

	// LoadIcon : ���� ���Ϸκ��� Ư�� Icon �ڿ��� ������ �Լ�
	// MAKEINTRESOURCE : �ݵ�� ����ؾ� �ϴ� ������ ��ũ��
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

	// Ŭ���̾�Ʈ ������ rw, widht�� ������ش�
	RECT rc = { 0,0, (LONG)m_RS.Width, (LONG)m_RS.Height };

	// ������ ������ ũ�⸦ �����Ѵ�.
	// 3��° ���� : �ش� Window�� menu�� ��� �ִ°� 
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ������ ũ�⸦ �����Ѵ�.
	// HWND_TOPMOST : Window�� ZOrder �� ���õ� ��� --> ���� ���� ���̵��� ?
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER); // SWP_NOZORDER = ���� Z Order ���� --> hWndInsertAfter  ���� ����

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

		// HDC : ȭ�鿡 ����ϱ� ���� �׸��� �����̴�.
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
