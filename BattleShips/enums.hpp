#ifndef __ENUMS_HPP__
#define __ENUMS_HPP__ // ������������ ���� � ������������� ��� ������������� �����

/***************************************************************************/

	enum class ShotResult { Missed, Damaged,Killed, TryAgain };

	enum class ShipType { Submarine = 1, Destroyer, Cruiser, BattleShip };

	enum class Direction { North, South, East, West };

	enum class State { Free, Busy, Missed, Damaged, Dead, Unknown, NonValid };

/***************************************************************************/

#endif