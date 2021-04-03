#ifdef GL_ES
#   ifdef GL_FRAGMENT_PRECISION_HIGH
        precision highp float;
#   else
        precision mediump float;
#   endif
#else
#   define lowp
#   define mediump
#   define highp
#endif