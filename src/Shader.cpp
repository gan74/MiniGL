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

#include "Shader.h"
#include "gl.h"

#include <numeric>
#include <unordered_map>

#include <y/core/Chrono.h>

static u32 createShader(const core::String& src, GLenum type) {
	u32 handle = glCreateShader(type);

	int len = src.size();
	const char* c_str = src.data();
	glShaderSource(handle, 1, &c_str, &len);

	{
		core::DebugTimer("Shader compilation", core::Duration::milliseconds(100));
		glCompileShader(handle);
		i32 res = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &res);
		if(!res) {
			i32 len = 0;
			char log[1024];
			glGetShaderInfoLog(handle, sizeof(log), &len, log);
			fatal(log);
		}
	}

	return handle;
}


static void linkProgram(u32 handle) {
	core::DebugTimer("Program linking", core::Duration::milliseconds(100));
	glLinkProgram(handle);

	i32 res = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &res);
	if(!res) {
		i32 len = 0;
		char log[1024];
		glGetProgramInfoLog(handle, sizeof(log), &len, log);
		fatal(log);
	}
}

static std::unordered_map<core::String, i32> fetch_textures(u32 handle) {
	i32 uniforms = 0;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &uniforms);

	std::unordered_map<core::String, i32> textures;
	i32 slots = 0;
	for(i32 i = 0; i != uniforms; ++i) {
		char name[1024];
		i32 len = 0;
		i32 discard = 0;
		GLenum type = GL_NONE;
		glGetActiveUniform(handle, i, sizeof(name), &len, &discard, &type, name);

		if(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE) {
			textures[name] = slots;
			glProgramUniform1i(handle, glGetUniformLocation(handle, name), slots++);
		}
	}
	return textures;
}

core::Rc<Shader> Shader::from_files(const core::String& prefix, std::initializer_list<core::String> defines) {
	return from_files(prefix, prefix, defines);
}

core::Rc<Shader> Shader::from_files(const core::String& frag, const core::String& vert, std::initializer_list<core::String> defines) {
	static std::unordered_map<core::String, core::Rc<Shader>> loaded;
	core::String defs = std::accumulate(defines.begin(), defines.end(), core::String("#version 430\n"), [](auto&& a, auto&& b) { return a + "#define " + b + "\n"; });
	core::String key = defs + vert + frag;
	auto it = loaded.find(key);
	if(it == loaded.end()) {
		return loaded[key] = core::Rc(new Shader(defs + read_text_file(frag + ".frag"), defs + read_text_file(vert + ".vert")));
	}
	return it->second;
}


Shader::Shader(const core::String& frag, const core::String& vert) : _handle(glCreateProgram()) {
	auto vert_h = createShader(vert, GL_VERTEX_SHADER);
	auto frag_h = createShader(frag, GL_FRAGMENT_SHADER);
	glAttachShader(_handle, vert_h);
	glAttachShader(_handle, frag_h);
	linkProgram(_handle);

	glDeleteShader(vert_h);
	glDeleteShader(frag_h);

	_textures = fetch_textures(_handle);
}


Shader::Shader(const core::String& comp) : _handle(glCreateProgram()) {
	auto comp_h = createShader(comp, GL_COMPUTE_SHADER);
	glAttachShader(_handle, comp_h);

	linkProgram(_handle);

	glDeleteShader(comp_h);

	_textures = fetch_textures(_handle);
}

Shader::Shader(Shader&& other) {
	std::swap(_handle, other._handle);
	std::swap(_textures, other._textures);
}

Shader::~Shader() {
	glDeleteProgram(_handle);
}

void Shader::bind() const {
	glUseProgram(_handle);
}

i32 Shader::location(const char* name) const {
	return glGetUniformLocation(_handle, name);
}

void Shader::set_uniform(const char* name, const math::Matrix4<>& value) {
	glProgramUniformMatrix4fv(_handle, location(name), 1, false, value.begin());
}

void Shader::set_uniform(const char* name, const math::Vec3& value) {
	glProgramUniform3fv(_handle, location(name), 1, value.begin());
}

void Shader::set_uniform(const char* name, float value) {
	glProgramUniform1f(_handle, location(name), value);
}

void Shader::set_texture(const core::String& name, const Texture& tex) {
	auto it = _textures.find(name);
	if(it != _textures.end()) {
		glActiveTexture(GL_TEXTURE0 + it->second);
		glBindTexture(tex.type(), tex.handle());
	}
}
