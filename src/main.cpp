#include <raylib.h>

#include <iostream>

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else  // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

constexpr int WIDTH = 960;
constexpr int HEIGHT = 544;

bool shouldRestart = false;
bool shouldStop = false;

class Context {
 public:
  Context() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WIDTH, HEIGHT, "ADORE2D");
    SetTargetFPS(30);
    // SetTextureFilter(GetFontDefault().texture, FILTER_POINT);
  }

  ~Context() {
    CloseWindow();
  }

  void Update() {
    // scene_manager.Update();
    BeginDrawing();
    ClearBackground(BLACK);
    // scene_manager.Draw();
    EndDrawing();
  }
  bool ShouldStop() const {
    return WindowShouldClose();
  }
};

Context* context;

void Update() {
  context->Update();
}

bool Run() {
  Context ctx;
  context = &ctx;

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(Update, 0, 1);
#else
  while (!ctx.ShouldStop()) {
    Update();
  }
#endif
}

int main() {
  Run();
  return 0;
}