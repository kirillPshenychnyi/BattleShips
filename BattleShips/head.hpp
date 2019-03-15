#ifndef __HEAD_HPP__
#define __HEAD_HPP__
#include "cell.hpp"
#include <memory>

struct Head
	: public Cell // класс "Голова судна". Наследник ячейки
{

/***************************************************************************/
	
	Head(int _x, int _y, ShipType _type, Direction _direction)
		:	m_x(_x)
		,	m_y(_y)
		,	m_type(_type)
		,	m_direction(_direction)
	{}

/***************************************************************************/

	const ShipType m_type;

	const Direction m_direction;

	const int m_x;

	const int m_y;

/***************************************************************************/

};

#endif // __HEAD_HPP__