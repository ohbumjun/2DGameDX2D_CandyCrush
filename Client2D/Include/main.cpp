#include "ClientManager.h"

// 1) ����ũ�� ���ø����̼� ( .exe )
// ���� ������ �����ų ������Ʈ
// ���� ������ ���۵Ǵ� ������Ʈ�� �ȴ�.

// 2) - ��� ���� ���丮 : ���������� ��������� ��  (lib ������ ���⿡ ������� ���̴� )
//     - �߰� ���丮 : Obj ������ ��������� ��
//	   - ����̸� : �ش� �̸����� lib ? ������ ������� ��

// Engine�� DLL, Header ���Ͽ� ��ȭ�� ���� ������, �ڵ����� Client �ʿ� �����ǵ��� ������ ���̴�.
// batch ���� �̿� --> ���� ��ɾ�.�� �����ϴ� ���� --> ���� ��ɾ�. �߿��� ���� ����.�� ���ִ� �Լ��� �ִ�
// ���� ���縦 ���ٱ� ? --> �������� ������, ���尡 ������, ���̺귯���� ������ ������� ���̴�
// ���̺귯�� ��������, �������� ������ ���� �۾��̴�.
// �����ϰ� ������ �ƴ϶�, ������ ������ �ݿ��ؾ� �Ѵ�. ��, ���̺귯�� ���Ͽ� �־��־�� �Ѵ�.
// �� �۾��� �ٷ� ��ũ�ϰ� �����ϴ� �۾��� �ش��Ѵ�.
// ��, ���尡 ���� ������, �� ���̺귯���� Ŭ���̾�Ʈ ������ �Űܶ� ! ��� ��ɾ ������־�� �Ѵ�.

// Engine �Ӽ� -> ���� �̺�Ʈ
// ��������� 3���� ���� �̺�Ʈ�� �����Ѵ�.
// ���� ��, ��ũ ��, ���� ��.
// �츮�� ���� �� �̺�Ʈ�� �̿��ؾ� �Ѵ�.
// ����ٿ��ٰ� ����� �Ἥ, ���� ���ϴ� ������ �����ض� ! ��� ������� �� �ִ�. // 

// resource.h �� icon id�� ����Ǿ� �ְ� �ȴ�. �װ��� ����� �� 

// Debug �϶��� _Debug.lib�� ��ũ�� �ɰ�
// Release �̸�, .lib�� ��ũ�� �ɰ�
#ifdef _DEBUG

#pragma comment(lib, "GameEngine_Debug.lib")

#else

#pragma comment(lib, "GameEngine.lib")

#endif // _DEBUG

#define _CRTDBG_MAP_ALLOC

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(3406);

	if (!CClientManager::GetInst()->Init(hInstance))
	{
		CClientManager::DestroyInst();
		return 0;
	}

	CClientManager::GetInst()->CreateDefaultSceneMode();

	int Ret = CClientManager::GetInst()->Run();

	CClientManager::DestroyInst();

	return Ret;
}
