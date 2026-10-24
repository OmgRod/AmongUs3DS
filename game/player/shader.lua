local playerShader = love.graphics.newShader[[
    extern vec4 bodyColor;
    extern vec4 shadowColor;
    extern vec4 visorColor;

    vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_coords) {
        vec4 tex = Texel(texture, texture_coords);
        
        vec3 finalRGB = (tex.r * bodyColor.rgb) + 
                         (tex.b * shadowColor.rgb) + 
                         (tex.g * visorColor.rgb);

        return vec4(finalRGB, tex.a * color.a);
    }
]]

return playerShader