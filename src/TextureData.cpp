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

#include "TextureData.h"

#include <y/io/File.h>

TextureData::TextureData(const math::Vec2ui& size) : _size(size) {
}

math::Vec2ui TextureData::mip_size(usize lvl) const {
	usize factor = 1 << lvl;
	return {std::max(usize(1), _size.x() / factor), std::max(usize(1), _size.y() / factor)};
}

usize TextureData::combined_byte_size() const {
	usize data_size = 0;
	for(usize i = 0; i != _mips; ++i) {
		auto size = mip_size(i);
		data_size += size.x() * size.y() * 4;
	}
	return data_size;
}

TextureData TextureData::from_file(const core::String& file_name) {
	auto reader = std::move(io::File::open(file_name).expected("Unable to open texture file."));

	const char* err_msg = "Unable to read image.";

	struct Header {
		u32 magic;
		u32 type;
		u32 version;

		u32 width;
		u32 height;
		u32 layers;
		u32 mips;
		u32 format;

		bool is_valid() const {
			return magic == 0x65766179 &&
				   type == 2 &&
				   version == 3 &&
				   format > 37 && // RGBA8
				   width != 0 &&
				   height != 0 &&
				   layers == 1 &&
				   mips != 0;
		}
	};

	Header header = reader.read_one<Header>().expected(err_msg);
	if(!header.is_valid()) {
		fatal(err_msg);
	}

	TextureData data;
	data._size = {header.width, header.height};
	data._mips = header.mips;

	usize data_size = data.combined_byte_size();

	data._data = new u8[data_size];

	reader.read(data._data, data_size).expected(err_msg);

	return data;
}
