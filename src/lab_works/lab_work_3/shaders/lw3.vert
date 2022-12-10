#version 450 
layout (location = 0) in vec3 aVertexPosition;//La position des sommets en 2D associ� � l'index 0
layout (location =1) in vec3 color;////La couleur des sommets � l'index 1

out vec3 colorf;

//uniform mat4 Modelmatrix;//repr�esentant la Model matrix
//uniform mat4 Viewmatrix;//repr�esentant la View matrix
//uniform mat4 Projectionmatrix;
uniform mat4 MVP;
void main() 
{
colorf=color;
//gl_Position=vec4(aVertexPosition,1.f);// La sortie
//gl_Position=Modelmatrix*vec4(aVertexPosition,1.f);// La sortie 

//gl_Position=Projectionmatrix*Viewmatrix*Modelmatrix*vec4(aVertexPosition,1.f);// La sortie 
gl_Position=MVP*vec4(aVertexPosition,1.f);// La sortie

}