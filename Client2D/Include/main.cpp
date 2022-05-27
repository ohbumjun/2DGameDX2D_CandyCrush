#include "ClientManager.h"

// 1) 데스크톱 어플리케이션 ( .exe )
// 실제 게임을 실행시킬 프로젝트
// 실제 게임이 동작되는 프로젝트가 된다.

// 2) - 출력 파일 디렉토리 : 실행파일이 만들어지는 곳  (lib 파일이 여기에 만들어질 것이다 )
//     - 중간 디렉토리 : Obj 파일이 만들어지는 곳
//	   - 대상이름 : 해당 이름으로 lib ? 파일이 만들어질 것

// Engine이 DLL, Header 파일에 변화가 생길 때마다, 자동으로 Client 쪽에 복제되도록 세팅할 것이다.
// batch 파일 이용 --> 도스 명령어.를 실행하는 파일 --> 도스 명령어. 중에는 파일 복사.를 해주는 함수가 있다
// 언제 복사를 해줄까 ? --> 컴파일이 끝나고, 빌드가 끝나면, 라이브러리가 온전히 만들어질 것이다
// 라이브러리 기준으로, 컴파일은 일종의 번역 작업이다.
// 번역하고 끝난게 아니라, 번역한 사항을 반영해야 한다. 즉, 라이브러리 파일에 넣어주어야 한다.
// 이 작업이 바로 링크하고 빌드하는 작업에 해당한다.
// 즉, 빌드가 끝난 다음에, 이 라이브러리를 클라이언트 측으로 옮겨라 ! 라고 명령어를 만들어주어야 한다.

// Engine 속성 -> 빌드 이벤트
// 윈도우즈에는 3가지 빌드 이벤트가 존재한다.
// 빌드 전, 링크 전, 빌드 후.
// 우리는 빌드 후 이벤트를 이용해야 한다.
// 명령줄에다가 명령을 써서, 내가 원하는 파일을 실행해라 ! 라고 얘기해줄 수 있다. // 

// resource.h 에 icon id가 저장되어 있게 된다. 그것을 사용할 것 

// Debug 일때는 _Debug.lib에 링크를 걸고
// Release 이면, .lib에 링크를 걸고
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
