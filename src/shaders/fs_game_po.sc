$input v_texcoord0, v_color0, v_normal, v_fogDist

#include <bgfx_shader.sh>

SAMPLER2D(s_tex, 0);

uniform vec4 u_flags;        
uniform vec4 u_fogColor;
uniform vec4 u_fogParams;    
uniform vec4 u_currentColor;
uniform vec4 u_polyOffset;   

uniform vec4 u_lightingFlags;   
uniform vec4 u_globalAmbient;
uniform vec4 u_light0Pos;       
uniform vec4 u_light0Diffuse;
uniform vec4 u_light0Ambient;
uniform vec4 u_light1Pos;
uniform vec4 u_light1Diffuse;
uniform vec4 u_light1Ambient;

void main() {
    vec4 col = mix(u_currentColor, v_color0, u_flags.y);

    if (u_flags.x > 0.5) {
        col *= texture2D(s_tex, v_texcoord0);
    }

    if (u_flags.z > 0.5) {
        float a = col.a, ref = u_flags.w, fn = u_flags.z;
        bool alpha_pass = fn < 1.5 ? false          
                  : fn < 2.5 ? a < ref        
                  : fn < 3.5 ? abs(a-ref) < (1.0/512.0) 
                  : fn < 4.5 ? a <= ref       
                  : fn < 5.5 ? a > ref        
                  : fn < 6.5 ? abs(a-ref) >= (1.0/512.0) 
                  :             a >= ref;      
        if (!alpha_pass) discard;
    }

    
    if (u_lightingFlags.x > 0.5) {
        vec3 n = normalize(v_normal);
        vec3 light = u_globalAmbient.rgb;
        if (u_lightingFlags.y > 0.5) {
            light += u_light0Ambient.rgb;
            float diff = max(dot(n, normalize(u_light0Pos.xyz)), 0.0);
            light += u_light0Diffuse.rgb * diff;
        }
        if (u_lightingFlags.z > 0.5) {
            light += u_light1Ambient.rgb;
            float diff = max(dot(n, normalize(u_light1Pos.xyz)), 0.0);
            light += u_light1Diffuse.rgb * diff;
        }
        col.rgb *= clamp(light, 0.0, 1.0);
    }

    float fogMode = u_fogParams.x;
    if (fogMode > 0.5) {
        float dist = v_fogDist;
        float fogFactor;
        if (fogMode < 1.5) {
            fogFactor = (u_fogParams.z - dist) / (u_fogParams.z - u_fogParams.y);
        } else if (fogMode < 2.5) {
            fogFactor = exp(-u_fogParams.w * dist);
        } else {
            float d = u_fogParams.w * dist;
            fogFactor = exp(-d * d);
        }
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        col.rgb = mix(u_fogColor.rgb, col.rgb, fogFactor);
    }

    gl_FragColor = col;

    float slope = max(abs(dFdx(gl_FragCoord.z)), abs(dFdy(gl_FragCoord.z)));
    gl_FragDepth = gl_FragCoord.z + u_polyOffset.x * slope + u_polyOffset.y * (1.0/16777216.0);
}
