#version 450

layout( location = 0 ) out vec4 fragColor;

uniform vec3 Camerapos ; 
uniform vec3 lightPos;
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 speculaire_color;
uniform float shininess;

in vec3 Normal;
in vec3 FragPos;

void main()
{

vec3 normal=normalize(Normal) ;


vec3 viewDir = normalize(Camerapos - FragPos);
vec3 lightDir  = Camerapos-FragPos;	
if(dot(lightDir,normal)<0)
normal=-normal;
vec3 Diffuse = max ( dot(normal , lightDir ), 0.f ) * diffuse_color;

vec3 Spec = speculaire_color* pow(max(dot(normal,normalize(lightDir+viewDir)  ), 0.0),shininess);

vec3 result =ambient_color+ Diffuse+Spec;

fragColor =vec4( result, 1.f);

}
