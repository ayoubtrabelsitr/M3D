#version 450

layout( location = 0 ) out vec4 fragColor;
layout( binding = 1 ) uniform sampler2D uPosition;
layout( binding = 2 ) uniform sampler2D uNormalMap;
layout( binding = 3 ) uniform sampler2D uAmbientMap;
layout( binding = 4 ) uniform sampler2D uDiffuseMap;
layout( binding = 5 ) uniform sampler2D uSpecularMap;
 uniform vec3 LightPOS;

void main()
{
ivec2 coord = ivec2(gl_FragCoord.xy);//R�cup�rer les coordonn�es du pixel courant via la variable int�gr�e gl_FragCoord
//R�cup�rer les donn�es depuis les textures via texelFetch
vec3 Position = texelFetch(uPosition, coord, 0).xyz;
vec3 Normal = texelFetch(uNormalMap, coord, 0).xyz;
vec3 Ambient = texelFetch(uAmbientMap, coord, 0).xyz;
vec3 Diffuse = texelFetch(uDiffuseMap, coord, 0).xyz;
vec4 Specular = texelFetch(uSpecularMap, coord, 0);

// L'�clairage
vec3 lightDir = normalize(LightPOS-Position);
vec3 Lo = normalize(-Position);
Normal = normalize(Normal);

if (dot(lightDir, Normal) < 0)
	Normal = -Normal;
//coefficient Diffuse
float angle = max(dot(Normal,lightDir), 0.f);
//coefficient Specular
float spec = pow(max(dot(Normal, normalize(lightDir + Lo)) , 0.f), Specular.w);

Diffuse=Diffuse*angle;

Specular=Specular*spec;

fragColor = vec4(Ambient + Diffuse + Specular.xyz, 1.f);
}
