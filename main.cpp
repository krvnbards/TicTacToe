#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <thread>
#include <limits>
#include <climits>
#include <cstdlib>
#include <ctime> 
#include <mutex>

#define SPACE "                    "
#define SPACE2 "     "

using namespace std;


//[--------------------------VARIABLES---------------------------]
// Gamit ta ug char diri para inig cout kay char mo gawas
// Plano pud nako nga naay option nga unsay character or symbol ang ganahan for player
// For now ana lang sa wala paman ta ka code sa gameplay jud


char board[3][3] = {
	{' ', ' ', ' '},
	{' ', ' ', ' '},
	{' ', ' ', ' '}
};

char legendBoard[3][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}
};

int currentPlayer;

char marker[] = {'O', 'X'};

char playerSymbols[2]; // changed marker to this para sa SettingsMenu()


int firstPlayer = 2;


// Game Settings
int GAME_MODE = -1; // 1 = PvP 2 = PvE
int GAME_DIFFICULTY = 0;
string PlayerName[2];
int GAME_ROUNDS = -1;
int currentRound = 1;
int playerScore[2];
int autoPause = 0; // Pause = 0 Pause = 1


string boardColor; // Default white
string backgroundColor; // Default black background
string colo = "color ";
string finalCombined = colo + backgroundColor + boardColor;
const char *charColor = finalCombined.c_str();

bool resetProgress = false;


// KERVIN: Save the player state
enum stat_list {
	MAINMENU = 1,
	HOW_TO_PLAY,
	DEVELOPERS,
	SETTINGS,
	EXITGAME,
	GAME_SELECT_MODE,
	GAME_ENTER_NAME1,
	GAME_ENTER_NAME2,
	GAME_SELECT_ROUNDS,
	GAME_SELECT_DIFFICULTY,
	GAME_STARTED,
	GAME_END
};

// KERVIN: Inialize para mo start sya sa main menu
enum stat_list STATUS = MAINMENU;
//[--------------------------------------------------------------]

//[----------------------------FUNCTION DECLARATIONS----------------------------]
void DisplayMenu(); // Display Menu
void TicTacToeArt(); // TicTacToe Logo
void StartGame();
void SelectMode();
void SelectRounds();
void DisplayBoard();
void clearBoard();

void roundEnd(int winner);

void How_To_Play();
void DevelopersPage();
void SettingsMenu();
void ExitGame();
void DevelopersArt();
bool placeMarker(int slot);
bool isDrawPossible();

void Winner(int winner);

void ResetGameSettings();
void Reset();

int checkWinner();
void Pause(int ms);
void clearKeyboardBuffer();

void aiEasyMove();
int evaluate();
bool availableMoves();
int minimax(int depth, bool isMaximizing);
void aiMiniMax();

void saveSettings(const std::string& boardColor, const std::string& backgroundColor, char playerSymbols[2], const std::string& filename);
void loadSettings(std::string& boardColor, std::string& backgroundColor, char playerSymbols[2], const std::string& filename);

std::mutex consoleMutex;

void DisplayConsoleSize();
void SetConsoleSize(int width, int height);


//[-----------------------------------------------------------------------------]
int main() {
	boardColor = "7";
	backgroundColor = "0";
	finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
	string filename = "C:/Users/Admin/Desktop/TicTacToe/settings.txt";

    // Load settings from file
    string loadedBoardColor;
    string loadedBackgroundColor;
    char loadedPlayerSymbols[2];
    loadSettings(loadedBoardColor, loadedBackgroundColor, loadedPlayerSymbols, filename);
    
    boardColor = loadedBoardColor;
	backgroundColor = loadedBackgroundColor;
	finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
	
	playerSymbols[0] = loadedPlayerSymbols[0];
	playerSymbols[1] = loadedPlayerSymbols[1];
    
	system(charColor);
	srand(time(0));
	ResetGameSettings();
	while(1) {
		switch(STATUS) {
			case MAINMENU: {
				DisplayMenu();
				break;
			}
			case GAME_SELECT_MODE: {
				SelectMode();
				break;
			}
			case GAME_SELECT_DIFFICULTY: {
				system("cls");
				TicTacToeArt();

				int choice;
				while(GAME_DIFFICULTY < 1) {
					cout << setw(50) << "\n" SPACE "Please select the level of difficulty for Computer:\n\n" SPACE "[1] Easy\n" SPACE "[2] Medium (not available)\n" SPACE "[3] Hard\n" SPACE "Enter your choice: ";
					cin >> choice;
					
					if(choice < 1 || choice > 3 || cin.fail()) {
						GAME_DIFFICULTY = 0;
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						continue;
					}
					
					GAME_DIFFICULTY = choice;
					STATUS = GAME_ENTER_NAME1;
						
				}
				break;
			}
			case GAME_ENTER_NAME1: {
				system("cls");
				TicTacToeArt();

				cout << endl << endl;
			 	cout << SPACE " ________________________________________________________________________ " << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|                   Please enter name for Player 1                       |" << endl;
			    cout << SPACE "|________________________________________________________________________|" << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|________________________________________________________________________|" << endl;
			    
			    
			    cout << "\033[2A";  
			    cout << "\033[46C"; 
				//cin >> PlayerName[0];
			    getline(cin >> ws, PlayerName[0]);
			    
			    //cin >> PlayerName[0];

				if(GAME_MODE == 1) {
					STATUS = GAME_ENTER_NAME2;
				}
				else {
					PlayerName[1] = "Computer";
					STATUS = GAME_SELECT_ROUNDS;
				}
				break;
			}
			case GAME_ENTER_NAME2: {
				system("cls");
				TicTacToeArt();

				cout << setw(50) << "\n\n";
				cout << endl << endl;
			 	cout << SPACE " ________________________________________________________________________ " << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|                   Please enter name for Player 2                       |" << endl;
			    cout << SPACE "|________________________________________________________________________|" << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|                                                                        |" << endl;
			    cout << SPACE "|________________________________________________________________________|" << endl;
			    
			    
			    cout << "\033[2A";  
			    cout << "\033[46C"; 
				//cin >> PlayerName[0];
			    getline(cin >> ws, PlayerName[1]);

				STATUS = GAME_SELECT_ROUNDS;
				break;
			}
			case GAME_SELECT_ROUNDS: {
				SelectRounds();
				break;
			}
			case GAME_STARTED: {
				system("cls");
				TicTacToeArt();
				StartGame();
				break;
			}
			case DEVELOPERS: {
				DevelopersPage();
				break;
			}
			case HOW_TO_PLAY: {
				How_To_Play();
				break;
			}
			case SETTINGS: {
				SettingsMenu();
				break;
			}
			case EXITGAME: {
                ExitGame();
                break;
            }
            case GAME_END: {
            	char choice;
            	while(1) {
            		system("cls");
	            	TicTacToeArt();
	            	cout << endl << setw(70) << "Note: Inputting No will bring you back to the main menu." << endl;
	            	cout << setw(48) << "Do you want to play again? (Y/N): ";
	            	cin >> choice;

	            	if(choice == 'Y' || choice == 'y') {
	            		ResetGameSettings();
	            		clearBoard();
	            		STATUS = GAME_STARTED;
		           		//StartGame();
						Pause(1000);
						//cin.ignore(numeric_limits<streamsize>::max(), '\n');
						break;
					}
					else if(choice == 'N' || choice == 'n') {
						cout << setw(55) << "Returning to main menu... Please wait.." << endl;
						//cin.ignore(numeric_limits<streamsize>::max(), '\n');
						Pause(2000);
						Reset();
						STATUS = MAINMENU;
						break;
					}

					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				break;
			}
		}
		//resizeThread.join();
	}

	return 1;
}

//[----------------------------FUNCTION DEFINITIONS----------------------------]

void Reset() {
	GAME_ROUNDS = -1;
	GAME_MODE = -1;
	GAME_DIFFICULTY = 0;
	clearBoard();
	ResetGameSettings();
}

void aiEasyMove() {
    int empty[9][2]; 
    int count = 0;

    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') { 
                empty[count][0] = i;
                empty[count][1] = j;
                count++;
            }
        }
    }

    
    if (count > 0) {
        int randomIndex = rand() % count; 
        int row = empty[randomIndex][0];
        int col = empty[randomIndex][1];

        board[row][col] = playerSymbols[1]; 
    }
}

#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>


void DisplayConsoleSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int prevWidth = 0, prevHeight = 0;

    while (true) {
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if (consoleWidth != prevWidth || consoleHeight != prevHeight) {
            std::lock_guard<std::mutex> lock(consoleMutex);

            // Save current cursor position
            CONSOLE_SCREEN_BUFFER_INFO currentInfo;
            GetConsoleScreenBufferInfo(hConsole, &currentInfo);
            COORD savedPosition = currentInfo.dwCursorPosition;

            // Display console size at the top
            COORD sizePosition = {0, 0};
            SetConsoleCursorPosition(hConsole, sizePosition);
            //std::cout << "Console Size: " << consoleWidth << "x" << consoleHeight << "     ";
            //exit(0);

            // Restore cursor position
            SetConsoleCursorPosition(hConsole, savedPosition);

            prevWidth = consoleWidth;
            prevHeight = consoleHeight;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void SetConsoleSize(int width, int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { (SHORT)width, (SHORT)height };
    SMALL_RECT windowSize = { 0, 0, (SHORT)(width - 1), (SHORT)(height - 1) };

    // Set the screen buffer size
    if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) {
        std::cerr << "Failed to set buffer size. Error: " << GetLastError() << std::endl;
        return;
    }

    // Set the window size
    if (!SetConsoleWindowInfo(hConsole, TRUE, &windowSize)) {
        std::cerr << "Failed to set window size. Error: " << GetLastError() << std::endl;
        return;
    }

    std::cout << "Console size set to " << width << "x" << height << std::endl;
}

void saveSettings(const std::string& boardColor, const std::string& backgroundColor, char playerSymbols[2], const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << boardColor << std::endl;
        file << backgroundColor << std::endl;
        file << playerSymbols[0] << std::endl;
        file << playerSymbols[1] << std::endl;
        std::cout << "Settings saved successfully!" << std::endl;
    } else {
        std::cerr << "Unable to open file for saving." << std::endl;
    }
}

void loadSettings(std::string& boardColor, std::string& backgroundColor, char playerSymbols[2], const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, boardColor);
        std::getline(file, backgroundColor);
        file >> playerSymbols[0];
        file >> playerSymbols[1];
        std::cout << "Settings loaded successfully!" << std::endl;
    } else {
        std::cerr << "Unable to open file for loading." << std::endl;
    }
}

void Winner(int winner) {
	int time=3000;
	while(time > 0)
	{
		system("cls");
		TicTacToeArt();

		cout << endl << setw(40) << PlayerName[winner] << " has won the overall rounds with a score of " << playerScore[winner] << "!" << endl;

		cout << setw(55) << "SCORE BREAKDOWN" << endl;
		cout << setw(50) << PlayerName[0] << " - " << playerScore[0] << endl;
		cout << setw(50) << PlayerName[1] << " - " << playerScore[1] << endl;
		cout << setw(50) << "Please wait " << time/1000 << " seconds..." << endl;
		Sleep(1000);
		clearKeyboardBuffer();
		time -= 1000;
	}

	STATUS=GAME_END;
	//system("pause");
}

void Pause(int ms) {

	if(autoPause == 1) {
		system("pause");
	} else {
		Sleep(ms); 
	    clearKeyboardBuffer();
	}
}

void roundEnd(int winner) {

	cout << setw(40) << PlayerName[winner] << " wins the round!" << endl;
	currentRound++;
	playerScore[winner]++;

	if (playerScore[0] > playerScore[1] && (playerScore[0] > GAME_ROUNDS/2)) {
		Pause(1500);
		Winner(0);
		//clearBoard();
		return;
	}
    else if (playerScore[1] > playerScore[0] && (playerScore[1] > GAME_ROUNDS/2)) {
    	Pause(1500);
    	Winner(1);
    	//clearBoard();
    	return;
	}
    //else if(playerScore[1] == playerScore[0] && currentRound == GAME_ROUNDS) cout << "The game ends in a tie!\n";

	clearBoard();

	//system("pause");
	Pause(2000);
	StartGame();
}

void clearBoard() {
	for(int i = 0; i < 3; i++) {
		for(int h = 0; h < 3; h++) {
			board[i][h] = ' ';
		}
	}
}

void TicTacToeArt() {
	string TicTacToeArt = R"(

  _____  _       _____            _____            
 |_   _|(_)  ___|_   _|__ _   ___|_   _|___    ___ 
   | |  | | / __| | | / _` | / __| | | / _ \  / _ \
   | |  | || (__  | || (_| || (__  | || (_) ||  __/
   |_|  |_| \___| |_| \__,_| \___| |_| \___/  \___|
                                                   

	)";

	istringstream stream(TicTacToeArt);
    string line;
    while (getline(stream, line)) {
        std::cout << std::setw(79) << line << std::endl; // Adjust 50 for alignment
    }

	cout << setw(55) << "C++ 2025" << endl;

	cout << "________________________________________________________________________________________________________________________\n";
}

int evaluate() {
    char p1 = playerSymbols[0]; 
    char p2 = playerSymbols[1]; 

    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == p2) return 1;  
            if (board[i][0] == p1) return -1; 
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == p2) return 1;
            if (board[0][i] == p1) return -1;
        }
    }

    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == p2) return 1;
        if (board[0][0] == p1) return -1;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == p2) return 1;
        if (board[0][2] == p1) return -1;
    }

    return 0;
}

bool availableMoves() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return true;
        }
    }
    return false;
}

int minimax(int depth, bool isMaximizing) {
    int score = evaluate();

    
    if (score == 1) return 10 - depth;
    
    if (score == -1) return depth - 10;
    
    if (!availableMoves()) return 0;

    if (isMaximizing) { // AI's turn
        int best = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') { 
                    board[i][j] = playerSymbols[1]; 
                    best = max(best, minimax(depth + 1, false));
                    board[i][j] = ' '; 
                }
            }
        }
        return best;
    } else { 
        int best = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = playerSymbols[0]; 
                    best = min(best, minimax(depth + 1, true));
                    board[i][j] = ' '; 
                }
            }
        }
        return best;
    }
}

void aiMiniMax() {
    int bestVal = INT_MIN;
    int bestRow = -1, bestCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = playerSymbols[1]; 
                int moveVal = minimax(0, false);
                board[i][j] = ' '; 

                if (moveVal > bestVal) {
                    bestRow = i;
                    bestCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        board[bestRow][bestCol] = playerSymbols[1]; // AI places best move
    }
}

void StartGame() {

	DisplayBoard();
	if(firstPlayer == 1)
		currentPlayer = 2;
	else
		currentPlayer = 1;

	int slot;
	firstPlayer = currentPlayer;
	int confirm = 0;
	
	//if(currentPlayer == 1 && GAME_MODE == 1)
	
	for(int i = 0; i < 9; i++) 
	{
		if(GAME_MODE == 2 && currentPlayer == 2) {
			cout << endl <<  setw(21) << PlayerName[currentPlayer-1] << " (" << playerSymbols[currentPlayer-1] << ") turn to move.. please wait.." << endl;
			Pause(1500);
			if(GAME_DIFFICULTY == 1)
				aiEasyMove();
			else if(GAME_DIFFICULTY == 3)
				aiMiniMax();
				
			DisplayBoard();

	        int winner = checkWinner();
	        if(winner != -1) {
	        	roundEnd(winner);
	        	return;
			}
	
			if (isDrawPossible()) {
				clearBoard();
				//StartGame();
	            cout << "" SPACE "This round ends in a draw! No more possible wins.\n";
	            Pause(3000);
	            return;
	    	}
	
			currentPlayer = 1;
	        continue;
		}
		
		cout << endl <<  setw(21) << PlayerName[currentPlayer-1] << " (" << playerSymbols[currentPlayer-1] << ") turn to play, place to mark slot (1-9): ";
		cin >> slot;


		if (slot < 0 || slot > 9 || !placeMarker(slot) || cin.fail()) {
			
			if(slot == 0) {
				char choice;
				while(choice != 'y' || choice != 'Y' || choice != 'N' || choice != 'n') {
					
					cout << SPACE "Do you want to return to the main menu? (y/n): ";
					cin >> choice;
					
					if(choice == 'y') {
						cout << SPACE "Returning to main menu! Please wait..." << endl;
						confirm = 1;
						break;
					}
					else if(choice == 'n') {
						cout << SPACE "Okay... going back to the game.. please wait...";
			            cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
			            i--; // Retain the turn
			            Pause(1500);
			            break;
					}
				}
				if(choice == 'y' || choice == 'Y')
					break;
				else continue;
			}
			
            cout << setw(39) << "Invalid move! Try again.\n";
            cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
            i--; // Retain the turn

            Pause(1000);
            continue;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');


        DisplayBoard();

        int winner = checkWinner();
        if(winner != -1) {
        	roundEnd(winner);
        	return;
		}

		if (isDrawPossible()) {
			if(currentPlayer == 1) currentPlayer = 2;
        	else currentPlayer = 1;

			clearBoard();
			//StartGame();
            cout << "\n" SPACE "This round ends in a draw! No more possible wins.\n";
            Pause(3000);
            return;
    	}

		if(currentPlayer == 1) currentPlayer = 2;
        else currentPlayer = 1;
	}
	
	if(confirm == 1) {
    	Pause(2000);
		Reset();
		ResetGameSettings();
		STATUS = MAINMENU;
		return;	
	}
}

void ResetGameSettings() {
	currentRound = 1;
	playerScore[0] = 0;
	playerScore[1] = 0;
}

void SelectMode() {
	while(GAME_MODE < 0) {
		system("cls");
		TicTacToeArt();
		cout << "\n                    [0] Back\n                    [1] Player vs Player\n                    [2] Player vs Computer\n\n                    Enter your choice: ";
		cin >> GAME_MODE;


		if(GAME_MODE < 1 || GAME_MODE > 2 || cin.fail()) {
			if(GAME_MODE == 0) break;
			GAME_MODE = -1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

	if(GAME_MODE == 0) {
		GAME_MODE = -1;
		STATUS = MAINMENU;
		return;
	}
	
	if(GAME_MODE == 1)
		STATUS = GAME_ENTER_NAME1;
	if(GAME_MODE == 2)
		STATUS = GAME_SELECT_DIFFICULTY;

}

int checkWinner() {
    char p1 = playerSymbols[0]; // Player 1 symbol
    char p2 = playerSymbols[1]; // Player 2 symbol

    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == p1) return 0; // Player 1 wins
            if (board[i][0] == p2) return 1; // Player 2 wins
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == p1) return 0; // Player 1 wins
            if (board[0][i] == p2) return 1; // Player 2 wins
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == p1) return 0; // Player 1 wins
        if (board[0][0] == p2) return 1; // Player 2 wins
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == p1) return 0; // Player 1 wins
        if (board[0][2] == p2) return 1; // Player 2 wins
    }
    return -1; // No winner yet
}


void SelectRounds() {
	while(GAME_ROUNDS < 0) {
		system("cls");
		TicTacToeArt();


		cout << "\n" SPACE "[1] 3 Rounds\n" SPACE "[2] 5 Rounds\n" SPACE "[3] 7 Rounds\n" SPACE "(Soon more rounds/custom)\n" SPACE "Enter your choice:";
		cin >> GAME_ROUNDS;

		if(GAME_ROUNDS < 1 || GAME_ROUNDS > 3 || cin.fail()) {
			GAME_ROUNDS = -1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch(GAME_ROUNDS) {
			case 1: GAME_ROUNDS = 3; break;
			case 2: GAME_ROUNDS = 5; break;
			case 3: GAME_ROUNDS = 7; break;
		}

		STATUS = GAME_STARTED;
	}
}

void DisplayBoard() {

	system("cls");
	TicTacToeArt();
	int confirm=0;
	cout << endl << setw(110) << "[0] Back" << endl;
	//int maxLength = max(PlayerName[0].length(), PlayerName[1].length());

	cout << setw(23) << "Score:\n";
	cout << setw(25) << right << PlayerName[0] << " - " << playerScore[0] << endl;
	cout << setw(25) << right << PlayerName[1] << " - " << playerScore[1] << endl << endl;

	cout << setw(21) << "Round: " << currentRound << " / " << GAME_ROUNDS  << setw(40) << "Board Slot" << setw(37) << "Legend" << endl;
	cout << setw(35) << "     |     |     " << setw(35) << "     |     |      " << setw(40) << "________________________" << endl; // 24
    cout << setw(20) <<"  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "  " << setw(19) <<"  " << legendBoard[0][0] << "  |  " << legendBoard[0][1] << "  |  " << legendBoard[0][2] << "    " << setw(15) << " |" <<  setw(25) << right <<  "|" << endl;

    //cout << setw(35) << "_____|_____|_____" << setw(35) << "_____|_____|_____ " << setw(16) << "|" << setw(12) << PlayerName[0] << " == " << playerS[0] <<  setw(8) << "|" << endl;
    //cout << setw(35) << "     |     |     " << setw(35) << "     |     |      " << setw(16) << "|" << setw(12) << PlayerName[1] << " == " << marker[1] <<  setw(8) << "|" << endl;

    cout << setw(35) << "_____|_____|_____" << setw(35) << "_____|_____|_____ " << setw(16) << "|" << setw(12) << PlayerName[0] << " == " << playerSymbols[0] <<  setw(8) << "|" << endl;
    cout << setw(35) << "     |     |     " << setw(35) << "     |     |      " << setw(16) << "|" << setw(12) << PlayerName[1] << " == " << playerSymbols[1] <<  setw(8) << "|" << endl;

    cout << setw(20) <<"  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "  " << setw(19) <<"  " << legendBoard[1][0] << "  |  " << legendBoard[1][1] << "  |  " << legendBoard[1][2] << "   " << setw(41) << "|________________________|" << endl;
    cout << setw(35) << "_____|_____|_____" << setw(35) << "_____|_____|_____\n";
    cout << setw(35) << "     |     |     " << setw(35) << "     |     |     \n";
    cout << setw(20) <<"  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "  " << setw(19) <<"  " << legendBoard[2][0] << "  |  " << legendBoard[2][1] << "  |  " << legendBoard[2][2] << "  \n";
    cout << setw(35) << "     |     |     " << setw(35) << "     |     |     \n";
}

void DevelopersPage() {
	system("cls");
	TicTacToeArt();
	DevelopersArt();
	cout << setw(76) << "====================================================" << endl;
	cout << setw(76) << "|                 Kervin Bardilas                  |" << endl;
	cout << setw(76) << "|                 Kendrick Lanuza                  |" << endl;
	cout << setw(76) << "|                  Jules Omambac                   |" << endl;
	cout << setw(76) << "====================================================" << endl;

	string choice;
	while (1) {
		//enter y - yes or n - no
		cout << setw(57) << endl << "See developers description(y/n): ";
		cin >> choice;

		if (choice == "Y" || choice == "y"){
			cout << setw(47) << endl << "Name:   Kervin Bardilas" << endl;
			cout << setw(35) << "Age:    Age" << endl;
			cout << setw(36) << "Gender: Male" << endl;
			cout << setw(37) << "Motto:  Motto" << endl;

			cout << setw(47) << endl <<"Name:   Kendrick Lanuza" << endl;
			cout << setw(35) << "Age:    Age" << endl;
			cout << setw(36) << "Gender: Male" << endl;
			cout << setw(37) << "Motto:  Motto" << endl;

			cout << setw(43) << endl <<"Name: Jules Omambac" << endl;
			cout << setw(32) << "Age: Age" << endl;
			cout << setw(36) << "Gender: Male" << endl;
			cout << setw(36) << "Motto: Motto" << endl;
			break;
		} else if (choice == "N" || choice == "n") {
			cout << "Returning to main menu!" << endl;
			STATUS = MAINMENU;
			break;
		} else {
			cout << setw(39) << "Invalid choice!" << endl;
		}

	}

	system("pause");
	STATUS = MAINMENU;
}

void How_To_Play() {
	system("cls");
	TicTacToeArt();
	cout << "\n" SPACE2 "How to Play Tic-Tac-Toe" << endl;
    cout << "\n" SPACE2 "1. The game is played on a 3x3 grid." << endl;
    cout << "" SPACE2 "2. Player 1 will be \"X\" and Player 2 (or Computer) will be \"O\"." << endl;
    cout << "" SPACE2 "3. Players take turns placing their mark (X or O) in an empty spot." << endl;
    cout << "" SPACE2 "4. The first player to get three of their marks in a row (vertically, horizontally, or diagonally) wins the round." << endl;
    cout << "" SPACE2 "5. If all 9 spaces are filled and no player has three in a row, the round is a draw." << endl;
    cout << "" SPACE2 "6. The game continues until all selected rounds are completed." << endl;

    cout << "\n" SPACE2 "Controls:" << endl;
    cout << "" SPACE2 "- Enter the number (1 to 9) corresponding to the space you want to mark." << endl;
    cout << "" SPACE2 "- Invalid moves (selecting an already occupied space) will result on giving you a chance to take another move." << endl;

    cout << "\n" SPACE2 "Have fun and enjoy the game!\n" << endl;
	system("pause");
	STATUS = MAINMENU;
}

void DevelopersArt() {
	string DevelopersArt =	 R"(                                            
      ___               _                           
     /   \_____   _____| | ___  _ __   ___ _ __ ___ 
    / /\ / _ \ \ / / _ | |/ _ \| '_ \ / _ | '__/ __|
   / /_/|  __/\ V |  __| | (_) | |_) |  __| |  \__ \
  /___,' \___| \_/ \___|_|\___/| .__/ \___|_|  |___/
                               |_|                  
	)";

	istringstream stream(DevelopersArt);
	string line;
	while(getline(stream, line)) {
		std::cout << std::setw(75) << line << std::endl;
	}
}

void ExitGame() {
    system("cls");
    TicTacToeArt();
    cout << setw(55) << "\nThank you for playing Tic-Tac-Toe!" << endl;
    cout << setw(55) << "\nExiting the game..." << endl;
    system("pause");
	STATUS = MAINMENU;
	exit(0);
	system("cls");
}

void ChangePlayerSymbols() { // Change symbol feature, Will check later if ok najud siya
	system("cls");
	TicTacToeArt();
    cout << setw(55) << "\nEnter symbol for Player 1: ";

    cin >> playerSymbols[0];
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << setw(55) << "\nEnter symbol for Player 2: ";
    cin >> playerSymbols[1];


    cout << setw(55) << "Player symbols updated!" << endl;
    string filename = "C:/Users/Admin/Desktop/TicTacToe/settings.txt";
    saveSettings(boardColor, backgroundColor, playerSymbols, filename);
    Pause(2000);
    STATUS=SETTINGS;
}

void ChangeBoardColor() {
    system("cls");
    TicTacToeArt();
    int colorChoice;
    cout << endl << SPACE "Choose a board color:" << endl;
    cout << SPACE "[0] Black" << endl << SPACE "[1] Soft Blush (#F2D7D9)" << endl << SPACE "[2] Pale Mint (#D5EED1)" << endl << SPACE "[3] Powder Blue (#D4E4F7)" << endl << SPACE << "[4] Lavender Mist (#E3DDF5)" << endl << SPACE "[5] Peach Fuzz (#FBE1C8)" << endl;
    cout << setw(60) << "Enter your choice (1-5): ";
    cin >> colorChoice;

    if(colorChoice < 0 || colorChoice > 5 || cin.fail()) {
        boardColor = "7";
        cout << setw(55) << "Invalid choice. Defaulting to white." << endl;
        Pause(700);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        STATUS = SETTINGS;
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch(colorChoice) {
        case 1: boardColor = "d"; break; // Soft Blush (#F2D7D9)
        case 2: boardColor = "a"; break; // Pale Mint (#D5EED1)
        case 3: boardColor = "b"; break; // Powder Blue (#D4E4F7)
        case 4: boardColor = "c"; break; // Lavender Mist (#E3DDF5)
        case 5: boardColor = "e"; break; // Peach Fuzz (#FBE1C8)
        default: boardColor = "0"; break;
    }

    cout << "\nBoard color updated!" << endl;
    finalCombined = colo + backgroundColor + boardColor;
    charColor = finalCombined.c_str();
    system(charColor);
    string filename = "C:/Users/Admin/Desktop/TicTacToe/settings.txt";
    saveSettings(boardColor, backgroundColor, playerSymbols, filename);
    Pause(3000);
    STATUS = SETTINGS;
}


void ChangeBackgroundColor() {
    system("cls");
    TicTacToeArt();
    int colorChoice;
    cout << endl << setw(55) << "Choose a board color:" << endl;
    cout << SPACE "[0] Black\n" << SPACE "[1] Dusty Mauve\n" << SPACE "[2] Deep Sage\n" << SPACE "[3] Stormy Blue\n" << SPACE "[4] Charcoal Lavender\n" << SPACE "[5] Slate Gray\n" << endl; 
    cout << setw(60) << "Enter your choice (1-5): ";
    cin >> colorChoice;

    if(colorChoice < 1 || colorChoice > 5 || cin.fail()) {
        cout << "Invalid choice. Defaulting to black." << endl;
        backgroundColor = "0";
        Pause(700);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        STATUS = SETTINGS;
        return;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch(colorChoice) {
        case 1: backgroundColor = "5"; break; // Dusty Mauve
        case 2: backgroundColor = "2"; break; // Deep Sage
        case 3: backgroundColor = "3"; break; // Stormy Blue
        case 4: backgroundColor = "6"; break; // Charcoal Lavender
        case 5: backgroundColor = "7"; break; // Muted Teal
        default: backgroundColor = "0"; break;
    }

    finalCombined = colo + backgroundColor + boardColor;
    charColor = finalCombined.c_str();
    system(charColor);
    string filename = "C:/Users/Admin/Desktop/TicTacToe/settings.txt";
    saveSettings(boardColor, backgroundColor, playerSymbols, filename);
    cout << "\nBackground color changed!" << endl;

    Pause(700);
    STATUS = SETTINGS;
}


bool isDrawPossible() {
    char p1 = playerSymbols[0];
    char p2 = playerSymbols[1];

    if (checkWinner() != -1) {
        return false; // The game is already won, no draw should be possible
    }


    // Check if there are still possible winning moves
    for (int i = 0; i < 3; i++) {
        // Check rows
        if ((board[i][0] != p1 && board[i][0] != p2) ||
            (board[i][1] != p1 && board[i][1] != p2) ||
            (board[i][2] != p1 && board[i][2] != p2)) {
            return false;  // A win is still possible
        }
        // Check columns
        if ((board[0][i] != p1 && board[0][i] != p2) ||
            (board[1][i] != p1 && board[1][i] != p2) ||
            (board[2][i] != p1 && board[2][i] != p2)) {
            return false;  // A win is still possible
        }
    }

    // Check diagonals
    if ((board[0][0] != p1 && board[0][0] != p2) ||
        (board[1][1] != p1 && board[1][1] != p2) ||
        (board[2][2] != p1 && board[2][2] != p2)) {
        return false;  // A win is still possible
    }

    if ((board[0][2] != p1 && board[0][2] != p2) ||
        (board[1][1] != p1 && board[1][1] != p2) ||
        (board[2][0] != p1 && board[2][0] != p2)) {
        return false;  // A win is still possible
    }

    return true;  // No more possible wins, declare a draw
}

void clearKeyboardBuffer() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void SettingsMenu() { // Not complete, but some progress made
    system("cls");
    TicTacToeArt();
    int choice;

    cout << "\n" SPACE2 "Settings Menu" << endl;
    cout << "" SPACE2 "1. Change Player Symbols" << endl;
    cout << "" SPACE2 "2. Change Board Color" << endl;
    cout << "" SPACE2 "3. Change Background " << endl;
    cout << "" SPACE2 "4. Exit Settings" << endl;
    cout << "\n" SPACE2 "Enter your choice: ";
    cin >> choice;

    if(choice < 1 || choice > 4 || cin.fail()) {

	    //STATUS = MAINMENU;
	    cout << "" SPACE2 "Invalid choice. Please enter a valid choice." << endl;

	    cin.clear();
	    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    Pause(800);
	    STATUS=SETTINGS;
	    return;
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1: ChangePlayerSymbols(); break;
        case 2: ChangeBoardColor(); break;
        case 3: ChangeBackgroundColor(); break;
        case 4: DisplayMenu(); break;
    }
}


void DisplayMenu() {
	system("cls");

	TicTacToeArt();

	int status=-1;
	
	string art = R"(                                             
 ___      _____ _            _____               
|_  |    |  _  | |___ _ _   |   __|___ _____ ___ 
 _| |_   |   __| | .'| | |  |  |  | .'|     | -_|
|_____|  |__|  |_|__,|_  |  |_____|__,|_|_|_|___|
                     |___|                                                                      
 ___    _____              _          _____ _         
|_  |  |  |  |___ _ _ _   | |_ ___   |  _  | |___ _ _ 
|  _|  |     | . | | | |  |  _| . |  |   __| | .'| | |
|___|  |__|__|___|_____|  |_| |___|  |__|  |_|__,|_  |
                                                 |___|                                           
 ___    ____              _                     
|_  |  |    \ ___ _ _ ___| |___ ___ ___ ___ ___ 
|_  |  |  |  | -_| | | -_| | . | . | -_|  _|_ -|
|___|  |____/|___|\_/|___|_|___|  _|___|_| |___|
                               |_|                                           
 ___    _____     _   _   _             
| | |  |   __|___| |_| |_|_|___ ___ ___ 
|_  |  |__   | -_|  _|  _| |   | . |_ -|
  |_|  |_____|___|_| |_| |_|_|_|_  |___|
                               |___|                      
 ___    _____     _ _   
|  _|  |   __|_ _|_| |_ 
|_  |  |   __|_'_| |  _|
|___|  |_____|_,_|_|_|  	          
	)";

	istringstream stream(art);
    string line;
    
    while (getline(stream, line)) {
    	ios init(NULL);
    	init.copyfmt(cout);
    	
        std::cout << " " SPACE" " << right << line << std::endl; // Adjust 50 for alignment
        
        cout.copyfmt(init);
    }


	//cout << "[1] Play Game\n[2] How to Play\n[3] Developers\n[4] Settings\n[5] Exit\n\n" << endl;
	cout << "" SPACE "   Enter your choice: ";
	cin >> status;

	if(status < 1 || status > 5 || cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		STATUS = MAINMENU;
	}

	switch(status) {
		case 1: STATUS = GAME_SELECT_MODE; break;
		case 2: STATUS = HOW_TO_PLAY; break;
		case 3: STATUS = DEVELOPERS; break;
		case 4: STATUS = SETTINGS; break;
		case 5: STATUS = EXITGAME; break;
		default: STATUS = MAINMENU; break;
	}
}

bool placeMarker(int slot) {
    int row = (slot - 1) / 3;
    int col = (slot - 1) % 3;

    if (board[row][col] == ' ') {

        board[row][col] = marker[currentPlayer-1];

        board[row][col] = playerSymbols[currentPlayer-1];

        return true;
    }
    return false;
}

//[----------------------------------------------------------------------------] 
