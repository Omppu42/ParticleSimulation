#pragma once

#include "GLUtils.h"
#include <GL/glew.h>
#include <vector>
#include "Vertex.h"

class VertexBuffer {
private:
	std::vector<Vertex> m_Verticies = { };

	unsigned int m_VertexSizeBytes;
	GLenum m_RenderingHint;
	unsigned int m_RendererID;

	std::vector<float> VertexVecToData();

public:
	VertexBuffer(GLenum renderHint);
	~VertexBuffer();

	void BindAndUpdateBuffer();
	void Bind() const;
	void Unbind() const;

	inline void AddVerticies(std::vector<Vertex>& newVerticies) { m_Verticies.insert(m_Verticies.end(), newVerticies.begin(), newVerticies.end()); }
	inline void AddVertex(Vertex newVertex) { m_Verticies.push_back(newVertex); }
	inline void ClearVerticiesData() { m_Verticies = { }; }
};