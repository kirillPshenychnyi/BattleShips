
#include <ctime>
#include <cstdlib>
#include <cassert>
#include "head.hpp"
#include "field.hpp"
#include "globals.hpp"


Field::Field()
{
	time_t current; // �������������. 
	time(&current);
	srand((unsigned int)current);

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			m_field[i][j] = std::make_unique <Cell>(); // ��������� ���� ������� ��������
}


/***************************************************************************/


void 
Field::placeShip(ShipType _type) // ��������� ������� � ����������� �� ����������� 
{
	Point point = getPoint(_type);
	
	if (point.m_direction == Direction::North || point.m_direction == Direction::South)
		placeVertical(_type, point); // ...�����������
	else 
		placeHorizontal(_type, point); // ...�������������

}


/*****************************************************************************/


void 
Field::placeHorizontal(ShipType _type, Point _point) // ���������� �������������
{
	int x = _point.m_i;
	int y = _point.m_j;
	
	Head * head = new Head(x,y,_type, _point.m_direction); // ������ "����������� ������"
	head->m_state = State::Busy;
	
	m_field[x][y].reset(); // ���������� ���������� ���������
	m_field[x][y] = std::unique_ptr<Cell>(head); // ������ ������ ���� "������"

	if (_point.m_direction == Direction::West) // � ����������� �� ����������� ��������� �������
		for (int i = 0; i < static_cast<int>(_type); i++)
		{
			m_field[x][y]->m_state = State::Busy;
			m_field[x][y]->m_head = head;
			y--;
		}
	else
		for (int i = 0; i < static_cast<int>(_type); i++)
		{
			m_field[x][y]->m_state = State::Busy;
			m_field[x][y]->m_head = head;
			y++;
		}

}


/*****************************************************************************/


void 
Field::placeVertical(ShipType _type, Point _point)
{
	int x = _point.m_i;
	int y = _point.m_j;

	Head * head = new Head(x, y, _type, _point.m_direction);
	head->m_state = State::Busy;

	m_field[x][y].reset();
	m_field[x][y] = std::unique_ptr<Cell>(head);

	if (_point.m_direction == Direction::North)
		for (int i = 0; i < static_cast<int>(_type); i++)
		{
			m_field[x][y]->m_state = State::Busy;
			m_field[x][y]->m_head = head;
			x--;
		}
	else 
		for (int i = 0; i < static_cast<int>(_type); i++)
		{
			m_field[x][y]->m_state = State::Busy;
			m_field[x][y]->m_head = head;
			x++;
		}
	
}


/*****************************************************************************/


bool 
Field::isFree(int i, int j) const // �������� �������� �������� �� ������
{
	if (m_field[i][j]->m_state != State::Free)
		return false;

	if (i > 0) // ���� ��������� � �� ������ ������ ������ 
	{
		if (m_field[i - 1][j]->m_state != State::Free)
			return false;

		if (j > 0) // �������� ����� �� ���������, �� ����� ��������� ������ � ������� ���� 
			if (m_field[i][j - 1]->m_state != State::Free
				|| 
				m_field[i - 1][j - 1]->m_state != State::Free)
				return false;
		if (j < 9) // �������� ������ �� ��������� 
			if (m_field[i][j + 1]->m_state != State::Free
				|| 
				m_field[i - 1][j + 1]->m_state != State::Free)
				return false;
	}

	if (i < 9) // ����������� �������� ��� ������� ���� 
	{
		if (m_field[i + 1][j]->m_state != State::Free)
			return false;
		if (j > 0)
			if (m_field[i + 1][j - 1]->m_state != State::Free
				|| 
				m_field[i][j - 1]->m_state != State::Free)
				return false;
		if (j < 9)
			if (m_field[i + 1][j + 1]->m_state != State::Free
				|| 
				m_field[i][j + 1]->m_state != State::Free)
				return false;
	}

	return true;
}


/*****************************************************************************/


bool 
Field::canBuild(ShipType _type, Point _point) const // �������� �������� ���������� �������� ���� ������� � ������� �����������
{
	int i = _point.m_i;
	int j = _point.m_j;

	int size = static_cast<int>(_type);

	if (_point.m_direction == Direction::North)
	{
		if (i - size + 1 < 0)
			return false;
		for (int k = 0; k < size; k++)
			if (!isFree(i--, j))
				return false;
		return true;
	}

	else if (_point.m_direction == Direction::South)
	{
		if (i + size - 1 > 9)
			return false;
		for (int k = 0; k < size; k++)
			if (!isFree(i++, j))
				return false;
		return true;
	}

	else if (_point.m_direction == Direction::West)
	{
		if (j - size + 1 < 0)
			return false;
		for (int k = 0; k < size; k++)
			if (!isFree(i, j--))
				return false;
		return true;
	}
	else
	{
		if (j + size - 1 > 9)
			return false;
		for (int k = 0; k < size; k++)
			if (!isFree(i, j++))
				return false;
		return true;
	}
	
}


/*****************************************************************************/


Point 
Field::getPoint(ShipType _type) const // �������� ��������� ����� �� ������� ����� ������� �������
{
	Point point;
	while (true)
	{
		point.m_direction = static_cast<Direction> (rand() % 4);
		point.m_i = rand() % 10;
		point.m_j = rand() % 10;

		if (canBuild(_type, point)) 
			return point;
	}
}


/***************************************************************************/


State
Field::acceptShoot(int i, int j) // ������� �� �������
{
	assert(i >= 0 && i < 10 && j >= 0 && j < 10);

	State current = m_field[i][j]->m_state;
	
	switch (current)
	{
	case State::Free:
		m_field[i][j]->m_state = State::Missed; // ���� ������ ������ - ������
		return State::Missed;

	case State::Busy:
		m_field[i][j]->m_state = State::Damaged; // ��� ��������� �������� ������ �������
		invalidPoint(i, j);
		if (isDead(*m_field[i][j]))
		{
			dead(*m_field[i][j]); 
			return State::Dead; // ���� ���������� ������ ������� ������, �� ������� ��������
		}
		return State::Damaged; // ����� - �����

	default:
		return State::NonValid; // ������� � ��� ��������� ������ 
	
	}
}


/***************************************************************************/

bool
Field::isDead(const Cell & _cell) const // �������� �������� ������� ������� (�������� �� ?)
{
	Head * head = _cell.m_head;
	Direction direction = head->m_direction;
	int size = static_cast<int>(head->m_type);
	int x = head->m_x;
	int y = head->m_y;

	if (direction == Direction::North)
	{
		for (int i = 0; i < size; i++)
			if (m_field[x--][y]->m_state != State::Damaged)
				return false;
	}
	else if (direction == Direction::South)
	{
		for (int i = 0; i < size; i++)
			if (m_field[x++][y]->m_state != State::Damaged)
				return false;
	}
	
	else if (direction == Direction::East)
	{
		for (int i = 0; i < size; i++)
			if (m_field[x][y++]->m_state != State::Damaged)
				return false;
	}
	
	else
	{
		for (int i = 0; i < size; i++)
			if (m_field[x][y--]->m_state != State::Damaged)
				return false;
	}

	return true;
}


/***************************************************************************/


void 
Field::dead(Cell & _cell) // ��������� ����� ������� ������� �� ���������� 
{
	Head * head = _cell.m_head;
	Direction direction = head->m_direction;
	int size = static_cast<int>(head->m_type);
	int x = head->m_x;
	int y = head->m_y;

	invalidHorizontal(x, y);
	invalidVertical(x, y);

	if (direction == Direction::North)
	{
		if( x < Globals::fieldSize - 1 )
			m_field[x + 1][y]->m_state = State::NonValid;
		
		for (int i = 0; i < size; i++)
			m_field[x--][y]->m_state = State::Dead;

		if( x > 0 )
			m_field[x][y]->m_state = State::NonValid;
	}
	else if (direction == Direction::South)
	{
		if (x > 0)
			m_field[x - 1][y]->m_state = State::NonValid;

		for (int i = 0; i < size; i++)
			m_field[x++][y]->m_state = State::Dead;

		if ( x < Globals::fieldSize - 1 )
			m_field[x][y]->m_state = State::NonValid;
	}

	else if (direction == Direction::East)
	{
		if (y > 0)
			m_field[x][y - 1]->m_state = State::NonValid;

		for (int i = 0; i < size; i++)
			m_field[x][y++]->m_state = State::Dead;

		if (y < Globals::fieldSize - 1)
			m_field[x][y]->m_state = State::NonValid;
	}

	else
	{
		if (y < Globals::fieldSize - 1)
			m_field[x][y + 1]->m_state = State::NonValid;

		for (int i = 0; i < size; i++)
			m_field[x][y--]->m_state = State::Dead;

		if (y > 0)
			m_field[x][y]->m_state = State::NonValid;
	}

}


/***************************************************************************/


void 
Field::invalidPoint( int i, int j )
{
	if (i > 0)
	{
		if (j > 0)
			m_field[i-1][j-1]->m_state = State::NonValid;

		if (j < Globals::fieldSize - 1)
			m_field[i - 1][j + 1]->m_state = State::NonValid;
	}

	if (i < Globals::fieldSize - 1)
	{
		if (j > 0)
			m_field[i + 1][j - 1]->m_state = State::NonValid;

		if (j < Globals::fieldSize - 1)
			m_field[i + 1][j + 1]->m_state = State::NonValid;
	}
}


/***************************************************************************/


void 
Field::invalidVertical(int x, int y)
{	
	if (x < Globals::fieldSize - 1)
		m_field[x + 1][y]->m_state = State::NonValid;
	if (x > 0)
		m_field[x - 1][y]->m_state = State::NonValid;
}


/***************************************************************************/


void 
Field::invalidHorizontal(int x, int y)
{
	if (y < Globals::fieldSize - 1)
		m_field[x][y + 1]->m_state = State::NonValid;
	if (y > 0)
		m_field[x][y - 1]->m_state = State::NonValid;
}


