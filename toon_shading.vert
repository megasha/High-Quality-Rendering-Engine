
varying vec3 v;
varying vec3 n;
varying vec3 vWorld;
varying vec3 nWorld;
varying vec3 lightDir0; 
varying vec3 lightDir1;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

uniform vec3 lightPos0;
uniform vec3 lightPos1;

void main()
{	
	v = vec3(View * Model * gl_Vertex);
	vWorld = vec3(Model * gl_Vertex);

	n = normalize(vec3(View * Model * vec4(gl_Normal,0.0)));
	nWorld = normalize(vec3(Model * vec4(gl_Normal,0.0)));

	lightDir0 = vec3(normalize(View*vec4(lightPos0,1.0)));
	lightDir1 = vec3(normalize(View*vec4(lightPos1,1.0)));

	gl_Position =  Projection * View * Model * gl_Vertex;
}
