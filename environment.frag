
varying vec3 v;
varying vec3 n;
varying vec3 TexCoords;

uniform samplerCube skybox;

void main()
{		
    gl_FragColor = texture(skybox, TexCoords);
}
