#ifndef MENU_HPP
#define MENU_HPP

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#define NK_INCLUDE_STANDARD_VARARGS

#include <config.h>
#include <multiplayer.hpp>

#include <raylib.h>
#include "raylib-nuklear.h"
#include <nlohmann/json.hpp>

#include <string>

using std::string;
using json = nlohmann::json;

std::string unescape_string(const std::string& input)
{
    std::string output;
    output.reserve(input.size()); // Reserve space to avoid frequent re-allocations

    for (size_t input_index = 0; input_index < input.length(); input_index++)
    {
        if (input[input_index] == '\\' && input_index + 1 < input.length()) // Check for escape character and ensure it's not at the end
        {
            switch (input[input_index + 1]) {
                case 'n': // Replace \n with newline
                    output += '\n';
                    input_index++;
                    break;

                case 't': // Replace \t with tab
                    output += '\t';
                    input_index++;
                    break;

                case '\\': // Replace double backslash
                    output += '\\';
                    input_index++;
                    break;

                case '"': // Replace \" with "
                    output += '"';
                    input_index++;
                    break;

                default:
                    output += input[input_index];
                    break; // In case of an unknown escape sequence, keep the original
            }
        }
        else
        {
            output += input[input_index]; // Copy the character as is
        }
    }

    return output;
}

struct Room{
    string username;
    string uniqueId;
    string offerSDP;

    static Room FromJson(const json& j){
        Room room;
        j.at("username").get_to(room.username);
        j.at("uniqueId").get_to(room.uniqueId);
        j.at("offerSDP").get_to(room.offerSDP);
        return room;
    }
};

class Menu
{
    int screenPosX;
    int screenPosY;
    int width;
    int height;
    Font font;
    struct nk_context *ctx;

    char playername[20] = {0};
    char messageInputfieldText[30] = {0};
    SocketClient* client;
    RtcClient* rtcClient;
    GAME_STATE currentState;

    string loadingText = "Loading";
    std::vector<Room> RoomList;

public:
    Menu(int screenPosX, int screenPosY, int width, int height, string fontPath, SocketClient& client, RtcClient& rtcClient)
    {
        this->screenPosX = screenPosX;
        this->screenPosY = screenPosY;
        this->width = width;
        this->height = height;
        this->font = LoadFont(fontPath.c_str());
        this->client = &client;
        this->rtcClient = &rtcClient;
        this->ctx = InitNuklear(10);

        client.AddCallback(RECEIVE_SOCKET_CONNECTED, [](string message){
            std::cout << "Connected to our main websocket server!" << std::endl;
        });

        client.AddCallback(RECEIVE_WAITING_ROOM, [&](string message){
            RoomList.clear();
            std::cout << "Waiting Room Reloaded!" << std::endl;
            json roomList = json::parse(message);
            for(json room: roomList){
                RoomList.emplace_back(Room::FromJson(room));
            }
        });

        client.AddCallback(RECEIVE_ANSWER_SDP_RECEIVED, [&](string message){
            std::cout << "setting answer sdp";
            json j = json::parse(message);
            rtcClient.SetRemoteDescription(rtc::Description(unescape_string(j["answerSDP"]), rtc::Description::Type::Answer));
        });

        ctx->style.window.fixed_background.data.color = nk_color{WHITE_PIECE_COLOR.r, WHITE_PIECE_COLOR.g, WHITE_PIECE_COLOR.b, 0};
        ctx->style.window.padding = nk_vec2(0, 0);
        // ctx->style.button.text_alignment = nk_text_alignment::NK_TEXT_LEFT;
        //  nk_style_push_color(ctx, &s->window.background, nk_rgba(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a));
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(1.0, 0.0, 0.0, 1.0)));
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_hide());
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0));
    }

    void SetState(GAME_STATE currentState){
        this->currentState = currentState;
    }

    void DrawWelcomeScreen()
    {
        // struct nk_style *s = &ctx->style;
        // nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 0));
        // nk_style_push_style_item(ctx, &s->text.padding, nk_style_item_(nk_rgba(0, 0, 0, 0)));
        if (nk_begin(ctx, "Welcome", nk_rect(screenPosX, screenPosY, width + 10, height), NK_WINDOW_NO_SCROLLBAR))
        {
            // nk_layout_row_dynamic(ctx, height, 3);
            nk_layout_row_begin(ctx, NK_DYNAMIC, height, 3);
            nk_layout_row_push(ctx, 0.1f);
            if (nk_group_begin(ctx, "1", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.8f);
            if (nk_group_begin(ctx, "2", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_static(ctx, height / 3, width * 0.69, 1);
                nk_spacer(ctx);
                if (nk_group_begin(ctx, "main", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
                {
                    nk_layout_row_static(ctx, 30, width * 0.63, 1);
                    nk_label(ctx, "Your name:", nk_text_alignment::NK_TEXT_CENTERED);
                    nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, playername, sizeof(playername) - 1, nk_filter_ascii);
                    if (nk_button_label(ctx, "Enter"))
                    {
                        // Button was clicked!
                        map<string, string> connectionMap = map<string, string>();
                        connectionMap["username"] = string(playername);
                        client->SendMessage(SEND_DISPLAY_NAME, connectionMap);
                        currentState = CONNECTED;
                        client->SendMessage(SEND_GET_WAITING_ROOM, map<string, string>());
                    }
                    nk_group_end(ctx);
                }
                // nk_layout_row_end(ctx);
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.1f);
            if (nk_group_begin(ctx, "3", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
            /*if(nk_group_begin(ctx, "2", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)){
            nk_layout_row_static(ctx, 30, width * 0.7, 1);
                if (nk_button_label(ctx, "Button"))
                {
                    // Button was clicked!
                }
                nk_edit_string_zero_terminated (ctx, NK_EDIT_FIELD, playername, sizeof(playername) - 1, nk_filter_default);
            }*/
        }
        nk_end(ctx);
        // nk_style_pop_style_item(ctx);
        // nk_style_pop_color(ctx);
        DrawNuklear(ctx);
        DrawTextEx(font, "Chess Company", Vector2{(float)screenPosX + 16, (float)screenPosY + 20}, 24, 0, WHITE);
        // DrawTextEx(font, "Chess Company", Vector2{static_cast<float>(screenPosX), static_cast<float>(screenPosY)}, 24, 0, WHITE_PIECE_COLOR);
    }

    void DrawLoadingScreen(){
        if (nk_begin(ctx, "LoadingScreen", nk_rect(screenPosX, screenPosY, width + 10, height), NK_WINDOW_NO_SCROLLBAR))
        {
            // nk_layout_row_dynamic(ctx, height, 3);
            nk_layout_row_begin(ctx, NK_DYNAMIC, height, 3);
            nk_layout_row_push(ctx, 0.25f);
            if (nk_group_begin(ctx, "1", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.5f);
            if (nk_group_begin(ctx, "2", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_static(ctx, height / 3, width * 0.45, 1);
                nk_spacer(ctx);
                nk_text_wrap(ctx, loadingText.c_str(), nk_text_alignment::NK_TEXT_CENTERED);
                nk_spacer(ctx);
                // nk_layout_row_end(ctx);
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.25f);
            if (nk_group_begin(ctx, "3", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
            /*if(nk_group_begin(ctx, "2", NK_WINDOW_BORDER|NK_WINDOW_NO_SCROLLBAR)){
            nk_layout_row_static(ctx, 30, width * 0.7, 1);
                if (nk_button_label(ctx, "Button"))
                {
                    // Button was clicked!
                }
                nk_edit_string_zero_terminated (ctx, NK_EDIT_FIELD, playername, sizeof(playername) - 1, nk_filter_default);
            }*/
        }
        nk_end(ctx);
        // nk_style_pop_style_item(ctx);
        // nk_style_pop_color(ctx);
        DrawNuklear(ctx);
        DrawTextEx(font, "Chess Company", Vector2{(float)screenPosX + 16, (float)screenPosY + 20}, 24, 0, WHITE);
    }

    void DrawWaitingRoomListScreen(){
                // struct nk_style *s = &ctx->style;
        // nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 0));
        // nk_style_push_style_item(ctx, &s->text.padding, nk_style_item_(nk_rgba(0, 0, 0, 0)));
        if (nk_begin(ctx, "Waiting Room List", nk_rect(screenPosX, screenPosY, width + 10, height), NK_WINDOW_NO_SCROLLBAR))
        {   
            nk_layout_row_begin(ctx, NK_DYNAMIC, 50, 3);
            nk_layout_row_push(ctx, 1.0f);
            if (nk_group_begin(ctx, "1", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
            nk_layout_row_begin(ctx, NK_DYNAMIC, 10, 3);
            nk_layout_row_push(ctx, 0.05f);
            nk_spacer(ctx);
            nk_layout_row_push(ctx, 0.2f);
            if(nk_button_label(ctx, "+")){
                currentState = LOADING;
                loadingText = "Generating RTC Description";
                rtcClient->CreateDataChannel();
                rtcClient->AddCallback("onLocalDescription", [&](){
                    map<string, string> createRoomMap = map<string, string>();
                    std::optional<rtc::Description> localDescriptionOptional = this->rtcClient->connection->localDescription();
                    if(localDescriptionOptional.has_value()){
                        createRoomMap["offerSDP"] = localDescriptionOptional.value();
                        client->SendMessage(SEND_CREATE_WAITING_ROOM, createRoomMap);
                        loadingText = "Waiting for new players to join";
                    }
                    rtcClient->RemoveCallbacks("onLocalDescription");
                });
            }
            nk_layout_row_push(ctx, 0.2f);
            if(nk_button_label(ctx, "\xE2\x9F\xB3")){
                client->SendMessage(SEND_GET_WAITING_ROOM, map<string, string>());
            }
            nk_layout_row_push(ctx, 0.55f);
            nk_label(ctx, "Room Lists", nk_text_alignment::NK_TEXT_CENTERED);
            nk_layout_row_end(ctx);
            nk_layout_row_begin(ctx, NK_DYNAMIC, height - 60, 3);
            nk_layout_row_push(ctx, 1.0f);
            if (nk_group_begin(ctx, "roomlists", NK_WINDOW_SCROLL_AUTO_HIDE | NK_WINDOW_BORDER))
            {
                nk_layout_row_static(ctx, 25, width * 0.9, 1);
                for(auto &room: RoomList){
                    if(nk_group_begin(ctx, "group1", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)){
                        nk_layout_row_begin(ctx, NK_DYNAMIC, 20, 3);
                        nk_layout_row_push(ctx, 0.7f);
                        nk_label(ctx, room.username.c_str(), nk_text_alignment::NK_TEXT_CENTERED);
                        nk_layout_row_push(ctx, 0.3f);
                        if(nk_button_label(ctx, "join")){
                            currentState = LOADING;
                            loadingText = "setting remote description locally"; 
                            std::cout << loadingText << std::endl;
                            rtcClient->SetRemoteDescription(rtc::Description(unescape_string(room.offerSDP), rtc::Description::Type::Offer));
                            rtcClient->AddCallback("onLocalCandidate", [&](){
                                std::map<string, string> answer;
                                answer["clientID"] = room.uniqueId;
                                std::cout << "Joining Room Description " << std::endl;
                                std::optional<rtc::Description> localDescriptionOptional = this->rtcClient->connection->localDescription();
                                if(localDescriptionOptional.has_value()){
                                    answer["answerSDP"] = localDescriptionOptional.value();
                                    client->SendMessage(SEND_SEND_ANSWER_SDP, answer);
                                    std::cout << "Sending this to server" << answer["answerSDP"] << std::endl; 
                                    rtcClient->RemoveCallbacks("onLocalCandidate");
                                }
                            });
                        }
                        nk_group_end(ctx);
                    }
                }
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
            // nk_layout_row_dynamic(ctx, height, 3);
        }
        nk_end(ctx);
        DrawNuklear(ctx);
        DrawTextEx(font, "Chess Company", Vector2{(float)screenPosX + 16, (float)screenPosY + 20}, 24, 0, WHITE);
    }

    void DrawGameScreen(){
        // struct nk_style *s = &ctx->style;
        // nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 0));
        // nk_style_push_style_item(ctx, &s->text.padding, nk_style_item_(nk_rgba(0, 0, 0, 0)));
        if (nk_begin(ctx, "Game", nk_rect(screenPosX, screenPosY, width + 10, height), NK_WINDOW_NO_SCROLLBAR))
        {
            nk_layout_row_begin(ctx, NK_DYNAMIC, height, 3);
            nk_layout_row_push(ctx, 0.05f);
            nk_spacer(ctx);

            nk_layout_row_push(ctx, 0.9f);
            
            if(nk_group_begin(ctx, "GameUIGroup", NK_WINDOW_NO_SCROLLBAR)){
                nk_layout_row_begin(ctx, NK_DYNAMIC, 50, 1);
                nk_layout_row_push(ctx, 1.0f);
                nk_spacer(ctx);
                nk_layout_row_end(ctx);

                nk_layout_row_begin(ctx, NK_DYNAMIC, height - 130, 1);
                nk_layout_row_push(ctx, 1.0f);
                if(nk_group_begin(ctx, "ChatGroup", NK_WINDOW_BORDER)){
                    
                    for(int i = 0; i < 30; i++){
                        nk_layout_row_dynamic(ctx, 21, 1);
                        nk_label_wrap(ctx, "Ethan : Message is here?");
                    }

                    nk_layout_row_dynamic(ctx, 21, 1);
                    nk_label_wrap(ctx, "Jack : Yes");

                    nk_group_end(ctx);
                }
                nk_layout_row_end(ctx);

                nk_layout_row_begin(ctx, NK_DYNAMIC, 20, 2);
                nk_layout_row_push(ctx, 0.8f);
                nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, messageInputfieldText, sizeof(messageInputfieldText) - 1, nk_filter_ascii);
                nk_layout_row_push(ctx, 0.2f);
                nk_button_label(ctx, "send");
                nk_layout_row_end(ctx);

                nk_layout_row_begin(ctx, NK_DYNAMIC, 50, 1);
                nk_layout_row_push(ctx, 1.0f);
                if(nk_button_label(ctx, "Resign")){
                    
                }
                nk_layout_row_end(ctx);
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.05f);
            nk_spacer(ctx);
            nk_layout_row_end(ctx);
        }
        nk_end(ctx);
        // nk_style_pop_style_item(ctx);
        // nk_style_pop_color(ctx);
        DrawNuklear(ctx);
        DrawTextEx(font, "Chess Company", Vector2{(float)screenPosX + 16, (float)screenPosY + 20}, 24, 0, WHITE);
    }

    void Update()
    {
        UpdateNuklear(ctx);
    }

    void Draw(){
        switch(currentState){
            case LOADING:
                DrawLoadingScreen();
                break;
            case CONNECTED_NAME_NOT_SET:
                DrawWelcomeScreen();
                break;
            case CONNECTED:
                DrawWaitingRoomListScreen();
                break;
            default:
                DrawWelcomeScreen();
        }
    }
};

#endif