$input a_position, a_texcoord0, a_color0, a_normal
$output v_texcoord0, v_color0, v_normal, v_fogDist

#include <bgfx_shader.sh>

uniform mat4 u_mvp;
uniform mat4 u_mv;      
uniform vec4 u_curNormal; 

void main() {
    vec4 clipPos = mul(u_mvp, vec4(a_position, 1.0));
    gl_Position  = clipPos;
    v_texcoord0  = a_texcoord0;
    v_color0     = a_color0;
    
    
    vec3 n = (u_curNormal.w > 0.5) ? a_normal.xyz : u_curNormal.xyz;
    v_normal  = normalize(mul(u_mv, vec4(n, 0.0)).xyz);
    v_fogDist = abs(clipPos.w);
}
