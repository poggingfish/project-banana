#include "consts.h"
#include <raylib.h>
#include <raymath.h>
#include "title.c"
#include <stdio.h>
#include <string.h>
#include "stack.c"
typedef struct character{
    Vector2 pos;
    int velocityY;
    bool grounded;
} character;
typedef struct banana{
    int idx;
    Vector2 pos;
    char *init;
} banana;
banana bananas[100];
stack free_bananas;
character Character;
bool ShouldClose = false;
void MakeBanana(){
    banana Banana;
    Banana.pos.x = GetRandomValue(1, WIDTH-60);
    Banana.pos.y = 30;
    Banana.idx = stackPop(&free_bananas);
    bananas[Banana.idx] = Banana;
}
int main(){
    free_bananas = stackInit();
    for (int i = 0; i < 100; i++){
        stackPush(i,&free_bananas);
    }
    int score = 0;
    int ctd = CTD_BEGIN;
    Character.pos.x = 50;
    Character.pos.y = 824;
    Character.grounded = true;
    Character.velocityY = 0;
    // Cursor
    SetTraceLogLevel(LOG_FATAL);
    InitWindow(WIDTH, HEIGHT, "Project: Banana");
    InitAudioDevice();
    SetTargetFPS(60);
    Image _cursor = LoadImage("assets/cursor.png");
    ImageResize(&_cursor, 32, 32);
    Texture2D cursor = LoadTextureFromImage(_cursor);
    UnloadImage(_cursor);
    // Clicking cursor
    Image _cursor_click = LoadImage("assets/click.png");
    ImageResize(&_cursor_click, 32, 32);
    Texture2D cursor_click = LoadTextureFromImage(_cursor_click);
    UnloadImage(_cursor_click);
    // Character
    Image _character = LoadImage("assets/character.png");
    Texture2D character = LoadTextureFromImage(_character);
    UnloadImage(_character);
    // Background
    Image _bg = LoadImage("assets/bg.png");
    Texture2D bg = LoadTextureFromImage(_bg);
    UnloadImage(_bg);
    // Banana
    Image _banana = LoadImage("assets/banana.png");
    ImageResize(&_banana, 32, 32);
    Texture2D banana = LoadTextureFromImage(_banana);
    UnloadImage(_banana);
    // Collecting
    Sound collect = LoadSound("assets/coin_collect.wav");
    // Game
    SetExitKey(KEY_F4);
    HideCursor();
    bool paused = false;
    while (!ShouldClose){
        if (WindowShouldClose()){
            ShouldClose = true;
        }
        BeginDrawing();
        ClearBackground(WHITE);
        if(title()){
            continue;
        }
        if (IsKeyReleased(KEY_ESCAPE)){
            paused = !paused;
        }
        if (paused){
            DrawText("Project: Banana is paused.", 10, 10, 43, BLACK);
            DrawText("Controls: A - Right, D - Left, SPACE - Jump", 10, HEIGHT-43, 20, BLACK);
            Rectangle button;
            button.height = 64;
            button.width = 128;
            button.x = 10;
            button.y = 80;
            Rectangle close;
            close.height = 64;
            close.width = 128;
            close.x = 10;
            close.y = 160;
            Vector2 mousepos = GetMousePosition();
            DrawRectangleLinesEx(button, 5, BLACK);
            DrawRectangleLinesEx(close, 5, BLACK);
            DrawText("resume.", button.x+10, button.y+5, 24, BLACK);
            DrawText("quit.", close.x+10, close.y+5, 24, BLACK);
            if (mousepos.x > button.x && mousepos.x < button.x+button.width && mousepos.y > button.y && mousepos.y < button.y+button.height){
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                    paused = !paused;
                }
                DrawTexture(cursor_click, GetMousePosition().x, GetMousePosition().y, WHITE);
            }
            else if (mousepos.x > close.x && mousepos.x < close.x+close.width && mousepos.y > close.y && mousepos.y < close.y+close.height){
                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                    ShouldClose = true;
                }
                DrawTexture(cursor_click, GetMousePosition().x, GetMousePosition().y, WHITE);
            }
            else{
                DrawTexture(cursor, GetMousePosition().x, GetMousePosition().y, WHITE);
            }
        }else {
            ctd--;
            if (ctd == 0){
                MakeBanana();
                ctd = CTD_BEGIN;
            }
            // Actual game loop
            if (IsKeyDown(KEY_A)){
                if (Character.pos.x >= 0){
                    if (!IsKeyDown(KEY_LEFT_SHIFT)){
                        Character.pos.x -= 3;
                    }
                    else{
                        Character.pos.x -= 9;
                    }
                }
            }
            if (IsKeyDown(KEY_D)){
                if (Character.pos.x <= WIDTH-character.width){
                    if (!IsKeyDown(KEY_LEFT_SHIFT)){
                        Character.pos.x += 3;
                    }
                    else{
                        Character.pos.x += 9;
                    }
                }
            }
            if (IsKeyPressed(KEY_SPACE)){
                if (Character.grounded){
                    Character.velocityY += 25;
                }
            }
            if (Character.pos.y < 825){
                Character.pos.y += 4;
                Character.grounded = false;
            }
            else{
                Character.grounded = true;
            }
            if(Character.velocityY > 0){
                Character.pos.y -= Character.velocityY;
                Character.velocityY -= 2;
            }
            DrawTexture(bg, 0,0,WHITE);
            DrawTexture(character, Character.pos.x, Character.pos.y, LIGHTGRAY);
            for (int i = 0;i < 100;i++){
                if (bananas[i].pos.x != 0){
                    bananas[i].pos.y += 3;
                    DrawTexture(banana, bananas[i].pos.x, bananas[i].pos.y, WHITE);
                    if (bananas[i].pos.x > Character.pos.x && bananas[i].pos.x < Character.pos.x+30+character.width+50 && bananas[i].pos.y > Character.pos.y && bananas[i].pos.y < Character.pos.y+30+character.height+10){
                        PlaySound(collect);
                        score++;
                        stackPush(bananas[i].idx, &free_bananas);
                        bananas[i].pos.x = 0;
                    }
                }
            }
            char fps_str[11];
            sprintf(fps_str, "%d", GetFPS());
            strcat(fps_str, "FPS");
            DrawText(fps_str, 0,0, 32, BLACK);
            char score_str[10];
            sprintf(score_str, "%d", score);
            DrawText(score_str, WIDTH/2, HEIGHT/2, 30, BLACK);
        }
        EndDrawing();
    }
    UnloadTexture(cursor);
    UnloadTexture(cursor_click);
    UnloadTexture(character);
    UnloadTexture(banana);
    CloseAudioDevice();
    UnloadSound(collect);
    CloseWindow();
}