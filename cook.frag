varying vec3 v;
varying vec3 n;
varying vec3 lightDir0;
varying vec3 lightDir1;


//gl_FrontLightProduct = gl_LightSource * gl_FrontMaterial



void main()
{		
	n = normalize(n);
	float fRef = 0.7;
	float m = 0.3;
	vec3 eye = normalize(-v);


	//Fresnel
	float fresnel;
	vec3 halfvec = normalize(lightDir0 + eye);
	fresnel = max(dot(eye, halfvec), 0.0);
	fresnel = 1.0- fresnel;
	fresnel = pow(fresnel, 5.0);
	fresnel *= (1.0-fRef);
	fresnel += fRef;

	
	//Roughness
	float roughness;
	float temp;
	roughness = max(dot(n, halfvec), 0.0);
	roughness = pow(roughness, 2.0);
	temp = roughness;
	roughness -= 1.0;
	temp *= (m * m);
	roughness /= temp;
	roughness = exp(roughness);
	temp = max(dot(n, halfvec), 0.0001);
	temp = pow(temp, 4.0);
	temp *= (3.14 * m * m);
	roughness /= temp;
	

	//Geometrical attenuation
	float geo;
	float gb;
	float gc;

	gb = max(dot(n, halfvec),0.0);
	gb *= max(dot(n, eye),0.0) * 2.0;
	gb /= max(dot(lightDir0, halfvec), 0.0001);

	gc = max(dot(n, halfvec),0.0);
	gc *= max(dot(n, lightDir0), 0.0) * 2.0;
	gc /= max(dot(lightDir0, halfvec),0.0001);

	geo = min(1.0, min(gb,gc));

	float rs;
	rs = max(dot(n, lightDir0), 0.0001);
	rs *= max(dot(n, eye), 0.0001) * 3.14;

	temp = fresnel * roughness * geo;
	rs = temp/rs; 

	float specular0 = 0.0;
	if(dot(n, lightDir0) > 0.0) specular0 = rs;
	

	/*******************************************************/

	halfvec = normalize(lightDir1 + eye);
	fresnel = max(dot(eye, halfvec), 0.0);
	fresnel = 1.0- fresnel;
	fresnel = pow(fresnel, 5.0);
	fresnel *= (1.0-fRef);
	fresnel += fRef;

	
	//Roughness
	roughness = max(dot(n, halfvec), 0.0);
	roughness = pow(roughness, 2.0);
	temp = roughness;
	roughness -= 1.0;
	temp *= (m * m);
	roughness /= temp;
	roughness = exp(roughness);
	temp = max(dot(n, halfvec), 0.0001);
	temp = pow(temp, 4.0);
	temp *= (3.14 * m * m);
	roughness /= temp;
	

	//Geometrical attenuation
	gb = max(dot(n, halfvec),0.0);
	gb *= max(dot(n, eye),0.0) * 2.0;
	gb /= max(dot(lightDir1, halfvec), 0.0001);

	gc = max(dot(n, halfvec),0.0);
	gc *= max(dot(n, lightDir1), 0.0) * 2.0;
	gc /= max(dot(lightDir1, halfvec),0.0001);

	geo = min(1.0, min(gb,gc));

	rs = max(dot(n, lightDir1), 0.0001);
	rs *= max(dot(n, eye), 0.0001) * 3.14;

	temp = fresnel * roughness * geo;
	rs = temp/rs; 

	float specular1 = 0.0;
	if(dot(n, lightDir1) > 0.0) specular1 = rs;

	
	vec4 diffuse0, diffuse1, ambient;

	diffuse0 = gl_FrontLightProduct[0].diffuse * max(dot(n, lightDir0),0.0);
	diffuse1 = gl_FrontLightProduct[1].diffuse * max(dot(n, lightDir1),0.0);	

	ambient = gl_FrontLightProduct[0].ambient;
	ambient += gl_FrontLightProduct[1].ambient;

	vec3 color = ambient + (diffuse0 * (gl_FrontLightProduct[0].diffuse + gl_FrontLightProduct[0].specular*specular0)) + (diffuse1 * (gl_FrontLightProduct[1].diffuse + gl_FrontLightProduct[1].specular*specular1));
	gl_FragColor = vec4(color, 1.0);

}
