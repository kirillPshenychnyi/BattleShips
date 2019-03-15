#ifndef __FIELD_HPP__
#define __FIELD_HPP__

#include "enums.hpp"
#include "cell.hpp"

struct Point // структура "Точка"
{
	int m_i; // координаты

	int m_j; 

	Direction m_direction; // направление 

};

class Field // класс "Поле"
{

public:

/*****************************************************************************/
	
	Field();

	void placeShip(ShipType _type);

	State getState(int i, int j) const;

	State acceptShoot(int i, int j);
	
/*****************************************************************************/
	
	Field(const Field &) = delete;

	Field & operator = (const Field &) = delete;

/*****************************************************************************/

private:

/*****************************************************************************/

	void placeHorizontal(ShipType _type, Point _point);

	void placeVertical(ShipType _type, Point _point);
	
	bool isFree(int i, int j) const;

	bool canBuild(ShipType _type, Point _point) const;

	Point getPoint(ShipType _type) const;

	bool isDead(const Cell & _cell) const;

	void dead(Cell & _cell);

	void invalidPoint(int x, int y);

	void invalidHorizontal( int x, int y );

	void invalidVertical( int x, int y );

/*****************************************************************************/

public:

	static const int ms_fieldSize = 10;
	
private:

	Cell::CellPtr m_field[ms_fieldSize][ms_fieldSize];
	
};


/*****************************************************************************/


inline State
Field::getState(int i, int j) const 
{
	return m_field[i][j]->m_state;
}


/***************************************************************************/


#endif // __FIELD_HPP__