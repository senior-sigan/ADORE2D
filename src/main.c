#include <stdio.h>
#include <raylib.h>
#include <duktape.h>

int main(void) {
  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, "1+2");
  printf("1+2=%d\n", (int) duk_get_int(ctx, -1));
  duk_destroy_heap(ctx);

  InitWindow(800, 600, "ADORE2D");
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle(32, 32, 32, 32, RED);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
