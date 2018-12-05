// -----------------------------------------
// blackbox.cpp
// Jeremy Campbell
// Problem 6 in applied algorithms
// -----------------------------------------
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::map;
using std::stringstream;

const int MAX = 8;
const char WALL = '#', ATOM = '@', NORTH = 'T', SOUTH = 'B', EAST = 'R', WEST = 'L',
NORTHWEST = 'Z', SOUTHWEST = 'X', NORTHEAST = 'C', SOUTHEAST = 'V', null = '0',
HIT = 'H', REFLECTION = 'R';

class BlackBox 
{
	struct Point 
	{
		int row, col;
		char direction = null;

		Point() = default;

		Point(char side, int num)
		{
			switch (side)
			{
			case NORTH:
				row = 0;
				col = num;
				direction = SOUTH;
				break;
			case SOUTH:
				row = MAX + 1;
				col = num;
				direction = NORTH;
				break;
			case WEST:
				row = num;
				col = 0;
				direction = EAST;
				break;
			case EAST:
				row = num;
				col = MAX + 1;
				direction = WEST;
				break;
			}
		}
		
		bool operator==(const Point& pOther) 
		{ return (this->row == pOther.row && this->col == pOther.col); }
	};
	
	vector<vector<char>> blackbox;
	vector<Point> entries;
	vector<string> results;

	// Private Methods
	Point moveRay(Point p, char direction);
	bool testPoint(const Point& pOld, const Point& pCurrent, char& direction);
	bool testForHit(const Point& p, char& direction);
	bool testForExit(const Point& pFirst, const Point& pCurrent, char direction, string& result);
	bool testForTurn(const Point& p, char& direction, string& result);
	bool makeTurn(const Point& p, char& direction, char leftDiag, char rightDiag, char left, char right, char reverse);
	bool testForAtom(const Point& p);

public:
	BlackBox();
	bool inputBoard(ifstream& fin);
	void inputMoves(ifstream& fin);
	void shootRays();
	void printResults(ofstream& fout);
	
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

		// Make sure the game is ready for the new board
		results.clear();
		entries.clear();
	}
	return true;
}

void BlackBox::inputMoves(ifstream& fin)
{
	int numRays;
	fin >> numRays;

	for (int i = 0; i < numRays; i++)
	{
		char side;
		int num;
		fin >> side >> num;
		entries.emplace_back(side, num);
	}
	// Get rid of the last endline
	string trash;
	getline(fin, trash);
}

void BlackBox::shootRays()
{
	for (Point& p : entries)
	{
		// Initial direction of the ray
		// Equals the side the ray is moving towards
		char direction = p.direction;
		Point pNew = p;

		while (!testPoint(p, pNew, direction))
		{
			pNew = moveRay(pNew, direction);
		}
	}
}

BlackBox::Point BlackBox::moveRay(Point p, char direction)
{
	switch (direction)
	{
		case NORTH:
			p.row--;
			break;
		case SOUTH:
			p.row++;
			break;
		case WEST:
			p.col--;
			break;
		case EAST:
			p.col++;
			break;
		case NORTHEAST:
			p.row--;
			p.col++;
			break;
		case SOUTHEAST:
			p.row++;
			p.col++;
			break;
		case SOUTHWEST:
			p.row++;
			p.col--;
			break;
		case NORTHWEST:
			p.row--;
			p.col--;
			break;
		case null:
			// Do nothing
			break;
	}
	p.direction = direction;
	return Point(p);
}

// Returns true if an atom has been hit or the ray has exited the box
// If the ray is reflected, testPoint will update the direction of the ray
bool BlackBox::testPoint(const Point& pOld, const Point& pCurrent, char& direction)
{
	bool finished = false;
	string result;
	if (testForExit(pOld, pCurrent, direction, result))
	{
		results.push_back(result);
		finished = true;
	}
	else if (testForHit(pCurrent, direction))
	{
		results.emplace_back(1, HIT);
		finished = true;
	}
	else if (testForTurn(pCurrent, direction, result))
	{
		results.push_back(result);
		finished = true;
	}
	return finished;
}

bool BlackBox::testForHit(const Point& p, char& direction)
{
	Point pNext = moveRay(p, direction);
	return testForAtom(pNext);
}

// Test for if the ray is going out of the box, or if the ray has not yet entered but is deflected.
bool BlackBox::testForExit(const Point& pFirst, const Point& pCurrent, char direction, string& result)
{
	Point pNext = moveRay(pCurrent, direction);
	// if the next point is out of the box
	if (pNext.row > MAX || pNext.row < 1 || pNext.col > MAX || pNext.col < 1)
	{
		if (pNext == pFirst)
			result = REFLECTION;
		else
		{
			stringstream ss;
			ss << direction << ((pNext.row > MAX || pNext.row < 1) ? pNext.col : pNext.row);
			result = ss.str();
		}
		
		return true;
	}
	return false;
}

// Returns true if there was a reflection that ended the game 
// Returns false if it simply deflected/reflected and moved on 
bool BlackBox::testForTurn(const Point& p, char& direction, string& result)
{
	bool finished = false;
	bool changed = false;
	switch (direction)
	{
		case NORTH:
			changed = makeTurn(p, direction, NORTHWEST, NORTHEAST, WEST, EAST, SOUTH);
			break;
		case SOUTH:
			changed = makeTurn(p, direction, SOUTHWEST, SOUTHEAST, WEST, EAST, NORTH);
			break;
		case EAST:
			changed =makeTurn(p, direction, NORTHEAST, SOUTHEAST, NORTH, SOUTH, WEST);
			break;
		case WEST:
			changed = makeTurn(p, direction, SOUTHWEST, NORTHWEST, SOUTH, NORTH, EAST);
			break;
	}
	// If point is outside of the box
	if ((p.row > MAX || p.row < 1 || p.col > MAX || p.col < 1) && changed)
	{
		finished = true;
		result = REFLECTION;
	}
	return finished;
}

// This seems awful
bool BlackBox::makeTurn(const Point& p, char& direction, char leftDiag, char rightDiag, char left, char right, char reverse)
{
	Point rd = moveRay(p, rightDiag);
	Point ld = moveRay(p, leftDiag);
	
	bool atomOnRightDiag = testForAtom(rd);
	bool atomOnLeftDiag = testForAtom(ld);

	if (atomOnRightDiag && atomOnLeftDiag)
	{
		direction = reverse;
		return true;
	}
	else if (atomOnRightDiag)
	{
		// deflect left
		direction = left;
		return true;
	}
	else if (atomOnLeftDiag)
	{
		// deflect right
		direction = right;
		return true;
	}
	// Nothing changed
	return false;
}

bool BlackBox::testForAtom(const Point& p)
{
	if (blackbox[p.row][p.col] == ATOM)
		return true;
	return false;
}

void BlackBox::printResults(ofstream& fout)
{
	size_t size = results.size();
	for (int i = 0; i < size; i++)
	{
		fout << results[i];
		if (i < size - 1)
			fout << endl;
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
	bool anotherRound = game.inputBoard(fin);
	
	while (anotherRound)
	{
		//game.printBoard(std::cout);
		game.inputMoves(fin);
		game.shootRays();
		game.printResults(fout);

		anotherRound = game.inputBoard(fin);
		if (anotherRound)
			fout << endl;
	}
}