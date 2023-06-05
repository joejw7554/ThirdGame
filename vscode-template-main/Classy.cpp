#include "raylib.h"
#include "raymath.h"

class Character
{
public:
    Vector2 getWorldPos() { return worldPos; }
    void setScreenPos(int winWidth, int winHeight);
    void tick(float deltaTime);

private:
    Texture2D texture{LoadTexture("Tiles/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("Tiles/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("Tiles/knight_run_spritesheet.png")};
    Vector2 screenPos;
    Vector2 worldPos;
    // 1: facing right -1: facing left
    float rightLeft{1.f};
    // anim varialbes
    float runningTime{};
    int frame{};
    const int maxFrames{6};
    const float updateTime{1.f / 12.f};
    const float speed{4.f};
};

void Character::setScreenPos(int winWidth, int winHeight)
{
    screenPos = {
        (float)winWidth / 2.0f - 4.0f * (0.5f * (float)texture.width / 6.0f),
        (float)winHeight / 2.0f - 4.0f * (0.5f * (float)texture.height)};
}

void Character::tick(float deltaTime)
{
    deltaTime = GetFrameTime();

    Vector2 direction{};
    if (IsKeyDown(KEY_A))
        direction.x -= 1.0;
    if (IsKeyDown(KEY_D))
        direction.x += 1.0;
    if (IsKeyDown(KEY_W))
        direction.y -= 1.0;
    if (IsKeyDown(KEY_S))
        direction.y += 1.0;

    if (Vector2Length(direction) != 0.0)
    {
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(direction), speed));
        direction.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        texture = run;
    }
    else
    {
        texture = idle;
    }

    // update anim frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame++;
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0;
    }
}

int main()
{
    const int windowWidth{384};
    const int windowHeight{384};
    InitWindow(windowWidth, windowHeight, "Classy Clash");

    Texture2D map = LoadTexture("Tiles/OpenWorldMap24x24.png");
    Vector2 mapPos{0.0, 0.0};
    float speed{4.0};

    Texture2D knight = LoadTexture("Tiles/knight_idle_spritesheet.png");
    Texture2D knight_idle = LoadTexture("Tiles/knight_idle_spritesheet.png");
    Texture2D knight_run = LoadTexture("Tiles/knight_run_spritesheet.png");
    Vector2 knightPos{
        (float)windowWidth / 2.0f - 4.0f * (0.5f * (float)knight.width / 6.0f),
        (float)windowHeight / 2.0f - 4.0f * (0.5f * (float)knight.height)};
    float rightLeft{1.f};

    // anim varialbes
    float runningTime{};
    int frame{};
    const int maxFrames{6};
    const float updateTime{1.f / 12.f};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // Draw the map
        DrawTextureEx(map, mapPos, 0, 4.0, WHITE);

        // draw the character
        Rectangle source = {frame * (float)knight.width / 6.0f, 0.f, rightLeft * (float)knight.width / 6.0f, (float)knight.height};
        Rectangle dest = {knightPos.x, knightPos.y, 4.0f * (float)knight.width / 6.0f, 4.0f * (float)knight.height};
        DrawTexturePro(knight, source, dest, Vector2{}, 0.f, WHITE);

        const float dT{GetFrameTime()};

        EndDrawing();
    }

    UnloadTexture(map);
    UnloadTexture(knight);
    UnloadTexture(knight_run);

    CloseWindow();
}