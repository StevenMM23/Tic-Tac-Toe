#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

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

void setConsoleColor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, fg + bg * 16);
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

vector<int> getValidSpaces(Game game)
{
	vector<int> validSpaces;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			if (game.table[i][j] == 0)
				validSpaces.push_back(i * 3 + j + 1);
		}
	}
	return validSpaces;
}


// 0 is no winner, 1 is X wins, 2 is O wins. -1 is tie
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

	// If there are no valid plays, return that it's a tie
	return getValidSpaces(board).empty() ? -1 : 0;
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
	setConsoleColor(1, 0);
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
			if (currentPlay == 1) {
				setConsoleColor(10, 0);
			}
			else if (currentPlay == 2) {
				setConsoleColor(12, 0);
			}
			else {
				setConsoleColor(15, 0);
			}
			cout << charToShow;
		}
	}
	setConsoleColor(15, 0);
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

int selectRandomSpace(Game game)
{
	vector<int> validSpaces = getValidSpaces(game);
	int randomIndex = rand() % validSpaces.size();
	return validSpaces[randomIndex];
}

// returns true if the tables are exactly equal
bool areTablesEqual(int firstTable[3][3], int secondTable[3][3]) {
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			if (firstTable[y][x] != secondTable[y][x]) {
				return false;
			}
		}
	}
	return true;
}

void copyTable(int fromTable[3][3], int toTable[3][3]) {
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			toTable[y][x] = fromTable[y][x];
		}
	}
}

int easyMachine(Game game) {
	vector<int> validSpaces = getValidSpaces(game);
	return validSpaces[rand() % validSpaces.size()];
}

// Gives a score to a given game state.
// Returns 2 if it is a guaranteed win on optimal plays
// Return 1 if it is a guaranteed tie on optimal plays
// Return -1 if it is a guaranteed loss on optimal plays
// Player number is which player is maximizing. 1 is X, 2 is O
int minimax(Game game, int depth = 0, bool isMaximizingPlayer = false, int playerNumber = 2) {
	// Base case. If the game is already over (tie, win, lose) return the score for that table.
	switch (isWinner(game))
	{
	case 2: 
		return (playerNumber - 1) ? 2 : -1; // Best score, optimal plays always win.
	case -1:
		return 1; // Average score, optimal plays always tie.
	case 1:
		return (playerNumber - 1) ? -1 : 2; // Bad score, optimal plays always lose.
	}
	// Take all valid spaces
	vector<int> validSpaces = getValidSpaces(game);
	int validSpacesAmount = validSpaces.size();
	// If we are the maximizing player, the default (first) play is the worst play.
	// If we are not the maximizing player, the default play is the best play.
	// This means, if we are the maximizing player, we are trying to get the best play.
	// If we are the minimizing player, we are trying to get the least bad play
	int bestVal = 3 * isMaximizingPlayer ? -1 : 1;

	// For each valid play
	for (int i = 0; i < validSpacesAmount; i++)
	{
		// Simulate a game
		Game tryGame = game;
		// Make that play
		makePlay(&tryGame, validSpaces[i]);
		// And get the score for that play for the following player.
		int value = minimax(tryGame, depth + 1, !isMaximizingPlayer, playerNumber);
		// If we are the maximizing player, get the maximum value of the score or what we have.
		// If we are de minimizing player, get the minimum value of the score or what we have.
		bestVal = isMaximizingPlayer ? max(bestVal, value) : min(bestVal, value);
	}
	return bestVal;
}

int masterMachine(Game game)
{
	vector<int> validSpaces = getValidSpaces(game);
	
	int bestMove = validSpaces[0];
	// Give the lowest possible score to the randomly selected move
	int bestMoveScore = -3;

	int validSpaceAmount = validSpaces.size();

	// Evaluate the score of each move
	// For each possible move
	for (int i = 0; i < validSpaceAmount; i++)
	{
		// On a "virtual" game
		Game tryGame = game;
		// Make that play
		makePlay(&tryGame, validSpaces[i]);
		int value = minimax(tryGame, 0, false, game.player);
		// If that play's score is better than the one we had
		if (value > bestMoveScore) {
			// Take that best move
			bestMove = validSpaces[i];
			// And assign the best score to the variable
			bestMoveScore = value;
		}
	} // This searches for the best possible move out of all possible
	// Best move is a winning move, second best is tying, worst is losing

	return bestMove; // Return the best possible move
}

// Gets a number from the user. Throws invalid_argumant if a letter is typed.
int getNumber() {
	string input;
	cin >> input;
	return stoi(input);
}

// Checks if a play is valid
bool isPlayValid(Game game, int slot)
{
	vector<int> validSpaces = getValidSpaces(game);
	int validSpaceNo = validSpaces.size();
	for (int i = 0; i < validSpaceNo; i++)
	{
		if (validSpaces[i] == slot)
			return true;
	}
	return false;
}

// Goes to a line (after the game grid)
void gotoLine(int line = 0) {
	setCursorPosition(0, line + 10);
}

// Clears a line (after the game grid)
void clearLine(int line = 0) {
	gotoLine(line);
	cout << "                                                        ";
	gotoLine(line);
}

// Writes a message (after the game grid)
void systemMessage(string message, int line = 0) {
	// Clear Line
	clearLine(line);
	gotoLine(line);
	cout << message;
	gotoLine(line);
}

// Gets a valid play from a player
int getPlayerPlay(Game game)
{
	systemMessage("En que casilla quiere jugar?");
	int selectedNumber;
	while (true) {
		try
		{
			clearLine(1);
			selectedNumber = getNumber();
			if (isPlayValid(game, selectedNumber)) {
				break;
			}
			else {
				systemMessage("Esa jugada no es valida");
			}
		}
		catch (invalid_argument e)
		{
			systemMessage("Por favor escriba un numero");
		}
	}
	return selectedNumber;
}

void playGame() {
	// Startup game
	Game game;
	// The game hasn't been won or tied.
	int hasWon = 0;
	// While the game hasn't been won or tied
	// Show a default system message (signaling the game is playing)
	game.player = 1; // Machine goes first

	while (!hasWon) {
		// Show the table
		showTable(game);
		switch (game.player)
		{
		case 1:
		{
			// Player goes
			setConsoleColor(2, 0);
			systemMessage("Hora de jugar!", -1);
			setConsoleColor(15, 0);
			int playerPlay = getPlayerPlay(game);
			makePlay(&game, playerPlay);
		}
		break;
		case 2:
		{
			// Machine goes
			setConsoleColor(4, 0);
			systemMessage("Pensando   ", -1);
			Sleep(250);
			systemMessage("Pensando.  ", -1);
			Sleep(250);
			systemMessage("Pensando.. ", -1);
			Sleep(250);
			systemMessage("Pensando...", -1);
			Sleep(250);
			setConsoleColor(15, 0);
			int machinePlay = masterMachine(game);
			makePlay(&game, machinePlay);
		}
		break;
		}
		// Update the win condition.
		hasWon = isWinner(game);
	}
	// Show the last table state
	showTable(game);

	// Show the end of game message
	switch (hasWon)
	{
	case -1:
		systemMessage(" Fue un empate");
		break;
	case 1:
		systemMessage(" Ha ganado el jugador de X");
		break;
	case 2:
		systemMessage(" Ha ganado el jugador de O");
		break;
	default:
		systemMessage(" Yo no se que te paso");
		systemMessage(" Coji un pote y te eploto");
	}
}

int main()
{
	// Startup random
	srand(time(NULL));

	while (true) {
		playGame();
		systemMessage(" Quiere jugar otra vez? S/N", 1);
		string usrin;
		cin >> usrin;
		if (usrin == "N") {
			break;
		}
		else if (usrin == "S") {
			continue;
		}
	}
}
