#include "ObjectHandler.h"
#include <iostream>

ObjectHandler::ObjectHandler(){}
ObjectHandler::~ObjectHandler(){}

void ObjectHandler::InitializeObjHandler(AudioHandler& _audio)
{
	audio = _audio;

	//Initializes an instance of all of the shaders and textures.
	shaders[0].InitializeShader("..\\res\\shaders\\basiclighting");
	shaders[1].InitializeShader("..\\res\\shaders\\lightcubeshader");
	textures[0].InitializeTexture("..\\res\\textures\\hair.jpg");
	textures[1].InitializeTexture("..\\res\\textures\\bricks.jpg");
	textures[2].InitializeTexture("..\\res\\textures\\backpack.jpg");

	//Initializes the model for the light source
	light.InitializeModel(textures[1], shaders[1], glm::vec3(0.0, 10.0, 0.0), glm::vec3(0.6, 0.6, 0.6), "..\\res\\objects\\cube.obj");
}

void ObjectHandler::CheckCollisions()
{
	//Loops through all the objects comparing them against all the other objects
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		for (int j = 0; j < NUM_OBJECTS; j++)
		{
			//If the current object is the same in both loops then continue to the next iteration of the loop
			//If either of the objects have not been initialized then continue to the next iteration of the loop
			if (!objects[i].isInitialized || !objects[i + 1].isInitialized || i == j)
				continue;

			//Check collisions between the objects 
			if (collision.CheckCollisions(objects[i].modelCollider.GetPos(), objects[j].modelCollider.GetPos(), objects[i].modelCollider.GetRadius(), objects[j].modelCollider.GetRadius()))
			{
				audio.playSoundEffect(0); //Play collision sound effect
			}
		}
	}
}

void ObjectHandler::CreateObject(TextureHandler& _texture, ShaderHandler& _shader, glm::vec3& _position, glm::vec3& _scale, const std::string& _modelFilePath)
{
	//If objects array is full return.
	if (createCount >= NUM_OBJECTS) { std::cout << "Error : Object Limit Reached" << "\n"; return; }

	std::cout << "Loading Model : " << _modelFilePath;
	objects[createCount] = GenerateObject(); //Create new model
	std::cout << "		Completed" << "\n";
	std::cout << "Initializing Object : " << _modelFilePath;
	objects[createCount].InitializeModel(_texture, _shader, _position, _scale, _modelFilePath); //Initialize Model
	std::cout << "	Completed" << "\n" << "\n";
	createCount++; //Mark object array size
}

Model& ObjectHandler::GenerateObject()
{
	Model* newObj = new Model(); //Creates a new instance of the model class
	return *newObj;
}

void ObjectHandler::DrawAllObjects(Viewport& _myViewPort, float& time)
{
	//Transform manipulation allowing for oscillating movement
	light.SetModelPos(glm::vec3(6.0f * sinf(time / 3), 3.0f, 2.0f));

	objects[0].SetModelPos(glm::vec3(5.0f + sinf(time), 0.0f, 2.0f * cosf(time)));
	
	objects[1].SetModelRot(glm::vec3(0.0f, 1.5f, 0.0f));
	objects[1].SetModelScale(glm::vec3(0.6f + 0.3f * sin(time / 2.0f)));
	
	objects[2].SetModelPos(glm::vec3(5.0f + sinf(time), 0.0f, 2.0f * -cosf(time)));
	objects[2].SetModelRot(glm::vec3(0.0f, 0.0f, time));
	
	//Draw the scene's light
	DrawLight(light, _myViewPort);

	//Loop through all the objects and draw them
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		DrawObject(objects[i], _myViewPort);
	}
}

void ObjectHandler::DrawObject(Model& _object, Viewport& _myViewPort)
{
	//Bind's the model's shaders and texture then draws it.
	_object.modelShader.BindShader();
	_object.modelShader.UpdateLightingShader(_object.modelTransform, _myViewPort, light.GetModelPos(), lightColor);
	_object.modelTexture.BindTexture(0);
	_object.modelMesh.DrawMesh();
}

void ObjectHandler::DrawLight(Model& _object, Viewport& _myViewPort)
{
	//Bind's the model's shaders and texture then draws it.
	_object.modelShader.BindShader();
	_object.modelShader.UpdateTransform(_object.modelTransform, _myViewPort);
	_object.modelTexture.BindTexture(0);
	_object.modelMesh.DrawMesh();
}



