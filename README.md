# BattleShips
BattleShip desktop win32 Api Game 

# Welcome to BattleShips project!

BattleShips is a simple game based on win32 API library written on C++ 11

# Rules 
In Battleships, an armada of battleships is hidden in a square grid of 10×10 small squares. For ease the ships are automatically by a game processor.
Each player has folling set of ships on his field
1. 1 four-decker ship
2. 2 three-decker ships
3. 3 two-decker ships
4. 4 one-decker ships

Each player makes its steps trying to destroy enemy's ships. There are some simple rules in armada placement which can help to discover their position:

1. Each cell occupied by a ship part cannot touch other ships 
2. Ships cannot overlap

# Start new game 
Start new game by pressing `Game`-> `Start new game` in menu

# Coloring 
For visual effect special coloring is used. Each cell may be in one of four states:
1. Unknown(white) - cell is ready for step.
2. Unavailable(black) - cell is unavailable for a making a step. That means that shooting at that coordinate is useless because ship cannot occupy this cell
3. Missed(pink) - there was a strike on that cell and it was a miss.
4. Hit(orange) - the strike was succesful and enemy's ship sunk
5. Ship down(red) - enemy's ship is drown.
6. Occupied(blue) - the cell is occupied by player's ship.


