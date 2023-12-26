#pragma once

#define _USE_MATH_DEFINES

#include "Vertex.h"
#include "SquareIndicies.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>
#include <cmath>
#include <math.h>
#include <iostream>

class ShapeGenerator {
public:

	static void DrawStraightLine  (std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo);
	static void DrawHorizontalLine  (std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo);
	static void DrawVerticalLine  (std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo);
	static void DrawCircle(std::vector<float> centerPos, float r, unsigned int triangles, std::vector<float> color, VertexBuffer& vbo, IndexBuffer& ibo);
};