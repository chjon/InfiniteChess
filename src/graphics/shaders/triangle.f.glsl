in vec4 gl_FragCoord;
uniform vec2 m_pos;
uniform float m_ang;

void main(void) {
    int size = 50;
    vec2 normPos;
    normPos.x = gl_FragCoord.x / size + fract(m_pos.x) * size;
    normPos.y = gl_FragCoord.y / size + fract(m_pos.y) * size;
    vec2 centre;
    centre.x = 0.5;
    centre.y = 0.5;
    float cos_ang = cos(m_ang);
    float sin_ang = sin(m_ang);

    if (mod(floor(normPos.x * cos_ang - normPos.y * sin_ang) + floor(normPos.x * sin_ang + normPos.y * cos_ang), 2) == 0) {
        gl_FragColor = vec4(240./255, 217./255, 183./255, 1);
    } else {
        gl_FragColor = vec4(180./255, 136./255, 102./255, 1);
    }
}