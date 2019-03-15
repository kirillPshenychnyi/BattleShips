#ifndef __AI_PLAYER_HPP__
#define __AI_PLAYER_HPP__

#include "player.hpp"
#include "enums.hpp"
#include "globals.hpp"
#include <unordered_set>
#include <vector>
#include <windows.h>

/***************************************************************************/

class Game;


/***************************************************************************/


class AIPlayer // класс противника - компьютера : наследник класса "Игрок"
	:	public Player
{
/***************************************************************************/
	
	typedef std::pair<POINT, ShotResult> shot;

/***************************************************************************/

public:

/***************************************************************************/

	AIPlayer( 
		std::string const & _name
	,	Player const & _enemy );

	AIPlayer(const AIPlayer &) = delete;

	AIPlayer & operator = (const AIPlayer &) = delete;

/***************************************************************************/

	State getState(POINT _point) const ;
	
	POINT makeStep();
	
	void insertShot(POINT _shot,ShotResult _res);

/***************************************************************************/

private:

	POINT getPoint();

	POINT shootOnRecomendation(int _pos);

/***************************************************************************/

private:

/***************************************************************************/
	
	Player const & m_enemy;

	std::vector<shot> m_shots;

/***************************************************************************/
};


#endif // __AI_PLAYER_HPP__