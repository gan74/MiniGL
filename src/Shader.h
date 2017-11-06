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

#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>

#include "Texture.h"

class Shader : NonCopyable {
	public:
		Shader(const core::String& frag, const core::String& vert);
		Shader(const core::String& comp);
		Shader(Shader&& other);

		static core::Rc<Shader> from_files(const core::String& prefix, std::initializer_list<core::String> defines = {});
		static core::Rc<Shader> from_files(const core::String& frag, const core::String& vert, std::initializer_list<core::String> defines = {});

		~Shader();

		void bind() const;
		void dispatch(const math::Vec3ui& size) const;

		void set_uniform(const char* name, const math::Matrix4<>& value);
		void set_uniform(const char* name, const math::Vec3& value);
		void set_uniform(const char* name, float value);


		void set_texture(const core::String& name, const Texture& tex);
		void set_image(const core::String& name, const Texture& image);

	private:
		i32 location(const char* name) const;

		u32 _handle = 0;
		std::unordered_map<core::String, i32> _textures;
};

#endif // SHADER_H
