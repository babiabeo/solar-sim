#include <raylib.h>
#include <raymath.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------

#define SIZE   600.0
#define WIDTH  SIZE
#define HEIGHT SIZE

#define HALF_W (WIDTH / 2)
#define HALF_H (HEIGHT / 2)

#define AU2PIXEL    (SIZE / 17)
#define ORBIT_COLOR DARKBLUE
#define BASE_POS    (Vector2){WIDTH / 2, HEIGHT / 2}

#define TAU (PI * 2)

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct {
    Vector2 pos;
    Color color;
    float radius;
    float orbit_radius;   // distance from sun (in pixel)
    float orbit_speed;
    float angle;
} planet_t;

static planet_t planet_new(Vector2 pos,
                           float radius,
                           float orbit_radius,
                           float orbit_speed,
                           Color color);
static void planet_draw(planet_t *planet);
static void planet_update(planet_t *planet, float delta);

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static int frames  = 0;
static float speed = 1.0f;
static planet_t sun, mercury, venus, earth, mars, jupiter, saturn, uranus,
    neptune;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

static void InitGame();
static void UpdateGame();
static void DrawGame();

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main()
{
    InitWindow(WIDTH, HEIGHT, "Solar Simulation");
    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    CloseWindow();
    return 0;
}

//------------------------------------------------------------------------------------
// Methods Definitions
//------------------------------------------------------------------------------------

static void update_pos(planet_t *planet)
{
    float nx = cosf(planet->angle) * planet->orbit_radius;
    float ny = sinf(planet->angle) * planet->orbit_radius;

    planet->pos.x = HALF_W + nx;
    planet->pos.y = HALF_H - ny;
}

planet_t planet_new(Vector2 pos,
                    float radius,
                    float orbit_radius,
                    float orbit_speed,
                    Color color)
{
    planet_t planet;

    orbit_radius *= AU2PIXEL;

    planet.color        = color;
    planet.orbit_radius = orbit_radius;
    planet.orbit_speed  = orbit_speed;
    planet.radius       = radius;
    planet.angle        = GetRandomValue(0, 360) * DEG2RAD;
    update_pos(&planet);

    return planet;
}

void planet_draw(planet_t *planet)
{
    DrawCircleLinesV(BASE_POS, planet->orbit_radius, ORBIT_COLOR);
    DrawCircleV(planet->pos, planet->radius, planet->color);
}

void planet_update(planet_t *planet, float delta)
{
    if (planet->angle > TAU) {
        planet->angle -= TAU;
    }

    update_pos(planet);

    float angular_vel = planet->orbit_speed / planet->orbit_radius;
    planet->angle += (angular_vel * 4 * delta);
}

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

void InitGame()
{
    SetTargetFPS(60);

    frames = 0;

    sun     = planet_new(BASE_POS, 15, 0, 0, ORANGE);
    mercury = planet_new(BASE_POS, 6, 1, 48.4, GRAY);
    venus   = planet_new(BASE_POS, 8, 2, 36.0, RAYWHITE);
    earth   = planet_new(BASE_POS, 8, 3, 30.8, BLUE);
    mars    = planet_new(BASE_POS, 7, 4, 25.1, RED);
    jupiter = planet_new(BASE_POS, 11, 5, 14.1, BROWN);
    saturn  = planet_new(BASE_POS, 10, 6, 10.7, BEIGE);
    uranus  = planet_new(BASE_POS, 9, 7, 7.8, SKYBLUE);
    neptune = planet_new(BASE_POS, 9, 8, 6.4, SKYBLUE);
}

void UpdateGame()
{
    float delta = GetFrameTime() * speed;

    if (frames % 10 == 0) {
        if (IsKeyDown(KEY_MINUS))
            speed -= 0.1;
        else if (IsKeyDown(KEY_EQUAL))
            speed += 0.1;

        speed  = Clamp(speed, 0, 5);
        frames = 0;
    }

    planet_update(&mercury, delta);
    planet_update(&venus, delta);
    planet_update(&earth, delta);
    planet_update(&mars, delta);
    planet_update(&jupiter, delta);
    planet_update(&saturn, delta);
    planet_update(&uranus, delta);
    planet_update(&neptune, delta);

    frames++;
}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(20, 10);
    DrawText(TextFormat("Speed: x%.1f", speed), 20, 30, 20, RAYWHITE);

    planet_draw(&sun);
    planet_draw(&mercury);
    planet_draw(&venus);
    planet_draw(&earth);
    planet_draw(&mars);
    planet_draw(&jupiter);
    planet_draw(&saturn);
    planet_draw(&uranus);
    planet_draw(&neptune);

    EndDrawing();
}
