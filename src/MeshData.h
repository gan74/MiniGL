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

#ifndef MESHDATA_H
#define MESHDATA_H

#include "utils.h"
#include "Vertex.h"

class MeshData {
	public:
		MeshData() = default;

		static MeshData from_file(const core::String& file_name);

		u32 vertex_count() const {
			return _vertices.size();
		}

		u32 triangle_count() const {
			return _triangles.size();
		}

		const Vertex* vertex_data() const {
			return _vertices.begin();
		}

		const IndexedTriangle* triangle_data() const {
			return _triangles.begin();
		}

		float radius() const {
			return _radius;
		}

	private:
		float _radius = 0.0f;

		core::Vector<Vertex> _vertices;
		core::Vector<IndexedTriangle> _triangles;
};

#endif // MESHDATA_H
