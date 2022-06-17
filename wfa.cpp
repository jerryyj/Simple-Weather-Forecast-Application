#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <iomanip>
#include "colors.h"
using namespace std;

// Declare prototype functions
void displayMainMenu();
bool readConfig();
void init2DArray(int, int);
void deallocateMemory(int, int);
void processCityLocation(string);
void processCloudCover(string);
void processPressure(string);
void displayCityMap();
void displayCloudCover();
void displayCloudLMH();
void displayAP();
void displayAPLMH();
void displayWeatherReport();
int checkIndexValue(int);
string checkLMHSymbol(float);
vector<float> getAvgData(int);
vector<string> getRainInfo(string, string);
vector<string> tokenizeString (string, string);
vector<int> getIntFromString (string);

// Declare struct to store city/weather info
struct LocationInfo
{
	string cityName;
	int cityID;
	int cloudCover;
	int pressure;
	
	LocationInfo()
	{
		cityName = "";
		cityID = 0;
		cloudCover = 0;
		pressure = 0;
	}
};

// Declare global variables
LocationInfo ** array2D = nullptr;
int gridX, gridY;

int main()
{	
	// Read in config file until success
	bool readSuccess;
	do
	{
		readSuccess = readConfig();
	} while (!readSuccess);
	
	int option = -1;
	do
	{
		displayMainMenu();
		
		cout << "PLease enter your choice: ";
		cin >> option;

		cin.clear(); // reset failbit
		cin.ignore(10000, '\n'); // clear input buffer

		// swtich-case statement to branch out options
		switch(option)
		{
			case 1:
			{
				readSuccess = readConfig();
				if (!readSuccess)
					cout << "Storing of records failed. Going back to main menu ..." << endl;
				
			} 
				break;
			case 2:
			{
				cout << endl;
				displayCityMap();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore the up to 10000 characters until user presses enter
			}
				break;
			case 3:
			{
				cout << endl;
				displayCloudCover();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore up to 10000 characters until user presses enter
			}
				break;
			case 4:
			{
				cout << endl;
				displayCloudLMH();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore up to 10000 characters until user presses enter
			}
				break;
			case 5:
			{
				cout << endl;
				displayAP();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore up to 10000 characters until user presses enter
			}
				break;
			case 6:
			{
				cout << endl;
				displayAPLMH();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore up to 10000 characters until user presses enter
			}
				break;
			case 7:
			{
				cout << endl;
				displayWeatherReport();
				cout << "Press <enter> to go back to main menu ...";
				cin.ignore(10000, '\n'); // Ignore up to 10000 characters until user presses enter
			}
				break;
		}
	}while (option != 8);
	
	// Deallocate memory of 2D array pointer
	deallocateMemory(gridX, gridY);
}

// Function to display student info and main menu options
void displayMainMenu()
{
	// DIsplay header
	cout << endl;
	cout << "Welcome to Weather Information Processing System!" << endl;
	
	// Display menu options
	cout << endl;
	cout << "1)" << '\t' << "Read in and process a configuration file" << endl;
	cout << "2)" << '\t' << "Display city map" << endl;
	cout << "3)" << '\t' << "Display cloud coverage map (cloudiness index)" << endl;
	cout << "4)" << '\t' << "Display cloud coverage map (LMH symbols)" << endl;
	cout << "5)" << '\t' << "Display atmospheric pressure map (pressure index)" << endl;
	cout << "6)" << '\t' << "Display atmospheric pressure map (LMH symbols)" << endl;
	cout << "7)" << '\t' << "Show weather forecast summary report" << endl;
	cout << "8)" << '\t' << "Quit" << endl;
	cout << endl;
	cout << RST; // reset colours
}


// Function to read and process weather info from config file
bool readConfig()
{
	// Declare c-string variable with a size of 101 to store file name
	char fileName[101];
	
	// Get user input for file name up to length of 100
	cout << endl;
	cout << "Please enter config filename: ";	
	cin.getline(fileName, 101);
	cout << endl;
	
	fstream inputFile (fileName, fstream::in);
	string aLine;
	int step = 1;
	
	// Get map info
	while (getline (inputFile, aLine))
	{
		// Ignore empty lines
		if (aLine.size() == 0)
			continue;

		switch(step)
		{
			case 1:
			{
				// Read X coordinates info
				size_t found = aLine.find("GridX_IdxRange");
				if (found != string::npos)
				{
					cout << "Reading in " << aLine << FMAG(" ... ");
					vector<int> xRangeList = getIntFromString(aLine);
					gridX = xRangeList.back() + 1;
					cout << FMAG("done!") << endl;
					step++;
				}
			}
				break;
			case 2:
			{
				// Read Y coordinates info and initialize 2D Array(map)
				size_t found = aLine.find("GridY_IdxRange");
				if (found != string::npos)
				{
					cout << "Reading in " << aLine << FMAG(" ... ");
					vector<int> yRangeList = getIntFromString(aLine);
					gridY = yRangeList.back() + 1;
					init2DArray(gridX, gridY);
					cout << FMAG("done!") << endl;
					step++;
				}
			}
				break;
			case 3:
			{
				// Read city location info
				size_t found = aLine.find(".txt");
				if (found != string::npos)
				{
					cout << endl << "Storing data from input file :" << endl;
					cout << KCYN;
					cout << aLine << FMAG(" ... ");
					processCityLocation(aLine);
					cout << FMAG("done!") << endl;
					step++;
				}
			}
				break;
			case 4:
			{
				// Read cloud cover info
				size_t found = aLine.find(".txt");
				if (found != string::npos)
				{
					cout << KCYN;
					cout << aLine << FMAG(" ... ");
					processCloudCover(aLine);
					cout << FMAG("done!") << endl;
					step++;
				}
			}
				break;
			case 5:
			{
				// Read pressure info
				size_t found = aLine.find(".txt");
				if (found != string::npos)
				{
					cout << KCYN;
					cout << aLine << FMAG(" ... ");
					processPressure(aLine);
					cout << FMAG("done!") << endl;
					cout << endl << "All records successfully stored. Going back to main menu ..." << endl;
					return true; // Reading of config file completed
				}
			}
				break;
		}
	}
	
	return false; // Reading of config file failed
}


// Function to initialize dynamic 2D Array
void init2DArray(int x, int y)
{
	array2D = new LocationInfo * [y];
	for (int i = 0; i < y; i++)
		array2D [i] = new LocationInfo [x];
}

// Function to deallocate memory from dynamic 2D array
void deallocateMemory(int x, int y)
{
	if (x <= 0)
		return;
	
	// Deletion of columns
	for (int i = 0; i < y; i++)
		delete [] array2D [i];
	
	// Deletion of rows
	delete [] array2D;
}

// FUnction to tokenize a string with respect to delimiter
vector<string> tokenizeString (string input, char delimiter)
{
	// Declare vector to store tokens
	vector<string> tokens;
	
	// Declare stringstream class and a string variable to save token
	stringstream checkLine(input);
	string item;
	
	// Use getline function to save tokens with respect to delimiter
	while(getline(checkLine, item, delimiter))
		tokens.push_back(item);

	return tokens;
}

// Function to retrieve all integers from string
vector<int> getIntFromString (string str)
{
	// Declare vector to store integers
	vector<int> intList;
	
	// Variables to store beginning and ending index of numbers found
	size_t begin = 0, end = 0;
	while(begin != string::npos)
    {
		// FInd the first instance of digit within string
        begin = str.find_first_of("0123456789", end);
        if(begin != string::npos) 
        {
			// FInd the index of the character after the number
            end = str.find_first_not_of("0123456789", begin);

			// Get substring of number from beginning index and length
            string numTemp = str.substr(begin, end - begin);

			// Convert string to integer and store into vector
            int number = stoi(numTemp);
            intList.push_back(number);
        }
    }
	
	return intList;
};

// Function to process city location text file
void processCityLocation(string fileName)
{
	fstream inputFile (fileName.c_str(), fstream::in);
	string aLine;
	while (getline(inputFile, aLine))
	{
		// Ignore empty lines
		if (aLine.size() == 0)
			break;
		
		// Call functions to retrieve info from text file
		vector<string> tokens = tokenizeString(aLine, '-');
		vector<int> coordinates = getIntFromString(tokens[0]);
		
		// Store city location information
		array2D[gridY - 1 - coordinates[1]][coordinates[0]].cityID = stoi(tokens[1]);
		array2D[gridY - 1 - coordinates[1]][coordinates[0]].cityName = tokens[2];
	}
}

// Function to process cloud cover text file
void processCloudCover(string fileName)
{
	fstream inputFile (fileName.c_str(), fstream::in);
	string aLine;

	while (getline(inputFile, aLine))
	{
		// Ignore empty lines
		if (aLine.size() == 0)
			break;
		
		// Call functions to retrieve info from text file
		vector<string> tokens = tokenizeString(aLine, '-');
		vector<int> coordinates = getIntFromString(tokens[0]);
		
		// Store city location information
		array2D[gridY - 1 - coordinates[1]][coordinates[0]].cloudCover = stoi(tokens[1]);
	}
}

// Function to process pressure text file
void processPressure(string fileName)
{
	fstream inputFile (fileName.c_str(), fstream::in);
	string aLine;
	
	while (getline(inputFile, aLine))
	{
		// Ignore empty lines
		if (aLine.size() == 0)
			break;
		
		// Call functions to retrieve info from text file
		vector<string> tokens = tokenizeString(aLine, '-');
		vector<int> coordinates = getIntFromString(tokens[0]);

		// Store city location information
		array2D[gridY - 1 - coordinates[1]][coordinates[0]].pressure = stoi(tokens[1]);
	}
}

// Function to display city map
void displayCityMap()
{
	cout << endl;
	int mapX = gridX + 3;
	int mapY = gridY + 3;
	
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			// First and second last row
			if (i == 0 || i == mapY - 2)
			{
				cout << KWHT; //white output
				if (j == 0)
					cout << "   ";
				else
					cout << "  " << "#";
			}
			else if (i == mapY - 1) // last row
			{
				cout << KWHT; //white output
				if (j < 2 || j == mapX - 1)
					cout << "   ";
				else
				{
					if (j < 13)
						cout << "  " << j - 2;
					else
						cout << " " << j - 2;
				}
			}else
			{
				cout << KWHT; //white output
				if (j == 0)
				{
					if (gridY - i < 10)
						cout << "  " << gridY - i;
					else
						cout << " " << gridY - i;
				}
				else if (j == 1 || j == mapX - 1)
					cout << "  " << "#";
				else
				{	
					cout << KYEL; // yellow output
					if (array2D[i - 1][j - 2].cityID > 0)
						cout << "  " << array2D[i - 1][j - 2].cityID;
					else
						cout << "   ";
				}
			}
		}
		cout << endl;
	}
	cout << RST; // reset colours
	cout << endl;
}

// Function to display cloudiness index map
void displayCloudCover()
{
	cout << endl;
	int mapX = gridX + 3;
	int mapY = gridY + 3;
	
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			// First and second last row
			if (i == 0 || i == mapY - 2)
			{
				cout << KWHT; //white output
				if (j == 0)
					cout << "   ";
				else
					cout << "  " << "#";
			}
			else if (i == mapY - 1) // last row
			{
				cout << KWHT; //white output
				if (j < 2 || j == mapX - 1)
					cout << "   ";
				else
				{
					if (j < 13)
						cout << "  " << j - 2;
					else
						cout << " " << j - 2;
				}
			}else
			{
				cout << KWHT; //white output
				if (j == 0)
				{
					if (gridY - i < 10)
						cout << "  " << gridY - i;
					else
						cout << " " << gridY - i;
				}
				else if (j == 1 || j == mapX - 1)
					cout << "  " << "#";
				else
					cout << KCYN << "  " << checkIndexValue(array2D[i - 1][j - 2].cloudCover);
			}
		}
		cout << endl;
	}
	cout << RST; // reset colours
	cout << endl;
}

// Function to display cloud coverage map in LMH symbol
void displayCloudLMH()
{
	cout << endl;
	int mapX = gridX + 3;
	int mapY = gridY + 3;
	
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			// First and second last row
			if (i == 0 || i == mapY - 2)
			{
				cout << KWHT; //white output
				if (j == 0)
					cout << "   ";
				else
					cout << "  " << "#";
			}
			else if (i == mapY - 1) // last row
			{
				cout << KWHT; //white output
				if (j < 2 || j == mapX - 1)
					cout << "   ";
				else
				{
					if (j < 13)
						cout << "  " << j - 2;
					else
						cout << " " << j - 2;
				}
			}else
			{
				cout << KWHT; //white output
				if (j == 0)
				{
					if (gridY - i < 10)
						cout << "  " << gridY - i;
					else
						cout << " " << gridY - i;
				}
				else if (j == 1 || j == mapX - 1)
					cout << "  " << "#";
				else
				{
					string symbol = checkLMHSymbol(array2D[i - 1][j - 2].cloudCover);
					if (symbol == "H")
						cout << KRED; // red output
					else if (symbol == "M")
						cout << KYEL; // yellow output
					else
						cout << KGRN; // green output
					cout << "  " << symbol;
				}
			}
		}
		cout << endl;
	}
	cout << RST; // reset colours
	cout << endl;
}

// Function to display atmospheric pressure map
void displayAP()
{
	cout << endl;
	int mapX = gridX + 3;
	int mapY = gridY + 3;
	
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			// First and second last row
			if (i == 0 || i == mapY - 2)
			{
				cout << KWHT; //white output
				if (j == 0)
					cout << "   ";
				else
					cout << "  " << "#";
			}
			else if (i == mapY - 1) // last row
			{
				cout << KWHT; //white output
				if (j < 2 || j == mapX - 1)
					cout << "   ";
				else
				{
					if (j < 13)
						cout << "  " << j - 2;
					else
						cout << " " << j - 2;
				}
			}else
			{
				cout << KWHT; //white output
				if (j == 0)
				{
					if (gridY - i < 10)
						cout << "  " << gridY - i;
					else
						cout << " " << gridY - i;
				}
				else if (j == 1 || j == mapX - 1)
					cout << "  " << "#";
				else
					cout << KMAG << "  " << checkIndexValue(array2D[i - 1][j - 2].pressure);
			}
		}
		cout << endl;
	}
	cout << RST; // reset colours
	cout << endl;
}

// Function to display atmospheric pressure map in LMH symbol
void displayAPLMH()
{
	cout << endl;
	int mapX = gridX + 3;
	int mapY = gridY + 3;
	
	for (int i = 0; i < mapY; i++)
	{
		for (int j = 0; j < mapX; j++)
		{
			// First and second last row
			if (i == 0 || i == mapY - 2)
			{
				cout << KWHT; //white output
				if (j == 0)
					cout << "   ";
				else
					cout << "  " << "#";
			}
			else if (i == mapY - 1) // last row
			{
				cout << KWHT; //white output
				if (j < 2 || j == mapX - 1)
					cout << "   ";
				else
				{
					if (j < 13)
						cout << "  " << j - 2;
					else
						cout << " " << j - 2;
				}
			}else
			{
				cout << KWHT; //white output
				if (j == 0)
				{
					if (gridY - i < 10)
						cout << "  " << gridY - i;
					else
						cout << " " << gridY - i;
				}
				else if (j == 1 || j == mapX - 1)
					cout << "  " << "#";
				else
				{
					string symbol = checkLMHSymbol(array2D[i - 1][j - 2].pressure);
					if (symbol == "H")
						cout << KRED; // red output
					else if (symbol == "M")
						cout << KYEL; // yellow output
					else
						cout << KGRN; // green output
					cout << "  " << symbol;
				}
			}
		}
		cout << endl;
	}
	cout << RST; // reset colours
	cout << endl;	
}

// Function to display weather forecast summary report
void displayWeatherReport()
{
	// Declare set variable to store unique pairs of cityName and cityID
	set <pair <string, int>> cities;

	for (int i = 0; i < gridY; i++)
	{
		for (int j = 0; j < gridX; j++)
		{
			if (array2D[i][j].cityID != 0)
				cities.insert(make_pair(array2D[i][j].cityName, array2D[i][j].cityID));
		}
	}
	
	// Display the info
	cout << endl;
	cout << "Weather Forecast Summary Report" << endl;
	cout << "-------------------------------" << endl;
	
	for (auto city : cities)
	{
		// Compute necessary data
		vector<float> data = getAvgData(city.second);
		vector<string> rainInfo = getRainInfo(checkLMHSymbol(data[1]), checkLMHSymbol(data[0]));
		
		// DIsplay Info
		cout << KWHT; // white output
		cout << "City Name : " << city.first << endl;
		cout << "City ID   : " << city.second << endl;
		cout << KMAG; // magenta output
		cout << fixed; // decimal part of floating point values are specified using setprecision
		cout << "Avg. Cloud Cover (ACC)     : " << setprecision(2) << data[0] << " (" << checkLMHSymbol(data[0]) << ")" << endl;
		cout << "Avg. Pressure    (AP)      : " << setprecision(2) << data[1] << " (" << checkLMHSymbol(data[1]) << ")" << endl;
		cout << KCYN; // cyan output
		cout << "Probability of Rain (%)    : " << setprecision(2) << static_cast<float>(stoi(rainInfo[0])) << endl;
		cout << rainInfo[1] << endl << endl;
		cout << RST;  // reset colours
	}
}

// Function to check and return cloudiness index
int checkIndexValue(int cc)
{
	if (cc < 10)
		return 0;
	else if (cc < 20)
		return 1;
	else if (cc < 30)
		return 2;
	else if (cc < 40)
		return 3;
	else if (cc < 50)
		return 4;
	else if (cc < 60)
		return 5;
	else if (cc < 70)
		return 6;
	else if (cc < 80)
		return 7;
	else if (cc < 90)
		return 8;
	else
		return 9;	
}

// Function to check LMH symbol
string checkLMHSymbol(float cc)
{
	if (cc < 35.0)
		return "L";
	else if (cc < 65.0)
		return "M";
	else
		return "H";
}

// Function to get rain info
vector<string> getRainInfo(string ap, string cc)
{
	vector <string> rainInfo;

	if (ap == "L")
	{
		if (cc == "H")
		{
			rainInfo.push_back("90");
			rainInfo.push_back("~~~~\n~~~~~\n\\\\\\\\\\");
		}
		else if (cc == "M")
		{
			rainInfo.push_back("80");
			rainInfo.push_back("~~~~\n~~~~~\n \\\\\\\\");
		}
		else
		{
			rainInfo.push_back("70");
			rainInfo.push_back("~~~~\n~~~~~\n  \\\\\\");
		}
	}
	else if (ap == "M")
	{
		if (cc == "H")
		{
			rainInfo.push_back("60");
			rainInfo.push_back("~~~~\n~~~~~\n   \\\\");
		}
		else if (cc == "M")
		{
			rainInfo.push_back("50");
			rainInfo.push_back("~~~~\n~~~~~\n    \\");
		}
		else
		{
			rainInfo.push_back("40");
			rainInfo.push_back("~~~~\n~~~~~");
		}
	}
	else
	{
		if (cc == "H")
		{
			rainInfo.push_back("30");
			rainInfo.push_back("~~~\n~~~~");
		}
		else if (cc == "M")
		{
			rainInfo.push_back("20");
			rainInfo.push_back("~~\n~~~");
		}
		else
		{
			rainInfo.push_back("10");
			rainInfo.push_back("~\n~~");
		}
	}
		
	return rainInfo;
}

// Function to get average cloud cover and pressure from city ID
vector<float> getAvgData(int cityID)
{
	// Declare variable to store computed data
	vector<float> data;
	
	// Declare variable to store array indices
	set <pair <int, int>> indexes;
	
	// Get and store indices of all locations with certain city ID
	for (int i = 0; i < gridY; i++)
	{
		for (int j = 0; j < gridX; j++)
		{
			if (array2D[i][j].cityID == cityID)
			{
				// Algorithm that adds all cities in the 3x3 cluster to a set of coordinates
				int startY = i - 1; // y-coordinate of the surrounding cities one level higher
		
				// The range of x-coordinates of all cities within the 3x3 cluster
				int leftX = j - 1;
				int midX = j;
				int rightX = j + 1;

				// Add all valid cities of the 3x3 cluster
				for (int i = 0; i <= 2; i++)
				{
					// if y-coordinate is out of bounds, skip iteration
					if (startY < 0 || startY > gridY - 1)
						continue;
					else
					{
						// Check if x-coordinate is out of bounds before adding
						if (!(leftX < 0) && !(leftX > gridX - 1))
							indexes.insert(make_pair(startY, leftX));
						
						if (!(midX < 0) && !(midX > gridX - 1))
							indexes.insert(make_pair(startY, midX));
						
						if (!(rightX < 0) && !(rightX > gridX - 1))
							indexes.insert(make_pair(startY, rightX));
					}
					
					startY++;
				}
				
			}
		}
	}
	
	// Compute average cloud cover
	float sum = 0;
	for (auto coord : indexes)
	{
		sum += array2D[coord.first][coord.second].cloudCover;
	}
	float avgCloudCover = sum / indexes.size();

	// Compute average pressure
	sum = 0;
	for (auto coord : indexes)
	{
		sum += array2D[coord.first][coord.second].pressure;
	}
	float avgPressure = sum / indexes.size();

	data.push_back(avgCloudCover);
	data.push_back(avgPressure);
	return data;
}
