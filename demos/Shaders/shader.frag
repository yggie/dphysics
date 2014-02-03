#version 110

#pragma debug(on)

uniform mat4 mNorm;

//uniform sampler2D tex;
//uniform vec4 uniformColor;

uniform vec3 diffuse;
uniform float alpha;

in vec3 pos;
in vec3 norm;
in vec4 color;

void main(void) {
  // define material constants
  const vec3 normalizedVertToLight = vec3(0.0, 1.0, 0.0);
  
  float diffuseTerm = clamp(pow(dot(norm, normalizedVertToLight), 3), 0.0, 1.0) + 0.1;
  float nalpha = alpha * pow(clamp(0.7*length(pos), 0.0, 1.0), 5);

  gl_FragColor = vec4(diffuseTerm * diffuse, nalpha);
}

