#include "ai_player.hpp"
#include "field.hpp"
#include "globals.hpp"
#include <ctime>
#include <cassert>

AIPlayer::AIPlayer(
		std::string const & _name
	,	Player const & _enemy)
	:	Player(_name)
	,	m_enemy(_enemy)
{
	time_t current;
	time( & current);
	srand(current);
}


/***************************************************************************/


POINT
AIPlayer::getPoint() // метод получения новой точки для выстрела 
{	
	int x  = rand() % Globals::fieldSize;
	int y = rand() % Globals::fieldSize;

	while (true)
	{
		if ( m_enemy.canShoot(x,y) )
			return POINT{ x,y };
		else 
			{
				x = rand() % Globals::fieldSize;
				y = rand() % Globals::fieldSize;
			}
	}
}


POINT 
AIPlayer::shootOnRecomendation(int _pos)
{
	assert(_pos >= 0);
	int size = m_shots.size();
	POINT point = m_shots.at(_pos).first;

	int i = point.x;
	int j = point.y;

	if ( i > 0 && m_enemy.canShoot(i-1, j))
		return POINT{ --i,j };
	else if ( i < Globals::fieldSize - 1 && m_enemy.canShoot(i+1, j) )
		return POINT{ ++i, j };
	else if ( j < Globals::fieldSize - 1 && m_enemy.canShoot(i, j+1) )
		return POINT{ i, ++j };
	else if( j > 0 && m_enemy.canShoot(i, j-1))
		return POINT{ i, --j };
	else 
	{
		int size = m_shots.size();
		shootOnRecomendation(_pos - 1);
	}

}


/***************************************************************************/


POINT
AIPlayer::makeStep() // сделать очередной шаг 
{
	if (m_shots.empty())
		return getPoint();

	ShotResult result = m_shots.at(m_shots.size() - 1).second;
	POINT point = m_shots.at(m_shots.size() - 1).first;

	if (result == ShotResult::Killed)
		return getPoint();

	point = shootOnRecomendation(m_shots.size() - 1);

	return point;
}


/***************************************************************************/


void  
AIPlayer::insertShot(POINT _point,ShotResult _shot) // вставка очередного выстрела
{
	int i = _point.x;
	int j = _point.y;

	if (_shot == ShotResult::Damaged || _shot == ShotResult::Killed) // если попал или убил - исключим соседние по диагонали точки и добавим себе очков
	{
		m_shots.push_back(std::make_pair(_point, _shot));
		addPoint();
	}
}


/***************************************************************************/

State 
AIPlayer::getState(POINT _point) const // замещенный метод из базового класса для отображения ячейки 
{
	State state = Player::getState(_point);
	return (state != State::Busy) ? state : State::Unknown;
	
}


/***************************************************************************/


