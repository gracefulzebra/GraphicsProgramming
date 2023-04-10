#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 vFragPosition;
    vec3 vNormals;
    vec2 vTexCoord;
} vs_out;

void main()
{
    //Increments the texture coordiantes
    vs_out.vTexCoord = texCoord; 

    //Sets the current fragment position in reference to the model
    vs_out.vFragPosition = vec3(model * vec4(aPos, 1.0));

    //Sets the normals and transposes and inverses them to reduce warping from scaling
    vs_out.vNormals = mat3(transpose(inverse(model))) * aNormal;  

    //Sets the position to the current MVP in referece to the position of the current fragment
    gl_Position = projection * view * vec4(vs_out.vFragPosition, 1.0);
    
}