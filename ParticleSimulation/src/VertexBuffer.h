#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class VertexBuffer {
private:
	unsigned int m_RendererID;
	GLenum m_RenderingHint;

	void VertexVecToData(std::vector<Vertex>& dataIn, std::vector<float>& dataOut);
public:
	//VertexBuffer(const void* data, unsigned int size, GLenum renderHint);
	// 
	//size - amount of positions
	VertexBuffer(std::vector<Vertex> data, unsigned int size, GLenum renderHint);
	~VertexBuffer();

	void UpdateBuffer(std::vector<Vertex> data, unsigned int size);
	void Bind() const;
	void Unbind() const;
};