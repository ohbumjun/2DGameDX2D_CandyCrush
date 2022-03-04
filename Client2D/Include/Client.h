#pragma once

#include <string>

enum class Cell_Type_Binary {
	Blue = 0x1,
	Red = 0x2,
	Orange = 0x4,
	Yellow = 0x8,
	//Green = 0x10,
	//All = Blue | Red | Orange | Yellow | Green
	All = Blue | Red | Orange | Yellow
};

enum class Cell_Type {
	Blue,
	Red,
	Orange,
	Yellow,
	// Green,
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
	Normal,
	Horizontal,
	Vertical,
	Bag,
	BagAfter,
	MirrorBall,
	BagAndBag,
	BagAndColLine,
	BagAndRowLine,
	BagAndMirrorBall_Bag, // 1st �ı� : �ش� Ball �� ������ ��� �����ϰ� �������.
	BagAndMirrorBall_Mirror, // 2nd �ı� : �ش� MirrorBall�� ������ ��� �����ϰ� �������.
	RowLineAndColLine,
	LineAndMirrorBall_Line,
	LineAndMirrorBall_Mirror,
	MirrorBallAndMirrorBall
};

enum class DestroyMark_State {
	None,
	Horizontal,
	Vertical,
	Bag,
	BagAfter,
	MirrorBall
};

enum class Comb_Priority {
	// �ٹ��� + �ٹ���
	LineAndLine,
	// ���� + �ٹ���
	BagAndLine,
	// ���� + ����
	BagAndBag,
	// ���� + MirrorBall
	MirrorBallAndBag,
	// �ٹ��� + MirrorBall
	MirrorBallAndLine,
	// MirrorBall + MirrorBall
	MirrorBallAndMirrorBall
};