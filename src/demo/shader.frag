#version 110

#pragma debug(on)

//uniform sampler2D tex;
//uniform vec4 uniformColor;

in vec4 vS1Color;
//in vec2 vS1TexCoord;

out vec4 vFragColor;

void main(void) {
  vFragColor = vS1Color;
}

