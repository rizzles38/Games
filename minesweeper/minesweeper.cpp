#include <stdio.h>
#include <iostream>
#include <vector>
#include <time.h>

int M = 10;
int N = 10;
int NMINES = 5;

bool gameOver = false;
bool lost = false;
int empty = M * N - NMINES;
int opened = 0;
int marked = 0;

using namespace std;

enum class Status {
	UNFLAGGED,
	FLAGGED,
	OPENED
};

class Cell {
public: 
	Cell();
	void display();
	void reveal(vector<vector<Cell>>& board, int x, int y); 
	int getValue();
	Status status; // maybe make this an enum?  // 0 == unflagged, 1 == flagged, 2 == opened
	int value;

};

Cell::Cell() {
	value = 0;
	status = Status::UNFLAGGED;
}

void Cell::display() {
	cout << " ";
	switch (status) {
		case Status::UNFLAGGED:
			if (gameOver && lost && (value == 9)) {
				cout << "*";
			} else {
				cout << "?";
			}
			break;
		case Status::FLAGGED:
			cout << "!";
			break;
		case Status::OPENED:
			if (value == 9) {
				cout << "*";
			}
			else if (value == 0) {
				cout << "_";
			}
			else {
				cout << value;
			}
			break;
		default:
			cout << "Invalid status" << endl;
			break;
	}

}

void Cell::reveal(vector<vector<Cell>>& board, int x, int y) {
	for (int i = x-1; i < x+2; i++) {
		for (int j = y-1; j < y+2; j++) {
			if (i < 0 || i >= M || j < 0 || j >= N ) {
				continue;
			}

			else {
				if (board[i][j].value !=9 && board[i][j].status != Status::OPENED) {
					cout << "opening coords: " << i << ", " << j << endl;
					board[i][j].status = Status::OPENED;
					opened++;
					if(board[i][j].value == 0) {
						board[i][j].reveal(board, i, j);
					}
				}
			}
		}
	}
}

// display board
void displayBoard(vector<vector<Cell>>& board) {
	cout << " ";
	for (int i = 0; i < board.size(); i++) {
		cout << " " << i;
	}
	cout << endl;
	for (int i = 0; i < board.size(); i++) {
		cout << i;
		for (int j = 0; j < board[i].size(); j++) {
			board[i][j].display();
		}
		cout << endl;
	}
}

void incrementNeighbors(vector<vector<Cell>>& board, int x, int y) {
	for (int i = x-1; i < x+2; i++) {
		for (int j = y-1; j < y+2; j++) {
			if (i < 0 || i >= M || j < 0 || j >= N ) {
				cout << "coords: " << i << ", " << j << " are not on the board" << endl;
				continue;
			}

			else {
				cout << "updating coords: " << i << ", " << j << endl;
				if (board[i][j].value != 9) {
				board[i][j].value = board[i][j].value + 1;
				}
			}
		}
	}
}

void populateBoard(vector<vector<Cell>>& board) {
	int count = NMINES;
	while (count > 0) {
		cout << "Inside the loop, populating" << endl;
		//choose x coord
		int x = rand()%M;
		//choose y coord
		int y = rand()%N;
		//if there is already a mine there, try again;
		if(board[x][y].value != 9) { // already a mine
			cout << "populating " << x << ", " << y << " with a mine" << endl;
			board[x][y].value = 9;
			incrementNeighbors(board, x, y);
			// increment value of all neighbors
			count--;
		}
	}
}

// process move
void processMove(vector<vector<Cell>>& board, char move, int x, int y) {
	if (move == 'O' || move == 'o') {
		Status status = board[x][y].status;
		if (status == Status::FLAGGED || status == Status::OPENED) {
			return;
		}
		else if (status == Status::UNFLAGGED) {
			// open the cell
			board[x][y].status = Status::OPENED;
			opened++;
			if (board[x][y].value == 9) {
				gameOver = true;
				return;
			}
			if (board[x][y].value == 0) {
				board[x][y].reveal(board, x, y);  // this seems like poor design - why does cell have the reveal function if i still need the whole board and the coordinates?
			}
		}
	}
	else if (move == 'F' || move == 'f') {
		Status status = board[x][y].status;
		if (status == Status::OPENED) {
			return;
		}
		else if (status == Status::FLAGGED) {
			board[x][y].status = Status::UNFLAGGED;
			marked--;
		}
		else if (status == Status::UNFLAGGED) {
			board[x][y].status = Status::FLAGGED;
			marked++;
		}
	} 
	else {
		cout << "Invalid move" << endl;
		return;
	}
}

int main() {
	srand(time(NULL));
	int level;
	cout << "Select level [1] for Beginner [2] for Intermediate [3] for Expert]: " << endl;
	cin >> level;

	if(level == 1) {
		M = 8;
		N = 8;
		NMINES = 10;
	} else if (level == 2) {
		M = 16;
		N = 16;
		NMINES = 40;
	} else if (level == 3) {
		M = 24;
		N = 24;
		NMINES = 99;
	} else {
		cout << "Invalid level, setting to beginner as default" << endl;
		M = 8;
		N = 8;
		NMINES = 10;
	}

	vector<vector<Cell>> board;
	for (int i = 0; i < M; i++) {
		vector<Cell> v;
		for (int j = 0; j < N; j++) {
			v.push_back(Cell());
		}
		board.push_back(v);
	}
	populateBoard(board);
	displayBoard(board);

	while(!gameOver) {
		if (opened == empty) {
			cout << "Game over!  YOU WIN! =D" << endl;
			displayBoard(board);
			return 0;
		}
		cout << "Remaining mines: " << NMINES-marked << endl;
		cout << "Make a move (O for Open, F for Flag): " << endl;
		char move;
		int x;
		int y;
		cin >> move;
		cin >> x;
		cin >> y;
		string s;
		//getLine(cin, s);

		processMove(board, move, x, y);
		displayBoard(board);
	}
	cout << "Game over!  You exploded =P " << endl;
	lost = true;
	displayBoard(board);

	return 0;
}