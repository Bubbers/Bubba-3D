#version 330
in vec2 texCoord;

uniform vec3 material_emissive_color;
uniform int has_diffuse_texture;
uniform sampler2D diffuse_texture;

// output to frame buffer.
out vec4 fragmentColor;

void main() {

    vec3 baseColor = texture(diffuse_texture, texCoord.xy).xyz;
    if(length(material_emissive_color) < 0.0001){
        fragmentColor = vec4(1.0,1.0,1.0,0.0);
    }else{
        fragmentColor = vec4(material_emissive_color.x, material_emissive_color.y, material_emissive_color.z,1.0);
    }

}