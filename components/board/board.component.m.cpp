#include <windows.h>
#include "board.component.h"

#define BUTTON_SIZE 100

void create_board_component(HWND hwndParent)
{
	int x, y;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			x = i * BUTTON_SIZE + 30;
			y = j * BUTTON_SIZE + 30;

			CreateWindowW(
					L"BUTTON",
					L"",
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER,
					x, y, BUTTON_SIZE, BUTTON_SIZE,
					hwndParent,
					(HMENU)(i * 3 + j + 1), // Button ID (1 to 9)
					(HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE),
					NULL);
		}
	}
}
