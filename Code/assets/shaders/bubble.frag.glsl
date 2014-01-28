varying vec3 normal, lightDir, halfVector;
varying vec4 diffuse, ambient;
varying float dotWithCam;

void main()
{
	vec3 n, halfV;
	float NdotL, NdotHV;

	vec4 color = ambient;
	n = normal;

	NdotL = dot(n, lightDir);

	if(NdotL > 0.0)
	{
		color += diffuse * NdotL;
		halfV = halfVector;
		NdotHV = max(dot(n, halfV), 0.0);

		color += vec4(1.0,1.0,1.0,1) * NdotHV * 2.0;
	}
	
	color.r = 1.0 - dotWithCam;
	color.g = max(0.75 - dotWithCam, 0.0);
	color.b *= 1.1;

    gl_FragColor = color;
}
