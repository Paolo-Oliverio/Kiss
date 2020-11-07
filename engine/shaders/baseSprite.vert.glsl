#version 450

in vec2 pos;
in vec2 tex;
in vec4 col;
uniform mat3 xform;

out vec4 color;
out vec2 texcoord;

void main() {
	gl_Position = vec4(xform * vec3 (pos, 1.0), 1);
	texcoord = tex;
	color = col;
}