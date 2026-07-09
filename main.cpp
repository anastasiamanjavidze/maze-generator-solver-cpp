// Maze Generator and Path Validator
// This program generates a random maze using a 2D array.
// The user can move through the maze with WASD keys, avoid walls,
// track valid moves, and try to reach the exit from S to E.

#include <iostream> // for input and output streams
#include <cstdlib>  // for generating random numbers
#include <ctime>    // to make random generation different each time
#include <string>   // for using the string data type
#include <limits>   // for numeric_limits, used to clear bad input

using namespace std; // so we do not have to write std:: every time

const int MAX_SIZE = 20; // maximum size of rows and columns
const int MIN_SIZE = 5;  // minimum size of rows and columns

int maze[MAX_SIZE][MAX_SIZE]; // 2D array: 1 means wall, 0 means path
int rows, cols;               // user inputs maze size

int playerRow = 0, playerCol = 0; // player starting position
string pathHistory = "";          // stores all valid moves made by the player
int moveCount = 0;                // counts how many valid moves the player makes

// Reads one integer safely. If the user types something that isn't a
// number (or input runs out), this clears the error and discards the
// bad input instead of leaving cin stuck forever.
int readInt(const string& prompt)
{
    int value;
    cout << prompt;

    while (!(cin >> value))
    {
        if (cin.eof())
        {
            // No more input is coming at all.
            // Looping would spin forever, so exit cleanly instead.
            cout << "\nNo more input available. Exiting.\n";
            exit(0);
        }

        cin.clear();                                         // clear the fail state
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // throw away the bad line
        cout << "That's not a valid number. " << prompt;
    }

    return value;
}

// This function resets the player's position and path data.
void resetPlayer()
{
    playerRow = 0;
    playerCol = 0;
    pathHistory = "";
    moveCount = 0;
}

// This function generates a new random maze.
void generateMaze()
{
    // First, we fill the whole maze with walls.
    for (int i = 0; i < rows; i++) // i is the row index
    {
        for (int j = 0; j < cols; j++) // j is the column index
        {
            maze[i][j] = 1; // fills every cell with a wall
        }
    }

    // Creating one guaranteed valid path from the start point to the end point.
    int r = 0, c = 0; // temporary variables for the path position
    maze[r][c] = 0;  // starting point becomes a path

    while (r != rows - 1 || c != cols - 1)
    {
        int move = rand() % 2; // generates random 0 or 1: 0 = right, 1 = down

        if (move == 0 && c + 1 < cols) // move right if possible
        {
            c++;
        }
        else if (r + 1 < rows) // otherwise move down if possible
        {
            r++;
        }
        else if (c + 1 < cols) // if down is impossible, move right
        {
            c++;
        }

        maze[r][c] = 0; // carve the path by placing 0
    }

    // Adding fake paths/dead ends so the maze is not just one simple path.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (maze[i][j] == 1) // fake paths can only be created from walls
            {
                if (rand() % 100 < 15) // about 15% chance of creating a fake path
                {
                    int length = 1 + rand() % 3; // fake path length: 1, 2, or 3
                    int currentRow = i;
                    int currentCol = j;

                    for (int step = 0; step < length; step++)
                    {
                        int direction = rand() % 4; // 0 = up, 1 = down, 2 = left, 3 = right
                        int newRow = currentRow;
                        int newCol = currentCol;

                        if (direction == 0)
                        {
                            newRow--; // move up
                        }
                        else if (direction == 1)
                        {
                            newRow++; // move down
                        }
                        else if (direction == 2)
                        {
                            newCol--; // move left
                        }
                        else
                        {
                            newCol++; // move right
                        }

                        // If the fake path goes too close to the border, stop extending it.
                        if (newRow <= 0 || newRow >= rows - 1 ||
                            newCol <= 0 || newCol >= cols - 1)
                        {
                            break;
                        }

                        // If the fake path reaches the real solution path, stop extending it.
                        if (maze[newRow][newCol] == 0)
                        {
                            break;
                        }

                        maze[newRow][newCol] = 0; // create fake path
                        currentRow = newRow;
                        currentCol = newCol;
                    }
                }
            }
        }
    }

    // Reset current player data for the new maze.
    resetPlayer();
}

// This function displays the maze and the player's current position.
void displayMaze()
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (i == playerRow && j == playerCol)
            {
                cout << "P "; // shows player's current position
            }
            else if (i == 0 && j == 0)
            {
                cout << "S "; // shows start position
            }
            else if (i == rows - 1 && j == cols - 1)
            {
                cout << "E "; // shows end position
            }
            else
            {
                cout << maze[i][j] << " "; // prints 0 for path or 1 for wall
            }
        }
        cout << endl;
    }
}

// This function checks whether the player's next move is valid.
bool isValidMove(int row, int col)
{
    // Checks if the move goes outside the maze borders.
    if (row < 0 || row >= rows || col < 0 || col >= cols)
    {
        cout << "You are out of borders!\n";
        return false;
    }

    // Checks if the player hits a wall.
    if (maze[row][col] == 1)
    {
        cout << "You hit the wall :(\n";
        return false;
    }

    return true;
}

// This function validates the sequence of WASD moves entered by the user.
void validatePath(const string& moves)
{
    for (size_t i = 0; i < moves.length(); i++)
    {
        int newRow = playerRow; // new row position
        int newCol = playerCol; // new column position

        char move = moves[i]; // current move of the player

        if (move == 'W' || move == 'w')
        {
            newRow--; // move up
        }
        else if (move == 'S' || move == 's')
        {
            newRow++; // move down
        }
        else if (move == 'A' || move == 'a')
        {
            newCol--; // move left
        }
        else if (move == 'D' || move == 'd')
        {
            newCol++; // move right
        }
        else
        {
            cout << "Please press a valid key! Use only W, A, S, or D.\n";
            return;
        }

        if (!isValidMove(newRow, newCol))
        {
            return;
        }

        // Now we apply the move.
        playerRow = newRow;
        playerCol = newCol;

        pathHistory += move; // saves each valid move
        moveCount++;         // counts all valid moves made

        // Print a clear separator before each snapshot so consecutive
        // moves don't visually run together into one giant grid.
        cout << "\nMove " << moveCount << ": '" << move << "'\n";

        displayMaze(); // shows updated maze after each valid move

        // Checks if the player reached the exit.
        if (playerRow == rows - 1 && playerCol == cols - 1)
        {
            cout << "Congrats! You have reached the exit in "
                << moveCount << " moves!\n";
            return;
        }
    }

    cout << "You could not reach the exit :(\n";
}


// This function asks the user to enter a valid maze size.
void inputMazeSize()
{
    string rowPrompt = "Enter the number of rows (" + to_string(MIN_SIZE) + "-" + to_string(MAX_SIZE) + "): ";
    do
    {
        rows = readInt(rowPrompt);

        if (rows < MIN_SIZE || rows > MAX_SIZE)
        {
            cout << "Rows must be between " << MIN_SIZE << " and " << MAX_SIZE << ".\n";
        }

    } while (rows < MIN_SIZE || rows > MAX_SIZE);

    string colPrompt = "Enter the number of columns (" + to_string(MIN_SIZE) + "-" + to_string(MAX_SIZE) + "): ";
    do
    {
        cols = readInt(colPrompt);

        if (cols < MIN_SIZE || cols > MAX_SIZE)
        {
            cout << "Columns must be between " << MIN_SIZE << " and " << MAX_SIZE << ".\n";
        }

    } while (cols < MIN_SIZE || cols > MAX_SIZE);
}

int main()
{
    srand(static_cast<unsigned int>(time(0))); // random seed, called once at the start

    inputMazeSize(); // asks user for maze size
    generateMaze();  // generates maze with chosen size
    displayMaze();   // displays the maze

    int choice = 0;

    do
    {
        choice = readInt("\nMenu:\n"
            "1. Generate new maze\n"
            "2. Change maze size\n"
            "3. Input and validate path\n"
            "4. Show current position\n"
            "5. Show path history\n"
            "6. Quit\n"
            "Choice: ");

        switch (choice)
        {
        case 1:
            generateMaze(); // creates new maze with same size
            displayMaze();  // displays it
            break;

        case 2:
            inputMazeSize(); // asks for new dimensions
            generateMaze();
            displayMaze();
            break;

        case 3:
        {
            string moves;
            cout << "Enter WASD moves: ";
            cin >> moves;
            cout << endl;
            validatePath(moves);
            break;
        }

        case 4:
            cout << "Current position: ("
                << playerRow << ", " << playerCol << ")\n";
            break;

        case 5:
            cout << "Path history: " << pathHistory << endl;
            cout << "Moves made: " << moveCount << endl;
            break;

        case 6:
            cout << "Goodbye!\n";
            break;

        default:
            cout << "Invalid menu choice. Please choose a number from 1 to 6.\n";
            break;
        }

    } while (choice != 6);

    return 0; // returning 0 means the program ended successfully
}