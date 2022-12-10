#version 450 
layout (location = 0) in vec3 aVertexPosition; //La position des sommets en 2D associé à l'index 0
layout (location =1) in vec3 color;////La couleur des sommets à l'index 1
out vec3 colorf;
uniform float uTranslationX;
void main() 
{
colorf=color;
//gl_Position=vec4(aVertexPosition,1.f);// La sortie 
gl_Position=vec4(aVertexPosition.x+uTranslationX,aVertexPosition.y,aVertexPosition.z,1.f); //la valeur de uTranslationX sera 
																							//ajoutée à la coordonnée x de chaque sommet
}