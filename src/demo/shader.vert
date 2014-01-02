#version 110

#pragma debug(on)

in vec3 vPos;
in vec4 vColor;
//in vec2 vTexCoord;

uniform mat4 mModel;
uniform mat4 mView;
uniform mat4 mProj;

out vec4 vS1Color;
//out vec2 vS1TexCoord;

void main(void) {
  gl_Position = mProj * mView * mModel * vec4(vPos, 1.0);
  vS1Color = vColor;
//  vS1TexCoord = vTexCoord;
}

