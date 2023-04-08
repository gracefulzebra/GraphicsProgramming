#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;

out vec3 FragPosition;
out vec3 Normals;
out vec2 texCoord0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //Increments the texture coordiantes
    texCoord0 = texCoord; 

    //Sets the current fragment position in reference to the model
    FragPosition = vec3(model * vec4(aPos, 1.0));

    //Sets the normals and transposes and inverses them to reduce warping from scaling
    Normals = mat3(transpose(inverse(model))) * aNormal;  

    //Sets the position to the current MVP in referece to the position of the current fragment
    gl_Position = projection * view * vec4(FragPosition, 1.0);
    
}