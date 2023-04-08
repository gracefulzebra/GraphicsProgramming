#pragma once

#include "ShaderHandler.h"
#include "transform.h"
#include "ShaderHandler.h"
#include <string>
#include <GL\glew.h>
#include <vector>
using namespace std;

class SkyBoxRenderer
{
public:
	SkyBoxRenderer();
	~SkyBoxRenderer();

	void PrepareVAO();

	void InitializeSkyBoxRenderer();
	void InitializeCubeMap();

	void DrawSkybox(Viewport& _mainViewport);
	void DrawReflectionCube(Viewport& _mainViewport, float& time);
	void DrawRefractionCube(Viewport& _mainViewport, float& time);

protected:
private:

	//Location to which the textures are bound
	unsigned int textureID = NULL;

	//Reference values for vertex reservation
	GLuint skyboxVAO, skyboxVBO = NULL;
	GLuint reflectionCubeVAO, reflectionCubeVBO = NULL;
	GLuint refractionCubeVAO, refractionCubeVBO = NULL;

	ShaderHandler reflectionShader, refractionShader, skyBoxShader;
	Transform transform;

	//Cubemap images stored in order
	vector<string> faces = {	
		"..\\res\\cubemap\\right.jpg",
		"..\\res\\cubemap\\left.jpg",
		"..\\res\\cubemap\\top.jpg",
		"..\\res\\cubemap\\bottom.jpg",
		"..\\res\\cubemap\\front.jpg",
		"..\\res\\cubemap\\back.jpg"
	};
};

