#pragma once

enum Cell_Type {
	Blue,
	Red,
	Orange,
	Yellow,
	Green
};

enum class Cell_State {
	Normal,
	RowLine,
	ColLine,
	Bag,
	Notice,
	MirrorBall
};

enum class Match_State {
	NoMatch,
	Normal,
	RowLine,
	ColLine,
	Bag,
	MirrorBall
};

enum Mouse_Click {
	None,
	First,
	Second
};