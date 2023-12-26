#include "ShapeGenerator.h"



void ShapeGenerator::DrawLine(std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo) {
	ASSERT(startPos.size() == 2);
	ASSERT(endPos.size() == 2);

	float deltaX = startPos[0] - endPos[0];
	float deltaY = startPos[1] - endPos[1];

	float startToEndLen = (float)sqrt(pow(deltaX, 2) + pow(deltaY, 2));


	// Rotate the vector first, then normalize it and then make it the desireable length. 
	// After that offset to be next to the starting or ending point

	vbo.AddVertex({ -deltaY / startToEndLen * (lineWidth / 2) + startPos[0],
					deltaX / startToEndLen * (lineWidth / 2) + startPos[1], 
					lineColor });

	vbo.AddVertex({ deltaY / startToEndLen * (lineWidth / 2) + startPos[0],
					-deltaX / startToEndLen * (lineWidth / 2) + startPos[1], 
					lineColor });

	vbo.AddVertex({ deltaY / startToEndLen * (lineWidth / 2) + endPos[0],
					-deltaX / startToEndLen * (lineWidth / 2) + endPos[1], 
					lineColor });

	vbo.AddVertex({ -deltaY / startToEndLen * (lineWidth / 2) + endPos[0],
					deltaX / startToEndLen * (lineWidth / 2) + endPos[1], 
					lineColor });

	unsigned int i = ibo.GetNextFreeIndicie();
	ibo.AddIndicies({ i + 0, i + 1, i + 2, i + 2, i + 3, i + 0 });
}