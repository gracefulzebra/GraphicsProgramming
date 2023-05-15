#version 440 core

uniform float time;

in vec2 texCoord0;

out vec4 FragColor;

vec3 color;

vec2 noise2x2(vec2 p) 
{
   float x = dot(p, vec2(123.4, 234.5));
   float y = dot(p, vec2(345.6, 456.7));
   vec2 noise = vec2(x, y);
   noise = sin(noise);
   noise = noise * 43758.5453;
   noise = fract(noise);
   return noise;
}

vec3 colorPow(vec3 _color, int indice)
{
   _color.x = pow(_color.x, indice);
   _color.y = pow(_color.y, indice);
   _color.z = pow(_color.z, indice);
   return _color;
}

void main() 
{
   vec2 uv = texCoord0;
   uv = uv * 24.0;

   vec2 currentGridId = floor(uv);
   vec2 currentGridCoord = fract(uv);
 
   currentGridCoord = currentGridCoord - 0.5;

   float minDistGridId = 100;
   for (float i = -1.0; i <= 1.0; i++) 
   {
      for (float j = -1.0; j <= 1.0; j++) 
      {
         vec2 adjacentCellCoords = vec2(i, j);

         vec2 noise = noise2x2(currentGridId + adjacentCellCoords);
         adjacentCellCoords += sin(time * noise) * 0.5;

         float dist = length(currentGridCoord - adjacentCellCoords);
         minDistGridId = min(dist, minDistGridId);
     }
   }

   color = vec3(smoothstep(0.25, 1.0, 1.0 - minDistGridId / 2));

   color = vec3(1) - color;

   color = colorPow(color, 5);

   color = color * vec3(0.26, 0.84, 1);
  
   color += vec3(0.2, 0.56, 0.95);

   FragColor = vec4(color, 1.0);
}
