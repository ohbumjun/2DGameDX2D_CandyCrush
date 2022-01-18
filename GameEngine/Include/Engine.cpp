#include "Engine.h"

DEFINITION_SINGLE(CEngine);

bool CEngine::m_Loop = true;

CEngine::CEngine() :
m_Play(true),
m_Start(false),
m_ClearColor {},
m_Timer(nullptr),
m_Space(Engine_Space::Space2D)
{
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* Name, unsigned Width, unsigned Height, int IconID, bool WindowMode)
{}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned Width, unsigned Height, int IconID, bool WindowMode)
{}

int CEngine::Run()
{}

void CEngine::Logic()
{}

bool CEngine::Update(float DeltaTime)
{}

bool CEngine::PostUpdate(float DeltaTime)
{}

bool CEngine::Render(float DeltaTime)
{}

ATOM CEngine::Register(const TCHAR* Name, int IconID)
{}

BOOL CEngine::Create(const TCHAR* Name)
{}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM wParam)
{}
