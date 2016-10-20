
varying vec3 v;
varying vec3 TexCoords;

uniform mat4 View;
uniform mat4 Projection;

void main()
{	
	gl_Position = Projection*View * gl_Vertex;

	TexCoords = gl_Vertex.xyz;

}
