// -----------------------------------------
// blackbox.cpp
// Jeremy Campbell
// Problem 6 in applied algorithms
// -----------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;

// Top, Bottom, Left, Right
enum Directions { T, B, L, R };

struct Point
{
	int row, col;
	
	Point(char side, int num)
	{
		switch (side)
		{
			case 'T':
				row = 0;
				col = num;
				break;
			case 'B':
				row = 
		}
	}

	
};

class BlackBox 
{
	const int MAX = 8;
	const char WALL = '#';
	vector<vector<char>> blackbox;
	vector<Point> entries;


public:
	BlackBox();
	bool inputBoard(ifstream& fin);
	void inputMoves(ifstream& fin);
	string shootRays();
	
	void printBoard(ofstream& fout);
};

BlackBox::BlackBox()
{
	blackbox.resize(MAX + 2, vector<char>(MAX + 2, WALL));
}

bool BlackBox::inputBoard(ifstream& fin)
{
	string line;
	getline(fin, line);
	if (line == "X")
		return false;
	
	for (int i = 1; i <= MAX; i++)
	{
		for (int j = 1; j <= MAX; j++)
		{
			blackbox[i][j] = line[j - 1];
		}

		// Not the prettiest
		if (i < MAX)
			getline(fin, line);
	}
}

void BlackBox::inputMoves(ifstream& fin)
{
	int numRays;
	fin >> numRays;
	entries.resize(numRays);

	for (int i = 0; i < numRays; i++)
	{
		fin >> entries[i].side >> entries[i].num;
	}
}

// Returns the result in the form of a string
string BlackBox::shootRays()
{
	for (EntryPoint ep : entries)
	{
		// Decode the entry point

	}
}

// Debug
void BlackBox::printBoard(ofstream& fout)
{
	fout << "---------------------------" << endl;
	for (int i = 1; i <= MAX; i++)
	{
		for (int j = 1; j <= MAX; j++)
		{
			fout << blackbox[i][j];
		}
		fout << endl;
	}
	fout << "---------------------------" << endl;
}


int main()
{
	ifstream fin("blackbox.in");
	if (!fin)
	{
		cerr << "There was a problem opening the input file. " << endl;
		return -1;
	}
	ofstream fout("blackbox.out");

	BlackBox game;
	
	if (game.inputBoard(fin))
	{
		//game.printBoard(std::cout);
		game.inputMoves(fin);
		fout << game.shootRays << endl;
	}
}