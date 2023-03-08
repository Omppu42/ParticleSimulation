#include "VertexBuffer.h"

#include <iostream>

#include "Renderer.h"
#include "Vertex.h"

void VertexBuffer::VertexVecToData(std::vector<Vertex>& dataIn, std::vector<float>& dataOut)
{
    for (Vertex _vertex : dataIn) {
        dataOut.push_back(_vertex.x);
        dataOut.push_back(_vertex.y);
    }
}

VertexBuffer::VertexBuffer(std::vector<Vertex> data, unsigned int size, GLenum renderHint) 
        : m_RenderingHint(renderHint) {

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    //UpdateBuffer(data, size);
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::UpdateBuffer(std::vector<Vertex> data, unsigned int size) {
    std::vector<float> dataFloats = {};
    VertexVecToData(data, dataFloats);

    //if (dataFloats.empty()) return;

    float* f = &dataFloats[0];
    const void* d = static_cast<const void*>(f);
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, d, m_RenderingHint));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
