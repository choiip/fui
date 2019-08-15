uniform mat4 u_mvp;
VERT_IN vec4 a_position;
VERT_IN vec4 a_color;
VERT_OUT vec4 v_color;

void main()
{
  v_color = a_color;
  gl_Position = u_mvp * a_position;
}
