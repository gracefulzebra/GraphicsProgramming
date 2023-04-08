#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    //Set the frag color to the texture colour at the sampled coords
    FragColor = texture(skybox, TexCoords);
}
