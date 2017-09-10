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

#include "Mesh.h"
#include "gl.h"

#include <unordered_map>

 core::Rc<Mesh> Mesh::from_file(const core::String& file_name) {
	static std::unordered_map<core::String, core::Rc<Mesh>> meshes;

	auto it = meshes.find(file_name);
	if(it == meshes.end()) {
		auto mesh = new Mesh(MeshData::from_file(file_name));
		it = meshes.insert(std::pair(file_name, mesh)).first;
	}
	return it->second;
}

Mesh::Mesh(const MeshData& data) : _triangle_count(data.triangle_count()), _radius(data.radius()) {
	glGenBuffers(2, reinterpret_cast<u32*>(&_buffers));

	glGenVertexArrays(1, &_handle);
	glBindVertexArray(_handle);

	glBindBuffer(GL_ARRAY_BUFFER, _buffers.vertex);
	glBufferData(GL_ARRAY_BUFFER, data.vertex_count() * sizeof(Vertex), data.vertex_data(),  GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffers.index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.triangle_count() * sizeof(IndexedTriangle), data.triangle_data(),  GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

Mesh::Mesh(Mesh&& other) {
	std::swap(_triangle_count, other._triangle_count);
	std::swap(_handle, other._handle);
	std::swap(_radius, other._radius);
	std::swap(_buffers, other._buffers);
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &_handle);
	glDeleteBuffers(2, reinterpret_cast<u32*>(&_buffers));
}

void Mesh::draw() const {
	glBindVertexArray(_handle);
	glDrawElements(GL_TRIANGLES, _triangle_count * 3, GL_UNSIGNED_INT, nullptr);
}
