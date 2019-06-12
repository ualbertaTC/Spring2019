#pragma once
#include "componentBase.h"

#include <vector>
#include <glm/glm.hpp>

class RenderComponent
{
protected:
    friend RenderSystem;
public:

};

class Shader;

struct Vertex
{
    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoord;
};

struct Texture 
{
    unsigned m_ID;
    std::string m_Type;
};

class Mesh
{
protected:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned> m_Indices;
    std::vector<Texture> m_Textures;

    int m_VAO, m_VBO, m_EBO;

    void SetupMesh();
public:
    Mesh(std::vector<Vertex> Vertices, std::vector<unsigned> Indices, std::vector<Texture> Textures);
    void Draw(Shader shader);
};

