#include "IndexBuffer.h"

#include "Renderer.h"
#include "SquareIndicies.h"
#include <iostream>

IndexBuffer::IndexBuffer(std::vector<SquareIndicies>& data, GLenum renderHint) : m_RenderingHint(renderHint) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

    UpdateIndicies(data);

}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::UpdateIndicies(std::vector<SquareIndicies>& data) {

    int bound;
    GLCall(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound)); //Check if bound indexBuffer is this one
    if (bound != m_RendererID) {
        std::cout << "Trying to update indicies of IndexBuffer ID: " << m_RendererID << ", while " << bound << " is bound. \nYou need to have this indexBuffer bound to update it." << std::endl;
        return;
    }

    m_Indicies = {};
    for (SquareIndicies i : data) {
        i.PushBackIndicies(m_Indicies);
    }
    m_Count = data.size() * 6;

    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), static_cast<const void*>(m_Indicies.data()), m_RenderingHint));
}