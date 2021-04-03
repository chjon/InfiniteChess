in vec4 gl_FragCoord;
uniform vec2 m_pos;

void main(void) {
    int size = 50;
    if (mod(floor(gl_FragCoord.x / size + fract(m_pos.x) * size) + floor(gl_FragCoord.y / size + fract(m_pos.y) * size), 2) == 0) {
        gl_FragColor = vec4(240./255, 217./255, 183./255, 1);
    } else {
        gl_FragColor = vec4(180./255, 136./255, 102./255, 1);
    }
}