#include <cstdio>
#include <cstdlib>
#include <vector>

#include <stdint.h>
#include "raylib.h"

#define MIN_INTERVAL (1.0 / 100.0)
#define MAX_LATENCY   0.5

const int screenWidth = 1024;
const int screenHeight = 600;
const Color BACKGROUND = {0, 166, 215};

double flap_rx = 100; // Don't need theese
double flap_ry = 110;

double flap_x = screenWidth / 8.5;
double flap_y = screenHeight / 2.0;

double flap_velocity_x = 0;
double flap_velocity_y = 1;

int pipe_begin_x;
int pipe_begin_y = 0;
int pipe_iterations = 7;

uint64_t counter = 0;
bool progress = true;
uint64_t score = 0;

Texture2D flappy;
Texture2D pipe_long;
Texture2D pipe_long_rot;

class Pipe {
public:
    Pipe(float x, float y, bool is_up);
    ~Pipe();
    float x;
    float y;
    Vector2 Get_Pos();
    Rectangle Get_Rect();
    void tick();
    int is_up;
};

Pipe::Pipe(float k, float c, bool is_up): x(k), y(c), is_up(is_up) {}

void Pipe::tick()
{
    if (progress)
        x -= 4.;
}

Vector2 Pipe::Get_Pos()
{
    Vector2 ret = {x, y};
    return ret;
}

Rectangle Pipe::Get_Rect()
{
    Rectangle ret = {(int)x, (int)y, pipe_long_rot.width, pipe_long_rot.height};
    return ret;
}

Pipe::~Pipe(){}

std::vector<Pipe> PipeList;

void Update(float dx)
{
    float next_y;
    const float Speed = 16.0;
    flap_velocity_y += 0.3;

    if ((counter++ % 75) == 0) {
        auto y = rand() % (screenHeight - 200 - 250 - 50) + 150;
        PipeList.emplace_back(screenWidth, y + 250, true);
        PipeList.emplace_back(screenWidth, y - 250, false);
        if (progress)
            score++;
    }

    for (auto &pipe : PipeList) {
        pipe.tick();
    }

    if (IsKeyPressed(KEY_SPACE) && progress) {

        next_y = flap_velocity_y - Speed;
        if (next_y < -12.0){
            flap_velocity_y = -8.0;
        } else {
            flap_velocity_y -= Speed;
        }
    }


#ifdef DEBUG
    printf("Current time:%f   ",GetFrameTime());
    printf("Dx is now:%f   ",dx);
    printf("flappy_velocity %f\n", flap_velocity_y);
#endif

    flap_y += flap_velocity_y;
}


void Draw()
{
    // Main game loop
    while (!WindowShouldClose()) {    // Detect window close button or ESC key

        // Update
        Update(GetFrameTime());

        // Draw
        BeginDrawing();
            ClearBackground(BACKGROUND);
            

            Vector2 flappy_vec = {(float)flap_x, (float)flap_y};
            Rectangle flappy_rect = {(int)flappy_vec.x, (int)flappy_vec.y, flappy.width-12, flappy.height-12};

            for(auto &pipe : PipeList) {
                Vector2 pos = pipe.Get_Pos();
                if (pipe.is_up) {

                    DrawTexture(pipe_long_rot, pos.x, pos.y, WHITE);
                    DrawRectangleLines(pos.x, pos.y, pipe_long_rot.width, pipe_long_rot.height, RED);

                } else {

                    DrawTexture(pipe_long, pos.x, pos.y, WHITE);
                    DrawRectangleLines(pos.x, pos.y, pipe_long.width, pipe_long.height, RED);

                }

                if (CheckCollisionRecs(flappy_rect, pipe.Get_Rect())) {
                    progress = false;
                }

            }
            DrawText(FormatText("Score: %i", score), 5, 5, 20, VIOLET);
            DrawTextureEx(flappy, flappy_vec, -flap_velocity_y * 3, 1.0, WHITE);
            DrawRectangleLines(flappy_vec.x, flappy_vec.y, flappy.width, flappy.height, RED);

            if (!progress)
            {
                DrawText("GAME OVER..", screenWidth / 3.0, screenHeight / 2.0, 68, WHITE);
            }


        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context
}


void Init()
{
    Image flappy_img = LoadImage("assets/flap.png");
    flappy = LoadTextureFromImage(flappy_img);

    Image pipe_end_img = LoadImage("assets/pipe_long.png");
    pipe_long = LoadTextureFromImage(pipe_end_img);

    Image pipe_ext_img = LoadImage("assets/pipe_long_rot.png");
    pipe_long_rot = LoadTextureFromImage(pipe_ext_img);
}

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT); // Use before InitWindow()
    InitWindow(screenWidth, screenHeight, "Dappy happy flappy");
    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second

    //Rectangle Small_Rec = {120, 120, 20, 20};
    //Rectangle Big_Rec = {20, 20, 80, 80};

    Init();
    Draw();

    return 0;
}
