#include "VertexBuffer.h"

#include <iostream>

#include "Renderer.h"
#include "Vertex.h"

void VertexBuffer::VertexVecToData(std::vector<Vertex>& dataIn, std::vector<float>& dataOut)
{
    for (Vertex _vertex : dataIn) {
        _vertex.GetData(dataOut);
    }
}

VertexBuffer::VertexBuffer(std::vector<Vertex> data, unsigned int bytesPerVertex, GLenum renderHint)
    : m_RenderingHint(renderHint) {

    m_VertexSizeBytes = bytesPerVertex;

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    UpdateBuffer(data);
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::UpdateBuffer(std::vector<Vertex>& data) {
    std::vector<float> dataFloats = {};
    VertexVecToData(data, dataFloats);

    float* floatPtr = &dataFloats[0];
    const void* voidData = static_cast<const void*>(floatPtr);
    GLCall(glBufferData(GL_ARRAY_BUFFER, data.size() * m_VertexSizeBytes, voidData, m_RenderingHint));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
