//Version number
#version 440 core

//Layout qualfier
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

//Passing in texture coordinates
in VS_OUT {
    vec3 vFragPosition;
    vec3 vNormals;
    vec2 vTexCoord;
} gs_in[];

out GS_OUT{
    vec3 gFragPosition;
    vec3 gNormals;
    vec2 gTexCoord;
} gs_out;

//Passing out texture coordinates
out vec2 texCoords; 

//Uniform variabe
uniform float time;


vec4 explode(vec4 position, vec3 normal)
{
    //Amout of explosion
    float magnitude = 8.0;
	//Direction of explosion, going along normal
    vec3 direction = normal * abs(sin(time)) / 5 * magnitude; 
	//Returning position
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
//Getting the normal vector of each vertex
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   //returns the cross product between the two vectors calculated
   return normalize(cross(a, b));
}

void main()
{
    //Getting normal
    vec3 normal = GetNormal();
    gs_out.gNormals = (gs_in[0].vNormals + gs_in[1].vNormals + gs_in[2].vNormals) / 3; //Sends average of normals to fragment
    gs_out.gFragPosition = gs_in[0].vFragPosition;

    //Setting current vertex position
    gl_Position = explode(gl_in[0].gl_Position, normal);
    gs_out.gTexCoord = gs_in[0].vTexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[1].gl_Position, normal);
    gs_out.gTexCoord = gs_in[1].vTexCoord;
    EmitVertex();

    gl_Position = explode(gl_in[2].gl_Position, normal);
    gs_out.gTexCoord = gs_in[2].vTexCoord;
    EmitVertex();

    EndPrimitive();
}  