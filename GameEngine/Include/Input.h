#pragma once

#include "GameInfo.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct KeyState {
	unsigned int Key;
	bool State[(int)Key_State::Key_Max];
};

struct KeyInfo {
	std::string m_Name;
	KeyState m_State;
	std::function<void()> m_Callback[(int)Key_State::Key_Max];
	bool Ctrl;
	bool Alt;
	bool Shift;
	KeyInfo() :
	Ctrl(false),
	Alt(false),
	Shift(false){}
};

class CInput
{
private :
	CInput();
	~CInput();
private :
	HINSTANCE m_HInst;
	HWND m_Hwnd;
	Resolution m_RS;
	std::vector<unsigned int> m_vecAddKey;
	std::vector<KeyState> m_vecKeyState;
	std::unordered_map<std::string, KeyInfo*> m_mapKeyInfo;
	unsigned int m_keyState[256];
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;
private :
	IDirectInput8* m_DirectInput;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	DIMOUSESTATE m_MouseState;
	Input_Type m_InputType;
	Vector2 m_MousePos;
	Vector2 m_MouseWorldPos;
	Vector2 m_MouseMove;
	bool m_CollisionWidget;
	bool m_LMouseDown;
	bool m_RMouseDown;
	
public :
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float DeltaTime);
	void CreateInputKey(const std::string& Name, unsigned int Key);
private :
	bool InitWindow();
	bool InitDirectX();
	void ReadDirectKeyBoard();
	void ReadDirectMouse();
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState(float DeltaTime);
	void UpdateKeyInfo(float DeltaTime);
private :
	KeyInfo* FindKeyInfo(const std::string& Name);
	unsigned char ConvertKey(unsigned char Key);
public :
	template<typename T>
	void SetKeyCallback(Key_State State, const std::string& Name,
		T* Obj, void(T::*Func)())
{
		KeyInfo* Info = FindKeyInfo(Name);
		if (!Info)
			return;
		Info->m_Callback[(int)State] = std::bind(Func, Obj);
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

