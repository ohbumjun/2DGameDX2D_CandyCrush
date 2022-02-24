#pragma once

enum class Cell_Type_Binary {
	Blue = 0x1,
	Red = 0x2,
	Orange = 0x4,
	Yellow = 0x8,
	// Green = 0x10,
	All = Blue | Red | Orange | Yellow
};

enum class Cell_Type {
	Blue,
	Red,
	Orange,
	Yellow,
	// Green = 0x10,
	End,
};

enum class Cell_State {
	Normal,
	RowLine,
	ColLine,
	Bag,
	Notice,
	MirrorBall,
	End
};

enum class Match_State {
	NoMatch,
	Normal,
	RowLine,
	ColLine,
	Bag,
	BagAfter,
	MirrorBall,
	End
};

enum Mouse_Click {
	None,
	First,
	Second
};

enum class Destroy_State {
	None,
	Horizontal,
	Vertical,
	Bag,
	BagAfter,
	MirrorBall
};

enum class DestroyMark_State {
	None,
	Horizontal,
	Vertical,
	Bag,
	BagAfter,
	MirrorBall
};