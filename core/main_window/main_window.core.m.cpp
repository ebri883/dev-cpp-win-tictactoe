#include <windows.h>
#include <iostream>
#include "../../components/board/board.component.h"

// Define the class name using wide characters
const wchar_t CLASS_NAME[] = L"main_window_class";

// Global handles for the UI elements
static HBRUSH brush;
static HWND hwndStatusText;
static HWND hwndWinMessage;
static HWND hwndBoardButtons[9]; // Array to hold button handles

// Player turn tracking
bool isPlayerOneTurn = true;

// Define a global array to track button states
wchar_t buttonStates[3][3] = {{L' ', L' ', L' '}, {L' ', L' ', L' '}, {L' ', L' ', L' '}};

// Function to setup the console for debugging
void setupConsole()
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout); // Redirect stdout to the console
	std::cout << "Console setup complete" << std::endl;
}

// Function to check if there is a winner
bool checkWinner()
{
	// Check rows and columns
	for (int i = 0; i < 3; ++i)
	{
		if (buttonStates[i][0] != L' ' &&
				buttonStates[i][0] == buttonStates[i][1] &&
				buttonStates[i][1] == buttonStates[i][2])
			return true;

		if (buttonStates[0][i] != L' ' &&
				buttonStates[0][i] == buttonStates[1][i] &&
				buttonStates[1][i] == buttonStates[2][i])
			return true;
	}

	// Check diagonals
	if (buttonStates[0][0] != L' ' &&
			buttonStates[0][0] == buttonStates[1][1] &&
			buttonStates[1][1] == buttonStates[2][2])
		return true;

	if (buttonStates[0][2] != L' ' &&
			buttonStates[0][2] == buttonStates[1][1] &&
			buttonStates[1][1] == buttonStates[2][0])
		return true;

	return false;
}

// Function to disable all buttons
void disableAllButtons(HWND hwnd)
{
	for (int i = 0; i < 9; ++i)
	{
		EnableWindow(hwndBoardButtons[i], FALSE);
	}
}

// Function to hide all game elements
void hideGameElements(HWND hwnd)
{
	for (int i = 0; i < 9; ++i)
	{
		ShowWindow(hwndBoardButtons[i], SW_HIDE);
	}
	ShowWindow(hwndStatusText, SW_HIDE);
}

// Function to show the winner message
void showWinnerMessage(const std::wstring &winnerMessage)
{
	SetWindowTextW(hwndWinMessage, winnerMessage.c_str());
	ShowWindow(hwndWinMessage, SW_SHOW);
}

// Window Procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, brush);
		EndPaint(hwnd, &ps);
		return 0;
	}

	case WM_COMMAND:
	{
		int buttonID = LOWORD(wParam);

		if (buttonID >= 1 && buttonID <= 9) // Check if it is a valid button ID
		{
			HWND currentButton = hwndBoardButtons[buttonID - 1];
			int row = (buttonID - 1) / 3;
			int col = (buttonID - 1) % 3;

			// Get current button text length
			int currentButtonTextLength = GetWindowTextLength(currentButton);
			if (currentButtonTextLength == 0) // If button is not clicked yet
			{
				wchar_t symbol = isPlayerOneTurn ? L'X' : L'O';
				buttonStates[row][col] = symbol; // Update button state

				// Set button text
				if (symbol == L'X')
				{
					SetWindowTextW(currentButton, L"X");
				}
				else
				{
					SetWindowTextW(currentButton, L"O");
				}

				// Check if there's a winner
				if (checkWinner())
				{
					std::wstring winnerMessage = L"Player " + std::wstring(isPlayerOneTurn ? L"1" : L"2") + L" wins!";
					hideGameElements(hwnd);						// Hide all game elements
					showWinnerMessage(winnerMessage); // Show winner message

					disableAllButtons(hwnd); // Disable all buttons to prevent further clicks
				}

				// Switch turn
				isPlayerOneTurn = !isPlayerOneTurn;
				SetWindowTextW(hwndStatusText, isPlayerOneTurn ? L"Bokem Turn (X)" : L"Ebri Baik Turn (O)");
			}
		}
		return 0;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
}

// Entry point for the application
void run_main_window(HINSTANCE hInstance, int nCmdShow)
{
	setupConsole();

	WNDCLASSW wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassW(&wc);

	HWND hwnd = CreateWindowExW(
			0,
			CLASS_NAME,
			L"Tic-Tac-Toe",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
			NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		return;
	}

	brush = CreateSolidBrush(RGB(255, 255, 255)); // Create a white brush for background color

	// Create status text for displaying player turn
	hwndStatusText = CreateWindowW(
			L"STATIC", L"Bokem Turn (X)",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			10, 10, 200, 20,
			hwnd, NULL, hInstance, NULL);

	// Create static text for displaying win message
	hwndWinMessage = CreateWindowW(
			L"STATIC", L"",
			WS_CHILD | WS_VISIBLE | SS_LEFT,
			40, 80, 200, 20,
			hwnd, NULL, hInstance, NULL);

	// Create board buttons and store handles
	for (int i = 0; i < 9; ++i)
	{
		hwndBoardButtons[i] = CreateWindowW(
				L"BUTTON", L"",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				10 + (i % 3) * 100, 50 + (i / 3) * 100, 100, 100,
				hwnd, (HMENU)(i + 1), hInstance, NULL);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	DeleteObject(brush); // Clean up the brush object
}
