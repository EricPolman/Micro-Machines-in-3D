varying vec3 normal;
varying vec3 vertex_to_light_vector;

void main()
{
	const vec4 AmbientColor = vec4(0.6, 0.6, 0.6, 1.0);
    const vec4 DiffuseColor = vec4(0.5, 0.5, 0.5, 1.0);
 
    vec3 normalized_normal = normalize(normal);
    vec3 normalized_vertex_to_light_vector = normalize(vec3(gl_LightSource[0].position));

    float DiffuseTerm = clamp(dot(normal, normalized_vertex_to_light_vector), 0.0, 1.0);
 
    gl_FragColor = AmbientColor + DiffuseColor * DiffuseTerm;
}
