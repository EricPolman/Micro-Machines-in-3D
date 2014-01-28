varying vec3 normal;
varying vec3 vertex_to_light_vector;

uniform float waveTime;

void main()
{
	vec4 vert = gl_Vertex;
	vert.y += sin(waveTime + vert.z / 3.0) * 0.4;
	gl_Position = gl_ModelViewProjectionMatrix * vert;
	normal = gl_NormalMatrix * gl_Normal;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}
