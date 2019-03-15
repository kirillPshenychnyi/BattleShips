#include "player.hpp"

/***************************************************************************/

Player::Player(std::string const & _name)
	: m_name(_name)
	, m_points(0)
{
	m_shipField = std::make_unique< Field >();
	
	m_shipField->placeShip(ShipType::BattleShip);

	for (int i = 0; i < 2; i++)
			m_shipField->placeShip(ShipType::Cruiser);
	
	for (int i = 0; i < 3; i++)
			m_shipField->placeShip(ShipType::Destroyer);

	for (int i = 0; i < 4; i++)
			m_shipField->placeShip(ShipType::Submarine);

}


/***************************************************************************/


State 
Player::getState(POINT _point) const
{
	return (m_shipField->getState(_point.x, _point.y));
}


/***************************************************************************/


ShotResult 
Player::shooted(POINT _point)
{
	State state = m_shipField->acceptShoot(_point.x, _point.y);

	switch (state)
	{
	case State::Missed:
		return ShotResult::Missed;
	case State::Damaged:
		return ShotResult::Damaged;
	case State::Dead:
		return ShotResult::Killed;
	default:
		return ShotResult::TryAgain;
	}

}

/***************************************************************************/
