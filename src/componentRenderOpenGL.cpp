#include "componentRenderOpenGL.h"

#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void Mesh::SetupMesh()
{
    glGenVertexArrays(1,&m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned), &m_Indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_TexCoords));

    glBindVertexArray(0);
}

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned> Indices, std::vector<Texture> Textures)
{
    m_Vertices = Vertices;
    m_Indices = Indices;
    m_Textures = Textures;

    SetupMesh();
}

void Mesh::Draw(Shader shader)
{
    unsigned diffuseNr = 1;
    unsigned specularNr = 1;
    for (int i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = m_Textures[i].m_Type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.SetFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_Textures[i].m_ID);
    }
	
	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Model::LoadModel(std::string Path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		// TODO: ERROR MESSAGES
		fprintf(stderr,"%s:%i\n", __FILE__, __LINE__);
		return;
	}

	m_Directory = Path.substr(0, Path.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode *Node, const aiScene *Scene)
{
	for (unsigned i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh *mesh = Scene->mMeshes[Node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, Scene));
	}
	
	for (unsigned i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene);
	}	

}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *Scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	std::vector<Texture> textures;

	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
			
		glm::vec3 vec;
		for (int j = 0; j < 3; j++)
		{
			vertex.m_Position[j] = mesh->mVertices[i][j];
			vertex.m_Normal[j] = mesh->mNormals[i][j];
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.m_TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.m_TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.m_TexCoords = glm::vec2(0.0f,0.0f);
		}

		vertices.push_back(vertex);

	
	}
		
	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = Scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial *Material, aiTextureType Type, std::string TypeName)
{
	std::vector<Texture> textures;
	for (unsigned i = 0; i < Material->GetTextureCount(Type); i++)
	{
		aiString str;
		Material->GetTexture(Type, i, &str);
		bool skip = false;
		for (auto&& j : m_LoadedTextures)
		{
			if (std::strcmp(j.m_Path.data(), str.C_Str()) == 0)
			{
				textures.push_back(j);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.m_ID = TextureFromFile(str.C_Str());
			texture.m_Type = TypeName;
			texture.m_Path = str.C_Str();
			textures.push_back(texture);
		}
	}
	return textures;
}

unsigned Model::TextureFromFile(const char *Path, bool Gamma)
{
	std::string filename = std::string(Path);
	filename = m_Directory + '/' + filename;

	unsigned textureID;
	glGenTextures(1, &textureID);

	int w, h, n;
	unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, 0);

	if (data)
	{
		GLenum format;
		switch (n){
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		fprintf(stderr,"%s:%i", __FILE__, __LINE__);
		// TODO: Error handling
	}
	
	return textureID;
}

void Model::Draw(Shader shader)
{
	for (auto&& mesh : m_Meshes){
		mesh.Draw(shader);
	}
}


