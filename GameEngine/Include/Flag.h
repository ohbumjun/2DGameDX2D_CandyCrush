#pragma once

enum AXIS {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

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

enum class Component_Type {
	SceneComponent,
	ObjectComponent
};

enum class Shader_Type {
	Graphic,
	Compute
};

enum class ConstantBuffer_Shader_Type {
	Vertex = 0x1,			//    0001 : ÀÌÁø¼ö
	Pixel = 0x2,			//    0010 
	Domain = 0x4,		//    0100
	Hull      = 0x8,		//    1000
	Geometry = 0x10,  //  10000
	Compute = 0x20,		// 100000
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute
};