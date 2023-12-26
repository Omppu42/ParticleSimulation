#pragma once
#include <vector>
#include <string>
#include <sstream>

struct Vertex {
private:
	float x = 1.0f;
	float y = 1.0f;
	float r = 1.0f;
	float g = 1.0f;
	float b = 1.0f;
	float a = 1.0f;

	int size = 0;

public:
	Vertex(float _x, float _y);
	Vertex(float _x, float _y, float _a);
	Vertex(float _x, float _y, float _r, float _g, float _b);
	Vertex(float _x, float _y, float _r, float _g, float _b, float _a);
	Vertex(float _x, float _y, std::vector<float> _col);
	
	void GetData(std::vector<float>& dataOut);
	inline int GetSize() { return size; }
	std::string GetDataString();
};