#pragma once
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class VertexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_VertexSizeBytes;
	GLenum m_RenderingHint;

	void VertexVecToData(std::vector<Vertex>& dataIn, std::vector<float>& dataOut);

public:
	VertexBuffer(std::vector<Vertex> data, unsigned int bytesPerVertex, GLenum renderHint);
	~VertexBuffer();

	void UpdateBuffer(std::vector<Vertex>& data);
	void Bind() const;
	void Unbind() const;
};