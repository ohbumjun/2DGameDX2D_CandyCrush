#pragma once

enum Cell_Type {
	Blue,
	Red,
	Orange,
	Yellow,
	Green,
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
	MirrorBall,
	End
};

enum Mouse_Click {
	None,
	First,
	Second
};