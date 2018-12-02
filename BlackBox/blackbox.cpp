// -----------------------------------------
// blackbox.cpp
// Jeremy Campbell
// Problem 6 in applied algorithms
// -----------------------------------------
#include <iostream>
#include <fstream>
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;




int main()
{
	ifstream fin("blackbox.in");
	if (!fin)
	{
		cerr << "There was a problem opening the input file. " << endl;
		return -1;
	}
	ofstream fout("blackbox.out");

}