#version 440 core

layout (binding = 1) uniform sampler2D diffuse;

uniform vec3 cameraPos;
uniform samplerCube skybox;

in Vertex_DATA{
    vec2 tC;
    vec3 Normal;
    vec3 Position;
} vs_in;

out vec4 FragColor;

void main()
{    
    vec3 I = normalize(vs_in.Position - cameraPos);
    vec3 R = reflect(I, normalize(vs_in.Normal));

    vec4 colour = vec4(texture(skybox, R).rgb, 1.0);
    vec4 objectColor = vec4(texture2D(diffuse, vs_in.tC).rgb, 1.0);
   
    FragColor = colour;
}
