#pragma once

#include <vector>
#include <iostream>

struct VertexBufferElement {
	unsigned int type;
	unsigned int count;
	unsigned char nolmalized;

	static unsigned int SizeOfType(unsigned int type) {
		switch (type) {
			case 5126: return sizeof(GLfloat);
			case 5125: return sizeof(GLuint);
			case 5121: return sizeof(GLubyte);
		}

		std::cout << "Invalid type: " << type << std::endl;
		__debugbreak();
		return 0;
	}
};

class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	void Push(unsigned int type, unsigned int count, unsigned char normalized) {
		m_Elements.push_back({ type, count, normalized });
		m_Stride += count * VertexBufferElement::SizeOfType(type);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};