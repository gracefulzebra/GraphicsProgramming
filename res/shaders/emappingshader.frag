#version 420 core

layout (binding = 1) uniform sampler2D diffuse;

uniform vec3 cameraPos;
uniform samplerCube skybox;


in vec2 tC;
in vec3 Normal;
in vec3 Position;


out vec4 FragColor;

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));

    vec4 colour = vec4(texture(skybox, R).rgb, 1.0);
    vec4 objectColor = texture2D(diffuse, tC);

    FragColor = mix(colour, objectColor, 0.4);
}
