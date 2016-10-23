#version 330
in vec2 texCoord;

uniform vec3 material_emissive_color;
uniform int has_emissive_texture;
uniform sampler2D emissive_texture;

// output to frame buffer.
out vec4 fragmentColor;

void main() {

    vec3 emissive = material_emissive_color;
    if(has_emissive_texture == 1){
        vec3 texEmissive = texture(emissive_texture,texCoord.xy).xyz;
        emissive.x *= texEmissive.x;
        emissive.y *= texEmissive.y;
        emissive.z *= texEmissive.z;
    }
    if(length(emissive) < 0.0001){
        fragmentColor = vec4(1.0,1.0,1.0,0.0);
    }else{
        fragmentColor = vec4(emissive,1.0);
    }

}