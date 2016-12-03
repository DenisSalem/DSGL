#version 430

layout (location = 0) in vec3 position;

out vec4 myColor;

void main() {
	gl_Position = vec4(position, 1.0f);

	myColor = vec4(position.xy,1.0,1.0);
}
