#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 Camerapos ; 

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

uniform bool uHasNormalMap;
layout(binding = 5) uniform sampler2D uNormalMap; 

in vec3 LightPos;
in vec2 texturecord;
in vec3 Normal;
in vec3 FragPos;

void main()
{

vec3 normal=normalize(Normal) ;

vec3 Var_Diffuse;
vec3 Var_Spec;

vec3 viewDir = normalize(Camerapos - FragPos);
vec3 lightDir  = normalize(LightPos-FragPos);	
if(texture(uDiffuseMap, texturecord).a < 0.5f)//Si alpha de la texture diffuse, s’il est inférieur à 0.5, ignorez le fragment via l’instruction discard
	discard;


if(uHasNormalMap)
{
	normal = texture(uNormalMap, texturecord).xyz;
    
    normal = normalize(normal * 2.0 - 1.0);
}
if(dot(lightDir,normal)<0)
normal=-normal;

if (uHasDiffuseMap)
	{
	 Var_Diffuse = vec3(texture(uDiffuseMap, texturecord))*max ( dot(normal , lightDir ), 0.f ) ;  
	}    
else {
	Var_Diffuse = max ( dot(normal , lightDir ), 0.f ) * diffuse_color;
	}
vec3 Var_ambient = ambient_color;
	if (uHasAmbientMap)
	{
	Var_ambient = vec3(texture(uAmbientMap, texturecord));   
	} 
float Shininess =shininess;
	if (uHasShininessMap)
	{
		Shininess = texture(uShininess, texturecord).x;
	}
	vec3 speculair = speculaire_color;
	if (uHasSpecularMap)
	{
	 Var_Spec = vec3(texture(uSpecularMap, texturecord)).xxx*pow(max(dot(normal,normalize(lightDir+viewDir)  ), 0.0),shininess);
	} 
	else {
	Var_Spec = speculaire_color* pow(max(dot(normal,normalize(lightDir+viewDir)  ), 0.0),shininess);
	 }
vec3 result =Var_ambient+ Var_Diffuse+Var_Spec;
fragColor =vec4( result, 1.f);
//fragColor =vec4( result, texture(uDiffuseMap, texturecord).a);// donnez la valeur du canal alpha de la texture diffuse à la couleur finale

}
