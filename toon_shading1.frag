
varying vec3 v;
varying vec3 n;
varying vec3 vWorld;
varying vec3 nWorld;
varying vec3 lightDir0;

uniform mat4 View;

//gl_FrontLightProduct = gl_LightSource * gl_FrontMaterial



void main()
{		
    float diffuseComp0, specularComp0, diffuseComp1, specularComp1;

	//Compute diffuse
	diffuseComp0 = max(dot(n, normalize(lightDir0)),0.0);	

	//Compute specular
	vec3 eye = normalize(-v);
	vec3 ref = normalize(-reflect(lightDir0,n));
	specularComp0 = pow(max(dot(ref, eye),0.0), gl_FrontMaterial.shininess);

	const int levels = 3;
	const float scale = 1.0/ levels;

	diffuseComp0 = floor(diffuseComp0 * levels) * scale;

	specularComp0 = floor(specularComp0 * levels) * scale;


	vec4 diffuse, specular, ambient;
	diffuse = gl_FrontLightProduct[0].diffuse * diffuseComp0;

	specular = gl_FrontLightProduct[0].specular * specularComp0;

	ambient = gl_FrontLightProduct[0].ambient;

	gl_FragColor =  ambient +diffuse + specular;
}
