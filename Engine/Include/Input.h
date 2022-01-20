#pragma once

#include "GameInfo.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct keyState {
	unsigned char Key;
	bool  KeyState[Key_Max];
	keyState() :
		KeyState{}{}
};

struct KeyInfo {
	std::string Name;
	keyState State;
	std::function<void(float)> Callback[Key_Max];

	bool Ctrl;
	bool Alt;
	bool Shift;


	KeyInfo() :
	Ctrl(false),
	Alt(false),
	Shift(false)
	{}
};

class CInput
{
private :
	CInput();
	~CInput();
private :
	HINSTANCE m_hInst;
	HWND m_hWnd;
	bool m_CollisionWidget;
	std::unordered_map<std::string, KeyInfo*> m_mapKeyInfo;
	std::vector<unsigned char> m_vecAddKey;
	std::vector<keyState> m_vecKeyState;
	unsigned char m_KeyState[256];
	Input_Type m_InputType;
	bool m_LMouseClick;
	bool m_RMouseClick;
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;
	bool m_MouseDown;
	bool m_MousePush;
	bool m_MouseUp;
	Vector2 m_MousePos;
	Vector2 m_MouseMove;
private :
	IDirectInput8* m_DirectInput;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	DIMOUSESTATE m_MouseState;
private :
	bool InitWindow();
	bool InitDirect();
	bool ReadKeyBoardState();
	bool ReadMouseState();
private :
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);
	void UpdateKeyInfo(float DeltaTime);
private :
	unsigned char ConvertKey(unsigned char Key);
	KeyInfo* FindKeyInfo(const std::string& Name);
	KeyInfo* CreateKeyInfo(const std::string& Name, unsigned char Key);
	void SetCtrlKey(const std::string& Name, bool State);
	void SetAltKey(const std::string& Name, bool State);
	void SetShiftKey(const std::string& Name, bool State);
	void ClearCallback(const std::string& Name);
public :
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float DeltaTime);
public :
	template<typename T>
	void SetKeyCallback(const std::string& Name, Key_State State, T* Obj, void(T::*Func)(float))
{
		KeyInfo* Key = FindKeyInfo(Name);
		if (!Key)
			return;
		Key->Callback[(int)State] = std::bind(Func, Obj, std::placeholders::_1);
}
private :
	static CInput* m_Inst;
public :
	static CInput* GetInst()
{
		if (!m_Inst)
			m_Inst = new CInput;
		return m_Inst;
}
	static void DestroyInst()
{
		SAFE_DELETE(m_Inst);
}
};

