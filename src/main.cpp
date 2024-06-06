#define SUPPORT_FILEFORMAT_SVG
#define GLSL_VERSION 100

#include "config.h"
#include "chess-engine.hpp"
#include "gui.hpp"
#include "menu.hpp"
#include "multiplayer.hpp"

#include "raylib.h"

#include <iostream>
#include <array>

using std::cout, std::endl, std::array;

Texture chessBoardTexture;
Shader chessPieceShader;
Shader chessBoardShader;
int isWhiteLoc;
int chessPieceTextureLoc;
int offsetLoc;

array<Vector2, 6> GetWhitePieces(){
    array<Vector2, 6> whitePieces;
    for(int i = 0; i < 6; i++){
        whitePieces[i].x = SVG_RES_X/6 * i;
        whitePieces[i].y = 0;
    }
    return whitePieces;
}

array<Vector2, 6> GetBlackPieces(){
    array<Vector2, 6> blackPieces;
    for(int i = 0; i < 6; i++){
        blackPieces[i].x = SVG_RES_X/6 * i;
        blackPieces[i].y = SVG_RES_Y/2;
    }
    return blackPieces;
}

int main(){
    #ifndef __EMSCRIPTEN__
    //rtc::InitLogger(rtc::LogLevel::Verbose);
    #endif
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME);
    SetTargetFPS(30);

    chessPieceShader = LoadShader("resources/shaders/base.vs", "resources/shaders/piece/piece.fs");
    int whiteColorLoc = GetShaderLocation(chessPieceShader, "whiteColor");
    int blackColorLoc = GetShaderLocation(chessPieceShader, "blackColor");
    isWhiteLoc = GetShaderLocation(chessPieceShader, "isWhite");

    chessBoardShader = LoadShader("resources/shaders/base.vs", "resources/shaders/chessboard/chessboard.fs");
    int whiteSpaceLoc = GetShaderLocation(chessBoardShader, "whiteSpace");
    int blackSpaceLoc = GetShaderLocation(chessBoardShader, "blackSpace");

    Vector4 whitePieceColor = Vector4{WHITE_PIECE_COLOR.r/255.0f, WHITE_PIECE_COLOR.g/255.0f, WHITE_PIECE_COLOR.b/255.0f, WHITE_PIECE_COLOR.a};
    Vector4 blackPieceColor = Vector4{BLACK_PIECE_COLOR.r/255.0f, BLACK_PIECE_COLOR.g/255.0f, BLACK_PIECE_COLOR.b/255.0f, BLACK_PIECE_COLOR.a};
    SetShaderValue(chessPieceShader, whiteColorLoc, &whitePieceColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(chessPieceShader, blackColorLoc, &blackPieceColor, SHADER_UNIFORM_VEC4);

    Vector4 whiteSpaceColor = Vector4{WHITE_SPACE_COLOR.r/255.0f, WHITE_SPACE_COLOR.g/255.0f, WHITE_SPACE_COLOR.b/255.0f, WHITE_SPACE_COLOR.a};
    Vector4 blackSpaceColor = Vector4{BLACK_SPACE_COLOR.r/255.0f, BLACK_SPACE_COLOR.g/255.0f, BLACK_SPACE_COLOR.b/255.0f, BLACK_SPACE_COLOR.a};
    SetShaderValue(chessBoardShader, whiteSpaceLoc, &whiteSpaceColor, SHADER_UNIFORM_VEC4);
    SetShaderValue(chessBoardShader, blackSpaceLoc, &blackSpaceColor, SHADER_UNIFORM_VEC4);

    RenderTexture2D backgroundTexture = LoadRenderTexture(SCREEN_HEIGHT, SCREEN_HEIGHT);
    RenderTexture2D chessboardTarget = LoadRenderTexture(SCREEN_HEIGHT, SCREEN_HEIGHT);
    BeginDrawing();
    BeginTextureMode(backgroundTexture);
    ClearBackground(WHITE);
    EndTextureMode();
    BeginTextureMode(chessboardTarget);
    BeginShaderMode(chessBoardShader);
    DrawTexture(backgroundTexture.texture, 0, 0, WHITE);
    EndShaderMode();
    EndTextureMode();
    EndDrawing();
    chessBoardTexture = chessboardTarget.texture;

    Board board = Board();
    GuiPieceRenderer renderer = GuiPieceRenderer({GetWhitePieces(), GetBlackPieces()}, CHESS_PIECE_SVG, SVG_RES_X, SVG_RES_Y);
    GuiChessboard boardGui = GuiChessboard(board, renderer, WHITE_SPACE_COLOR, BLACK_SPACE_COLOR, SELECTION_SPACE_COLOR, SCREEN_HEIGHT);
    GuiInputManager inputManager = GuiInputManager(boardGui, SCREEN_HEIGHT);
   
    SocketClient client = SocketClient("wss://chess-company-api.htetaung.com");
    rtc::Configuration config;
    config.iceServers.emplace_back("stun:stun.l.google.com:19302");
    //config.iceServers.emplace_back("stun:stun.l.google.com:5349");
    //config.iceServers.emplace_back("stun:stun1.l.google.com:19302");
    //config.iceServers.emplace_back("stun:stun1.l.google.com:5349");
    config.iceServers.emplace_back("stun:global.stun.twilio.com:3478");
    RtcClient rtcClient = RtcClient(config);

    Menu menu = Menu(static_cast<int>(SCREEN_HEIGHT), 0, static_cast<int>(SCREEN_WIDTH - SCREEN_HEIGHT), static_cast<int>(SCREEN_HEIGHT), FONT_PATH, client, rtcClient);
    client.CreateClient(
    [&menu](){
        menu.SetState(LOADING);
    },
    [&menu](){
        menu.SetState(CONNECTED_NAME_NOT_SET);
    });

    /*auto connection = std::make_shared<rtc::PeerConnection>(config);
    cout << "started peer connection" << std::endl;
    connection->onLocalDescription([](rtc::Description sdp)
                                  { std::cout << sdp << std::endl; });

    connection->createDataChannel("gg");*/
    //connection->setLocalDescription(rtc::Description::Type::Offer);

    while(!WindowShouldClose()){
        inputManager.Update();
        menu.Update();

        BeginDrawing();
        ClearBackground(BLACK_SPACE_COLOR);
        boardGui.RenderBoard();
        boardGui.RenderPieces();
        boardGui.RenderSelection();
        
        menu.Draw();
        EndDrawing();
    }
    CloseWindow();
}