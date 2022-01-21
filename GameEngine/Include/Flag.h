#pragma once

enum Key_State {
	Key_Down,
	Key_Push,
	Key_Up,
	Key_Max
};

enum class Input_Type {
	Window,
	Direct
};

enum class Image_Type {
	Atlas,
	Frame,
	Array
};