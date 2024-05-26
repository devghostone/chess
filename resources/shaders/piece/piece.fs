#version 100

precision mediump float;

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D chessPieceTexture;
uniform float isWhite;
uniform vec4 whiteColor;
uniform vec4 blackColor;
uniform vec2 offset;

void main() {
    vec4 currentTexture = texture2D(chessPieceTexture, fragTexCoord).rgba;
    if(currentTexture.r < 0.1 && currentTexture.g < 0.1 && currentTexture.b < 0.1){
        //gl_FragColor = vec4(fragTexCoord.y, 0.0, 0.0, 1.0);
        gl_FragColor = vec4(0.0, 0.0, 0.0, texture2D(chessPieceTexture, fragTexCoord).a);
        return;
    }
    if(isWhite > 0.5){
        gl_FragColor = vec4(whiteColor.rgb, texture2D(chessPieceTexture, fragTexCoord).a);
    }else{
        gl_FragColor = vec4(blackColor.rgb, texture2D(chessPieceTexture, fragTexCoord).a);
    }
}