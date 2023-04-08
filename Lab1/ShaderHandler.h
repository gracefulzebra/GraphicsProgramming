#pragma once

#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "Viewport.h"

class ShaderHandler
{
public:
	ShaderHandler();
	~ShaderHandler();

	void BindShader(); //Set gpu to use our shaders
	void UpdateTransform(const Transform& transform, const Viewport& Viewport);
	void UpdateSkyBoxTransform(const Transform& transform, const Viewport& Viewport);
	void UpdateReflections(const Transform& transform, const Viewport& Viewport);
	void UpdateLightingShader(const Transform& transform, const Viewport& Viewport, glm::vec3& lightPos, glm::vec3 lightColor);
	void InitializeShader(const std::string& filename);

	std::string ShaderHandler::LoadShaderFromFile(const std::string& fileName);
	void ShaderHandler::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint ShaderHandler::CreateShader(const std::string& text, unsigned int type);

protected:
private:

	enum
	{
		TRANSFORM_U,
		MODEL_U,
		PROJECTION_U,
		VIEW_U,
		CAMERAPOSITION_U,
		SKYBOX_U,
		LIGHTPOS_U,
		LIGHTCOLOR_U,
		NUM_UNIFORMS
	};

	static const unsigned int NUM_SHADERS = 2; // number of shaders
	GLuint program = NULL; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables
};
