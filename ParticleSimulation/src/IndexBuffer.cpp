#include "IndexBuffer.h"

#include "SquareIndicies.h"
#include <iostream>


IndexBuffer::IndexBuffer(GLenum renderHint) : m_RenderingHint(renderHint) {
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
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

//void IndexBuffer::ValidateBoundBuffer() {
//    int bound;
//    GLCall(glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &bound)); //Check if bound indexBuffer is this one
//    if (bound != m_RendererID) {
//        std::cout << "Trying to update indicies of IndexBuffer ID: " << m_RendererID << ", while " << bound << " is bound. \nYou need to have this indexBuffer bound to update it." << std::endl;
//        ASSERT(false);
//
//        return;
//    }
//}

void IndexBuffer::BindAndUpdateIndicies() {
    Bind();

    m_Count = m_Indicies.size();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), static_cast<const void*>(m_Indicies.data()), m_RenderingHint));
}

void IndexBuffer::AddIndicies(std::vector<unsigned int> newIndicies)
{
    m_Indicies.insert(m_Indicies.end(), newIndicies.begin(), newIndicies.end());

    m_NextFreeIndicie = *std::max_element(std::begin(newIndicies), std::end(newIndicies)) + 1;
}