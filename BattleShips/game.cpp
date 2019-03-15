#include "game.hpp"
#include "resource.h"
#include <windows.h>
#include <cassert>

Game::Game()
{
	m_player = std::make_unique<Player>("John"); // �������� ���� ������� � ����������� � 1� (��� ������ ������������)
	
	Sleep(1000);
	
	m_AI = std::make_unique<AIPlayer>("Sam", *m_player);

	int first = rand() % 10;

	if (first % 2) // ��� �������� ������ ?
		shoot();	
}


/***************************************************************************/


void
Game::shoot() // �������� ���������
{	
	POINT shot = m_AI->makeStep();

	ShotResult result = m_player->shooted(shot);

	m_AI->insertShot(shot, result); // ��������� ����� � ��������� � ��������� ���������

	if (result != ShotResult::Missed)
		shoot();
}


/***************************************************************************/


ShotResult
Game::shoot(int i, int j) // �������� �����
{
	ShotResult result = m_AI->shooted(POINT{ i, j });

	if (result == ShotResult::TryAgain)
		return ShotResult::TryAgain;

	m_player->addStep();

	if ( result != ShotResult::Missed ) 
	{
		m_player->addPoint();
		return ShotResult::Damaged;
	}

	return ShotResult::Missed;
}


/***************************************************************************/
