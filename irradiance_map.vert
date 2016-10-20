
varying vec3 v;
varying vec3 n;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	v = vec3(gl_Vertex);
	n = normalize(vec3(vec4(gl_Normal,0.0)));

	gl_Position = Projection * View * gl_Vertex;
}