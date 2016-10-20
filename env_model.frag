
varying vec3 v;
varying vec3 n;

uniform samplerCube skybox;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

void main()
{		
	vec3 eye = normalize(v);
	vec3 ref = normalize(reflect(eye,n));
	ref = vec3(inverse(View) * vec4(ref,0.0));

	


	gl_FragColor = texture(skybox, ref);
}
