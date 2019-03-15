#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <string>
#include <memory>
#include "enums.hpp"
#include "field.hpp"
#include <windows.h>


class Player // класс игрок
{
/***************************************************************************/

public:

/***************************************************************************/

	Player(std::string const & _name);

	virtual ~Player() {}

	Player(const Player &) = delete;

	Player & operator = (const Player &) = delete;

/***************************************************************************/

public:

/***************************************************************************/

	virtual State getState(POINT _point) const; // виртуальный метод дл€ доступа к состо€нию текущей €чейки

	ShotResult shooted(POINT _point); // реакци€ на выстрел

	void addPoint();

	void addStep();

	int getPoints() const;

	int getSteps() const;

	bool canShoot(int i, int j) const;

/***************************************************************************/

private:

/***************************************************************************/
	
	 const std::string m_name;

	 int m_points;

	 int m_steps;

	 std::unique_ptr<Field> m_shipField;

/***************************************************************************/

}; // class Player


/***************************************************************************/


inline int
Player::getPoints() const
{
	return m_points;
}


/***************************************************************************/

inline int
Player::getSteps() const
{
	return m_steps;
}

/***************************************************************************/


inline void 
Player::addPoint()
{
	m_points++;
}


/***************************************************************************/


inline void
Player::addStep()
{
	m_steps++;
}

/***************************************************************************/

inline bool
Player::canShoot(int i, int j) const 
{
	State state = m_shipField->getState(i, j);

	return ( state == State::Busy || state == State::Free );
}

/***************************************************************************/

#endif // __PLAYER_HPP__