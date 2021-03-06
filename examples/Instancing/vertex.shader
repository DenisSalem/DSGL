#version 330

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 offset;

out vec4 myColor;

void main() {
	gl_Position = vec4(position *0.05 + offset, 0.0f, 1.0f);

	myColor = vec4(offset,vec2(1.0));
}
