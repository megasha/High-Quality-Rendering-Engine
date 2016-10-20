varying vec3 v;
varying vec3 n;
varying vec3 lightDir0;
varying vec3 lightDir1;

//gl_FrontLightProduct = gl_LightSource * gl_FrontMaterial



void main()
{		
    vec4 diffuse, ambient, specular;

	//Compute diffuse
	diffuse = gl_FrontLightProduct[0].diffuse * max(dot(n, normalize(lightDir0)),0.0);	
	diffuse += gl_FrontLightProduct[1].diffuse * max(dot(n, normalize(lightDir1)),0.0);	


	//Compute specular

	vec3 eye = normalize(-v);

	vec3 ref = normalize(-reflect(lightDir0,n));
	specular = gl_FrontLightProduct[0].specular * pow(max(dot(ref, eye),0.0), gl_FrontMaterial.shininess);
	
	ref = normalize(-reflect(lightDir1,n));
	specular += gl_FrontLightProduct[1].specular * pow(max(dot(ref, eye),0.0), gl_FrontMaterial.shininess);
	

	//Compute ambient
	ambient = gl_FrontLightProduct[0].ambient;
	ambient += gl_FrontLightProduct[1].ambient;

	gl_FragColor = gl_FrontLightModelProduct.sceneColor + diffuse + specular + ambient;
}
