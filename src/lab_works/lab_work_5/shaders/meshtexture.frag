#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 Camerapos ; 
uniform vec3 lightPos;
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 speculaire_color;
uniform float shininess;


uniform bool uHasDiffuseMap;
layout( binding = 1 ) uniform sampler2D uDiffuseMap ;

uniform bool uHasAmbientMap;
layout(binding = 2) uniform sampler2D uAmbientMap;

uniform bool uHasSpecularMap;
layout(binding = 3) uniform sampler2D uSpecularMap;

uniform bool uHasShininessMap;
layout(binding = 4) uniform sampler2D uShininess;


in vec2 texturecord;
in vec3 Normal;
in vec3 FragPos;

void main()
{

vec3 normal=normalize(Normal) ;

vec3 Diffuse;
vec3 Spec;

vec3 viewDir = normalize(Camerapos - FragPos);
vec3 lightDir  = normalize(lightPos-FragPos);	
if(dot(lightDir,normal)<0)
normal=-normal;
if (uHasDiffuseMap)
	{
	 Diffuse = vec3(texture(uDiffuseMap, texturecord))*max ( dot(normal , lightDir ), 0.f ) ;  
	}    
else {
	Diffuse = max ( dot(normal , lightDir ), 0.f ) * diffuse_color;
	}
vec3 ambient = ambient_color;
	if (uHasAmbientMap)
	{
	ambient = vec3(texture(uAmbientMap, texturecord));   
	} 
float Shininess =shininess;
	if (uHasShininessMap)
	{
		Shininess = texture(uShininess, texturecord).x;
	}
	vec3 speculair = speculaire_color;
	if (uHasSpecularMap)
	{
	 Spec = vec3(texture(uSpecularMap, texturecord)).xxx*pow(max(dot(normal,normalize(lightDir+viewDir)  ), 0.0),shininess);
	} 
	else {
	Spec = speculaire_color* pow(max(dot(normal,normalize(lightDir+viewDir)  ), 0.0),shininess);
	 }
vec3 result =ambient_color+ Diffuse+Spec;

fragColor =vec4( result, 1.f);

}
