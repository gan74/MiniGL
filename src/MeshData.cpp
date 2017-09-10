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

#include "MeshData.h"
#include <y/io/File.h>

MeshData MeshData::from_file(const core::String& file_name) {
	auto reader = std::move(io::File::open(file_name).expected("Unable to open texture file."));

	const char* err_msg = "Unable to load mesh.";

	struct Header {
		u32 magic;
		u32 type;
		u32 version;

		float radius;

		u32 bones;
		u32 vertices;
		u32 triangles;

		bool is_valid() const {
			return magic == 0x65766179 &&
				   type == 1 &&
				   version == 5 &&
				   vertices != 0 &&
				   triangles != 0;
		}
	};

	Header header = reader.read_one<Header>().expected(err_msg);
	if(!header.is_valid()) {
		fatal("Invalid header.");
	}

	MeshData mesh;
	mesh._radius = header.radius;
	mesh._vertices = core::Vector<Vertex>(header.vertices, Vertex{});
	mesh._triangles = core::Vector<IndexedTriangle>(header.triangles, IndexedTriangle{});

	reader.read(mesh._vertices.begin(), header.vertices * sizeof(Vertex)).expected(err_msg);
	reader.read(mesh._triangles.begin(), header.triangles * sizeof(IndexedTriangle)).expected(err_msg);

	return mesh;
}

