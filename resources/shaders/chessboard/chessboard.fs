#version 100

precision mediump float;

uniform vec4 whiteSpace;
uniform vec4 blackSpace;
uniform float cellSize;

varying vec2 fragTexCoord;

void main() {

    float file = floor(fragTexCoord.x * 8.0);
    float rank = floor(fragTexCoord.y * 8.0);
    vec3 color;
    if(mod(file + rank, 2.0) != 0.0){
        color = whiteSpace.rgb;
    }else{
        color = blackSpace.rgb;
    }
    gl_FragColor = vec4(color, 1.0);
}