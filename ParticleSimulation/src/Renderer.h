#pragma once

#include "GLUtils.h"

#include <vector>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class RendererState {

public:
    VertexArray vao;
    VertexBuffer vbo;
    VertexBufferLayout vboLayout;
    IndexBuffer ibo;
    Shader shader;

    RendererState(unsigned int vbRenderingHint, unsigned int ibRenderingHint, std::string shaderPath);

    void AddVBLayoutElement(unsigned int type, unsigned int count);
    void BindAll() const;
    void UnBindAll() const;

    inline void UpdateVBOandIBO() { vbo.BindAndUpdateBuffer(); ibo.BindAndUpdateIndicies(); }
    inline void UpdateVBO() { vbo.BindAndUpdateBuffer(); }
    inline void UpdateIBO() { ibo.BindAndUpdateIndicies(); }
};


class Renderer {
public:
    static void BindState(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);
    static void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader);

    static void Draw(const RendererState& renderer);
};