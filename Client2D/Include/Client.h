#pragma once

enum class Cell_Type_Binary {
	Blue = 0x1,
	Red = 0x2,
	Orange = 0x4,
	Yellow = 0x8,
	Green = 0x10,
	All = Blue | Red | Orange | Yellow | Green
};

enum class Cell_Type {
	Blue,
	Red,
	Orange,
	Yellow,
	Green,
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
	MirrorBall,
	BagAndBag,
	BagAndColLine,
	BagAndRowLine,
	BagAndMirrorBall_Bag, // 1st 파괴 : 해당 Ball 의 색상을 모두 제거하고 사라진다.
	BagAndMirrorBall_Mirror, // 2nd 파괴 : 해당 MirrorBall의 색상을 모두 제거하고 사라진다.
	RowLineAndColLine,
	LineAndMirrorBall_Line,
	LineAndMirrorBall_Mirror
};

enum class DestroyMark_State {
	None,
	Horizontal,
	Vertical,
	Bag,
	BagAfter,
	MirrorBall
};