varying vec4 vWorld;
varying vec4 vLightSpace;
varying vec4 vLightSpace1;
varying vec3 vEye;
varying vec3 nEye;
varying vec3 lightDir0;
varying vec3 lightDir1;
varying vec2 TexCoords;


uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform vec4 matShininess;
uniform mat4 Model;
uniform mat4 View;
uniform bool bumpOn;



uniform sampler2D depthTexture;
uniform sampler2D depthTexture1;
uniform sampler2D modelTexture;
uniform sampler2D normalTexture;


void main()
{		

	vec3 convCoord = vLightSpace.xyz * 0.5 + 0.5;
	float mapDepth = texture(depthTexture, convCoord.xy).r;
	float currDepth = convCoord.z;
	float f = currDepth >= mapDepth ? 0.0 : 1.0;
	if(convCoord.z > 1.0) f = 1.0;

	vec3 convCoord1 = vLightSpace1.xyz * 0.5 + 0.5;
	float mapDepth1 = texture(depthTexture1, convCoord1.xy).r;
	float currDepth1 = convCoord1.z;
	float f1 = currDepth1 >= mapDepth1 ? 0.0 : 1.0;
	if(convCoord1.z > 1.0) f1 = 1.0;

	//f = f * f1;

	vec3 texNorm = normalize(texture(normalTexture, (TexCoords*0.05)).rgb*2.0-1.0);
	vec3 texEye = View* Model * vec4(texNorm,0.0);
	float diffuseConst = max(dot(texEye, normalize(lightDir0)),0.0);
	if(bumpOn)
		gl_FragColor = 0.4*texture(modelTexture, (TexCoords*0.05)) +f*0.6*texture(modelTexture, (TexCoords*0.05)) - diffuseConst*texture(modelTexture, (TexCoords*0.05));
	else
		gl_FragColor = 0.4*texture(modelTexture, (TexCoords*0.05)) + f*0.6*texture(modelTexture, (TexCoords*0.05));

}
