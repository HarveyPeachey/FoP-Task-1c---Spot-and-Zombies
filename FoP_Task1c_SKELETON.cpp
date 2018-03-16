//include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
#include <ctime>
#include <time.h>

using namespace std;

//include our own libraries
#include "RandomUtils.h"    //for Seed, Random
#include "ConsoleUtils.h"	//for Clrscr, Gotoxy, etc.

//---------------------------------------------------------------------------
//----- define constants
//---------------------------------------------------------------------------

//defining the size of the grid
const int  SIZEX(22);    	//horizontal dimension
const int  SIZEY(15);		//vertical dimension
							//defining symbols used for display of the grid and content
const char SPOT('@');   	//spot
const char TUNNEL(' ');    	//tunnel
const char WALL('#');    	//border
const char HOLE('0');		//hole
const char PILL('*');
//defining the command letters to move the spot on the maze
const int  UP(72);			//up arrow
const int  DOWN(80); 		//down arrow
const int  RIGHT(77);		//right arrow
const int  LEFT(75);		//left arrow
							//defining the other command letters
const char QUIT('Q');		//to end the game
const char FREEZE_ZOMBIE('F');
const char KILL_ZOMBIE('X');

struct Item {
	int x, y;
	char symbol;
};

struct Stats
{
	string name;
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void EntryScreen();
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot);
	void paintGame(const char g[][SIZEX], string mess);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot);
	void endProgram();

	//local variable declarations 
	char grid[SIZEY][SIZEX];			//grid for display
	char maze[SIZEY][SIZEX];			//structure of the maze

	Item spot = { 0, 0, SPOT }; 		//spot's position and symbol


	string message("LET'S START...");	//current message to player

	string rem_lives("REMAINING LIVES: ");
	string rem_zombies("ZOMBIES LEFT: ");
	string rem_pills("PILLS LEFT: ");



	//action...
	Seed();								//seed the random number generator
										//SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	EntryScreen();
	initialiseGame(grid, maze, spot);	//initialise grid (incl. walls and spot)
	paintGame(grid, message);			//display game info, modified grid and messages
	int key;							//current key selected by player

	do {
		//TODO: command letters should not be case sensitive
		key = getKeyPress(); 	//read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			updateGameData(grid, spot, key, message);		//move spot in that direction
			updateGrid(grid, maze, spot);					//update grid information
		}
		else
			message = "INVALID KEY!";	//set 'Invalid key' message
		paintGame(grid, message);		//display game info, modified grid and messages
	} while (!wantsToQuit(key));		//while user does not want to quit
	endProgram();						//display final message

	return 0;
}


//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot)
{ //initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b);

	setInitialMazeStructure(maze);		//initialise maze
	setSpotInitialCoordinates(spot);
	updateGrid(grid, maze, spot);		//prepare grid
}

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
  //TODO: Spot should not spwan on inner walls
	spot.y = Random(SIZEY - 2);      //vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = Random(SIZEX - 2);      //horizontal coordinate in range [1..(SIZEX - 2)]
}

void setInitialMazeStructure(char maze[][SIZEX])
{ //set the position of the walls in the maze
  //TODO: initial maze configuration should be amended (size changed and inner walls removed)
  //initialise maze configuration
	char initialMaze[SIZEY][SIZEX] 	//local array to store the maze structure
		= {
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
			{ '#', 1, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 1, '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
			{ '#', 1, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 1, '#' },
			{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' }
	};
	//with '#' for wall, ' ' for tunnel, etc. 
	//copy into maze structure with appropriate symbols
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				//not a direct copy, in case the symbols used change
			case '#':
				maze[row][col] = WALL;
				break;
			case ' ': maze[row][col] = TUNNEL;
				break;
			case '0': maze[row][col] = HOLE;
				break;
			case '*': maze[row][col] = PILL;
				break;
			case 1: maze[row][col] = 'Z';
			}

	//generate holes 
	int k;
	int j;
	int max_size_holes = 12;
	int holeCount = 0;

	while (holeCount < max_size_holes)
	{

		k = rand() % SIZEY + 1;
		j = rand() % SIZEX + 1;

		switch (maze[k][j])
		{
		case TUNNEL:
			maze[k][j] = HOLE;
			holeCount = holeCount + 1;
			break;
		case WALL:
			maze[k][j] = WALL;
			break;
		case HOLE:
			maze[k][j] = HOLE;
			break;
		case 'Z':
			maze[k][j] = 'Z';
			break;
		};

	}

	//generate pills
	int a;
	int b;
	int max_size_pills = 8;
	int pillCount = 0;

	while (pillCount < max_size_pills)
	{

		a = rand() % SIZEY + 1;
		b = rand() % SIZEX + 1;

		switch (maze[a][b])
		{
		case TUNNEL:
			maze[a][b] = PILL;
			pillCount = pillCount + 1;
			break;
		case WALL:
			maze[a][b] = WALL;
			break;
		case HOLE:
			maze[a][b] = HOLE;
			break;
		case 'Z':
			maze[a][b] = 'Z';
			break;

		};

	}
}

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item spot)
{ //update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item spot);

	setMaze(grid, maze);	//reset the empty maze configuration into grid
	placeItem(grid, spot);	//set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{ //reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item item)
{ //place item at its new position in grid

	g[item.y][item.x] = item.symbol;

}

//---------------------------------------------------------------------------
//----- move items on the grid
//---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess)
{ //move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	assert(isArrowKey(key));

	//reset message to blank
	mess = "                                         ";		//reset message to blank

															//calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	//check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{			//...depending on what's on the target position in grid...
	case TUNNEL:		//can move
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		break;
	case WALL:  		//hit a wall and stay there
						//TODO: remove alarm when bumping into walls - too annoying
		cout << '\a';	//beep the alarm
		mess = "CANNOT GO THERE!";
		break;
	case HOLE:
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		mess = "SPOT FELL INTO A HOLE";
		break;
	case PILL:
		spot.y += dy;	//go in that Y direction
		spot.x += dx;	//go in that X direction
		cout << '\a';	//beep the alarm
		mess = "SPOT ATE A PILL!";
		break;
	}
}
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{ //calculate direction indicated by key
  //TODO: UP and DOWN arrow keys should work
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	//...depending on the selected key...
	{
	case LEFT:  	//when LEFT arrow pressed...
		dx = -1;	//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	//when RIGHT arrow pressed...
		dx = +1;	//increase the X coordinate
		dy = 0;
		break;
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
	}
}

int getKeyPress()
{ //get key or command (in uppercase) selected by user
  //KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224) 		//ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		//return it in uppercase 
}

bool isArrowKey(const int key)
{	//check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	//TODO: UP and DOWN arrow keys should be detected
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}
bool wantsToQuit(const int key)
{	//check if the user wants to quit (when key is 'Q' or 'q')
	return toupper(key) == QUIT;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------

void EntryScreen()

{
	string filename;
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	showMessage(clDarkGrey, clYellow, 10, 5, "--------------------");
	showMessage(clDarkGrey, clYellow, 10, 6, "| SPOT AND ZOMBIES |");
	showMessage(clDarkGrey, clYellow, 10, 7, "--------------------");

	showMessage(clDarkGrey, clYellow, 10, 10, "   CS3 GROUP 3 - 2017/18   ");
	showMessage(clDarkGrey, clYellow, 10, 11, "Alex Parkin-Coates 25028273");
	showMessage(clDarkGrey, clYellow, 10, 12, "Harvey Peachy      27040030");
	showMessage(clDarkGrey, clYellow, 10, 13, "Micheal Turner     00000000");

	showMessage(clDarkGrey, clYellow, 40, 6, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clDarkGrey, clYellow, 40, 7, "TO FREEZE ZOMBIES PRESS 'F' ");
	showMessage(clDarkGrey, clYellow, 40, 8, "TO KILL ZOMBIES PRESS 'X'   ");
	showMessage(clDarkGrey, clYellow, 40, 9, "TO QUIT ENTER 'Q'           ");

	showMessage(clDarkGrey, clYellow, 10, 17, "Enter name: ");

	SelectTextColour(clRed);
	if (getline(cin, filename));
	system("CLS");
	string UserName(filename.substr(0, 20));
	showMessage(clDarkGrey, clYellow, 40, 16, "CURRENT PLAYER: " + UserName);

}
string tostring(int x)
{	//convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
string tostring(char x)
{	//convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}
void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{	//display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}
void paintGame(const char g[][SIZEX], string mess)
{ //display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX]);
	//TODO: Change the colour of the messages
	//display game title
	showMessage(clDarkGrey, clYellow, 0, 0, "___SPOT AND ZOMBIES GAME___");
	//TODO: Date and time should be displayed from the system
	string dateInWords();
	string dateInNumbers();
	string thetime();
	showMessage(clDarkGrey, clYellow, 40, 0, "CS3 Group 3: " + dateInWords());
	showMessage(clDarkGrey, clYellow, 40, 1, "DATE: " + dateInNumbers());
	showMessage(clDarkGrey, clYellow, 40, 2, "TIME: " + thetime());


	//display menu options available
	showMessage(clDarkGrey, clYellow, 40, 6, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clDarkGrey, clYellow, 40, 7, "TO FREEZE ZOMBIES PRESS 'F' ");
	showMessage(clDarkGrey, clYellow, 40, 8, "TO KILL ZOMBIES PRESS 'X'   ");
	showMessage(clDarkGrey, clYellow, 40, 9, "TO QUIT ENTER 'Q'           ");

	//print auxiliary messages if any
	showMessage(clDarkGrey, clYellow, 0, 18, mess);	//display current message
													//display grid contents
	paintGrid(g);
}

string dateInWords()
{
	//local variable declarations

	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);
	ostringstream dateNow;
	string month;
	switch ((newtime.tm_mon) + 1)
	{
	case 1: month = "January";
		break;
	case 2: month = "February";
		break;
	case 3: month = "March";
		break;
	case 4: month = "April";
		break;
	case 5: month = "May";
		break;
	case 6: month = "June";
		break;
	case 7: month = "July";
		break;
	case 8: month = "August";
		break;
	case 9: month = "September";
		break;
	case 10: month = "October";
		break;
	case 11: month = "November";
		break;
	case 12: month = "December";
		break;

	}

	dateNow << (newtime.tm_mday) << " " << month << " " << (newtime.tm_year + 1900);

	return dateNow.str();
}

string thetime()
{
	ostringstream thisTime;
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	int hour = newtime.tm_hour;
	int minute = newtime.tm_min;
	int second = newtime.tm_sec;

	thisTime << setfill('0')
		<< setw(2) <<
		(newtime.tm_hour) << ":"

		<< setfill('0')
		<< setw(2) <<
		(newtime.tm_min) << ":"

		<< setfill('0')
		<< setw(2) <<
		(newtime.tm_sec) << endl;

	return thisTime.str();

}

string dateInNumbers()
{
	ostringstream dateinNums;
	struct tm newtime;
	time_t now = time(0);
	localtime_s(&newtime, &now);

	// print various components of tm structure.
	dateinNums <<

		setfill('0')
		<< setw(2) <<
		newtime.tm_mday << "/" <<

		setfill('0')
		<< setw(2) <<
		1 + newtime.tm_mon << "/" << (newtime.tm_year + 1900);

	return dateinNums.str();

}

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	SelectBackColour(clBlack);
	SelectTextColour(clDarkGrey);
	Gotoxy(0, 2);

	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			if (g[row][col] == SPOT)
			{
				SelectTextColour(clWhite);

				cout << g[row][col];
			}
			else if (g[row][col] == WALL)
			{
				SelectTextColour(clDarkGrey);

				cout << g[row][col];
			}
			else if (g[row][col] == PILL)
			{
				SelectTextColour(clYellow);

				cout << g[row][col];
			}
			else if (g[row][col] == HOLE)
			{
				SelectTextColour(clRed);

				cout << g[row][col];
			}
			else if (g[row][col] == 'Z')
			{
				SelectTextColour(clGreen);

				cout << g[row][col];
			}
			else
			{
				cout << g[row][col];	//output cell content
			}
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	system("cls");
	showMessage(clRed, clYellow, 15, 10, "GAME OVER! USER HAS QUIT   ");

	system("pause");	//hold output screen until a keyboard key is hit
}




