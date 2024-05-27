#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

#define APP_NAME "Chess Company"
#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 496
#define BACKGROUND_COLOR Color{0x1b, 0x1a, 0x1d, 0xff}
#define WHITE_SPACE_COLOR (Color){0xd7, 0xaa, 0x60, 0xff}
#define BLACK_SPACE_COLOR (Color){0x1b, 0x1a, 0x1d, 0xff}
#define WHITE_PIECE_COLOR (Color){0xd0, 0xca, 0xa9, 0xff}
#define BLACK_PIECE_COLOR (Color){0x4b, 0x4f, 0x53, 0xff}
#define SELECTION_SPACE_COLOR (Color){251, 192, 147, 128}

#define CHESS_PIECE_SVG "resources/sprites/chess_pieces.svg"
#define FONT_PATH "resources/font/millimetre.otf"
#define SVG_RES_X 1350
#define SVG_RES_Y 450

#define SEND_DISPLAY_NAME "create_username"
#define SEND_CREATE_WAITING_ROOM "create_waiting_room"
#define SEND_REMOVE_WAITING_ROOM "remove_waiting_room"
#define SEND_GET_WAITING_ROOM "get_waiting_room"
#define SEND_SEND_ANSWER_SDP "send_answer_sdp"
#define SEND_ICE_CANDIDATE "send_ice_candidate"

#define RECEIVE_SOCKET_CONNECTED "socket_connected"
#define RECEIVE_WAITING_ROOM "waiting_room"
#define RECEIVE_ANSWER_SDP_RECEIVED "answer_sdp_received"
#define RECEIVE_ICE_CANDIDATE "receive_ice_candidate"

extern Texture chessPiecesTexture;
extern Texture chessBoardTexture;
extern Shader chessPieceShader;
extern Shader chessBoardShader;
extern int isWhiteLoc;
extern int chessPieceTextureLoc;
extern int offsetLoc;

enum GAME_STATE{
    NOT_CONNECTED,
    CONNECTED_NAME_NOT_SET,
    CONNECTED,
    EXCHANGING_INFORMATION,
    IN_GAME,
    LOADING
};

#endif