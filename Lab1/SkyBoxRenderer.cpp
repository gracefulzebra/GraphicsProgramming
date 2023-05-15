#include "SkyBoxRenderer.h"
#include "stb_image.h"
#include <iostream>

SkyBoxRenderer::SkyBoxRenderer()
{
    skyboxVAO = NULL;
    skyboxVBO = NULL;
    reflectionCubeVAO = NULL; 
    reflectionCubeVBO = NULL;
    refractionCubeVAO = NULL;
    refractionCubeVBO = NULL;
    textureID = NULL;
}

SkyBoxRenderer::~SkyBoxRenderer() {}

void SkyBoxRenderer::InitializeSkyBoxRenderer()
{
    PrepareVAO();
    InitializeCubeMap();

    //Initializes the cubemap shaders
    skyBoxShader.InitializeShader("..\\res\\shaders\\skyboxshader");
    reflectionShader.InitializeShader("..\\res\\shaders\\reflectionshader");
    refractionShader.InitializeShader("..\\res\\shaders\\refractionshader");
    emappingShader.InitializeShader("..\\res\\shaders\\emappingshader");
    transform.SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
    emapTexture.InitializeTexture("..\\res\\textures\\hair.jpg");
    emapMesh.LoadModelFromFile("..\\res\\objects\\monkey3.obj");
}

//Vertices used to draw skybox mesh
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//vertices and normals for drawing cube objects for cubemap
float cubeVertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f   
     
};

//Reads the cube map faces and assigns them to the textureID location
void SkyBoxRenderer::InitializeCubeMap()
{
    int width, height, nrChannels;
    unsigned char* data;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); //Binds the texture as cubemap

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

}

//Prepares the buffers for drawing the cubemap objects
void SkyBoxRenderer::PrepareVAO()
{
    glGenVertexArrays(1, &emappingCubeVAO);
    glGenBuffers(1, &emappingCubeVBO);
    glBindVertexArray(emappingCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, emappingCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenVertexArrays(1, &reflectionCubeVAO);
    glGenBuffers(1, &reflectionCubeVBO);
    glBindVertexArray(reflectionCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, reflectionCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenVertexArrays(1, &refractionCubeVAO);
    glGenBuffers(1, &refractionCubeVBO);
    glBindVertexArray(refractionCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, refractionCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyBoxRenderer::DrawSkybox(Viewport& _mainViewport)
{
    skyBoxShader.BindShader();

    glm::mat4 mv = (glm::mat3(_mainViewport.GetViewMatrix()));
    glm::mat4 mp = _mainViewport.GetProjectionMatrix();
    GLuint skyBox = 0;

    skyBoxShader.setMat4("projection", mp);
    skyBoxShader.setMat4("view", mv);
    skyBoxShader.setInt("skybox", skyBox);

    glDepthFunc(GL_LEQUAL); //Set the depth function to draw at the maximum depth (behind the rest of the scene)
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); //Binds the texture as cubemap
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void SkyBoxRenderer::DrawReflectionCube(Viewport& _mainViewport, float& time)
{
    //Transform manipulation to spin object
    transform.SetPos(glm::vec3(-1, 0, -3));
    transform.SetRot(glm::vec3(0, time * 0.2, 0));
    transform.SetScale(glm::vec3(1.0, 1.0, 1.0));
    
    reflectionShader.BindShader();

    GLuint skyBox = 0;
    glm::vec3 cameraPos = _mainViewport.GetPos();
    glm::mat4 model = transform.GetModel();
    glm::mat4 mv = _mainViewport.GetViewMatrix();
    glm::mat4 mp = _mainViewport.GetProjectionMatrix();

    reflectionShader.setVec3("cameraPos", cameraPos);
    reflectionShader.setMat4("model", model);
    reflectionShader.setMat4("projection", mp);
    reflectionShader.setMat4("view", mv);
    reflectionShader.setInt("skybox", skyBox);
    
    glBindVertexArray(reflectionCubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void SkyBoxRenderer::DrawRefractionCube(Viewport& _mainViewport, float& time)
{
    //Transform manipulation to spin object
    transform.SetPos(glm::vec3(1, 0, -3));
    transform.SetRot(glm::vec3(0, -time * 0.2, 0));
    transform.SetScale(glm::vec3(1.0, 1.0, 1.0));

    refractionShader.BindShader();

    GLuint skyBox = 0;
    glm::vec3 cameraPos = _mainViewport.GetPos();
    glm::mat4 model = transform.GetModel();
    glm::mat4 mv = _mainViewport.GetViewMatrix();
    glm::mat4 mp = _mainViewport.GetProjectionMatrix();

    reflectionShader.setVec3("cameraPos", cameraPos);
    reflectionShader.setMat4("model", model);
    reflectionShader.setMat4("projection", mp);
    reflectionShader.setMat4("view", mv);
    reflectionShader.setInt("skybox", skyBox);

    glBindVertexArray(refractionCubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void SkyBoxRenderer::DraweMapCube(Viewport& _mainViewport, float& time)
{
    transform.SetPos(glm::vec3(0, 2, -10));
    transform.SetRot(glm::vec3(0, time * 0.2, 0));
    transform.SetScale(glm::vec3(2.0, 2.0, 2.0));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    emappingShader.BindShader();

    glm::mat4 model = transform.GetModel();
    glm::mat4 mvp = _mainViewport.GetViewProjectionMatrix() * model;
    glm::vec3 cameraPos = _mainViewport.GetPos();

    emappingShader.setMat4("model", model);
    emappingShader.setMat4("transform", mvp);
    emappingShader.setVec3("cameraPos", cameraPos); 

    GLuint diffuseTexLocation = glGetUniformLocation(emappingShader.getID(), "diffuse");
    GLuint skyboxTexLocation = glGetUniformLocation(emappingShader.getID(), "skybox");

    glUniform1i(diffuseTexLocation, 1);
    glUniform1i(skyboxTexLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, emapTexture.getID());
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    emapMesh.DrawMesh();
}


