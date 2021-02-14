#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include <fstream>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

enum color { Green = 10, Blue, Red, Purple };

struct player {
	int box = 1;
	bool home = true, won = false;
};

void printMsg(const char msg[], int dim) {
	gotoRowCol(10 + 4, dim * 10 + 5);
	cout << "                                            ";
	gotoRowCol(10 + 4, dim * 10 + 5);
	cout << msg;
}

void printBox(int sr, int sc, int dim, char s) {
	for (int ri = 0; ri < dim; ri++) {
		for (int ci = 0; ci < dim; ci++) {
			if (ri == 0 || ri == dim - 1) {
				gotoRowCol(sr + ri, sc + ci);
				cout << s;
			}
			else if (ci == 0 || ci == dim - 1) {
				gotoRowCol(sr + ri, sc + ci);
				cout << s;
			}
		}
	}

}

void printGrid(int dim, char s) {
	for (int ri = 0; ri < 10; ri++) {
		for (int ci = 0; ci < 10; ci++) {
			if (ri == 0 && ci == 0) {
				SetConsoleTextAttribute(hConsole, 6);
				printBox(ri * dim, ci * dim, dim, s);
				SetConsoleTextAttribute(hConsole, 7);
			}
			else {
				printBox(ri * dim, ci * dim, dim, s);
			}
		}
	}


}

void getBoxPos(int& rpos, int& cpos, int dim, int boxN) {
	rpos = dim * (9 - ((boxN - 1) / 10));
	if (rpos % 2 == 0) {
		cpos = dim * (9 - ((boxN - 1) % 10));
	}
	else {
		cpos = dim * ((boxN - 1) % 10);
	}
}

void boxCenter(int& rpos, int& cpos, int dim, int boxN) {
	getBoxPos(rpos, cpos, dim, boxN);
	rpos += dim / 2;
	cpos += dim / 2 - 1;
}

void topLeft(int& rpos, int& cpos, int dim, int boxN) {
	getBoxPos(rpos, cpos, dim, boxN);
	rpos += 1;
	cpos += 1;
}

void topRight(int& rpos, int& cpos, int dim, int boxN) {
	getBoxPos(rpos, cpos, dim, boxN);
	rpos += 1;
	cpos += dim - 2;
}

void bottomLeft(int& rpos, int& cpos, int dim, int boxN) {
	getBoxPos(rpos, cpos, dim, boxN);
	rpos += dim - 2;
	cpos += 1;
}

void bottomRight(int& rpos, int& cpos, int dim, int boxN) {
	getBoxPos(rpos, cpos, dim, boxN);
	rpos += dim - 2;
	cpos += dim - 2;
}

void displayPlayer(int& rpos, int& cpos, int dim, int boxN, int playerN, char C) {
	switch (playerN) {
	case 0:
		topLeft(rpos, cpos, dim, boxN);
		SetConsoleTextAttribute(hConsole, Green);
		break;
	case 1:
		topRight(rpos, cpos, dim, boxN);
		SetConsoleTextAttribute(hConsole, Blue);
		break;
	case 2:
		bottomLeft(rpos, cpos, dim, boxN);
		SetConsoleTextAttribute(hConsole, Red);
		break;
	case 3:
		bottomRight(rpos, cpos, dim, boxN);
		SetConsoleTextAttribute(hConsole, Purple);
		break;
	}
	gotoRowCol(rpos, cpos);
	cout << C;
}

int getDiceRoll(int dim, int col) {
	int r = 1;
	SetConsoleTextAttribute(hConsole, col);
	gotoRowCol(10 + 2, dim * 10 + 5);
	cout << "                     ";
	gotoRowCol(10 + 2, dim * 10 + 5);
	cout << "......";
	while (!_kbhit()) {
		r = rand() % 6 + 1;
		gotoRowCol(5, dim * 10 + 10);
		cout << r;
	}
	_getch();
	r = rand() % 6 + 1;
	gotoRowCol(5, dim * 10 + 10);
	cout << r;
	gotoRowCol(10 + 2 , dim * 10 + 5);
	cout << "YOU GOT " << r;
	printMsg("Press any key to continue..", dim);
	_getch();
	return r;
}

struct object {
	int start, end;
};

void initList(ifstream& reader, object* &list, int &size) {
	reader >> size;
	list = new object[size];
	for (int i = 0; i < size; i++)
		reader >> list[i].start >> list[i].end;
}

void printObjects(object* list, int size, int dim) {
	int start = 0, end = 0;
	int rpos, cpos;
	
	int ri = 6;
	gotoRowCol(10 + ri, dim * 10 + 5);
	cout << "OBJECTS:  ";
	ri += 2;
	gotoRowCol(10 + ri, dim * 10 + 5);
	cout << "START\t\tEND";

	for(int i = 0; i < size; i++){
		start = list[i].start;
		end = list[i].end;
		if (start > end) {
			SetConsoleTextAttribute(hConsole, 4);
		}
		else {
			SetConsoleTextAttribute(hConsole, 2);
		}
		getBoxPos(rpos, cpos, dim, start);
		printBox(rpos, cpos, dim, -37);
		getBoxPos(rpos, cpos, dim, end);
		printBox(rpos, cpos, dim, -37);
		ri += 2;
		gotoRowCol(10 + ri, dim * 10 + 5);
		cout << start << "\t\t" << end;
	}
	
}

int checkList(int boxN, object* list, int size) {
	for (int i = 0; i < size; i++) {
		if (boxN == list[i].start)
			return list[i].end;
	}
	return 0;
}

int main() {
	srand(time(0));
	char block = -37;
	int dim = 7;
	int rpos = 0, cpos = 0;
	printGrid(dim, 'x');
	for (int i = 1; i <= 100; i++) {
		boxCenter(rpos, cpos, dim, i);
		gotoRowCol(rpos, cpos);
		cout << setw(3) << i;
	}

	int size;
	object* snakesLadders = nullptr;
	ifstream reader("objects.txt");
	initList(reader, snakesLadders, size);

	printObjects(snakesLadders, size, dim);
	
	
	SetConsoleTextAttribute(hConsole, 7);

	printBox(0, dim * 10 + 5, 10, block);

	int NOP = 0;
	player* Players = nullptr;

	do {
		printMsg("Number of Players:  ", dim);
		cin >> NOP;
	} while (NOP < 1 || NOP > 4);
	
	Players = new player[NOP];

	for (int i = 0; i < NOP; i++) {
		displayPlayer(rpos, cpos, dim, 1, i, block);
	}

	int	playerNum = rand() % NOP;
	int turn;
	int total;
	int winCount = 0;
	do {
		playerNum++;
		if (playerNum == NOP)
			playerNum = 0; 
		if (!Players[playerNum].won) {
			SetConsoleTextAttribute(hConsole, Green + playerNum);
			printMsg("Press any key to get Dice Roll...",dim);
			total = 0;
			do {
				turn = getDiceRoll(dim, Green + playerNum);
				total += turn;
			} while (turn == 6 && total < 18);
			if (total < 18 && Players[playerNum].box + total <= 100) {
				if(Players[playerNum].home){
					if (total > 6) {
						total -= 6;
						Players[playerNum].home = false;
						printMsg("You Got Out of the Home...", dim);
						_getch();
					}
					else {
						continue;
					}
				}
				for (int i = 0; i < total; i++) {
					displayPlayer(rpos, cpos, dim, Players[playerNum].box, playerNum, ' ');
					Players[playerNum].box ++;
					displayPlayer(rpos, cpos, dim, Players[playerNum].box, playerNum, block);
					Sleep(100);
				}

				int check;
				if (check = checkList(Players[playerNum].box, snakesLadders, size)) {
					displayPlayer(rpos, cpos, dim, Players[playerNum].box, playerNum, ' ');
					Players[playerNum].box = check;
					displayPlayer(rpos, cpos, dim, Players[playerNum].box, playerNum, block);
				}
				if (Players[playerNum].box == 100) {
					Players[playerNum].won = true, winCount++;
					printMsg("THIS COLOR WON!", dim);
					_getch();
				}
				
			}
		}
	} while (winCount < NOP - 1);
	SetConsoleTextAttribute(hConsole, 7);
	printMsg("GAME END", dim);
	_getch();

}