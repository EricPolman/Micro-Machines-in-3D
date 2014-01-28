varying vec3 normal, lightDir, halfVector;
varying vec4 diffuse, ambient;
varying float dotWithCam;

uniform vec3 camPos;

void main()
{
	normal = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(vec3(gl_LightSource[0].position));

	halfVector = normalize(gl_LightSource[0].halfVector.xyz);
	
	diffuse = vec4(0.5, 0.5, 0.5, 0.5);
	ambient = vec4(0.2, 0.2, 0.2, 0.5);
	
	gl_Position = ftransform();
	dotWithCam = dot(normalize(camPos.xyz - gl_Vertex.xyz), normal);
}
