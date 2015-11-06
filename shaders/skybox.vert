#version 130

in vec3 position;
uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
out vec3 texCoord;

void main() {
	vec4 wvp_pos = viewProjectionMatrix * modelMatrix * vec4(position, 1.0);
	texCoord = position;
	gl_Position = wvp_pos.xyww;
}