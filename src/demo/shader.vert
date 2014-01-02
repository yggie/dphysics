#version 110

#pragma debug(on)

in vec3 vPos;
in vec3 vNorm;
in vec4 vColor;
//in vec2 vTexCoord;

uniform mat4 mModelView;
uniform mat4 mProj;
uniform mat4 mNorm;

//out vec2 vS1TexCoord;
out vec3 norm;
out vec4 color;

void main(void) {
  // vertex is transformed by the MVP matrix
  gl_Position = mProj * mModelView * vec4(vPos, 1.0);
  
  // transform normal by Normal matrix
  norm = mNorm * vec4(vNorm, 0.0);
  
  // pass through
  color = vColor;
}

