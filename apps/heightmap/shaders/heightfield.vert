#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

uniform vec3 mesh_origin;
uniform float mesh_resolution;

uniform int heightmap_rows;
uniform int heightmap_cols;
uniform vec2 heightmap_bottomleft;
uniform vec2 heightmap_topright;
uniform float heightmap_altmin;
uniform float heightmap_altmax;

uniform samplerBuffer zvalues_texture;

out float v_hole;
out float v_outside;

void main()
{
    vec3 raw_pos = mesh_origin + position * mesh_resolution;
    float float_col = round(heightmap_cols * (raw_pos.x - heightmap_bottomleft.x) / (heightmap_topright.x - heightmap_bottomleft.x));
    float float_row = round(heightmap_rows * (raw_pos.y - heightmap_bottomleft.y) / (heightmap_topright.y - heightmap_bottomleft.y));
    int int_col = int(clamp(float_col,0,heightmap_cols - 1));
    int int_row = int(clamp(float_row,0,heightmap_rows - 1));

    float z_normalized = texelFetch(zvalues_texture, heightmap_rows * int_col + int_row).r;
    raw_pos.z = heightmap_altmin + (heightmap_altmax - heightmap_altmin) * z_normalized;

    v_hole = z_normalized < 0 ? 1 : 0;

    float overflow_left = 0 - float_col;
    float overflow_right = float_col - heightmap_cols;
    float overflow_top = 0 - float_row;
    float overflow_bottom = float_row - heightmap_rows;
    v_outside = max(overflow_left, max(overflow_right, max(overflow_top, overflow_bottom)));
    //v_outside = (float_col < 0 || float_col > heightmap_cols - 1 || float_row < 0 || float_row > heightmap_rows - 1)  ? 1 : 0;
    gl_Position = projection_matrix * model_view_matrix * vec4(raw_pos, 1.0);
}
