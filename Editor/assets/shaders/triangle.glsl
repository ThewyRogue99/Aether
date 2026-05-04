#version 330 core

// ── Shared — prepended to both stages ───────────────────────────────────────
layout(std140) uniform Camera { mat4 u_ViewProjection; };
layout(std140) uniform Object { mat4 u_Model; };

// ── Vertex stage ─────────────────────────────────────────────────────────────
#pragma vertex

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main() {
    v_TexCoord  = a_TexCoord;
    gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}

// ── Fragment stage ───────────────────────────────────────────────────────────
#pragma fragment

layout(std140) uniform Material { vec4 u_BaseColor; };

in  vec2 v_TexCoord;
out vec4 FragColor;

uniform sampler2D u_Albedo;

void main() {
    vec4 tex  = texture(u_Albedo, v_TexCoord);
    FragColor = tex * u_BaseColor;
}
