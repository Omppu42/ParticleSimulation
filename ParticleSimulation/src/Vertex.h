#pragma once
#include <vector>

struct Vertex {
public:
	float x = 0.0f;
	float y = 0.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	Vertex(float _x, float _y, float _r, float _g, float _b, float _a);
	void GetData(std::vector<float>& dataOut);
};