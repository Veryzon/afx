
layout(std140) uniform sgl_view
{
    mat4 v;
    mat4 iv;
    mat4 p;
    mat4 ip;
    mat4 vp;
    mat4 ivp;
    vec4 viewpoint; // view point
    vec2 viewExtent;

    vec3 starDir;
    vec4 starPos;
    vec4 starKd;
};