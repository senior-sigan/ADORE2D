#include <raylib.h>

#include <iostream>
#include <string>

#include "lua.hpp"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION 330
#else  // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION 100
#endif

constexpr int WIDTH = 960;
constexpr int HEIGHT = 544;

const char* script_path = "main.lua";

bool shouldRestart = false;
bool shouldStop = false;

static int line(lua_State* L) {
  int n = lua_gettop(L);
  if (n == 4) {
    int startPosX = lua_tonumber(L, 1);
    int startPosY = lua_tonumber(L, 2);
    int endPosX = lua_tonumber(L, 3);
    int endPosY = lua_tonumber(L, 4);
    DrawLine(startPosX, startPosY, endPosX, endPosY, RED);
  } else {
    luaL_error(L, "invalid parameters, line(x0,y0,x1,y1)\n");
  }
  return 0;
}

class LuaAPI {
  lua_State* L;

  void callFunc(std::string name) {
    lua_getglobal(L, "adore");
    lua_getfield(L, -1, name.c_str());
    if (lua_isfunction(L, -1)) {
      if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        auto msg = lua_tostring(L, -1);
        std::cerr << "ERROR: " << msg << std::endl;
      }
    } else {
      std::cerr << "ERROR: 'adore." << name << "' is not a function"
                << "\n";
    }
  }

 public:
  LuaAPI() {
    L = luaL_newstate();
    luaL_openlibs(L);
  }

  void Init() {
    lua_newtable(L);

    lua_pushcfunction(L, line);
    lua_setfield(L, -2, "line");

    lua_setglobal(L, "adore");

    int err = luaL_dofile(L, script_path);

    if (err != LUA_OK) {
      auto msg = lua_tostring(L, -1);
      std::cerr << "ERROR: " << msg << std::endl;
      exit(1);
    }

    callFunc("init");
  }

  void Draw() {
    callFunc("draw");
  }

  void Update() {
    callFunc("update");
  }

  ~LuaAPI() {
    lua_close(L);
  }
};

class Context {
  LuaAPI api;

 public:
  Context() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WIDTH, HEIGHT, "ADORE2D");
    SetTargetFPS(30);
    // SetTextureFilter(GetFontDefault().texture, FILTER_POINT);

    api.Init();
  }

  ~Context() {
    CloseWindow();
  }

  void Update() {
    api.Update();
    BeginDrawing();
    ClearBackground(BLACK);
    api.Draw();
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

void Run() {
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

int main(int argc, char** argv) {
  if (argc > 1) {
    script_path = argv[1];
  }

  std::cout << script_path << std::endl;

  Run();
  return 0;
}