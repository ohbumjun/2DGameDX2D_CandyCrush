#include "Input.h"


CInput::CInput() :
	m_CollisionWidget(false),
	m_LMouseClick(false),
	m_RMouseClick(false),
	m_Ctrl(false),
	m_Alt(false),
	m_Shift(false),
	m_DirectInput(nullptr),
	m_Keyboard(nullptr),
	m_Mouse(nullptr),
	m_mapKeyInfo{},
	m_vecAddKey{}
{
	m_vecKeyState.resize(256);
	for (int i = 0; i < 256; i++)
	{
		m_vecKeyState[i].Key = i;
	}
}

CInput::~CInput()
{
	SAFE_RELEASE(m_DirectInput);
	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);

	auto iter = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CInput::InitWindow()
{
	return true;
}

bool CInput::InitDirect()
{
	HRESULT result = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_DirectInput, NULL);

	if (FAILED(result))
		return false;

	// Key Board 
	result = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
	if (FAILED(result))
		return false;

	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	// Mouse
	result = m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	if (FAILED(result))
		return false;

	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	return true;
}

bool CInput::ReadKeyBoardState()
{
	HRESULT result = m_Keyboard->GetDeviceState(sizeof(m_KeyState), (LPVOID)&m_KeyState);

	if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	{
		m_Keyboard->Acquire();
	}
}

bool CInput::ReadMouseState()
{
	HRESULT result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE),
		(LPVOID)&m_MouseState);

	if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
	{
		m_Mouse->Acquire();
	}
}

void CInput::UpdateMouse(float DeltaTime)
{
	bool LMousePush = false;

	switch (m_InputType)
	{
	case Input_Type::Direct :
		{
			if (m_MouseState.rgbButtons[0] & 0x80)
			{
				m_LMouseClick = true;
				LMousePush = true;
			}
			else
				m_LMouseClick = false;

			if (m_MouseState.rgbButtons[1] & 0x80)
				m_RMouseClick = true;
			else
				m_RMouseClick = false;
		}
		break;
	case Input_Type::Window:
	{
		if (GetAsyncKeyState(MK_LBUTTON) & 0x8000)
		{
			m_LMouseClick = true;
			LMousePush = true;
		}
		else
			m_LMouseClick = false;

		if (GetAsyncKeyState(MK_RBUTTON) & 0x8000)
			m_RMouseClick = true;
		else
			m_RMouseClick = false;
	}
		break;
	}

	if (LMousePush)
	{
		if (!m_MouseDown && !m_MousePush)
		{
			m_MouseDown = true;
			m_MousePush = true;
			m_MouseUp = false;
		}
		if (m_MousePush)
		{
			m_MouseDown = false;
		}
	}
	else
	{
		if (m_MousePush)
		{
			m_MouseDown = false;
			m_MousePush = false;
			m_MouseUp = true;
		}
		else
		{
			m_MouseUp = false;
		}
	}
}

void CInput::UpdateKeyState(float DeltaTime)
{
	// Alt, Shift, Ctrl
	switch (m_InputType)
	{
	case Input_Type::Direct:
	{
		if (m_KeyState[DIK_LSHIFT] & 0x80)
		{
			m_Shift = true;
		}
		else
			m_Shift = false;

		if (m_KeyState[DIK_LMENU] & 0x80)
		{
			m_Alt = true;
		}
		else
			m_Alt = false;

		if (m_KeyState[DIK_LCONTROL] & 0x80)
		{
			m_Ctrl = true;
		}
		else
			m_Ctrl = false;
	}
	break;
	case Input_Type::Window:
	{
		if (GetAsyncKeyState(MK_CONTROL) & 0x8000)
			m_Ctrl = true;
		else
			m_Ctrl = false;

		if (GetAsyncKeyState(MK_SHIFT) & 0x8000)
			m_Shift = true;
		else
			m_Shift = false;

		if (GetAsyncKeyState(VK_MENU) & 0x8000)
			m_Alt = true;
		else
			m_Alt = false;
	}
	break;
	}


	// 등록된 키들 비교
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
	{
		unsigned char Key = ConvertKey(m_vecAddKey[i]);

		bool KeyPush = false;

		switch (m_InputType)
		{
		case Input_Type::Direct :
		{
			switch (Key)
			{
			case DIK_MOUSELBUTTON :
				{
				if (m_MouseState.rgbButtons[0] & 0x80 && !m_CollisionWidget)
					KeyPush = true;
				}
				break;
			case DIK_MOUSERBUTTON :
			{
				if (m_MouseState.rgbButtons[1] & 0x80 && !m_CollisionWidget)
					KeyPush = true;
			}
				break;
			default:
				if (m_KeyState[Key] & 0x80)
					KeyPush = true;
				break;
			}
		}
			break;
		case Input_Type::Window:
			if (GetAsyncKeyState(Key) & 0x8000)
				KeyPush = true;
			break;
		}

		// KeyState 정보를 바꿔줄 것이다. 
		if (KeyPush)
		{
			if (!m_vecKeyState[Key].KeyState[Key_Down] &&
				!m_vecKeyState[Key].KeyState[Key_Push])
			{
				m_vecKeyState[Key].KeyState[Key_Down] = true;
				m_vecKeyState[Key].KeyState[Key_Push] = true;
				m_vecKeyState[Key].KeyState[Key_Up] = false;
			}
			else if (m_vecKeyState[Key].KeyState[Key_Push])
			{
				m_vecKeyState[Key].KeyState[Key_Down] = false;
			}
		}
		else
		{
			if (m_vecKeyState[Key].KeyState[Key_Push])
			{
				m_vecKeyState[Key].KeyState[Key_Down] = false;
				m_vecKeyState[Key].KeyState[Key_Push] = false;
				m_vecKeyState[Key].KeyState[Key_Up] = true;
			}
			else
			{
				m_vecKeyState[Key].KeyState[Key_Up] = false;
			}
		}
	}
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	auto iter = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->State.KeyState[Key_Down] &&
			iter->second->Ctrl == m_Ctrl && 
			iter->second->Alt == m_Alt && 
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[Key_Down])
				iter->second->Callback[Key_Down]();
		}

		if (iter->second->State.KeyState[Key_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[Key_Push])
				iter->second->Callback[Key_Push]();
		}

		if (iter->second->State.KeyState[Key_Up] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[Key_Up])
				iter->second->Callback[Key_Up]();
		}
	}
}



KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
	auto iter = m_mapKeyInfo.find(Name);
	if (iter == m_mapKeyInfo.end())
		return nullptr;
	return iter->second;
}

KeyInfo* CInput::CreateKeyInfo(const std::string& Name, unsigned char Key)
{
	KeyInfo* Info = FindKeyInfo(Name);
	if (Info)
		return Info;

	Info = new KeyInfo;
	Info->State.Key = Key;

	m_mapKeyInfo.insert(std::make_pair(Name, Info));

	bool Add = false;
	for (size_t i = 0; i < m_vecAddKey.size(); i++)
	{
		if (m_vecAddKey[i] == Key)
		{
			Add = true;
			break;
		}
	}

	if (!Add)
	{
		m_vecAddKey.push_back(Key);
	}
}

bool CInput::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	switch (m_InputType)
	{
		case Input_Type::Window :
			return InitWindow();
		case Input_Type::Direct:
			return InitDirect();
	}
	return true;
}

void CInput::Update(float DeltaTime)
{
	if (m_InputType == Input_Type::Direct)
	{
		ReadKeyBoardState();
		ReadMouseState();
	}

	// Mouse Update

	// CollisionWidget
	m_CollisionWidget = false;

	// KeyState

	// KeyInfo
}



unsigned char CInput::ConvertKey(unsigned char Key)
{
	if (m_InputType == Input_Type::Direct)
	{
		switch (Key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_MBUTTON:
			return DIK_MOUSEWHEEL;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
	}

	return Key;
}
