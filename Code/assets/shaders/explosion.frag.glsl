uniform sampler2D texture1;

void main()
{
	vec4 texCol = texture2D(texture1, gl_TexCoord[0].st);
    gl_FragColor = texCol;
}
