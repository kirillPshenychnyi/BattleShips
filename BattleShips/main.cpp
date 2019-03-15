#include <windows.h>         // подключение библиотеки с функциями API
#include "resource.h"
#include "game.hpp"
#include "globals.hpp"
#include "enums.hpp"
#include "player.hpp"
#include <cassert>
#include <string>

// Глобальные переменные:
HINSTANCE hInst; 	// Указатель приложения
int g_nCmdShow;
// Предварительное описание функций

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Основная программа 
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	MSG msg;

	// Регистрация класса окна 
	MyRegisterClass(hInstance);

	// Создание окна приложения
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	// Цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

//  FUNCTION: MyRegisterClass()
//  Регистрирует класс окна 

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// стиль окна
	wcex.lpfnWndProc = (WNDPROC)WndProc; // оконная процедура
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;		// указатель приложения
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDB_PNG1));		// определение иконки
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);    // определение курсора
	wcex.hbrBackground = GetSysColorBrush(COLOR_WINDOW);   // установка фона
	wcex.lpszMenuName = NULL;		// определение меню
	wcex.lpszClassName = Globals::szWindowClass;	// имя класса
	wcex.hIconSm = NULL;

	return RegisterClassEx(&wcex); // регистрация класса окна
}

// FUNCTION: InitInstance(HANDLE, int)
// Создает окно приложения и сохраняет указатель приложения в переменной hInst

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	g_nCmdShow = nCmdShow;

	hInst = hInstance; // сохраняет указатель приложения в переменной hInst
	HMENU menu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));

	hWnd = CreateWindow(
		Globals::szWindowClass, // имя класса окна
		Globals::szTitle,   // имя приложения
		WS_OVERLAPPEDWINDOW, // стиль окна
		CW_USEDEFAULT,	// положение по Х
		CW_USEDEFAULT, 	// положение по Y
		CW_USEDEFAULT,    // размер по Х
		CW_USEDEFAULT,    // размер по Y
		NULL,	// описатель родительского окна
		menu,       // описатель меню окна
		hInstance,  // указатель приложения
		NULL);     // параметры создания.

	if (!hWnd) // Если окно не создалось, функция возвращает FALSE
	{
		return FALSE;
	}
	ShowWindow(hWnd, nCmdShow);		// Показать окно
	UpdateWindow(hWnd);			// Обновить окно
	return TRUE;				//Успешное завершение функции
}

//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//  Оконная процедура. Принимает и обрабатывает все сообщения, приходящие в приложение

void drawLetters(HDC hdc, int x_offset, int y_offset) // функция отрисовки системы координат в соответствии со смщением 
{
	if (!Globals::shouldDisplayLetter)
		return;

	for (int i = 0; i < Globals::fieldSize; i++)
	{
		TextOut(
			hdc
			, (i + x_offset) * Globals::width + Globals::width / 2
			, Globals::width / 2
			, Globals::letters[i], 2
			);

		TextOut(
			hdc
			, (x_offset - 1) * Globals::width
			, (i + y_offset) * Globals::width + Globals::width / 2
			, Globals::buf
			, wsprintf(Globals::buf, "%d", i + 1));
	}
}

void drawField(HDC hdc, const Player & _player, int x_offset, int y_offset, int _x, int _y) // отрисовка поля в соответствии со смещением 
{
	static HBRUSH brush = CreateSolidBrush(RGB(150, 166, 255)); // кисть для фона
	static HBRUSH damaged = CreateSolidBrush(RGB(255, 127, 80)); // кисть для поврежденного корабля
	static HBRUSH missed = CreateSolidBrush(RGB(255, 165, 198)); // кисть для промаха
	static HBRUSH dead = CreateSolidBrush(RGB(255, 0, 0)); // кисть для потопленого судна
	static HBRUSH nonValid = CreateSolidBrush(RGB(0, 0, 0));
	static HPEN simple = CreatePen(PS_SOLID, 1, RGB(150, 166, 255)); // перо для ячеек
	static HPEN selected = CreatePen(PS_SOLID, 1, RGB(0, 255, 255)); // перо для выбранной ячейки

	for (int i = 0; i < Globals::fieldSize; i++)
		for (int j = 0; j < Globals::fieldSize; j++)
		{
			POINT point{ i,j };
			State state = _player.getState(point);

			if (state == State::Busy) // выбираем кисть в соответсвии с состоянием 
				SelectObject(hdc, brush);
			else if (state == State::Damaged)
				SelectObject(hdc, damaged);
			else if (state == State::Missed)
				SelectObject(hdc, missed);
			else if (state == State::Dead)
				SelectObject(hdc, dead);
			else if (state == State::NonValid)
				SelectObject(hdc, nonValid);
			else
				SelectObject(hdc, GetStockObject(NULL_BRUSH));

			if (j == _x && i == _y)
				SelectObject(hdc, selected);
			else
				SelectObject(hdc, simple);

			Rectangle(
				hdc
				, (j + x_offset) *   Globals::width + Globals::margin
				, (i + y_offset) *   Globals::width + Globals::margin
				, (j + x_offset + 1) * Globals::width
				, (i + y_offset + 1) * Globals::width);
		}

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	SelectObject(hdc, GetStockObject(NULL_PEN));
	
	struct  Wrapper
	{
		~Wrapper()
		{
			DeleteObject(brush);
			DeleteObject(damaged);
			DeleteObject(missed);
			DeleteObject(dead);
			DeleteObject(simple);
			DeleteObject(selected);
		}
	};

	static Wrapper s_wrapper;

}

void displayDialog(HWND hWnd, int _steps, int _points, const char * _message)
{
	char steps[10];
	char accuracy[10];

	double _accuracy = (double(_points) / _steps) * 100.0;

	sprintf(steps, "%d", _steps);
	sprintf(accuracy, "%.2lf", _accuracy);

	std::string result = _message;

	result += '\n';
	result += "Steps: ";
	result += steps;
	result += '\n';

	result += "Accuracy: ";
	result += accuracy;

	MessageBox(hWnd, result.c_str(), "Game's over", MB_OK);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rt;

	static int cWidth;
	static int cHeight;
	static int x, y;

	static std::unique_ptr<Game> game;

	/***************************************************************************/

	switch (message)
	{
	case WM_CREATE: // Сообщение приходит при создании окна

/***************************************************************************/

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWGAME:
			if (game)
				game.reset();
			game = std::make_unique<Game>(); // создаём новую игру 
			InvalidateRect(hWnd, NULL, TRUE); // перерисовывем рабочую область 
			break;
		default:
			break;
		}
		break;
	case WM_SIZE:

		/***************************************************************************/

		Globals::shouldDisplayLetter = TRUE; // при изменение размера окна - перерисуем систему координат 

		Globals::width = LOWORD(lParam) / Globals::fieldSize / Globals::sizeCoefficient; // новая ширина

		break;

		/***************************************************************************/

	case WM_PAINT:  // Перерисовать окно

/***************************************************************************/
		if (!game) // если игра не запущена - ничего не происходит
			break;
		hdc = BeginPaint(hWnd, &ps);	// Начать графический вывод
		GetClientRect(hWnd, &rt); // Область окна для рисования

		drawLetters(hdc, Globals::x_offset, Globals::y_offset);
		drawLetters(hdc, 1, 1);
		Globals::shouldDisplayLetter = FALSE;

		drawField(hdc, game->getPlayer(), 1, 1, -1, -1); // отрисовать поле игрока 
		drawField(hdc, game->getAI(), Globals::x_offset, Globals::y_offset, x, y); // отрисовать поле противника 

		EndPaint(hWnd, &ps);	// Закончить графический вывод
		break;

		/***************************************************************************/

	case WM_LBUTTONDOWN:

		/***************************************************************************/
				/*Зацикливание ходов*/

		ShotResult result;
		x = LOWORD(lParam) / Globals::width - Globals::x_offset;
		y = HIWORD(lParam) / Globals::width - Globals::y_offset;

		if (x >= 0 && y >= 0
			&&
			x < Globals::fieldSize  && y < Globals::fieldSize)
		{
			result = game->shoot(y, x);
			InvalidateRect(hWnd, NULL, FALSE);
			if (result == ShotResult::Damaged || result == ShotResult::Killed)
			{
				if (game->endGame())
				{

					LoadString(hInst, ID_WIN, Globals::buf, 50);

					auto const & player = game->getPlayer();

					displayDialog(
						hWnd
						, player.getSteps()
						, player.getPoints(),
						Globals::buf);
					game.reset();
					InvalidateRect(hWnd, NULL, TRUE);
				}
				return 0;
			}

			else if (result == ShotResult::TryAgain)
				break;
			else
			{
				game->shoot();
				InvalidateRect(hWnd, NULL, FALSE);
				if (game->endGame())
				{
					LoadString(hInst, ID_LOSE, Globals::buf, 50);

					auto const & player = game->getPlayer();

					displayDialog(
						hWnd
						, player.getSteps()
						, player.getPoints(),
						Globals::buf);

					game.reset();
					InvalidateRect(hWnd, NULL, TRUE);
					return 0;
				}
			}
		}
		break;

		/***************************************************************************/

	case WM_MOUSEMOVE:

		/***************************************************************************/
				/*Передвижение мышки по полю*/

		x = LOWORD(lParam) / Globals::width - Globals::x_offset;
		y = HIWORD(lParam) / Globals::width - Globals::y_offset;

		if (x > Globals::fieldSize  && y > Globals::fieldSize)
		{
			x = -1;
			y = -1;
		}

		InvalidateRect(hWnd, NULL, FALSE);
		break;
		/***************************************************************************/

	case WM_DESTROY: // Завершение работы

/***************************************************************************/

		PostQuitMessage(0);
		break;

		/***************************************************************************/

	default:
		// Обработка сообщений, которые не обработаны пользователем
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}