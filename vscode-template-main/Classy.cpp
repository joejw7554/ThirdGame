#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"

int main()
{
    const int windowWidth{384};
    const int windowHeight{384};
    InitWindow(windowWidth, windowHeight, "Classy Clash");

    Texture2D map = LoadTexture("Tiles/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    const float mapScale{4.0f};

    Character knight{windowWidth, windowHeight};

    Prop props[2]{
        Prop{Vector2{600.f, 300.f},
             LoadTexture("Tiles/Rock.png")},
        Prop{Vector2{400.f, 500.f},
             LoadTexture("Tiles/Log.png")}
             };

    Enemy goblin{
        Vector2{},
        LoadTexture("Tiles/goblin_idle_spritesheet.png"),
        LoadTexture("Tiles/goblin_run_spritesheet.png")
        };

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1);
        // Draw the map
        DrawTextureEx(map, mapPos, 0, mapScale, WHITE);

        // Draw Props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        knight.tick(GetFrameTime());

        // Check map bounds
        if (knight.getWorldPos().x < 0.f || knight.getWorldPos().y < 0.f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale || knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // Check prop collision
        for (auto prop : props)
        {
            if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
            {
                knight.undoMovement();
            }
        }

        goblin.tick(GetFrameTime());

        EndDrawing();
    }

    CloseWindow();
}