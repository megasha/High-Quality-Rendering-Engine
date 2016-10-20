
uniform mat4 proj;
uniform mat4 model;

void main()
{	
	gl_Position = proj * model * gl_Vertex ;
}
