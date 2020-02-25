#pragma once
#include "componentRender.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

class OpenGLRenderComponent
{
protected:
    friend RenderSystem;
    glm::mat4 m_ModelMatrix;
    Model m_Model;
public:
    OpenGLRenderComponent(const char *Path);

};

class Shader;

struct Vertex
{
    glm::vec3 m_Position;
    glm::vec3 m_Normal;
    glm::vec2 m_TexCoords;
};

struct Texture 
{
    unsigned m_ID;
    std::string m_Type;
	std::string m_Path;
};

class Mesh
{
protected:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned> m_Indices;
    std::vector<Texture> m_Textures;

    unsigned m_VAO, m_VBO, m_EBO;

    void SetupMesh();
public:
    Mesh(std::vector<Vertex> Vertices, std::vector<unsigned> Indices, std::vector<Texture> Textures);
    void Draw(Shader shader);
};

class Model
{
protected:
	std::vector<Texture> m_LoadedTextures;
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	void LoadModel(std::string Path);
	void ProcessNode(aiNode *Node, const aiScene *Scene);
	Mesh ProcessMesh(aiMesh *Mesh, const aiScene *Scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial *Material, aiTextureType Type, std::string TypeName);
	unsigned TextureFromFile(const char *Path, bool Gamma = false);

public:

	Model(const char *Path)
	{
		LoadModel(Path);
	}
	
	void Draw(Shader shader);
};


