#pragma once

#include "GL/glew.h"

namespace AW
{
	class VertexBufferProxy
	{
	public:
		VertexBufferProxy(const GLuint id, const unsigned int size) : id(id), size(size) {}
		~VertexBufferProxy() { if (id != 0) glDeleteBuffers(1, &id); }

		const GLuint id;
		const unsigned int size;
	};
}
