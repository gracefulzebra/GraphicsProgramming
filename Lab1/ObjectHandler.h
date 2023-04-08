#pragma once

#include "MeshHandler.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "transform.h"
#include "Viewport.h"
#include "Collision.h"
#include "AudioHandler.h"

struct SphereCollider
{
	SphereCollider() {}

	glm::vec3& GetPos() { return pos; }
	float& GetRadius() { return radius; }

	void SetPos(glm::vec3& pos)
	{
		this->pos = pos;
	}

	void SetRadius(float& radius)
	{
		this->radius = radius;
	}

	void SetRadius(glm::vec3 scale)
	{
		//Sets the collider's radius to the
		//largest component of the scale vector
		float radius = std::max(scale.x, scale.y);
		radius = std::max(radius, scale.z);

		this->radius = radius;
	}

private:
	glm::vec3 pos;
	float radius = 0;
};

struct Model
{
public:
	
	Model(){};

	//Initializes the model with all the parameters it needs to construct an instance of an object
	void InitializeModel(TextureHandler& _texture, ShaderHandler& _shader, glm::vec3& _position, glm::vec3& _scale, const std::string& _modelFilePath)
	{
		this->modelTexture = _texture;
		this->modelShader = _shader;
		modelMesh.LoadModelFromFile(_modelFilePath);
		InitializeTransform(_position, _scale);
		isInitialized = true; //Marks the object as having been initialized
	}

	//Setters to access the object's transform
	//Updates the scale and position of the object's collider to match
	void SetModelPos(glm::vec3 pos) { modelTransform.SetPos(pos); modelCollider.SetPos(pos); }
	void SetModelRot(glm::vec3 rot) { modelTransform.SetRot(rot); }
	void SetModelScale(glm::vec3 scale) { modelTransform.SetScale(scale); modelCollider.SetRadius(scale); }

	glm::vec3& GetModelPos() { return *modelTransform.GetPos(); }

	//Object properties
	TextureHandler modelTexture;
	MeshHandler modelMesh;
	Transform modelTransform;
	ShaderHandler modelShader;
	SphereCollider modelCollider;

	bool isInitialized = false;

protected:
private:
	void InitializeTransform(glm::vec3& position, glm::vec3& scale)
	{
		SetModelPos(glm::vec3(position));
		SetModelRot(glm::vec3(0.0, 0.0, 0.0));
		SetModelScale(glm::vec3(scale.x, scale.y, scale.z));

		InitializeCollider(scale.x, scale.y, scale.z);
	}

	void InitializeCollider(float x, float y, float z)
	{
		//Sets the collider's radius to the
		//largest component of the object's scale vector
		float scale = std::max(x, y);
		scale = std::max(scale, z);

		modelCollider.SetRadius(scale);
		modelCollider.SetPos(*modelTransform.GetPos());
	}
};

class ObjectHandler
{
public:
	ObjectHandler();
	~ObjectHandler();

	void InitializeObjHandler(AudioHandler& _audio);
	void DrawAllObjects(Viewport& _myViewPort, float& time);
	void CreateObject(TextureHandler& _texture, ShaderHandler& _shader, glm::vec3& _position, glm::vec3& _scale, const std::string& _modelFilePath);
	void CheckCollisions();
	
	enum Textures {
		BRICK_TEXTURE,
		HAIR_TEXTURE,
		BACKPACK_TEXTURE,
		NUM_TEXTURES
	};

	enum Shaders{
		BASICLIGHTING_SHADER,
		LIGHTCUBE_SHADER,
		NUM_SHADERS
	};

	enum Objects {
		MONKEY_OBJ,
		CUBE_OBJ,
		BACKPACK_OBJ,
		NUM_OBJECTS
	};
	
	AudioHandler audio;
	Collision collision;
	TextureHandler textures[NUM_TEXTURES];
	ShaderHandler shaders[NUM_SHADERS];
	Model objects[NUM_OBJECTS];
	Model light;

	glm::vec3 lightColor = (glm::vec3(1.0f, 1.0f, 1.0f));
	
protected:
private:
	void DrawObject(Model& _object, Viewport& _myViewPort);
	void DrawLight(Model& _object, Viewport& _myViewPort);
	Model& GenerateObject();

	unsigned int createCount = 0;
	
};



