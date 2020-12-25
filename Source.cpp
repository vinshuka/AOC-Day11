#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <map>
#include <string>
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

//Part 2
//The rules have changed slightly, now the we need to look for the first visible seat in each adjacent direction.
//1.If a seat is empty (L) and there are no visble occupied seats adjacent to it, the seat becomes occupied.
//2.If a seat is occupied(#) and five or more visble seats adjacent to it are also occupied, the seat becomes empty.
//3.Otherwise, the seat's state does not change.

//Now we need to look in each direction until we find the first visble seat and check if that seat is occupied. So in order to make this more reasonable we are going to 
//create a map of <int, vector<pairs<int, int> where for each seat we create a list of its 8 visble neighbours this way we don't have to process and search each time we
//just look up the vector for that location and check each location in the list. The key will be the 2d to 1d coversion of our input data, this way we can freely convert
//our [x, y] using x * width + y

std::vector<std::pair<int, int>> neighbours;

std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt");
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

std::vector<std::string> getTestInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("test-input.txt");
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}


int checkNeighbours(int row, int col, const std::vector<std::string>& input, std::vector<std::pair<int, int>> neighbours)
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

int countOccupied(const std::vector<std::string>& input)
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

void printVector(const std::vector<std::string>& input)
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

//Much faster after changing to const refs
int findAnswerPart1(std::vector<std::string> input)
{
	std::vector<std::string> previousInput = input;
	std::vector<std::string> newInput = input;

	
	bool stabilize = false;

	//printVector(previousInput);

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

		//printVector(newInput);
		stabilize = (previousInput == newInput);
		previousInput = newInput;
	}

	return countOccupied(newInput);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------
//Part2 starts here

std::pair<int, int> search(const std::vector<std::string>& input, int posX, int posY, int x, int y)
{
	posX += x;
	posY += y;

	while (true)
	{
		//check if x is out of bounds
		if (posX < 0 || posX > input[0].size() - 1)
			return std::make_pair(-1, -1); //returns useless pair, which will be caught by outside function, this happens when a point has less than 8 visble seats

		//check if y is out of bounds
		if (posY < 0 || posY > input.size() - 1)
			return std::make_pair(-1, -1);
		//found our visible seat location return it
		if (input[posY][posX] == 'L')
			return std::make_pair(posY, posX);

		//added to count occupied seats as visible
		if (input[posY][posX] == '#')
			return std::make_pair(posY, posX);

		//if floor check next visible position
		if (input[posY][posX] == '.')
		{
			posX += x;
			posY += y;
		}
	}

}

std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> createVisibleNeighbourMap(const std::vector<std::string>& input)
{
	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> map;
	for (int y = 0; y < input.size(); y++)
	{
		for (int x = 0; x < input[0].size(); x++)
		{
			std::pair<int, int> key = std::make_pair(y, x);

			std::vector<std::pair<int, int>> posNeighbours; //holds our list of visible seats for a position


			for (auto n : neighbours)
			{
				std::pair<int, int> result = search(input, x, y, n.first, n.second); //uses search to find the position of the first visible seat

				if (result.first >= 0 || result.second >= 0) // checks if it is valid seat
					posNeighbours.push_back(result); 
			}
			map.insert(std::make_pair(key, posNeighbours));
		}
	}
	return map;
}

//goes through a given list and checks the seats if they are occupied
int checkVisibleNeighbours(const std::vector<std::string>& input, const std::vector<std::pair<int, int>>& visible)
{
	int count = 0;

	for (auto v : visible)
	{
		if (input[v.first][v.second] == '#')
			count++;
	}

	return count;
}

int findAnswerPart2(std::vector<std::string> input, const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>& map)
{
	std::vector<std::string> previousInput = input;
	std::vector<std::string> newInput = input;


	bool stabilize = false;

	//printVector(previousInput);

	while (!stabilize)
	{
		//std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> map = createVisibleNeighbourMap(previousInput);

		for (int i = 0; i < previousInput.size(); i++)
		{
			for (int j = 0; j < previousInput[0].size(); j++)
			{
				if (previousInput[i][j] == '.')
				{
					continue;
				}

				std::pair<int, int> key = std::make_pair(i, j);

				std::vector<std::pair<int, int>> list = map.at(key);

				int check = checkVisibleNeighbours(previousInput, list);

				
				if (check == 0)
				{
					newInput[i][j] = '#';
				}

				if (check >= 5)
				{
					newInput[i][j] = 'L';
				}
			}
		}

		//printVector(newInput);
		stabilize = (previousInput == newInput);
		previousInput = newInput;
	}

	printVector(newInput);

	return countOccupied(newInput);
}

int main()
{
	std::vector<std::string> input = getPuzzleInput();

	std::vector<std::string> test = getTestInput();
	
	neighbours.push_back(std::make_pair(-1, -1));
	neighbours.push_back(std::make_pair(1, 1));
	neighbours.push_back(std::make_pair(-1, 1));
	neighbours.push_back(std::make_pair(1, -1));
	neighbours.push_back(std::make_pair(-1, 0));
	neighbours.push_back(std::make_pair(1, 0));
	neighbours.push_back(std::make_pair(0, -1));
	neighbours.push_back(std::make_pair(0, 1));

	//auto begin = std::chrono::high_resolution_clock::now();

	//std::cout << findAnswerPart1(input) << '\n';

	//auto end = std::chrono::high_resolution_clock::now();
	//auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	//std::printf("Time measured: %.3f seconds. \n", elapsed.count() * 1e-9);

	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> map = createVisibleNeighbourMap(input);

	std::cout << findAnswerPart2(input, map) << '\n';

	//std::cout << findAnswerPart2(test);
	
	return 0;
}