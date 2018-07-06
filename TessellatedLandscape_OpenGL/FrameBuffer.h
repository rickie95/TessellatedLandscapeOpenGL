#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>

struct FrameBufferParams {
	int width, height, nAttachments, nSamples, nCSamples;
	bool hasDepth;
	GLenum format, depthFormat, type;
};

class FrameBuffer
{


public:
	FrameBuffer(FrameBufferParams params);
	~FrameBuffer();
	void release();
	void bind();

protected:
	void allocFrameBuffer(FrameBufferParams params);

	GLuint depth_, *color_, id_;
	FrameBufferParams params_;
};

