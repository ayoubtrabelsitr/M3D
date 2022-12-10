#version 450 
layout (location = 0) in vec3 aVertexPosition; //La position des sommets en 2D associé à l'index 0
void main() 
{
gl_Position=vec4(aVertexPosition,1.f);// La sortie 

}

