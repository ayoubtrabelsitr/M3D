#version 450
layout( location = 0) out vec4 fragColor; //couleur par fragment
in vec3 colorf;

void main() {


fragColor = vec4 (colorf,1.0f);
}