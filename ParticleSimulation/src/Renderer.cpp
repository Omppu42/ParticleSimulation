#include "Renderer.h"

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
    shader.UseProgram();
    vao.Bind();
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const RendererState& renderer) {
    renderer.BindAll();

    GLCall(glDrawElements(GL_TRIANGLES, renderer.ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::BindState(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
    shader.UseProgram();
    vao.Bind();
    ibo.Bind();
}

RendererState::RendererState(unsigned int vbRenderingHint, unsigned int ibRenderingHint, std::string shaderPath)
  : vao(),        
    vbo(vbRenderingHint),
    vboLayout(),
    ibo(ibRenderingHint),
    shader(shaderPath) {
    
    BindAll();
}

void RendererState::AddVBLayoutElement(unsigned int type, unsigned int count)
{
    vboLayout.Push(type, count, GL_FALSE);
    vao.AddBuffer(vbo, vboLayout);
}

void RendererState::BindAll() const
{
    vao.Bind();
    vbo.Bind();
    ibo.Bind();
    shader.UseProgram();
}

void RendererState::UnBindAll() const
{
    vao.Unbind();
    vbo.Unbind();
    ibo.Unbind();
    shader.DisableProgram();
}