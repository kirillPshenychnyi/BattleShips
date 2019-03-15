#ifndef __GLOBALS_HPP__
#define __GLOBALS_HPP__
// файл с глобальными константами
namespace Globals 
{
	static const char * letters[10] = { "a", "b", "c", "d", "e", "f", "g", "h","i","j" };
	static const char * szWindowClass = "QWERTY";
	static const char * szTitle = "BattleShips";
	
	static char buf[50];
	static char buf_2[10];

	static const int fieldSize = 10;

	static const int winPoinst = 20;

	static const int sizeCoefficient = 3;
	static const int margin = 1;
	static const int x_offset = 20;
	static const int y_offset = 1;
	static bool shouldDisplayLetter;
	static int width;
	
} // namespace Globals

#endif // __GLOBALS_HPP__