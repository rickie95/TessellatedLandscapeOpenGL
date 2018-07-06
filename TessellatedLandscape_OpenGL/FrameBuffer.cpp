#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(FrameBufferParams params)
{
	params_ = params;
}

FrameBuffer::~FrameBuffer()
{
	this->bind();
	if (params_.nSamples > 1) {
		//glDeleteRenderbuffersEXT(params_.nAttachments, &color_[0]);
		glDeleteRenderbuffers(params_.nAttachments, &color_[0]);
		glDeleteRenderbuffers(1, &depth_);
	}
	else {
		glDeleteTextures(params_.nAttachments, &color_[0]);
		glDeleteTextures(1, &depth_);
	}

	this->release();
	glDeleteFramebuffers(1, &id_);

	if (color_) delete[] color_;
}

void FrameBuffer::release()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, id_);
}

void FrameBuffer::allocFrameBuffer(FrameBufferParams params)
{
	glGenFramebuffers(1, &id_);
	
	bool isCSAA = false; 
	//glewIsSupported("GL_NV_framebuffer_multisample_coverage") && (params.nCSamples > params.nSamples) && params.nSamples > 1;
	GLint coverageSampleConfigs;
	GLint *coverageConfigs = 0;
		
	if (!params.nAttachments) return;
	this->bind();

	if (params.type == GL_TEXTURE_2D)
		color_ = new GLuint[params.nAttachments];
	else if (params.type == GL_TEXTURE_3D)
		color_ = new GLuint[1];

	if (params.nSamples > 0) { //create multisample targets
		GLint maxSamples = 0;
		glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		if (params.nSamples > maxSamples) {
			std::cerr << "Warning: maximum number of samples supported is " << maxSamples << " but requested number of samples is " <<
				params.nSamples << ".  Falling back to " << maxSamples << " samples." << std::endl;
			params.nSamples = maxSamples;
		}

		if (params.type == GL_TEXTURE_2D) {

			glGenRenderbuffers(params.nAttachments, &color_[0]);
			for (int i = 0; i<params.nAttachments; i++) {
				glBindRenderbuffer(GL_RENDERBUFFER, color_[i]);

				glRenderbufferStorageMultisample(GL_RENDERBUFFER, params.nSamples, params.format, params.width, params.height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, color_[i]);
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		
		}else if (params.type == GL_TEXTURE_3D) {
			std::cerr << "3D textures with multisample framebuffers currently not supported.";
			assert(0);
		}

	}
	else { //create regular targets

		if (params.type == GL_TEXTURE_2D) {
			glGenTextures(params.nAttachments, &color_[0]);

			for (int i = 0; i<params.nAttachments; i++) {
				glBindTexture(params.type, color_[i]);
				glTexParameterf(params.type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameterf(params.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glTexImage2D(GL_TEXTURE_2D, 0, params.format, params.width, params.height, 0, GL_LUMINANCE, GL_FLOAT, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, color_[i], 0);
			}

			glBindTexture(params.type, 0);
		}
		else if (params.type == GL_TEXTURE_3D) {
			glGenTextures(1, &color_[0]);
			glBindTexture(params.type, color_[0]);

			glTexParameterf(params.type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(params.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage3D(params.type, 0, params.format, params.width, params.height,
				params.nAttachments, 0, GL_LUMINANCE, GL_FLOAT, 0);
			for (int i = 0; i<params.nAttachments; i++) {
				glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, color_[0], 0, i);
				//glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_3D, color_[0], 0, i);
			}
			glBindTexture(params.type, 0);

		}
	}

	if (params.hasDepth) {
		if (params.nSamples > 0) {
			glGenRenderbuffers(1, &depth_);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, params.nSamples, params.depthFormat, params.width, params.height);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			
		}
		else {
			glGenTextures(1, &depth_);
			glBindTexture(params.type, depth_);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(params.type, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(params.type, 0, params.depthFormat, params.width, params.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_, 0);
			glBindTexture(params.type, 0);
		}

	}

	//if(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) == GL_FRAMEBUFFER_COMPLETE_EXT) { cerr << "herp derp" << endl; }
	//else cerr << "oops" << endl;
	delete[] coverageConfigs;
	this->release();
}


