// EECS 281, Project 1

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

struct Point
{
    char symbol;
    char parent;
};

struct Tuple
{
    char color;
    short int x;
    short int y;
};

struct ModeTuple
{
    char c;
    string s;
};

// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *argv[]) 
{
    cout << "Usage: " << argv[0] << " [-m resize|reserve|nosize]|-h" << "\n";
    cout << "This program is to find a path in a map" << "\n";
    cout << "-help / -h" << "\n";
    cout << "-queue / -q" << "\n";
    cout << "-stack / -s" << "\n";
    cout << "-output / -o with optional arguments" << "\n";
} // printHelp()


// TODO: Finish this function, look for the individual TODO comments internally.
// Process the command line; the only thing we need to return is the mode
// when the user specifies the -m/--mode option.
ModeTuple getMode(int argc, char * argv[]) 
{
    bool modeSpecified = false;

    char mode = '\0';
    string outputtype = "map";

    // These are used with getopt_long()
    opterr = true; // Give us help with errors
    int choice;
    int option_index = 0;
    option long_options[] = {
        { "help",    no_argument,       nullptr, 'h'},
        { "queue",   no_argument,       nullptr, 'q'},
        { "stack",   no_argument,       nullptr, 's'},
        { "output",  required_argument, nullptr, 'o'},
        { nullptr, 0,                   nullptr, '\0' }
    };

    // DONE: Fill in the double quotes, to match the mode and help options.
    while ((choice = getopt_long(argc, argv, "hqso:", long_options, &option_index)) != -1) {
        switch (choice) {
            case 'h':
                printHelp(argv);
                exit(0);

            // TODO: 
            case 'q':
                // cout << "queue\n";
                modeSpecified = true;
                mode = 'q';
                break;

            // TODO: 
            case 's':
                // cout << "stack\n";
                modeSpecified = true;
                mode = 's';
                break;

            // TODO: 
            case 'o':
                outputtype = optarg;
                if((outputtype != "list") && (outputtype != "map"))
                {
                    cerr << "Error: invalid output type" << "\n";
                    exit(1);
                }
                break;

            default:
                cerr << "Error: invalid option" << "\n";
                exit(1);
        } // switch
    } // while

    ModeTuple modetuple = ModeTuple{mode, outputtype};

    if (!modeSpecified) {
        cerr << "Error: no mode specified" << "\n";
        exit(1);
    } // if

    return modetuple;
} // getMode()



int colorToInt(char c)
{
    if(c == '^')
    {
        return 0;
    }
    else
    {
        return ((int)c) - ((int)'a') + 1;    
    }
}

char intToColor(int i)
{
    if(i == 0)
    {
        return '^';
    }
    else
    {
        return (char)(((int)'a') + i - 1);
    }
}

char intToColorUpper(int i)
{
    if(i == 0)
    {
        return '^';
    }
    else
    {
        return (char)(((int)'A') + i - 1);
    }
}


bool isTrapOrButton(char c)
{
    if(c == '^')
    {
        return true;
    }
    else if((((int)c) - ((int)'a') >= 0) && (((int)'z') - ((int)c) >= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isClosedDoor(char c)
{
    if((((int)c) - ((int)'A') >= 0) && (((int)'Z') - ((int)c) >= 0))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isSymbol(char c)
{
    if((c == '@') || (c == '?') || (c == '.') || (c == '#'))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isRoot(char c, int x, int y, vector<vector<vector<Point>>>& puzzle)
{
    if(c == '^' && puzzle[colorToInt(c)][x][y].symbol == '@')
    {
        return true;   
    }
    else
    {
        return false;
    }
}

void printPuzzle(vector<vector<vector<Point>>>& puzzle)
{
    /*
	vector<vector<vector<Point>>>::iterator color;
    vector<vector<Point>>::iterator row;
    vector<Point>::iterator col;
    int coloridx = 0;

    Tuple initialstate = pathstack.back();
    Tuple finalstate = pathstack.front();
	
	for (vector<Tuple>::reverse_iterator i = pathstack.rbegin(); i != pathstack.rend(); ++i) 
	{
		Tuple nextstate = *i;
        if(!((nextstate.color == initialstate.color) && (nextstate.x == initialstate.x) && (nextstate.y == initialstate.y)) 
            && !((nextstate.color == finalstate.color) && (nextstate.x == finalstate.x) && (nextstate.y == finalstate.y)))
        {
            puzzle[colorToInt(nextstate.color)][nextstate.x][nextstate.y].symbol = '+';
        }
        if(isTrapOrButton(puzzle[colorToInt(nextstate.color)][nextstate.x][nextstate.y].parent))
        {
             puzzle[colorToInt(nextstate.color)][nextstate.x][nextstate.y].symbol = '@';
             puzzle[colorToInt(puzzle[colorToInt(nextstate.color)][nextstate.x][nextstate.y].parent)][nextstate.x][nextstate.y].symbol = '%';
        }
    } 
	*/
	vector<vector<vector<Point>>>::iterator color;
	vector<vector<Point>>::iterator row;
    vector<Point>::iterator col;
	int coloridx = 0;
	
    for(color = puzzle.begin(); color != puzzle.end(); ++color)
    {
        cout << "// color ";
        cout << intToColor(coloridx) << '\n';
		string mapbuffer; 
        for(row = color->begin(); row != color->end(); ++row)
        {
            for(col = row->begin(); col != row->end(); ++col)
            {
                // if()
                //cout << col->symbol;
			    mapbuffer += col->symbol;
            }
            //cout << '\n';
			mapbuffer += '\n';
        }
        coloridx++;
		cout << mapbuffer;
    }
}
//TODO: try one 2d vec of positions instead of 2 vecs of ints
void printUnsolvablePuzzle(int rowinput, int colinput, vector<vector<vector<Point>>>& puzzle, 
						   vector<int>& trapx, vector<int>& trapy)
{
	//vector<vector<Point>> unsolvablemap = puzzle[0];
	// missing right now
	// missing ^ cells (handled)
	// missing extra # cells for not visited (handled)
	// missing @ symbol (handled)
	
	for(vector<int>::size_type i = 0; i != trapx.size(); i++) 
	{
		puzzle[0][trapx[i]][trapy[i]].symbol = '^';
	}
	
    string mapbuffer;
    for(int row = 0; row < rowinput; ++row)
    {
        for(int col = 0; col < colinput; ++col)
        {
            //cout << unsolvablemap[row][col];
			if (puzzle[0][row][col].parent == '\0')
			{
				if (puzzle[0][row][col].symbol == '@') 
				{
					mapbuffer += '@';
				} 
				else 
				{
					mapbuffer += '#';
				}	
			} 
			else
			{
				mapbuffer += puzzle[0][row][col].symbol;
			}
			
			
        }
        //cout << '\n';
		mapbuffer += '\n';
    }
    cout << mapbuffer;
}

int main(int argc, char * argv[])
{
    ModeTuple modetuple = getMode(argc, argv);
    char mode = modetuple.c;
    string outputtype = modetuple.s;

    int colorinput;
    int rowinput;
    int colinput;
    char symbolinput;

    cin >> colorinput;
    cin >> rowinput;
    cin >> colinput;
    // TODO: error check inputs
    if(!((colorinput >= 0) && (colorinput <= 26)))
    {
        cerr << "Error: color out of range" << "\n";
        exit(1);
    }
    if(!((rowinput >= 1) && (colinput >= 1)))
    {
        cerr << "Error: rowinput or colinput out of range" << "\n";
        exit(1);
    }

    vector<vector<vector<Point>>> puzzle(colorinput+1, vector<vector<Point>>(rowinput, vector<Point>(colinput)));
    Tuple startingpoint;
	Tuple endingpoint;
	vector<int> trapx;
	vector<int> trapy;

    int numstartingpoint = 0;
    int numendingpoint = 0;

    for(int j = 0; j < rowinput; ++j)
    {
        for(int k = 0; k < colinput; ++k)
        {
            cin >> symbolinput;
            while(symbolinput == '/')
            {
                string junk;
                getline (cin, junk);
                cin >> symbolinput;
            }
            
            if(symbolinput == '@')
            {
                numstartingpoint += 1;
            }
            
            if(symbolinput == '?')
            {
                numendingpoint += 1;
            }
            

            if((!(isTrapOrButton(symbolinput))) && (!(isClosedDoor(symbolinput))) 
                && (!(isSymbol(symbolinput))))
            {
                cerr << "Error: invalid symbol" << "\n";
                exit(1);
            }
            if(colorinput == 0)
            {
                if((isClosedDoor(symbolinput)) 
                    || ((((int)symbolinput) - ((int)'a') >= 0) && (((int)'z') - ((int)symbolinput) >= 0)))
                {
                    cerr << "Error: symbol out of range" << "\n";
                    exit(1);
                }
            }
            else if(colorinput != 0)
            {
                if(isClosedDoor(symbolinput))
                {
                    if(((int)symbolinput) - ((int)'A') + 1 > colorinput)
                    {
                        cerr << "Error: wrong door" << "\n";
                        exit(1);
                    }
                }
                if(((((int)symbolinput) - ((int)'a') >= 0) && (((int)'z') - ((int)symbolinput) >= 0)))
                {
                    if(((int)symbolinput) - ((int)'a') + 1 > colorinput)
                    {
                        cerr << "Error: wrong button" << "\n";
                        exit(1);
                    }
                }
            }
            
            for(int i = 0; i < colorinput+1; ++i)
            {
                Point cell;
                cell.symbol = symbolinput;
                cell.parent = '\0';
                if(symbolinput == '@')
                {
                    startingpoint = Tuple{'^', (short int) j, (short int) k};
                    // cell.parent = 'I';
                }
                if((symbolinput == intToColorUpper(i)) || (symbolinput == intToColor(i)))
                {
                    cell.symbol = '.';
                }
                if((symbolinput == '@') && (i != 0))
                {
                    cell.symbol = '.';
                }
				if((symbolinput == '^') && (i == 0)) 
				{
				    trapx.push_back(j);
					trapy.push_back(k);
				}
                puzzle[i][j][k] = cell;
            } 
        }         
    }
    if((numstartingpoint != 1) || (numendingpoint != 1))
    {
        cerr << "Error: wrong number of startingpoint or endingpoint" << "\n";
        exit(1);
    }
    
    
    deque<Tuple> reachablecollection;
    reachablecollection.push_back(startingpoint);
    bool issolvable = false;
    Tuple S;

    while(!reachablecollection.empty())
    {
        if(mode == 'q')
        {
            S = reachablecollection.front();
            reachablecollection.pop_front();
        }
        else if(mode == 's')
        {
            S = reachablecollection.back();
            reachablecollection.pop_back();
        }
		
		if ((S.color != '^') && (puzzle[0][S.x][S.y].parent == '\0')) 
		{
		    puzzle[0][S.x][S.y].parent = '*';
		}
		
        if(puzzle[colorToInt(S.color)][S.x][S.y].symbol == '?')
        {
            issolvable = true;
			endingpoint = S;
            break;
        }
        if((S.color != puzzle[colorToInt(S.color)][S.x][S.y].symbol) 
            && (isTrapOrButton(puzzle[colorToInt(S.color)][S.x][S.y].symbol)))
        {
            if((puzzle[colorToInt(puzzle[colorToInt(S.color)][S.x][S.y].symbol)][S.x][S.y].parent == '\0') || 
			   (puzzle[colorToInt(puzzle[colorToInt(S.color)][S.x][S.y].symbol)][S.x][S.y].parent == '*'))
            {
                reachablecollection.push_back(Tuple{puzzle[colorToInt(S.color)][S.x][S.y].symbol, S.x, S.y});
                puzzle[colorToInt(puzzle[colorToInt(S.color)][S.x][S.y].symbol)][S.x][S.y].parent = S.color;
            }
            
        }
        else
        {
            if((S.x - 1 >= 0) 
                && (puzzle[colorToInt(S.color)][S.x - 1][S.y].symbol != '#')
                && (!isRoot(S.color, S.x - 1, S.y, puzzle))
                && ((puzzle[colorToInt(S.color)][S.x - 1][S.y].parent == '\0') || (puzzle[colorToInt(S.color)][S.x - 1][S.y].parent == '*'))
                && (!isClosedDoor(puzzle[colorToInt(S.color)][S.x - 1][S.y].symbol)))
            {   
                reachablecollection.push_back(Tuple{S.color, (short int)((int)S.x - 1), S.y});
                puzzle[colorToInt(S.color)][S.x - 1][S.y].parent = 'N';
            }
            if((S.y + 1 < colinput) 
                && (puzzle[colorToInt(S.color)][S.x][S.y + 1].symbol != '#')
                && (!isRoot(S.color, S.x, S.y + 1, puzzle))
                && ((puzzle[colorToInt(S.color)][S.x][S.y + 1].parent == '\0') || (puzzle[colorToInt(S.color)][S.x][S.y + 1].parent == '*'))
                && (!isClosedDoor(puzzle[colorToInt(S.color)][S.x][S.y + 1].symbol)))
            {   
                reachablecollection.push_back(Tuple{S.color, S.x, (short int)((int)S.y + 1)});
                puzzle[colorToInt(S.color)][S.x][S.y + 1].parent = 'E';
            }
            if((S.x + 1 < rowinput) 
                && (puzzle[colorToInt(S.color)][S.x + 1][S.y].symbol != '#')
                && (!isRoot(S.color, S.x + 1, S.y, puzzle))
                && ((puzzle[colorToInt(S.color)][S.x + 1][S.y].parent == '\0') || (puzzle[colorToInt(S.color)][S.x + 1][S.y].parent == '*'))
                && (!isClosedDoor(puzzle[colorToInt(S.color)][S.x + 1][S.y].symbol)))
            {   
                reachablecollection.push_back(Tuple{S.color, (short int)((int)S.x + 1), S.y});
                puzzle[colorToInt(S.color)][S.x + 1][S.y].parent = 'S';
            }
            if((S.y - 1 >= 0) 
                && (puzzle[colorToInt(S.color)][S.x][S.y - 1].symbol != '#')
                && (!isRoot(S.color, S.x, S.y - 1, puzzle))
                && ((puzzle[colorToInt(S.color)][S.x][S.y - 1].parent == '\0') || (puzzle[colorToInt(S.color)][S.x][S.y - 1].parent == '*')) 
                && (!isClosedDoor(puzzle[colorToInt(S.color)][S.x][S.y - 1].symbol)))
            {   
                reachablecollection.push_back(Tuple{S.color, S.x, (short int)((int)S.y - 1)});
                puzzle[colorToInt(S.color)][S.x][S.y - 1].parent = 'W';
            }
        }
    }
	reachablecollection.clear();
    if(issolvable)
    {
		if(outputtype == "list")
        {
			vector<Tuple> pathstack;
			pathstack.push_back(endingpoint);
			Tuple currentstate = endingpoint;

			while((puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent != '\0') && 
				  (puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent != '*'))
			{

				char parentchar = puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent;
				if(parentchar == 'N')
				{
					currentstate = Tuple{currentstate.color, (short int)((int)currentstate.x + 1), currentstate.y};
				}
				else if(parentchar == 'E')
				{
					currentstate = Tuple{currentstate.color, currentstate.x, (short int)((int)currentstate.y - 1)};
				}
				else if(parentchar == 'S')
				{
					currentstate = Tuple{currentstate.color, (short int)((int)currentstate.x - 1), currentstate.y};
				}
				else if(parentchar == 'W')
				{
					currentstate = Tuple{currentstate.color, currentstate.x, (short int)((int)currentstate.y + 1)};
				}
				else
				{
					currentstate = Tuple{parentchar, currentstate.x, currentstate.y};
				}

				pathstack.push_back(currentstate);
			}
			for (vector<Tuple>::reverse_iterator i = pathstack.rbegin(); i != pathstack.rend(); ++i) 
			{
				cout << '(' << (*i).color << ", (" << (*i).x << ", " << (*i).y << "))" << '\n';
			}
            return 0;
        }
        else if(outputtype == "map")
        {
			Tuple currentstate = endingpoint;
			while(currentstate.color != '\0')
			{
				if(!((currentstate.color == startingpoint.color) && (currentstate.x == startingpoint.x) && (currentstate.y == startingpoint.y)) 
					&& !((currentstate.color == endingpoint.color) && (currentstate.x == endingpoint.x) && (currentstate.y == endingpoint.y)))
				{
					if (puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].symbol != '%') 
					{
						puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].symbol = '+';
					}
				}
				if(isTrapOrButton(puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent))
				{
					 puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].symbol = '@';
					 puzzle[colorToInt(puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent)][currentstate.x][currentstate.y].symbol = '%';
				}
				
				char parentchar = puzzle[colorToInt(currentstate.color)][currentstate.x][currentstate.y].parent;
				if(parentchar == 'N')
				{
					currentstate = Tuple{currentstate.color, (short int)((int)currentstate.x + 1), currentstate.y};
				}
				else if(parentchar == 'E')
				{
					currentstate = Tuple{currentstate.color, currentstate.x, (short int)((int)currentstate.y - 1)};
				}
				else if(parentchar == 'S')
				{
					currentstate = Tuple{currentstate.color, (short int)((int)currentstate.x - 1), currentstate.y};
				}
				else if(parentchar == 'W')
				{
					currentstate = Tuple{currentstate.color, currentstate.x, (short int)((int)currentstate.y + 1)};
				}
				else if((parentchar == '\0') || (parentchar == '*'))
				{
					currentstate = Tuple{'\0', (short int)-1, (short int)-1};
				}
				else
				{
					currentstate = Tuple{parentchar, currentstate.x, currentstate.y};
				}
			}
			printPuzzle(puzzle);
            return 0;
        }
    }
    else
    {
        cout << "No solution." << '\n';
        cout << "Reachable:" << '\n';
        printUnsolvablePuzzle(rowinput, colinput, puzzle, trapx, trapy);
        return 0;
    }
    return 0;
}
