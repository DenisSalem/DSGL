#version 330

in vec2 TexCoord;
in vec4 gl_FragCoord ;

out vec4 color;

// Do not forget to use the right samplerBuffer type
uniform usamplerBuffer currentTexture;

void main() {
  	int coordinates = int(floor(gl_FragCoord.x) + (floor(gl_FragCoord.y) * 640));		// We must compute texture coordinates by ourself
	color = texelFetch(currentTexture, coordinates) / vec4(4294967295.0);	// We must normalize by ourself
}
