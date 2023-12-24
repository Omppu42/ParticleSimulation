#include "Vertex.h"
#include <vector>

//Vertex::Vertex(float _x, float _y, float _r, float _g, float _b, float _a) {
//	x = _x;
//	y = _y;
//	r = _r;
//	g = _g;
//	b = _b;
//	a = _a;
//}

//void Vertex::GetData(std::vector<float>& dataOut) {
//	dataOut.push_back(x);
//	dataOut.push_back(y);
//	dataOut.push_back(r);
//	dataOut.push_back(g);
//	dataOut.push_back(b);
//	dataOut.push_back(a);
//}

Vertex::Vertex(float _x, float _y, float _a) {
	x = _x;
	y = _y;
	a = _a;
}

void Vertex::GetData(std::vector<float>& dataOut) {
	dataOut.push_back(x);
	dataOut.push_back(y);
	dataOut.push_back(a);
}

std::string Vertex::GetDataString() {
	std::ostringstream os;
	os << x << ", " << y << ", " << a << ".";

	return os.str();
}
