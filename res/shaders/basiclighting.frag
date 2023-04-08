#version 330 core

in vec2 texCoord0;
in vec3 FragPosition;
in vec3 Normals;

out vec4 FragColor;

uniform vec3 lightPos; 
uniform vec3 cameraPos; 
uniform vec3 lightColor;
uniform sampler2D sampler;

void main()
{
    //Sets the objectColor the the current sampled coordinates of the texture
	vec3 objectColor = vec3(texture2D(sampler, texCoord0));
    
    // ambient light
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse light
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(cameraPos - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    //calculates the result as the sum of the lighting set to the colour of the object
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1);
}
