#version 410
in vec3 vp;

uniform mat4 model;

void main () {
	gl_Position = model * vec4 (vp, 1.0);
}
