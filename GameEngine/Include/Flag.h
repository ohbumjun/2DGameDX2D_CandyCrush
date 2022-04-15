#pragma once

enum AXIS {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class RenderState_Type {
	Blend,
	Rasterizer,
	DepthStencil,
	Max
};

enum class Camera_Type {
	UI,
	Camera2D,
	Camera3D
};

enum Key_State {
	Key_Down,
	Key_Push,
	Key_Up,
	Key_Max
};

enum Button_State {
	Normal,
	MouseOn,
	Click,
	Disable,
	Max
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

enum class Collision_State {
	Begin,
	End,
	Max
};

enum class Collider_Type {
	Circle,
	Box2D,
	Pixel
};

enum class Collision_Interaction {
	Collision,
	Ignore
};

enum class Collision_Channel {
	Object,
	Player,
	PlayerAttack,
	Monster,
	MonsterAttack,
	Channel1,
	Channel2,
	Channel3,
	Channel4,
	Channel5,
	Channel_Max
};

enum class Component_Type {
	SceneComponent,
	ObjectComponent
};

enum class Shader_Type {
	Graphic,
	Compute
};

enum class Sampler_Type {
	Point,
	Linear,
	Anisotropic
};

enum class Buffer_Shader_Type {
	Vertex = 0x1,			//    0001 : ÀÌÁø¼ö
	Pixel = 0x2,			//    0010 
	Domain = 0x4,		//    0100
	Hull      = 0x8,		//    1000
	Geometry = 0x10,  //  10000
	Compute = 0x20,		// 100000
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};

enum class Buffer_Type {
	Vertex,
	Index
};

enum class ProgressBar_Dir {
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};