# ![DSGL](https://raw.githubusercontent.com/DenisSalem/DSGL/789ca6d88e197f5a806179337ba8a793c4fc6968/doc/dsglLogo.png "DSGL")

# Damn Simple OpenGL

1. [About](#about)
2. [Installation and compilation](#installation-and-compilation)
3. [API](#api)
	1. [Core](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md)
		1. [DSGL_TRACE](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#dsgl_trace)
		2. [PrintNicelyWorkGroupsCapabilities](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#printnicelyworkgroupscapabilities)
		3. [Exception](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#exception)
		4. [Context](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#context)
		5. [VertexBufferObject](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#vertexbufferobject)
		6. [VertexArrayObject](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#vertexarrayobject)
		7. [Elements](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#elements)
		8. [Shader](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#shader)
		9. [ShaderProgram](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#shaderprogram)
		10. [PipelineProgram](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#pipelineprogram)
		11. [ComputeProgram](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md#computeprogram)
	2. [Meshes](https://github.com/DenisSalem/DSGL/blob/master/doc/meshesEN.md)
		1. [Triangle](https://github.com/DenisSalem/DSGL/blob/master/doc/meshesEN.md#triangle)
		2. [Quad](https://github.com/DenisSalem/DSGL/blob/master/doc/meshesEN.md#quad)
4. [Examples](https://github.com/DenisSalem/DSGL/tree/master/examples)
5. [FAQ](#faq)
	1. [How do I use my extensions loader](#how-do-i-use-my-extensions-loader)

## About

DSGL is a tiny and simple modern OpenGL wrapper libraries written in C++ and intented to do not hide OpenGL but simply
make the programmer's life easier by providing some general purpose classes.

__Careful by the way, DSGL is highly experimental and for instance documentation isn't complete. Feedback and bug reports will be appreciated!__

DSGL is a free software under [GNU/GPLv3](https://www.gnu.org/licenses/quick-guide-gplv3.en.html) license.

## Installation and compilation

DSGL support some nice libraries you may want to use by passing to compiler some option depending of what you want.

- [glfw](http://www.glfw.org/) : compile with *-D DSGL_GLFW*.
- [gl3w](https://github.com/skaslev/gl3w) : compile with *-D DSGL_GL3W*.


Include [dsgl.hpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.hpp) and [dsgl.cpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.cpp) from [Core](https://github.com/DenisSalem/DSGL/tree/master/core) in your project for general purposes features.

Depending of your needs one may want to also includes all sources files from

- [meshes](https://github.com/DenisSalem/DSGL/tree/master/meshes)
- [images](https://github.com/DenisSalem/DSGL/tree/master/images)
- [generative](https://github.com/DenisSalem/DSGL/tree/master/generative)

## API

- [Core](https://github.com/DenisSalem/DSGL/blob/master/doc/coreEN.md) contains main DSGL features, each of them are defined within DSGL namespace.
- [Meshes](https://github.com/DenisSalem/DSGL/blob/master/doc/meshesEN.md) hold some simples meshes for debugging, testing. Also useful for basic needs. Everything there is defined within DSGL::Meshes namespace.

## FAQ

## How do I use my extensions loader

You must include by yourself your alternative library headers __IN__ dsgl.hpp and every dsgl headers you want to include in your project.
