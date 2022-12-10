#version 450

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 MVPMatrix; // Projection * View * Model
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;


out vec3 lightposout;
out vec3 FragPos;
out vec3 Normal ; 
out vec2 texturecord;
void main()
{
	texturecord = aVertexTexCoords;
    FragPos = vec3( ViewMatrix * ModelMatrix * vec4(aVertexPosition, 1.f) );
	Normal = mat3( NormalMatrix) * aVertexNormal ;
	

gl_Position = MVPMatrix * vec4(aVertexPosition, 1.f);



	
}

