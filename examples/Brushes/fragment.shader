#version 430

in vec2 TexCoord;
out vec4 color;

uniform usamplerBuffer currentTexture;

void main() {
  	int coordinates = int(floor(gl_FragCoord.x) + (floor(gl_FragCoord.y) * 2048));	// We must compute texture coordinates by ourself
	color = texelFetch(currentTexture, coordinates);// / vec4(4294967295.0);		// We must normalize by ourself
}
