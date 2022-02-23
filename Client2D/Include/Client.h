#pragma once

enum Cell_Type {
	Blue,
	Red,
	Orange,
	// Yellow,
	// Green,
	End
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