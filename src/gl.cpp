/*******************************
Copyright (c) 2016-2017 Gr�goire Angerand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**********************************/


#include "utils.h"

#include "gl.h"

#include "Shader.h"
#include "Mesh.h"

GLAPIENTRY void debug_out(GLenum, GLenum type, GLuint, GLuint sev, GLsizei, const char* msg, const void*) {
	if(sev == GL_DEBUG_SEVERITY_NOTIFICATION) {
		return;
	}
	auto log_type = Log::Info;
	if(type == GL_DEBUG_TYPE_PERFORMANCE) {
		log_type = Log::Perf;
	}
	if(type == GL_DEBUG_TYPE_ERROR) {
		log_type = Log::Error;
	}
	log_msg((sev == GL_DEBUG_SEVERITY_HIGH ? "[GL][HIGH] " : "[GL] ") + core::str(msg), log_type);
}



void initialize_gl() {
	static bool init = false;

	if(!init) {
		init = true;
		glewExperimental = true;
		if(glewInit() != GLEW_OK) {
			fatal("Unable to initialize glew.");
		}
		glDebugMessageCallback(&debug_out, 0);
		glEnable(GL_DEBUG_OUTPUT);

		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glGetError();
	}
}



void fullscreen(const core::Rc<Shader>& shader) {
	static core::Rc<Mesh> screen;
	if(!screen) {
		screen = Mesh::from_file("../data/meshes/plane.ym");
	}

	glDisable(GL_DEPTH_TEST);
	shader->bind();
	screen->draw();
	glEnable(GL_DEPTH_TEST);
}
