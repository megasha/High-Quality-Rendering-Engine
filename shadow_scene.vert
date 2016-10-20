
varying vec4 vWorld;
varying vec4 vLightSpace;
varying vec4 vLightSpace1;
varying vec3 vEye;
varying vec3 nEye;
varying vec3 lightDir0;
varying vec3 lightDir1;
varying vec2 TexCoords;


uniform mat4 LightProjection;
uniform mat4 LightProjection1;
uniform mat4 LightView;
uniform mat4 LightView1;
uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;

void main()
{	
    vWorld = Model * gl_Vertex;
	vEye = vec3(View * Model * gl_Vertex);
	vLightSpace = LightProjection * LightView * vWorld;
	vLightSpace1 = LightProjection1 * LightView1 * vWorld;
	nEye = normalize(vec3(View * Model * vec4(gl_Normal,0.0)));

	TexCoords = gl_Vertex.xz;



	gl_Position = Projection * View * Model * gl_Vertex;
	lightDir0 = vec3(normalize(View*gl_LightSource[0].position));
	lightDir1 = vec3(normalize(View*gl_LightSource[1].position));
	//gl_Position = LightProjection * LightView * Model * gl_Vertex;
}
