#ifndef MENU_HPP
#define MENU_HPP

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#define NK_INCLUDE_STANDARD_VARARGS

#include <config.h>

#include <raylib.h>
#include "raylib-nuklear.h"

#include <string>

using std::string;

class Menu
{
    int screenPosX;
    int screenPosY;
    int width;
    int height;
    Font font;
    struct nk_context *ctx;
    char playername[20] = {0};

public:
    Menu(int screenPosX, int screenPosY, int width, int height, string fontPath)
    {
        this->screenPosX = screenPosX;
        this->screenPosY = screenPosY;
        this->width = width;
        this->height = height;
        this->font = LoadFont(fontPath.c_str());
        this->ctx = InitNuklearEx(font, 10);
        ctx->style.window.fixed_background.data.color = nk_color{WHITE_PIECE_COLOR.r, WHITE_PIECE_COLOR.g, WHITE_PIECE_COLOR.b, 0};
        // ctx->style.button.text_alignment = nk_text_alignment::NK_TEXT_LEFT;
        //  nk_style_push_color(ctx, &s->window.background, nk_rgba(BACKGROUND_COLOR.r, BACKGROUND_COLOR.g, BACKGROUND_COLOR.b, BACKGROUND_COLOR.a));
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(1.0, 0.0, 0.0, 1.0)));
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_hide());
        //  nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(0,0,0,0));
    }

    void DrawWelcomeScreen()
    {
        // struct nk_style *s = &ctx->style;
        // nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 0));
        // nk_style_push_style_item(ctx, &s->text.padding, nk_style_item_(nk_rgba(0, 0, 0, 0)));
        if (nk_begin(ctx, "Nuklear", nk_rect(screenPosX, screenPosY, width, height), NK_WINDOW_NO_SCROLLBAR))
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
                    if (nk_button_label(ctx, "Connect"))
                    {
                        // Button was clicked!
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

    void UpdateMenu()
    {
        UpdateNuklear(ctx);
    }
};

#endif