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

#include <iostream>
#include <y/math/Vec.h>
#include <y/core/Chrono.h>


#include <SDL2/SDL.h>

#include <src/Mesh.h>
#include <src/Shader.h>
#include <src/Framebuffer.h>
#include <src/gl.h>

using namespace y;
using namespace y::math;

Vec2ui win_size = {1024, 768};

SDL_Window* create_window() {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
		fatal("Unable to initilize SDL");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window* window = SDL_CreateWindow("MiniGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			win_size.x(), win_size.y(),
			SDL_WINDOW_OPENGL
		);

	SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(1);

	return window;
}

void close_window(SDL_Window* window) {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool run(SDL_Window* window) {
	SDL_GL_SwapWindow(window);

	SDL_Event e;

	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
			return false;
		}
	}

	return true;
}


int main(int, char**) {
	auto window = create_window();

	initialize_gl();

	Shader shader(read_text_file("../shaders/basic.frag"), read_text_file("../shaders/basic.vert"));
	auto mesh = Mesh::from_file("../models/cube.ym");
	Framebuffer framebuffer(win_size);

	shader.bind();

	core::Chrono timer;
	while(run(window)) {

		framebuffer.bind();
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		float angle = timer.elapsed().to_secs();

		auto view = look_at(500.0f * Vec3{std::cos(angle), std::sin(angle), -1.0f}, Vec3{}, Vec3{0, 0, 1});
		auto proj = perspective(to_rad(90.0f), float(win_size.x()) / float(win_size.y()), 1.0f, 10000.0f);

		shader.set_uniform("in_view_proj", proj * view);
		shader.set_uniform("in_model", Matrix4<>::identity());
		shader.set_uniform("light_dir", Vec3{0.75f, 0.5f, -1.0f}.normalized());

		mesh->draw();

		framebuffer.unbind();
		framebuffer.blit();

		if(glGetError()) {
			fatal("GL error!");
		}
	}

	close_window(window);

	return 0;
}
