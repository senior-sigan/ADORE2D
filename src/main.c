#include <stdio.h>
#include <raylib.h>

int main(void) {
  printf("Hello world\n");

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
