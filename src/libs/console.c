

#include "console.h"



/******************************************************************************/
/**< console functions */
void console_set_size(HANDLE hnd, short xsize, short ysize)
{
  COORD console_sz;
  console_sz.X = xsize;
  console_sz.Y = ysize;
  SetConsoleScreenBufferSize(hnd, console_sz);
}
void console_set_canvasSize(HANDLE hnd, short xsize, short ysize)
{
  SMALL_RECT canvas_sz = {0};
  canvas_sz.Right = xsize;
  canvas_sz.Bottom = ysize;
  SetConsoleWindowInfo(hnd, 1, &canvas_sz);
}


void console_cls(HANDLE hnd)
{
  DWORD length,written;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD topLeft = {0};
  SetConsoleCursorPosition(hnd, topLeft);

  /* std::cout uses a buffer to batch writes to the underlying console. */
  /* We need to flush that to the console because we're circumventing */
  /* std::cout entirely; after we clear the console, we don't want */
  /* stale buffered text to randomly be written out. */
  /*std::cout.flush(); */
  /*FlushConsoleInputBuffer(hnd); */

  /* Figure out the current width and height of the console window */
  if (!GetConsoleScreenBufferInfo(hnd, &csbi))
    abort();

  /* Flood-fill the console with spaces to clear it */
  length = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hnd, TEXT(' '), length, topLeft, &written);

  /* Reset the attributes of every character to the default. */
  /* This clears all background colour formatting, if any. */
  FillConsoleOutputAttribute(hnd, csbi.wAttributes, length, topLeft, &written);

  /* Move the cursor back to the top left for the next sequence of writes*/
  SetConsoleCursorPosition(hnd, topLeft);
  /* Black and white */
  SetConsoleTextAttribute(hnd, 0xf);
}




void console_replace(HANDLE hnd)
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  COORD topLeft = { 0, 0 };
  SetConsoleCursorPosition(hnd, topLeft);
  if (!GetConsoleScreenBufferInfo(hnd, &csbi)) abort();
  SetConsoleCursorPosition(hnd, topLeft);
}
