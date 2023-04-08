#include "ShaderHandler.h"
#include <iostream>
#include <fstream>

ShaderHandler::ShaderHandler(){}
ShaderHandler::~ShaderHandler()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(program, shaders[i]); //detach shader from program
		glDeleteShader(shaders[i]); //delete the sahders
	}
	glDeleteProgram(program); // delete the program
}

void ShaderHandler::InitializeShader(const std::string& filename)
{
	program = glCreateProgram(); // create shader program (openGL saves as ref number)
	shaders[0] = CreateShader(LoadShaderFromFile(filename + ".vert"), GL_VERTEX_SHADER); // create vertex shader
	shaders[1] = CreateShader(LoadShaderFromFile(filename + ".frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(program, shaders[i]); //add all our shaders to the shader program "shaders" 
	}

	glLinkProgram(program); //create executables that will run on the GPU shaders
	CheckForShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(program); //check the entire program is valid
	CheckForShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform"); // associate with the location of uniform variable within a program
	uniforms[MODEL_U] = glGetUniformLocation(program, "model");
	uniforms[PROJECTION_U] = glGetUniformLocation(program, "projection");
	uniforms[VIEW_U] = glGetUniformLocation(program, "view");
	uniforms[CAMERAPOSITION_U] = glGetUniformLocation(program, "cameraPos");
	uniforms[SKYBOX_U] = glGetUniformLocation(program, "skybox");
	uniforms[LIGHTPOS_U] = glGetUniformLocation(program, "lightPos");
	uniforms[LIGHTCOLOR_U] = glGetUniformLocation(program, "lightColor");
}

void ShaderHandler::BindShader()
{
	glUseProgram(program); //installs the program object specified by program as part of rendering state
}

void ShaderHandler::UpdateTransform(const Transform& transform, const Viewport& Viewport)
{
	glm::mat4 mvp =  transform.GetMVP(Viewport); //Get the model view projection matrix
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]); //Pass the MVP into the shader as a uniform
}

void ShaderHandler::UpdateSkyBoxTransform(const Transform& transform, const Viewport& Viewport)
{
	glm::mat4 mv = (glm::mat3(Viewport.GetViewMatrix())); //Cast to mat3 and then back to mat4 to remove w column translation values.
	glm::mat4 mp = Viewport.GetProjectionMatrix();

	GLuint skyBox = 0; //Value representing location for cubemap sampler

	//Pass the values into the shaders as uniform
	glUniformMatrix4fv(uniforms[VIEW_U], 1, GLU_FALSE, &mv[0][0]); 
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GLU_FALSE, &mp[0][0]);
	glUniform1i(uniforms[SKYBOX_U], skyBox);
}

void ShaderHandler::UpdateReflections(const Transform& transform, const Viewport& Viewport)
{
	GLuint skyBox = 0;
	glm::vec3 cameraPos = Viewport.GetPos();
	glm::mat4 model = transform.GetModel();
	glm::mat4 mv = Viewport.GetViewMatrix();
	glm::mat4 mp = Viewport.GetProjectionMatrix();

	//Pass the values into the shaders as uniform
	glUniform3fv(uniforms[CAMERAPOSITION_U], 1, &cameraPos[0]);
	glUniform1i(uniforms[SKYBOX_U], skyBox);
	glUniformMatrix4fv(uniforms[MODEL_U], 1, GLU_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniforms[VIEW_U], 1, GLU_FALSE, &mv[0][0]);
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GLU_FALSE, &mp[0][0]);
}

void ShaderHandler::UpdateLightingShader(const Transform& transform, const Viewport& Viewport, glm::vec3& lightPos, glm::vec3 lightColor)
{
	glm::vec3 cameraPos = Viewport.GetPos();
	glm::mat4 model = transform.GetModel();
	glm::mat4 mv = Viewport.GetViewMatrix();
	glm::mat4 mp = Viewport.GetProjectionMatrix();

	//Pass the values into the shaders as uniform
	glUniform3fv(uniforms[CAMERAPOSITION_U], 1, &cameraPos[0]);
	glUniform3fv(uniforms[LIGHTCOLOR_U], 1, &lightColor[0]);
	glUniform3fv(uniforms[LIGHTPOS_U], 1, &lightPos[0]);
	glUniformMatrix4fv(uniforms[MODEL_U], 1, GLU_FALSE, &model[0][0]);
	glUniformMatrix4fv(uniforms[VIEW_U], 1, GLU_FALSE, &mv[0][0]);
	glUniformMatrix4fv(uniforms[PROJECTION_U], 1, GLU_FALSE, &mp[0][0]);
}

GLuint ShaderHandler::CreateShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	if (shader == 0) //if == 0 shader no created
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); //send source code to opengl
	glCompileShader(shader); //get open gl to compile shader code

	CheckForShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string ShaderHandler::LoadShaderFromFile(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void ShaderHandler::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

