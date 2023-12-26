#pragma once

#include "Vertex.h"
#include "SquareIndicies.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>
#include <cmath>
#include <iostream>

class ShapeGenerator {
public:

	static void DrawLine(std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo);
};