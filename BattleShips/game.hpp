#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <memory>
#include "enums.hpp"
#include "player.hpp"
#include "ai_player.hpp"

class Game // класс "Игра"
{
public:

/***************************************************************************/

	Game();

	Game(const Game &) = delete;

	Game & operator = (const Game &) = delete;

/***************************************************************************/

	ShotResult shoot(int i, int k);

	void shoot();

	const Player & getAI() const;

	const Player & getPlayer() const;

	bool endGame() const;

/***************************************************************************/

private:
		
	std::unique_ptr < Player > m_player; // игрок 

	std::unique_ptr < AIPlayer > m_AI; // противник-компьютер

}; // class Game

/***************************************************************************/

inline const Player & 
Game::getPlayer() const
{
	return *m_player;
}



/***************************************************************************/


inline const Player &
Game::getAI() const
{
	return *m_AI;
}


/***************************************************************************/


inline bool
Game::endGame() const
{
	int player = m_player->getPoints();

	if (m_player->getPoints() == Globals::winPoinst )
		return true;
	else if (m_AI->getPoints() == Globals::winPoinst )
		return true;

	return false;
}


/***************************************************************************/


#endif // __GAME_HPP_