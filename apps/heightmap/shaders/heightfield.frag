#version 330 core

in float v_hole;
in float v_outside;

out vec4 FragColor;

uniform vec4 mesh_color;
uniform vec4 mesh_outside_color;

void main()
{
    if (v_hole != 0)
        discard;

    FragColor = v_outside > 0 ? mesh_outside_color : mesh_color;
}
