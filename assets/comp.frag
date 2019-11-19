#version 330

//image to apply as texture
uniform sampler2D	tex0;
uniform sampler2D	tex1;
uniform vec2        offset1;
uniform vec2        offsetArray[5];
in vec2 vTexCoord0;
out vec4 oColor;

void main()
{
    //offset moves it left to right
    float scale = 0.76;
    vec2 val = offsetArray[0];
    vec2 rawCoords = (vTexCoord0 + val)*scale;
  
    vec3 comp = texture(tex1,rawCoords).rgb;
    int numLayers = 5;
    for(int i = 0; i < numLayers; i++){
        rawCoords = (vTexCoord0 + offsetArray[i])*scale;
//        rawCoords = (vTexCoord0 + i*offset1)*scale;
        comp = comp + texture(tex1, rawCoords).rgb;
    }
//    comp = comp / numLayers;
    //getting color at tex0 at those coordinates
    vec3 col = texture(tex0,vTexCoord0).rgb;
    //getting color at tex1 at those coordinates
    //additive layering of the colors
    oColor.rgb = (col+comp) / numLayers;
    oColor.a = 0.25;
    
}
