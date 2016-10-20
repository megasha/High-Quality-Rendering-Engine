
varying vec3 v;
varying vec3 n;

uniform samplerCube skybox;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 colorTest;
uniform int height;
uniform int width;
uniform int currFace;

void main() {

		vec3 eye = normalize(-v);
		vec3 ref = normalize(reflect(eye,n));

		gl_FragColor = texture(skybox, ref);


}