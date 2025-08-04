#include <raylib.h>
#include <raymath.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------

#define SIZE      650.0
#define HALF_SIZE (SIZE / 2)
#define BASE_POS  ((Vector2){HALF_SIZE, HALF_SIZE})

#define TAU (PI * 2)

#define FONT_LG 20
#define FONT_SM 10

#define FOR_PLANETS(n) for (int i = 0; i < (n); ++i)

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct {
    char is_sun;
    Vector2 pos;
    Color color;
    float radius;
    float orbit_radius;   // distance from sun (in pixel)
    float orbit_speed;
    float angle;
    float year;           // year on the planet
} planet_t;

static planet_t planet_new(Vector2 pos,
                           float radius,
                           float orbit_radius,
                           float orbit_speed,
                           Color color,
                           char is_sun);
static void planet_draw(planet_t *planet);
static void planet_update(planet_t *planet, float delta);

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int WIDTH  = SIZE;
static const int HEIGHT = SIZE;

static const float AU = SIZE * 0.058;   // AU to pixel (5.8% of window size)
static const float KM = 5.0f;           // km to pixel

static int frames          = 0;
static float speed         = 1.0f;
static char toggle_orbit   = 0;
static planet_t planets[9] = {0};

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

    planet->pos.x = HALF_SIZE + nx;
    planet->pos.y = HALF_SIZE - ny;
}

planet_t planet_new(Vector2 pos,
                    float radius,
                    float orbit_radius,
                    float orbit_speed,
                    Color color,
                    char is_sun)
{
    planet_t planet;

    planet.is_sun       = is_sun;
    planet.color        = color;
    planet.orbit_radius = orbit_radius * AU;
    planet.orbit_speed  = orbit_speed * KM;
    planet.radius       = radius;
    planet.angle        = is_sun ? 0 : GetRandomValue(0, 360) * DEG2RAD;
    planet.year         = 0;
    update_pos(&planet);

    return planet;
}

void planet_draw(planet_t *planet)
{
    DrawCircleV(planet->pos, planet->radius, planet->color);

    if (toggle_orbit)
        DrawCircleLinesV(BASE_POS, planet->orbit_radius, planet->color);

    if (!planet->is_sun) {
        float year          = planet->year + (planet->angle / TAU);
        const char *yeartxt = TextFormat("%.3f", year);
        float width         = MeasureText(yeartxt, FONT_SM);

        DrawText(TextFormat("%.3f", year), planet->pos.x - (width / 2),
                 planet->pos.y + (planet->radius + 5), 10, RAYWHITE);
    }
}

void planet_update(planet_t *planet, float delta)
{
    if (planet->angle > TAU) {
        planet->angle -= TAU;
        planet->year++;
    }

    update_pos(planet);

    float angular_vel = planet->orbit_speed / planet->orbit_radius;
    planet->angle += (angular_vel * delta);
}

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------

void InitGame()
{
    SetTargetFPS(60);

    planets[0] = planet_new(BASE_POS, 15, 0, 0, ORANGE, 1);       // sun
    planets[1] = planet_new(BASE_POS, 6, 1, 48.4, GRAY, 0);       // mercury
    planets[2] = planet_new(BASE_POS, 8, 2, 36.0, RAYWHITE, 0);   // venus
    planets[3] = planet_new(BASE_POS, 8, 3, 30.8, BLUE, 0);       // earth
    planets[4] = planet_new(BASE_POS, 7, 4, 25.1, RED, 0);        // mars
    planets[5] = planet_new(BASE_POS, 11, 5, 14.1, BROWN, 0);     // jupiter
    planets[6] = planet_new(BASE_POS, 10, 6, 10.7, BEIGE, 0);     // saturn
    planets[7] = planet_new(BASE_POS, 9, 7, 7.8, SKYBLUE, 0);     // uranus
    planets[8] = planet_new(BASE_POS, 9, 8, 6.4, DARKBLUE, 0);    // neptune
}

void UpdateGame()
{
    float delta = GetFrameTime() * speed;

    if (IsKeyPressed(KEY_SPACE)) toggle_orbit = !toggle_orbit;

    if (frames == 7) {
        if (IsKeyDown(KEY_MINUS))
            speed -= 0.1;
        else if (IsKeyDown(KEY_EQUAL))
            speed += 0.1;

        speed  = Clamp(speed, 0, 5);
        frames = 0;
    }

    // sun doesn't need to be updated
    FOR_PLANETS(8)
    {
        planet_update(&planets[i + 1], delta);
    }

    frames++;
}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(20, 10);
    DrawText(TextFormat("Speed: x%.1f", speed), 20, 30, FONT_LG, RAYWHITE);

    FOR_PLANETS(9)
    {
        planet_draw(&planets[i]);
    }

    EndDrawing();
}
