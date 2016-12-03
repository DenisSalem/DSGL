# ![DSGL](https://framagit.org/denissalem/DSGL/raw/a0abd277147e41a7e0846b84d4a14dfed1a3e8a6/doc/dsglLogo.png "DSGL")

# Damn Simple OpenGL

1. [About](#about)
2. [Installation and compilation](#installation-and-compilation)
3. [API](#api)
	1. [Core](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md)
		1. [DSGL_TRACE](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#dsgl_trace)
		2. [PrintNicelyWorkGroupsCapabilities](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#printnicelyworkgroupscapabilities)
		3. [Exception](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#exception)
		4. [Context](https://framagit.org/denissalem/blob/master/doc/coreEN.md#context)
		5. [VertexBufferObject](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#vertexbufferobject)
		6. [VertexArrayObject](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#vertexarrayobject)
		7. [Elements](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#elements)
		8. [Shader](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#shader)
		9. [ShaderProgram](https://framagit.org/denissalemDSGL/blob/master/doc/coreEN.md#shaderprogram)
		10. [PipelineProgram](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#pipelineprogram)
		11. [ComputeProgram](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md#computeprogram)
	2. [Meshes](https://framagit.org/denissalem/DSGL/blob/master/doc/meshesEN.md)
		1. [Triangle](https://framagit.org/denissalem/DSGL/blob/master/doc/meshesEN.md#triangle)
		2. [Quad](https://framagit.org/denissalem/DSGL/blob/master/doc/meshesEN.md#quad)
4. [Examples](https://framagit.org/denissalem/DSGL/tree/master/examples)
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


Include [dsgl.hpp](https://framagit.org/denissalem/DSGL/blob/master/core/dsgl.hpp) and [dsgl.cpp](https://framagit.org/denissalem/DSGL/blob/master/core/dsgl.cpp) from [Core](https://framagit.org/denissalem/DSGL/tree/master/core) in your project for general purposes features.

Depending of your needs one may want to also includes all sources files from

- [meshes](https://framagit.org/denissalem/DSGL/tree/master/meshes)
- [images](https://framagit.org/denissalem/DSGL/tree/master/images)
- [generative](https://framagit.org/denissalem/DSGL/tree/master/generative)

## API

- [Core](https://framagit.org/denissalem/DSGL/blob/master/doc/coreEN.md) contains main DSGL features, each of them are defined within DSGL namespace.
- [Meshes](https://framagit.org/denissalem/DSGL//blob/master/doc/meshesEN.md) hold some simples meshes for debugging, testing. Also useful for basic needs. Everything there is defined within DSGL::Meshes namespace.

## FAQ

## How do I use my extensions loader

You must include by yourself your alternative library headers __IN__ dsgl.hpp and every dsgl headers you want to include in your project.
