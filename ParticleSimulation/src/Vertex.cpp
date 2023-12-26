#include "Vertex.h"
#include "Renderer.h"

#include <vector>
#include <iostream>

Vertex::Vertex(float _x, float _y) : 
	x(_x), y(_y) {
	size = 2;
}

Vertex::Vertex(float _x, float _y, float _a) : 
	x(_x), y(_y), a(_a) {
	size = 3;
}

Vertex::Vertex(float _x, float _y, float _r, float _g, float _b) :
	x(_x), y(_y), r(_r), g(_g), b(_b) {
	size = 5;
}

Vertex::Vertex(float _x, float _y, float _r, float _g, float _b, float _a) :
	x(_x), y(_y), r(_r), g(_g), b(_b), a(_a) {
	size = 6;
}

Vertex::Vertex(float _x, float _y, std::vector<float> _col) :
	x(_x), y(_y) {
	
	int colors = _col.size();

	switch (colors)
	{
	case 1:
		r = _col[0];
		break;
	case 2:
		r = _col[0];
		g = _col[1];
		break;
	case 3:
		r = _col[0];
		g = _col[1];
		b = _col[2];
		break;
	case 4:
		r = _col[0];
		g = _col[1];
		b = _col[2];
		a = _col[3];
		break;
	default:
		std::cout << "Trying to pass an invalid amount of color data to a vertex (" << colors << ")" << std::endl;
		ASSERT(false)
		break;
	}

	size = 2 + colors;
}

void Vertex::GetData(std::vector<float>& dataOut) {

	if (size == 2) {
		dataOut.push_back(x);
		dataOut.push_back(y);
	}
	else if (size == 3) {
		dataOut.push_back(x);
		dataOut.push_back(y);
		dataOut.push_back(a);
	}
	else if (size == 5) {
		dataOut.push_back(x);
		dataOut.push_back(y);
		dataOut.push_back(r);
		dataOut.push_back(g);
		dataOut.push_back(b);
	}
	else if (size == 6) {
		dataOut.push_back(x);
		dataOut.push_back(y);
		dataOut.push_back(r);
		dataOut.push_back(g);
		dataOut.push_back(b);
		dataOut.push_back(a);
	}
	else {
		// Unknown size
		std::cout << "Unknown Vertex size of " << size << ", inserting a breakpoint here." << std::endl;
		ASSERT(false);
	}

}

std::string Vertex::GetDataString() {
	std::ostringstream os;

	std::vector<float> elements = { };
	GetData(elements);

	for (const float& e : elements) {
		os << e << ", ";
	}

	return os.str();
}
