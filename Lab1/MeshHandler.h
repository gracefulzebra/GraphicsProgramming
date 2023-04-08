#pragma once

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "obj_loader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& position, const glm::vec2& textureCoordinate)
	{
		this->position = position;
		this->textureCoordinate = textureCoordinate;
		this->normal = normal;
	}

	glm::vec3* GetPosition() { return &position; }
	glm::vec2* GetTextureCoordinate() { return &textureCoordinate; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 position;
	glm::vec2 textureCoordinate;
	glm::vec3 normal;
};

class MeshHandler
{
public:
	MeshHandler();
	~MeshHandler();

	void DrawMesh();
	void InitializeMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	void LoadModelFromFile(const std::string& filename);
	
private:
	void InitializeModel(const IndexedModel& model);

	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};
	
	GLuint vertexArrayObject = NULL;
	GLuint vertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int drawCount; //how much of the vertexArrayObject do we want to draw
};