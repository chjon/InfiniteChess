in vec4 gl_FragCoord;
uniform mat3 m_wts;

void main(void) {
    vec3 pos = m_wts * gl_FragCoord.xyw;
    if (mod(pos.x, 2) >= 1 && mod(pos.y, 2) >= 1 || mod(pos.x, 2) < 1 && mod(pos.y, 2) < 1) {
        gl_FragColor = vec4(240./255, 217./255, 183./255, 1);
    } else {
        gl_FragColor = vec4(180./255, 136./255, 102./255, 1);
    }
}