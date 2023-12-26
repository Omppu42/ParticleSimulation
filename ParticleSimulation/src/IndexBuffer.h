#pragma once

#include "GLUtils.h"
#include "SquareIndicies.h"

#include <GL/glew.h>

#include <vector>
#include <algorithm>
#include <sstream>

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	unsigned int m_RenderingHint;
	unsigned int m_NextFreeIndicie = 0;

	std::vector<unsigned int> m_Indicies = {};
	
	//void ValidateBoundBuffer();

public:
	IndexBuffer(GLenum renderHint);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
		
	void BindAndUpdateIndicies();
	void AddIndicies(std::vector<unsigned int> newIndicies);

	std::string GetIndiciesStr();

	inline void ClearIndicies() { m_Indicies = { }; m_NextFreeIndicie = 0; }
	inline unsigned int GetCount() const { return m_Count; }
	inline unsigned int GetNextFreeIndicie() const { return m_NextFreeIndicie; }
};