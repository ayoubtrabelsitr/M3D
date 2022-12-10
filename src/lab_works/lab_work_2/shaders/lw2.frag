#version 450
layout( location = 0) out vec4 fragColor; //couleur par fragment
in vec3 colorf;
uniform float c_luminosite; //variable uniforme correspondant à la luminosité

void main() {
fragColor = vec4 (colorf*c_luminosite,1.0f); 
}