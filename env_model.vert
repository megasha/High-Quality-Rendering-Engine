
varying vec3 v;
varying vec3 n;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;



void main()
{	
	v = vec3(View * Model * gl_Vertex);

	n = normalize(vec3(View * Model * vec4(gl_Normal,0.0)));

	gl_Position =  Projection * View  * Model * gl_Vertex;
}
