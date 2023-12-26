#include "ShapeGenerator.h"



void ShapeGenerator::DrawStraightLine(std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo) {
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

void ShapeGenerator::DrawHorizontalLine(std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo) {
	ASSERT(startPos.size() == 2);
	ASSERT(endPos.size() == 2);

	float halfLineWidth = lineWidth / 2;

	vbo.AddVertex({ startPos[0],	startPos[1] + halfLineWidth, lineColor});
	vbo.AddVertex({ startPos[0],	startPos[1] - halfLineWidth, lineColor});
	vbo.AddVertex({   endPos[0],	  endPos[1]	- halfLineWidth, lineColor});
	vbo.AddVertex({   endPos[0],	  endPos[1]	+ halfLineWidth, lineColor});

	unsigned int i = ibo.GetNextFreeIndicie();
	ibo.AddIndicies({ i + 0, i + 1, i + 2, i + 2, i + 3, i + 0 });
}

void ShapeGenerator::DrawVerticalLine(std::vector<float> startPos, std::vector<float> endPos, float lineWidth, std::vector<float> lineColor, VertexBuffer& vbo, IndexBuffer& ibo) {
	ASSERT(startPos.size() == 2);
	ASSERT(endPos.size() == 2);

	float halfLineWidth = lineWidth / 2;

	vbo.AddVertex({ startPos[0] + halfLineWidth,	startPos[1], lineColor });
	vbo.AddVertex({ startPos[0] - halfLineWidth,	startPos[1], lineColor });
	vbo.AddVertex({   endPos[0] - halfLineWidth,	  endPos[1], lineColor });
	vbo.AddVertex({   endPos[0] + halfLineWidth,	  endPos[1], lineColor });

	unsigned int i = ibo.GetNextFreeIndicie();
	ibo.AddIndicies({ i + 0, i + 1, i + 2, i + 2, i + 3, i + 0 });
}

void ShapeGenerator::DrawCircle(std::vector<float> centerPos, float r, unsigned int triangles, std::vector<float> color, VertexBuffer& vbo, IndexBuffer& ibo) {
	ASSERT(centerPos.size() == 2);
	if (triangles < 3) {
		std::cout << "Can't draw a circle with less than 3 triangles" << std::endl;
		ASSERT(false);
	}

	float theta = 2 * M_PI / triangles;
	unsigned int nFree = ibo.GetNextFreeIndicie();
	
	vbo.AddVertex({ centerPos[0], centerPos[1], color});

	for (unsigned int i = 0; i < triangles; i++) {
		vbo.AddVertex({ r * cosf(theta * i) + centerPos[0], r * sinf(theta * i) + centerPos[1], color });
		
		// if last triangle, connect with the second indicie
		if (i == triangles - 1) {
			ibo.AddIndicies({ nFree + 0, nFree + i + 1, nFree + 1 });
		}
		else {
			ibo.AddIndicies({ nFree + 0, nFree + i + 1, nFree + i + 2 });
		}
	}
}
