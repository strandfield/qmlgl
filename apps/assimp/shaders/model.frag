#version 330 core

#if defined(MESH_HAS_COLORS)
in vec3 v_color;
#endif

#if defined(MESH_HAS_UV)
in vec2 v_uv;
#endif

#if defined(MESH_HAS_NORMALS)
in vec3 v_normal;
#endif

#if defined(MATERIAL_FLAT_COLOR)
uniform vec3 flat_color;
#endif

#if defined(MATERIAL_TEXTURE)
uniform sampler2D texture_diffuse;
#endif

out vec4 FragColor;

void main()
{
    float opacity = 1;
#if defined(MATERIAL_FLAT_COLOR)
    vec3 result_color = flat_color;
#elif defined(MATERIAL_TEXTURE)
    vec4 texColor = texture(texture_diffuse, v_uv.xy);
    vec3 result_color = texColor.rgb;
    opacity = texColor.a;
#elif defined(MESH_HAS_COLORS)
    vec3 result_color = v_color / 255;
#endif

    FragColor = vec4(result_color, opacity);
}
