# DSGL

Damn Simple OpenGL

1. [About](https://github.com/DenisSalem/DSGL#about)
2. [Installation and compilation](https://github.com/DenisSalem/DSGL#installation-and-compilation)
3. [API](https://github.com/DenisSalem/DSGL#api)
	1. [core](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md)
		1. [DSGL_TRACE](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#dsgl_trace)
		2. [Exception](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#exception)
		3. [Context](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#context)
		4. [VertexBufferObject](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#vertexbufferobject)
		5. [VertexArrayObject](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#vertexarrayobject)
		6. [Elements](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#elements)
		7. [Shader](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#shader)
		8. [ShaderProgram](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md#shaderprogram)

4. [Examples](https://github.com/DenisSalem/DSGL/tree/master/examples)
5. [FAQ](https://github.com/DenisSalem/DSGL/blob/master/README.md#faq)
	1. [How do I use my extensions loader](https://github.com/DenisSalem/DSGL/blob/master/README.md#how-do-i-use-my-extensions-loader)

## About

DSGL is a tiny and simple modern OpenGL wrapper libraries written in C++ and intented to do not hide OpenGL but simply
make the programmer's life easier by providing some general purpose classes.

## Installation and compilation

DSGL support some nice libraries you may want to use by passing to compiler some option depending of what you want.

- [glfw](http://www.glfw.org/) : compile with *-D DSGL_GLFW*.
- [gl3w](https://github.com/skaslev/gl3w) : compile with *-D DSGL_GL3W*.


Include [dsgl.hpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.hpp) and [dsgl.cpp](https://github.com/DenisSalem/DSGL/blob/master/core/dsgl.cpp) from [core](https://github.com/DenisSalem/DSGL/tree/master/core) in your project.

## API

Each classes and functions are defined within the DSGL namespace.

- [core](https://github.com/DenisSalem/DSGL/blob/master/doc/core.md) contains main DSGL features.

## FAQ

## How do I use my extensions loader

You must include by yourself your alternative library headers __IN__ dsgl.hpp and every dsgl headers you want to include in your project.
