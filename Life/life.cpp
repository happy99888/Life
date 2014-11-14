// Authors:
//     Mateen Ulhaq,    301250828, <mulhaq@sfu.ca>
//     Greyson Wang,    301249759, <greysonw@sfu.ca>
//     Kismen Sneddon,  301265599, <ksneddon@sfu.ca>
//     Dayton Pukanich, 301252869, <dpukanic@sfu.ca>

// Start Date: 4 November 2014
// Completion Date: 7 November 2014

// Description: Computer simulation of Conway's Game of Life using an original board contained in a data file specified by the user.


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
const int MIN_ARRAY_SIZE = 0;
const int MAX_ARRAY_SIZE = 40;
const int MIN_GENERATIONS = 1;
const char DEAD_CELL = '.';
const char LIVE_CELL = 'X';


// Prototypes
void CopyBoard(char dest[][MAX_ARRAY_SIZE],      char src[][MAX_ARRAY_SIZE], int rows,  int cols);
bool ReadGen  (char lifeBoard[][MAX_ARRAY_SIZE], istream &is,                int &rows, int &cols, int &gen);
void PrintRow (char lifeRow[MAX_ARRAY_SIZE],     ostream &os,                           int cols);
void PrintGen (char lifeBoard[][MAX_ARRAY_SIZE], ostream &os,                int rows,  int cols);
void NextGen  (char lifeBoard[][MAX_ARRAY_SIZE],                             int rows,  int cols);


// Main
int main()
{
	// Variables
	char initBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {DEAD_CELL};  // initial array
	char lifeBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {DEAD_CELL};  // working array

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

	// Open files
	inputFile.open(inputFilename);

	if(inputFile.fail())
	{
		cerr << "ERROR: input file not opened correctly" << endl;
		return 1;
	}

	outputFile.open(outputFilename);
	
	if(outputFile.fail())
	{
		cerr << "ERROR: output file not opened correctly" << endl;
		inputFile.close();
		return 2;
	}

	// Read from file
	if(ReadGen(initBoard, inputFile, rows, cols, gen))
	{
		// Error happened! End program.
		inputFile.close();
		outputFile.close();
		return 3;
	}

	// Output initial board
	cout << "LIFE initial game board\n" << endl;
	PrintGen(initBoard, cout, rows, cols);
	cout << endl;

	outputFile << "LIFE initial game board\n" << endl;
	PrintGen(initBoard, outputFile, rows, cols);
	outputFile << endl;

	// Copy initial board into current board
	CopyBoard(lifeBoard, initBoard, rows, cols);

	// Loop through each generation
	for(int n = 0; n < gen; ++n)
	{
		// Calculate next generation
		NextGen(lifeBoard, rows, cols);

		// Output new generation to cout
		cout << "\n\n\n" << "LIFE gameboard: generation " << (n + 1) << endl;
		PrintGen(lifeBoard, cout, rows, cols);
		cout << endl;

		// Output new generation to outputFile
		outputFile << "\n\n\n" << "LIFE gameboard: generation " << (n + 1) << endl;
		PrintGen(lifeBoard, outputFile, rows, cols);
		outputFile << endl;
	}

	// Close files
	inputFile.close();
	outputFile.close();

	return 0;
}


// Copies src board and writes it into dest board.
void CopyBoard(char dest[][MAX_ARRAY_SIZE], char src[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], int rows, int cols)
{
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < cols; ++i)
		{
			dest[j][i] = src[j][i];
		}
	}
}


// Reads number parameters and board from given stream and checks to ensure original board conforms to expected standards.
bool ReadGen(char lifeBoard[][MAX_ARRAY_SIZE], istream &is, int &rows, int &cols, int &gen)
{
	// Checks to ensure validity of number parameters in file
	if(!(is >> rows))
	{
		cerr << "ERROR: Cannot read number of rows";
		return 1;
	}

	if((rows <= MIN_ARRAY_SIZE) || (rows >= MAX_ARRAY_SIZE))
	{
		cerr << "ERROR: Read an illegal number of rows for the board";
		return 1;
	}

	if(!(is >> cols))
	{
		cerr << "ERROR: Cannot read number of columns";
		return 1;
	}

	if((cols <= MIN_ARRAY_SIZE) || (cols >= MAX_ARRAY_SIZE))
	{
		cerr << "ERROR: Read an illegal number of columns for the board";
		return 1;
	}

	if(!(is >> gen))
	{
		cerr << "ERROR: Cannot read the number of generations";
		return 1;
	}

	if(gen < MIN_GENERATIONS)
	{
		cerr << "ERROR: Read an illegal number of generations";
		return 1;
	}

	// Build array
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < cols; ++i)
		{
			// Read next character from file
			do
			{
				if(!(is >> lifeBoard[j][i]))
				{
					cerr << "ERROR: Not enough data in the input file";
					return 1;
				}
			} while((lifeBoard[j][i] == ' ') || (lifeBoard[j][i] == '\t'));

			// Check if character is valid
			if((lifeBoard[j][i] != LIVE_CELL) && (lifeBoard[j][i] != DEAD_CELL))
			{
				cerr << "ERROR: Input data for initial board is incorrect" << endl;
				cerr << "Location (" << j << ", " << i << ") is not valid";
				return 1;
			}
		}
	}

	// Checks to ensure borders of initial board (generation 0) have no live organisms
	for(int j = 0; j < rows; ++j)
	{
		// Left column
		if(lifeBoard[j][0] != DEAD_CELL)
		{
			cerr << "ERROR: organisms are present in the border of the board, please correct your input file";
			return 1;
		}

		// Right column
		if(lifeBoard[j][cols - 1] != DEAD_CELL)
		{
			cerr << "ERROR: organisms are present in the border of the board, please correct your input file";
			return 1;
		}
	}

	for(int i = 0; i < cols; ++i)
	{
		// Top row
		if(lifeBoard[0][i] != DEAD_CELL)
		{
			cerr << "ERROR: organisms are present in the border of the board, please correct your input file";
			return 1;
		}

		// Bottom row
		if(lifeBoard[rows - 1][i] != DEAD_CELL)
		{
			cerr << "ERROR: organisms are present in the border of the board, please correct your input file";
			return 1;
		}
	}

	return 0;
}


// Outputs row to stream.
void PrintRow(char lifeRow[MAX_ARRAY_SIZE], ostream &os, int cols)
{
	for(int i = 0; i < (cols - 1); ++i)
	{
		os << lifeRow[i] << ' ';
	}

	os << lifeRow[cols - 1];
}


// Outputs board to stream.
void PrintGen(char lifeBoard[][MAX_ARRAY_SIZE], ostream &os, int rows, int cols)
{
	for(int j = 0; j < (rows - 1); ++j)
	{
		PrintRow(lifeBoard[j], os, cols);
		os << endl;
	}

	PrintRow(lifeBoard[rows - 1], os, cols);
}


// Calculates next board
void NextGen(char lifeBoard[][MAX_ARRAY_SIZE], int rows, int cols)
{
	static char nextBoard[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE] = {DEAD_CELL};

	for(int j = 1; j < (rows - 1); ++j) 
	{
		for(int i = 1; i < (cols - 1); ++i)
		{
			int counter = 0;

			// Count number of alive neighbors

			if(lifeBoard[j + 1][i + 1] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j + 1][i] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j + 1][i - 1] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j][i - 1] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j][i + 1] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j - 1][i - 1] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j - 1][i] == LIVE_CELL)
			{
				counter++;
			}

			if(lifeBoard[j - 1][i + 1] == LIVE_CELL)
			{
				counter++;
			}
			
			// Determine if next generation is alive or dead based on number of neighbors

			if(lifeBoard[j][i] == LIVE_CELL)
			{
				// Death by Overcrowding
				if(counter >= 4)
				{
					nextBoard[j][i] = DEAD_CELL;
				}
				// Death by Loneliness
				else if(counter < 2)
				{
					nextBoard[j][i] = DEAD_CELL;
				}
				// Survival
				// IT LIIIVEEES!!!
				else
				{
					nextBoard[j][i] = LIVE_CELL;
				}
			}
			else if(lifeBoard[j][i] == DEAD_CELL)
			{
				// Birth
				if(counter == 3)
				{
					nextBoard[j][i] = LIVE_CELL;
				}
				// Remains dead
				else
				{
					nextBoard[j][i] = DEAD_CELL;
				}
			}
		}
	}

	// Fill borders with DEAD_CELL
	for(int j = 0; j < rows; j++)
	{
		nextBoard[j][0] = DEAD_CELL;        // left column
		nextBoard[j][cols - 1] = DEAD_CELL; // right column
	}

	for(int i = 0; i < cols; i++)
	{
		nextBoard[0][i] = DEAD_CELL;        // top row
		nextBoard[rows - 1][i] = DEAD_CELL; // bottom row
	}

	// Copy data lifeBoard = nextBoard
	CopyBoard(lifeBoard, nextBoard, rows, cols);
}

