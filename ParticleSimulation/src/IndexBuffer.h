#pragma once

#include <GL/glew.h>
#include <vector>
#include "SquareIndicies.h"

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	unsigned int m_RenderingHint;

	std::vector<unsigned int> m_Indicies = {};  //Make squareIndicies vector into unsigned int vector
	
public:
	IndexBuffer(std::vector<SquareIndicies>& data, GLenum renderHint);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateIndicies(std::vector<SquareIndicies>& data);
		
	inline unsigned int GetCount() const { return m_Count; }
};