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

#include "Texture.h"

#include "gl.h"

#include <unordered_map>


core::Rc<Texture> Texture::from_file(const core::String& file_name) {
	static std::unordered_map<core::String, core::Rc<Texture>> loaded;
	auto it = loaded.find(file_name);
	if(it == loaded.end()) {
		return loaded[file_name] = core::Rc(new Texture(TextureData::from_file(file_name)));
	}
	return it->second;
}

Texture::Texture() : _type(GL_TEXTURE_2D) {
}

Texture::Texture(const TextureData& data) : _size(data.size()) {
	glGenTextures(1, &_handle);
	glBindTexture(_type = GL_TEXTURE_2D, _handle);

	glTexStorage2D(GL_TEXTURE_2D, std::max(usize(1), data.mipmaps()), GL_RGBA8, data.size().x(), data.size().y());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data.mipmaps() > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	usize data_offset = 0;
	for(usize i = 0; i != data.mipmaps(); ++i) {
		const u8* mip_data = data.data() + data_offset;
		math::Vec2ui mip_size = data.mip_size(i);
		data_offset += mip_size.x() * mip_size.y() * 4;

		glTexSubImage2D(GL_TEXTURE_2D, i,
						0, 0,
						mip_size.x(), mip_size.y(),
						GL_RGBA, GL_UNSIGNED_BYTE,
						mip_data);
	}
}

Texture::~Texture() {
	if(_handle) {
		glDeleteTextures(1, &_handle);
	}
}


