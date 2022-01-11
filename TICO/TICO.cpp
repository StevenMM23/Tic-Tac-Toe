#include <iostream>
#include <windows.h>

using namespace std;

// Represents a game table and turn
struct Game
{
	int table[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
	int player = 1;		// 1 is X, 2 is O
};

// Toggles the current player
// If player was 1, make it 2
// If player was anything else (probably 2), make it 1
void togglePlayer(Game* game) {
	game->player = (game->player == 1) ? 2 : 1;
}

// Moves the console cursor position to an x, y coordinate
void setCursorPosition(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x, y };
	SetConsoleCursorPosition(hConsole, coord);
}

// Translates a player number into a char
char playToChar(int player) {
	char exitChar;
	switch (player)
	{
	case 0:
		exitChar = ' ';
		break;
	case 1:
		exitChar = 'X';
		break;
	case 2:
		exitChar = 'O';
		break;
	default:
		exitChar = 'E';
	}
	return exitChar;
}

// 0 is no winner, 1 is X wins, 2 is O wins.
int isWinner(Game board)
{
	for (int i = 0; i < 3; i++)
	{
		// Check Rows
		if (board.table[i][0] == board.table[i][1] && board.table[i][1] == board.table[i][2]) return board.table[i][0];
		// Check Columns
		if (board.table[0][i] == board.table[1][i] && board.table[1][i] == board.table[2][i]) return board.table[0][i];

	}
	// Check Diagonals
	if ((board.table[0][0] == board.table[1][1] && board.table[1][1] == board.table[2][2])
		|| board.table[0][2] == board.table[1][1] && board.table[1][1] == board.table[2][0]) return board.table[1][1];

	return 0;
}

// Returns 0 if empty, returns 1 if X, return 2 if O
int getField(Game game, int x, int y)
{
	return game.table[x][y];
}

// Shows the table showing slot numbers where slots are empty
// Always shows table at the top of the screen
void showTable(Game game)
{
	setCursorPosition(0, 0);
	cout << "       |       |       \n";
	cout << "       |       |       \n";
	cout << "_______|_______|_______\n";
	cout << "       |       |       \n";
	cout << "       |       |       \n";
	cout << "_______|_______|_______\n";
	cout << "       |       |       \n";
	cout << "       |       |       \n";
	cout << "       |       |       \n";
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			int currentPlay = game.table[y][x];
			char charToShow = currentPlay ? playToChar(currentPlay) : ((y * 3 + x + 1) + 48);
			setCursorPosition(3 + 8 * x, 1 + 3 * y);
			cout << charToShow;
		}
	}
	setCursorPosition(0,10);
}

// Tries to make a play with the current game player
// If the play was possible the player is alternated
// (if X then O, if O then X)
// returns true if the play could be made else return false
bool makePlay(Game* game, int slot)
{
	int row = (slot - 1) / 3;
	int col = (slot - 1) % 3;

	bool playPossible = !getField(*game, row, col);
	if (playPossible) {
		game->table[row][col] = game->player;
		togglePlayer(game);
	}
	return playPossible;
}

int main()
{
	Game game;
	makePlay(&game, 5);
	makePlay(&game, 6);
	showTable(game);
}
