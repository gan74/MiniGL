#version 420


layout(location = 0) out vec4 out_color;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;

uniform vec3 light_dir;

void main() {
	float lambert = max(0.0, dot(normalize(v_normal), light_dir));
	out_color = vec4(lambert * 0.8 + 0.2);
}
