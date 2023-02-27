#include <raylib.h>
int tick = 0;
bool title(){
    if (tick > 128){
        return false;
    }
    else{
        DrawText("Project: Banana", 10, 10, 64, BLUE);
        EndDrawing();
        tick++;
        return true;
    }
}