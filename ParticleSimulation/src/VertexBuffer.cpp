#include "VertexBuffer.h"

#include <iostream>

#include "Vertex.h"


std::vector<float> VertexBuffer::VertexVecToData()
{
    std::vector<float> out = { };

    for (Vertex _vertex : m_Verticies) {
        _vertex.GetData(out);
    }

    return out;
}

VertexBuffer::VertexBuffer(GLenum renderHint)
    : m_RenderingHint(renderHint) {

    GLCall(glGenBuffers(1, &m_RendererID));
    Bind();
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::BindAndUpdateBuffer() {
    Bind();

    int bound;
    GLCall(glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &bound)); //Check if bound vertexbuffer is this one
    if (bound != m_RendererID) {
        std::cout << "Trying to update vertex buffer of VertexBuffer ID: " << m_RendererID << ", while " << bound << " is bound. \nYou need to have this Vertex Buffer bound to update it." << std::endl;
        ASSERT(false);
        return;
    }

    std::vector<float> dataFloats = VertexVecToData();

    float* floatPtr = &dataFloats[0];
    const void* voidData = static_cast<const void*>(floatPtr);
    GLCall(glBufferData(GL_ARRAY_BUFFER, dataFloats.size() * sizeof(float), voidData, m_RenderingHint));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
