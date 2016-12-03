#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba32f, binding = 0) uniform image2D img_output;

void main() {
  ivec2 coords = ivec2(gl_GlobalInvocationID);

  vec4 pixel;
  if ( ((gl_WorkGroupID.x & 1u) != 1u) != ((gl_WorkGroupID.y & 1u) == 1u)) {
    pixel = vec4(1.0,.5,.0,1.0);
  }
  else {
    pixel = vec4(.0,.5,1.0,1.0);
  }

  imageStore(img_output, coords, pixel);
}
