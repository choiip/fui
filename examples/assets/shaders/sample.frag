FRAG_IN vec4 v_color;
#ifdef FRAG_OUT
FRAG_OUT vec4 outColor;
#endif

void main()
{
#ifdef FRAG_OUT
  outColor = v_color;
#else
  gl_FragColor = v_color;
#endif
}
