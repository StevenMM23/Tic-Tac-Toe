
#include <iostream>

using namespace std;

struct game
{
	char table[3][3] = { {'1','2','3'},{'4','5','6'},{'7','8','9'} };
	int player = 1;
	int position =0;
};

bool isWinner()
{
	bool winner = false;
	game* board = new game;
	//check the rows

	//Primera fila
	if (board->table[0][0] == board->table[0][1] && board->table[0][1] == board->table[0][2]) winner = true;
	if (board->table[1][0] == board->table[1][1] && board->table[1][1] == board->table[1][2]) winner = true;
	if (board->table[2][0] == board->table[2][1] && board->table[2][1] == board->table[2][2]) winner = true;


	//Check the colums
	if (board->table[0][0] == board->table[1][0] && board->table[1][0] == board->table[2][0]) winner = true;
	if (board->table[1][0] == board->table[1][1] && board->table[1][1] == board->table[2][1]) winner = true;
	if (board->table[2][0] == board->table[2][1] && board->table[2][1] == board->table[2][2]) winner = true;

	//Check the diagonals
	if (board->table[0][0] == board->table[1][1] && board->table[1][1] == board->table[2][2]) winner = true;
	if (board->table[0][2] == board->table[1][1] && board->table[1][1] == board->table[2][0]) winner = true;


	return winner;
}

bool isFill()
{
	game* evaluate = new game;
	bool fill = true;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j ++)
		{
			if (evaluate->table[i][j] != 'X' || evaluate->table[i][j] != 'O') fill = false;
		}
	}

	return fill;
}
void showTable()
{
	game* game = new struct game;
	cout << "       |       |      \n";
	cout << "   " << game->table[0][0] << "   |   "<<game->table[0][1]<<"   |   "<<game->table[0][2]<<"\n";
	cout << "_______|_______|_______\n";
	cout << "       |       |      \n";
	cout << "   " << game->table[1][0] << "   |   " << game->table[1][1] << "   |   " << game->table[1][2]<<"\n";
	cout << "_______|_______|_______\n";
	cout << "       |       |      \n";
	cout << "   " << game->table[2][0] << "   |   " << game->table[2][1] << "   |   " << game->table[2][2] << "\n";
	cout << "       |       |      \n";

}
int main()
{
	showTable();
}


