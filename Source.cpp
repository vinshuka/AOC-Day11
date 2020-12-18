#include <vector>
#include <fstream>
#include <iostream>

//Day11 Part 1
//Given a layout of seats, where an empty seat is represented by 'L', an occupied seat is represented by '#' and the floor is represented by '.', using the 
//following rules for how seats become empty or occupied 
//1.If a seat is empty (L) and there are no occupied seats adjacent to it, the seat becomes occupied.
//2.If a seat is occupied(#) and four or more seats adjacent to it are also occupied, the seat becomes empty.
//3.Otherwise, the seat's state does not change.
//Using these rules simulate the seats becoming empty and occupied, eventually the simulation will stabilize and changes will no longer happen, count the 
//number of occupied seats after the simulation stabilizes

//For this we need to consider some things, first how will we store the seat layout, we'll start off with a vector and see if that will work. The changes 
//need to happen simutaneously so we will need to have a temp vector to hold each round of simulation. How will we know when the simulation stabilizes? We
//can compare the previous simulation with the current finished simulation and see if they are different, if they are we can set the previous to the new 
//and run the simulation again, else we can just count the occupied seats in the previous simulation. We can run the simulation using a while loop and 
//use a flag to stop it when the previous simulation matches the current. We could also try using a bit vector to see if changes have happened.
//We need to check all 8 neighbouring cells. 
//[x][y] top-left:[-1, -1], bottom-right:[1, 1], bottom-left:[-1, 1], top-right:[1, -1], center-left:[-1, 0], center-right:[1, 0], 
//center-top:[0, -1], center-bottom:[0, 1] adding these values to the [x][y] value we are checking will get each neighbouring location
//we can use a bitset as our bit vector and access it the same way as an array
//we can covert a 2d aaray to a 1d array using x * Width + y
//Width is 92, Height is 93

std::vector<std::pair<int, int>> neighbours;

std::vector<std::vector<char>> getPuzzleInput()
{
	std::vector<std::vector<char>> input;

	std::ifstream file;
	file.open("puzzle-input.txt", std::ios::in);
	char c;
	
	std::vector<char> line;
	
	while (!file.eof())
	{
		file.get(c);
		
		if (c == '\n')
		{
			input.push_back(line);
			line.clear();
		}
		else
		{
			line.push_back(c);
		}
	}
	line.pop_back(); //remove extra character added by eof

	input.push_back(line); //push final line

	return input;
}

int checkNeighbours(int row, int col, std::vector<std::vector<char>> input, std::vector<std::pair<int, int>> neighbours)
{
	int neighbourCount = 0;

	for (auto n : neighbours)
	{
		if (row + n.first < 0 || col + n.second < 0)
		{
			continue;
		}

		if (row + n.first >= input.size()|| col + n.second >= input[0].size())
		{
			continue;
		}

		if (input[row + n.first][col + n.second] == '#')
		{
			neighbourCount++;
		}
	}

	return neighbourCount;
}

int countOccupied(std::vector<std::vector<char>> input)
{
	int count = 0;

	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 0; j < input[0].size(); j++)
		{
			if (input[i][j] == '#')
				count++;
		}
	}

	return count;
}

void printVector(std::vector<std::vector<char>> input)
{
	for (auto l : input)
	{
		for (int j = 0; j < l.size(); j++)
		{
			std::cout << l[j];
		}
		std::cout << '\n';
	}
}

bool checkStable(std::vector<std::vector<char>> inputOld, std::vector<std::vector<char>> inputNew)
{
	for (int i = 0; i < inputOld.size(); i++)
	{
		for (int j = 0; j < inputOld[0].size(); j++)
		{
			if (inputOld[i][j] != inputNew[i][j])
				return false;
		}
	}
	return true;
}


//This is sloooooooooooooooowwwwww
int findAnswerPart1(std::vector<std::vector<char>> input)
{
	std::vector<std::vector<char>> previousInput = input;
	std::vector<std::vector<char>> newInput = input;

	bool stabilize = false;

	printVector(previousInput);

	while (!stabilize)
	{
		for (int i = 0; i < previousInput.size(); i++)
		{
			for (int j = 0; j < previousInput[0].size(); j++)
			{
				int check = checkNeighbours(i, j, previousInput, neighbours);

				if (previousInput[i][j] == '.')
				{
					continue;
				}
				if (check == 0)
				{
					newInput[i][j] = '#';
				}

				if (check >= 4)
				{
					newInput[i][j] = 'L';
				}
			}
		}

		printVector(newInput);
		stabilize = checkStable(previousInput, newInput);
		previousInput = newInput;
	}

	return countOccupied(newInput);
}

int main()
{
	std::vector<std::vector<char>> input = getPuzzleInput();
	
	neighbours.push_back(std::make_pair(-1, -1));
	neighbours.push_back(std::make_pair(1, 1));
	neighbours.push_back(std::make_pair(-1, 1));
	neighbours.push_back(std::make_pair(1, -1));
	neighbours.push_back(std::make_pair(-1, 0));
	neighbours.push_back(std::make_pair(1, 0));
	neighbours.push_back(std::make_pair(0, -1));
	neighbours.push_back(std::make_pair(0, 1));

	std::cout << findAnswerPart1(input) << '\n';

	return 0;
}