// Authors:
//     Mateen Ulhaq,    301250828, <mulhaq@sfu.ca>,
//     Greyson Wang,    301249759, <greysonw@sfu.ca>
//     Kismen Sneddon,  301265599, <ksneddon@sfu.ca>
//     Dayton Pukanich, 301252869, <dpukanic@sfu.ca>

// Start Date: 4 November 2014
// Completion Date: One hour before the project is due

// Description: Uses input file to generate next step in Life simulation.


// Includes
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ostream>
#include <string>
#include <cmath>


// Namespaces
using namespace std;


// Constants
const int MAX_ARRAY_SIZE = 100;
const char DEAD_CELL = '.';
const char LIVE_CELL = 'X';


// Prototypes
void CopyBoard(char dest[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],      char src[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE],       int rows,  int cols);
void ReadGen  (char lifeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], istream &is,                                    int &rows, int &cols, int &gen);
void PrintGen (char lifeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], ostream &os,                                    int rows,  int cols,  int gen);
void NextGen  (char nextBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], char currBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int rows,  int cols);


// Main
int main()
{
	// Variables
	char initBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {'O'};  // initial array
	char currBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {'O'};  // working array
	char nextBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {'O'};  // next array

	string inputFilename = "";
	string outputFilename = "";

	ifstream inputFile;
	ofstream outputFile;

	int rows = 0;
	int cols = 0;
	int gen = 0;


	// Ask user for file names
	cout << "Enter the name of the input file: ";
	cin >> inputFilename;
	
	cout << "Enter the name of the output file: ";
	cin >> outputFilename;

	cout << "initial game board\n" << endl;


	// Open files
	inputFile.open(inputFilename);

	if(inputFile.fail())
	{
		cout << "you failed" << endl;
	}

	outputFile.open(outputFilename);
	
	if(outputFile.fail())
	{
		cout << "you failed" << endl;
	}

	// Do error messages, cerr


	// Read from file
	ReadGen(initBoard, inputFile, rows, cols, gen);
	
	// Output initial board
	PrintGen(initBoard, cout, rows, cols, gen);

	// Copy initial board into current board
	CopyBoard(currBoard, initBoard, rows, cols);


	// Loop through each generation
	for(int n = 0; n < gen; ++n)
	{
		// Calculate next generation
		NextGen(nextBoard, currBoard, rows, cols);

		// Copy currBoard = nextBoard
		CopyBoard(currBoard, nextBoard, rows, cols);

		// Output new generation
		cout << "gameboard: generation " << n << endl;
		//PrintGen(currBoard, cout, rows, cols, gen);
		PrintGen(nextBoard, cout, rows, cols, gen);

	}


	// Close files
	inputFile.close();
	outputFile.close();

	return 0;
}


// Copies src board and writes it into dest board.
void CopyBoard(char dest[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], char src[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int rows, int cols)
{
	// i = 'X', j = y
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < cols; ++i)
		{
			dest[j][i] = src[j][i];
		}
	}
}


// Reads numbers and board from given file stream.
void ReadGen(char lifeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], istream &is, int &rows, int &cols, int &gen)
{
	is >> rows >> cols >> gen;
	
	// i = 'X', j = y
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < cols; ++i)
		{
			is >> lifeBoard[j][i];
		}
	}
}


// Outputs board to stream (screen).
// NOTE: this outputs extra space at the end of each row.
void PrintGen(char lifeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], ostream &os, int rows, int cols, int gen)
{
	// i = 'X', j = y
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < cols; ++i)
		{
			os << lifeBoard[j][i] << ' ';
		}

		os << endl;
	}
}


// Calculates next board
/*
The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells, each of which is in one of two possible states,
alive or dead. Every cell interacts with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent.
At each step in time, the following transitions occur:

    Any live cell with fewer than two live neighbours dies, as if caused by under-population.
    Any live cell with two or three live neighbours lives on to the next generation.
    Any live cell with more than three live neighbours dies, as if by overcrowding.
    Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

The initial pattern constitutes the seed of the system.
The first generation is created by applying the above rules simultaneously to every cell in the seed—births and deaths occur simultaneously,
and the discrete moment at which this happens is sometimes called a tick (in other words, each generation is a pure function of the preceding one).
The rules continue to be applied repeatedly to create further generations.
*/
void NextGen(char nextBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], char currBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int rows, int cols)
{
	for(int j = 1; j < (rows - 1); j++) 
	{
		for(int i = 1; i < (cols - 1); i++)
		{
			int counter = 0;

			// Count number of alive neighbors

			if (currBoard[j+1][i+1] == 'X')
			{
				counter++;
			}

			if (currBoard[j+1][i] == 'X')
			{
				counter++;
			}

			if (currBoard[j+1][i-1] == 'X')
			{
				counter++;
			}

			if (currBoard[j][i-1] == 'X')
			{
				counter++;
			}

			if (currBoard[j][i+1] == 'X')
			{
				counter++;
			}

			if (currBoard[j-1][i-1] == 'X')
			{
				counter++;
			}

			if (currBoard[j-1][i] == 'X')
			{
				counter++;
			}

			if (currBoard[j-1][i+1] == 'X')
			{
				counter++;
			}

			// Determine if alive or dead and write to next generation

			// Birth
			if (counter == 3)
			{
				nextBoard[j][i] = 'X';
			}
			// Death by Overcrowding
			else if (counter >= 4)
			{
				nextBoard[j][i] = '.';
			}
			// Death by Loneliness
			else if (counter < 2)
			{
				nextBoard[j][i] = '.';
			}
			// Survival
			else
			{
				nextBoard[j][i] = currBoard [j][i];
			}
		}
	}
}


/*
Todo:
 - Algorithm
 - Error messages
*/

