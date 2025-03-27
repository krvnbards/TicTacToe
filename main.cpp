#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <conio.h>
#include <thread>
#include <limits>
#include <climits>
#include <cctype>
#include <cstdlib>
#include <ctime> 
#include <mutex>

#define SPACE "                      "
#define SPACE2 "       "

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

int ENABLE_SOUNDS = 1;

int enableLoading = 0; // Disable ni para mawala ang loading sa start

string boardColor; // Default white
string backgroundColor; // Default black background
string colo = "color ";
string finalCombined = colo + backgroundColor + boardColor;
const char *charColor = finalCombined.c_str();

bool resetProgress = false;
string LOADING_FILE = "resources/settings.txt";

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
	GAME_END,
	SELECT_BOARDCOLOR,
	SELECT_BACKGROUNDCOLOR,
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

void ChangeBoardColor();
void ChangeBackgroundColor();

int tawgako(string input);

void PrintCharDelay ();
void PrintCharDelay2 ();
void PrintAnimation ();

void aiEasyMove();
int evaluate();
bool availableMoves();
int minimax(int depth, bool isMaximizing);
void aiMiniMax();

void enableANSI();
void showProgressBar(int total, int consoleWidth);
int getConsoleWidth();

void saveSettings(const std::string& boardColor, const std::string& backgroundColor, char playerSymbols[2], int& enableSounds, const std::string& filename);
bool loadSettings(std::string& boardColor, std::string& backgroundColor, char playerSymbols[2], int& enableSounds, std::string& filename);

std::mutex consoleMutex;

void DisplayConsoleSize();
void SetConsoleSize(int width, int height);


//[-----------------------------------------------------------------------------]
int main() {
	
	SetConsoleOutputCP(CP_UTF8);  
    enableANSI();                 

	if(enableLoading == 1) {
		int consoleWidth = getConsoleWidth(); // Get the console width
	    cout << "\n\n\n\n\n\n\n\n\n\n\n\n" SPACE SPACE "        \033[0mLOADING GAME...\n"; 
	    showProgressBar(100, consoleWidth);
	}
	
	string filename = LOADING_FILE;
    
    if(!loadSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename)) {
    	boardColor = "7";
		backgroundColor = "0";
		playerSymbols[0] = 'X';
		playerSymbols[1] = 'O';
		
		finalCombined = colo + backgroundColor + boardColor;
		charColor = finalCombined.c_str();
	
	    saveSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename);
	}

	finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
	
	if(ENABLE_SOUNDS)
		PlaySound(TEXT("resources/gamemusic-6082.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	
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
					cout << setw(50) << "\n" SPACE "Please select the level of difficulty for Computer:\n\n" SPACE "[1] Easy\n" SPACE "[2] Medium\n" SPACE "[3] Hard\n" SPACE "Enter your choice: ";
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
			 	cout << SPACE "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << endl;
			    cout << SPACE "█                   Please enter name for Player 1                       █" << endl;
			    cout << SPACE "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█" << endl;
			    cout << SPACE "█                                                                        █" << endl;
			    cout << SPACE "█                                                                        █" << endl;
			    cout << SPACE "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█" << endl;
			    
			    
			    cout << "\033[2A";  
			    cout << "\033[46C"; 
				//cin >> PlayerName[0];
			    getline(cin >> ws, PlayerName[0]);
			    
			    //cin >> PlayerName[0];

				if(GAME_MODE == 1) {
					STATUS = GAME_ENTER_NAME2;
				}
				else {
					if(GAME_DIFFICULTY == 1)
						PlayerName[1] = "Computer (E)";
					else if(GAME_DIFFICULTY == 2)
						PlayerName[1] = "Computer (M)";
					else if(GAME_DIFFICULTY == 3)
						PlayerName[1] = "Computer (H)";
						
					STATUS = GAME_SELECT_ROUNDS;
				}
				break;
			}
			case GAME_ENTER_NAME2: {
				system("cls");
				TicTacToeArt();

				cout << endl << endl;
			 	cout << SPACE "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << endl;
			    cout << SPACE "█                   Please enter name for Player 2                       █" << endl;
			    cout << SPACE "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█" << endl;
			    cout << SPACE "█                                                                        █" << endl;
			    cout << SPACE "█                                                                        █" << endl;
			    cout << SPACE "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█" << endl;
			    
			    
			    cout << "\033[2A";  
			    cout << "\033[46C"; 
				//cin >> PlayerName[0];
			    getline(cin >> ws, PlayerName[1]);
			    
			    if(PlayerName[0].compare(PlayerName[1]) == 0) {
			    	cout << endl << endl << SPACE "The name cannot be the same with Player 1.";
			    	Pause(1000);
			    	STATUS = GAME_ENTER_NAME2;
			    	break;
				}

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
            case SELECT_BOARDCOLOR: {
            	ChangeBoardColor();
				break;
			}
			case SELECT_BACKGROUNDCOLOR: {
				ChangeBackgroundColor();
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

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void showProgressBar(int total, int consoleWidth) {
    const int barWidth = 50; // Width of the progress bar
    int padding = (consoleWidth - barWidth - 4) / 2; // Centering spaces

    std::cout << "\033[1;0m"; // Set green color

    for (int i = 0; i <= total; ++i) {
        int pos = (i * barWidth) / total;

        std::cout << "\r" << std::string(padding, ' ') << "[";
        for (int j = 0; j < barWidth; ++j) {
            std::cout << (j < pos ? "█" : "░");
        }
        std::cout << "] " << i << "% " << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80; // Default width
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}

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

void saveSettings(const std::string& boardColor, const std::string& backgroundColor, char playerSymbols[2], int& enableSounds, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << boardColor << std::endl;
        file << backgroundColor << std::endl;
        file << playerSymbols[0] << std::endl;
        file << playerSymbols[1] << std::endl;
        file << enableSounds;
        //std::cout << "Settings saved successfully!" << std::endl;
    } else {
        //std::cerr << "Unable to open file for saving." << std::endl;
    }
}

bool  loadSettings(std::string& boardColor, std::string& backgroundColor, char playerSymbols[2], int& enableSounds, std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::getline(file, boardColor);
        std::getline(file, backgroundColor);
        file >> playerSymbols[0];
        file >> playerSymbols[1];
        file >> ENABLE_SOUNDS;
        //std::cout << "Settings loaded successfully!" << std::endl;
        return true;
    } else {
        //std::cerr << "Unable to open file for loading." << std::endl;
        return false;
    }
}

void Winner(int winner) {
	
	if(ENABLE_SOUNDS)
			PlaySound(TEXT("resources/overall-victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
		
	int time=3000;
	while(time > 0)
	{
		system("cls");
		TicTacToeArt();
			
		cout << endl << setw(40) << PlayerName[winner] << " has won the overall rounds with a score of " << playerScore[winner] << "!" << endl;

		cout << setw(55) << "SCORE BREAKDOWN" << endl;
		cout << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << endl << endl;
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
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	    clearKeyboardBuffer();
	}
}

void roundEnd(int winner) {

	if(GAME_MODE == 1) {
		if(ENABLE_SOUNDS)
			PlaySound(TEXT("resources/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
	} else {
		if(ENABLE_SOUNDS)
			if(winner == 0)
				PlaySound(TEXT("resources/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
			else
				PlaySound(TEXT("resources/lose.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	cout << endl << setw(40) << PlayerName[winner] << " wins the round!" << endl;
	
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
 ██████████████        ███████████             ███████████             
 █   ███               █   ███   █             █   ███   █             
     ███   ████  █████     ███  ██████   █████     ███  ██████  ██████ 
     ███    ███ ███  ███   ███      ███ ███  ███   ███ ███  ██████  ███
     ███    ███ ███        ███  ███████ ███        ███ ███  ██ ███████ 
     ███    ███ ███  ███   ███ ███  ███ ███  ███   ███ ███  ██ ███     
    █████  ████  ██████   ████  ███████  ██████   ████  ██████  ██████ 
                                                                       
	)";

	istringstream stream(TicTacToeArt);
    string line;
    while (getline(stream, line)) {
        std::cout << SPACE << line << std::endl; // Adjust 50 for alignment
    }

	cout << SPACE SPACE "C++ 2025" << endl;

	//cout << "________________________________________________________________________________________________________________________\n";
	cout << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n";
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

void aiMediumMove() {
	for (int i = 0; i < 3; ++i) {
          for (int j = 0; j < 3; ++j) {
              if (board[i][j] == ' ') {
                  board[i][j] = playerSymbols[1];
                  if (checkWinner() == 1) {
                      return; 
                  }
                  board[i][j] = playerSymbols[0];
                  if (checkWinner() == 0) {
                      board[i][j] = playerSymbols[1]; 
                      return;
                  }
                  board[i][j] = ' ';
              }
          }
      }
      
      if (board[1][1] == ' ') {
          board[1][1] = playerSymbols[1];
          return;
      }
      
      int corners[4][2] = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
      for (auto &corner : corners) {
          if (board[corner[0]][corner[1]] == ' ') {
              board[corner[0]][corner[1]] = playerSymbols[1];
              return;
          }
      }
      
      aiEasyMove();
}

void StartGame() {

	DisplayBoard();
	if(firstPlayer == 1)
		currentPlayer = 2;
	else
		currentPlayer = 1;

	string input;
	firstPlayer = currentPlayer;
	int confirm = 0;
	
	//if(currentPlayer == 1 && GAME_MODE == 1)
	
	for(int i = 0; i < 9; i++) 
	{
		if(GAME_MODE == 2 && currentPlayer == 2) {
			cout << endl <<  setw(21) << PlayerName[currentPlayer-1] << " (" << playerSymbols[currentPlayer-1] << ") turn to move.. please wait.." << endl;
			Pause(1500);
			if(GAME_DIFFICULTY == 1) {
				aiEasyMove();
			}
			else if(GAME_DIFFICULTY == 2) {
				aiMediumMove();
			}
			else if(GAME_DIFFICULTY == 3) {
				if (rand() % 10 < 3) {  
			    	aiEasyMove();
				} else {
				    aiMiniMax();
				}
			}
			
			if(ENABLE_SOUNDS)
				PlaySound(TEXT("resources/mark.wav"), NULL, SND_FILENAME | SND_ASYNC);
			
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
		cin >> input;
		
		bool isValid = true;
	    for (char ch : input) {
	        if (!isdigit(ch)) {
	            isValid = false;
	            break;
	        }
	    }
	    
	    if(isValid == false) {
	    	cout << setw(39) << "Invalid move! Try again.\n";
            cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
            i--; // Retain the turn

            Pause(1000);
            continue;
		}
		
		int slot;
        stringstream(input) >> slot;
        
        if(slot < 0 || slot > 9 || !placeMarker(slot)) {
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
		cout << "\n                    [0] Back\n                    [1] Player vs Player\n                    [2] Player vs Computer\n\n                    ";
		//cin >> GAME_MODE;
		string input;
		fflush(stdin);
		input = _getch();
		//FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		//cout << input;
		//system("pause");
			
		bool isValid = true;
	    for (char ch : input) {
	        if (!isdigit(ch)) {
	            isValid = false;
	            break;
	        }
	    }
	    
	    if(isValid == false) {
			if(GAME_MODE == 0) break;
			GAME_MODE = -1;
			cin.clear();
			//cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		
		int status;
    	stringstream(input) >> status;
    	
    	GAME_MODE = status;


		if(GAME_MODE < 1 || GAME_MODE > 2) {
			if(GAME_MODE == 0) break;
			GAME_MODE = -1;
			cin.clear();
			//cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		//cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    char p1 = playerSymbols[0]; 
    char p2 = playerSymbols[1]; 

    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == p1) return 0; 
            if (board[i][0] == p2) return 1; 
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == p1) return 0; 
            if (board[0][i] == p2) return 1; 
        }
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == p1) return 0; 
        if (board[0][0] == p2) return 1; 
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == p1) return 0; 
        if (board[0][2] == p2) return 1; 
    }
    return -1; // No winner yet
}


void SelectRounds() {
	while(GAME_ROUNDS < 0) {
		system("cls");
		TicTacToeArt();


		cout << "\n" SPACE "[0] Back\n" SPACE "[1] 3 Rounds\n" SPACE "[2] 5 Rounds\n" SPACE "[3] 7 Rounds\n" SPACE "(Soon more rounds/custom)\n" SPACE "Enter your choice:";
		
		string input;
		fflush(stdin);
		input = _getch();
		
		bool isValid = true;
	    for (char ch : input) {
	        if (!isdigit(ch)) {
	            isValid = false;
	            break;
	        }
	    }
	    
		if(isValid == false) {
			GAME_ROUNDS = -1;
			cin.clear();
			//cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		
		stringstream(input) >> GAME_ROUNDS;

		if(GAME_ROUNDS < 1 || GAME_ROUNDS > 3 || cin.fail()) {
			GAME_ROUNDS = -1;
			cin.clear();
			//cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		cin.clear();
		//cin.ignore(numeric_limits<streamsize>::max(), '\n');

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

	string SPACES = "         ";

	cout << SPACE << "Round: " << currentRound << " / " << GAME_ROUNDS  << setw(40) << "Board Slot" << setw(30) << "Legend" << endl << endl;
	cout << SPACE << "     █     █     " << SPACE << "     █     █      " << SPACES << "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" << endl; // 24
    cout << SPACE <<"  " << board[0][0] << "  █  " << board[0][1] << "  █  " << board[0][2] << "  " << SPACE <<"  " << legendBoard[0][0] << "  █  " << legendBoard[0][1] << "  █  " << legendBoard[0][2] << SPACES << "   █" << SPACES << SPACES << "    █" << endl;
    cout << SPACE << "▄▄▄▄▄█▄▄▄▄▄█▄▄▄▄▄" << SPACE << "▄▄▄▄▄█▄▄▄▄▄█▄▄▄▄▄ " << setw(13) << " █ " << setw(12) << PlayerName[0] << " == " << playerSymbols[0] << setw(7) << "█" << endl;
    cout << SPACE << "     █     █     " << SPACE << "     █     █      " << setw(13) << " █ " << setw(12) << PlayerName[1] << " == " << playerSymbols[1] << setw(7) << "█" << endl;
    cout << SPACE <<"  " << board[1][0] << "  █  " << board[1][1] << "  █  " << board[1][2] << "  " << SPACE <<"  " << legendBoard[1][0] << "  █  " << legendBoard[1][1] << "  █  " << legendBoard[1][2] << SPACES << "   █" << SPACES << SPACES << "    █" << endl;
    cout << SPACE << "▄▄▄▄▄█▄▄▄▄▄█▄▄▄▄▄" << SPACE << "▄▄▄▄▄█▄▄▄▄▄█▄▄▄▄▄" << SPACES << " █▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█" << endl;
    cout << SPACE << "     █     █     " << SPACE << "     █     █     \n";
    cout << SPACE <<"  " << board[2][0] << "  █  " << board[2][1] << "  █  " << board[2][2] << "  " << SPACE <<"  " << legendBoard[2][0] << "  █  " << legendBoard[2][1] << "  █  " << legendBoard[2][2] << "  \n";
    cout << SPACE << "     █     █     " << SPACE << "     █     █     \n";
}

void PrintCharDelay (string text, int delay = 50) {
	for (char ch : text) {
		cout << ch;
		Sleep(delay);
	}
	cout << endl;
}
void PrintCharDelay2 (string text, int delay = 50) {
	for (char ch : text) {
		cout << ch;
		Sleep(delay);
	}
}

void PrintAnimation(const string lines[], int size, int delay = 500) {
	for (int i = 0; i < size; i++) {
        cout << lines[i] << endl;
        this_thread::sleep_for(chrono::milliseconds(delay)); 
	}
}

void DevelopersPage() {
	system("cls");
	TicTacToeArt();
	DevelopersArt();
	const string devNames[] =  { {(SPACE SPACE2 "                                                    ")},
			  	                 {(SPACE SPACE2 "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄")},
				                 {(SPACE SPACE2 "█                                                  █") },
				                 {(SPACE SPACE2 "█                 Kervin Bardilas                  █") },
			                     {(SPACE SPACE2 "█                 Kendrick Lanuza                  █") },
			                     {(SPACE SPACE2 "█                  Jules Omambac                   █") },
			                     {(SPACE SPACE2 "█                                                  █") },
			                     {(SPACE SPACE2 "█▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄█")}};

	int devSize = sizeof(devNames) / sizeof(devNames[0]);
	PrintAnimation(devNames, devSize, 200);
	
	string choice;
	while (1) {
		//enter y - yes or n - no
		cout << endl;
		PrintCharDelay2(SPACE SPACE2 "See developers description(y/n): ");
		
		cin >> choice;
		
		if (choice == "Y" || choice == "y"){
			
			const string DEVDESCRIPTION1[] = { 
							{(SPACE SPACE2 "Age:    20")},	
						        {(SPACE SPACE2 "Gender: Male")}, 
							{(SPACE SPACE2 "Motto:  Mottotoy\n")} };  

			const string DEVDESCRIPTION2[] = { 
						        {(SPACE SPACE2 "Age:    20")},	
							{(SPACE SPACE2 "Gender: Male")}, 
							{(SPACE SPACE2 "Motto:  Mottotoy\n")} };

			const string DEVDESCRIPTION3[] = { 
							{(SPACE SPACE2 "Age:    20")},	
							{(SPACE SPACE2 "Gender: Male")}, 
							{(SPACE SPACE2 "Motto:  Mottotoy\n")} };
											  
			const string* devDescription[] = {DEVDESCRIPTION1,  DEVDESCRIPTION2, DEVDESCRIPTION3};
			int devSizes[] = {
    						sizeof(DEVDESCRIPTION1) / sizeof(DEVDESCRIPTION1[0]),
    						sizeof(DEVDESCRIPTION2) / sizeof(DEVDESCRIPTION2[0]),
   						sizeof(DEVDESCRIPTION3) / sizeof(DEVDESCRIPTION3[0]) };
   												  
			const string DEVNAMES[] = { 
						{("Name:   Kervin Bardilas")},	
						{("Name:   Kendrick Lanuza")}, 
						{("Name:   Jules Omambac")} }; 
											  
			int numDetails = sizeof(devDescription) / sizeof(devDescription[0]);
											  
			for(int i = 0; i < numDetails; i++) {
				PrintCharDelay(SPACE SPACE2 + DEVNAMES[i], 25);
				PrintAnimation(devDescription[i], devSizes[i],200);
			}
			break;
			
			} else if (choice == "N" || choice == "n") {

			cout << SPACE SPACE2 "Returning to main menu!" << endl;
			STATUS = MAINMENU;
			break;
			} else {
				cout << SPACE SPACE2 "Invalid choice!" << endl;
		}

	}

	//system("pause");
	cout << SPACE "  Press any key to continue ...";
	_getch();
	STATUS = MAINMENU;
}

void How_To_Play() {
	system("cls");
	TicTacToeArt();
	cout << "\n" SPACE "How to Play Tic-Tac-Toe" << endl;
    cout << "\n" SPACE "1. The game is played on a 3x3 grid." << endl;
    cout << "" SPACE "2. Player 1 will be \"X\" and Player 2 (or Computer) will be \"O\"." << endl;
    cout << "" SPACE "3. Players take turns placing their mark (X or O) in an empty spot." << endl;
    cout << "" SPACE "4. The first player to get three of their marks in a row" << endl;
	cout << "" SPACE "   (vertically, horizontally, or diagonally) wins the round." << endl;
    cout << "" SPACE "5. If all 9 spaces are filled and no player has three in a row, the" << endl;
    cout << "" SPACE "   round is a draw." << endl;
    cout << "" SPACE "6. The game continues until all selected rounds are completed." << endl;

    cout << "\n" SPACE "Controls:" << endl;
    cout << "" SPACE "- Enter the number (1 to 9) corresponding to the space you want to mark." << endl;
    cout << "" SPACE "- Invalid moves (selecting an already occupied space) will result on" << endl;
    cout << "" SPACE "  giving you a chance to take another move." << endl;

    cout << "\n" SPACE "Have fun and enjoy the game!\n" << endl;
	cout << SPACE "Press any key to continue ...";
	_getch();
	STATUS = SETTINGS;
}

void DevelopersArt() {
	string DevelopersArt =	 R"(                                            
 █▀▄ █▀▀ █ █ █▀▀ █   █▀█ █▀█ █▀▀ █▀▄ █▀▀  
 █ █ █▀▀ ▀▄▀ █▀▀ █   █ █ █▀▀ █▀▀ █▀▄ ▀▀█  
 ▀▀  ▀▀▀  ▀  ▀▀▀ ▀▀▀ ▀▀▀ ▀   ▀▀▀ ▀ ▀ ▀▀▀  
	)";

	istringstream stream(DevelopersArt);
	string line;
	while(getline(stream, line)) {
		std::cout << SPACE SPACE2 "     "<< line << std::endl;
	}
}

void ExitGame() {
    system("cls");
    TicTacToeArt();
    cout << endl << SPACE "Thank you for playing Tic-Tac-Toe!" << endl;
    cout << endl << SPACE << "Exiting the game..." << endl;
    cout << SPACE "Press any key to continue ...";
	_getch();
	STATUS = MAINMENU;
	exit(0);
	system("cls");
}

void ChangePlayerSymbols() { // Change symbol feature, Will check later if ok najud siya
	system("cls");
	TicTacToeArt();
    cout << SPACE << "\nEnter symbol for Player 1: ";

    cin >> playerSymbols[0];
    ////cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << SPACE << "\nEnter symbol for Player 2: ";
    cin >> playerSymbols[1];


    cout << setw(55) << "Player symbols updated!" << endl;
    string filename = LOADING_FILE;
    saveSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename);
    Pause(2000);
    STATUS=SETTINGS;
}

void ChangeBoardColor() {
    system("cls");
    TicTacToeArt();
    string input;
 
    cout << endl << SPACE "Choose a board color:" << endl;
    cout << SPACE "[0] Back" << endl << SPACE "[1] Soft Blush" << endl << SPACE "[2] Pale Mint" << endl << SPACE "[3] Powder Blue" << endl << SPACE << "[4] Lavender Mist" << endl << SPACE "[5] Peach Fuzz" << endl << SPACE "[6] Black" << endl;
   // cout << SPACE "Enter your choice (1-5): ";
   // cin >> colorChoice;
   
   input = _getch();
   int colorChoice = tawgako(input);

    if(colorChoice < 0 || colorChoice > 6 || colorChoice == -1) {
        //boardColor = "7";
        cout << setw(55) << "Invalid choice." << endl;
        Pause(700);
        cin.clear();
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        STATUS = SELECT_BOARDCOLOR;
        return;
    }

    if(colorChoice == 0) {
    	STATUS = SETTINGS;
    	return;
	}

    switch(colorChoice) {
        case 1: boardColor = "d"; break; // Soft Blush (#F2D7D9)
        case 2: boardColor = "a"; break; // Pale Mint (#D5EED1)
        case 3: boardColor = "b"; break; // Powder Blue (#D4E4F7)
        case 4: boardColor = "c"; break; // Lavender Mist (#E3DDF5)
        case 5: boardColor = "e"; break; // Peach Fuzz (#FBE1C8)
        case 6: boardColor = "0"; break;
        default: boardColor = "0"; break;
    }

    cout << endl << SPACE "Board color updated! Please wait a moment..." << endl;
    finalCombined = colo + backgroundColor + boardColor;
    charColor = finalCombined.c_str();
    system(charColor);
    string filename = LOADING_FILE;
    saveSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename);
    Pause(1500);
    STATUS = SELECT_BOARDCOLOR;
}

void ResetDisplaySettings() {
	string filename = LOADING_FILE;
	boardColor = "7";
	backgroundColor = "0";
	playerSymbols[0] = 'X';
	playerSymbols[1] = 'O';
		
	finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
	finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
	
	system(charColor);
	
	saveSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename);
	
	cout << endl << SPACE "Display settings have been reset. Please wait a moment..." << endl;
	Pause(1500);
    STATUS = SETTINGS;
}

void ChangeBackgroundColor() {
    system("cls");
    TicTacToeArt();
    string input;
    
    cout << endl << SPACE "Choose a background color:" << endl;
    cout << SPACE "[0] Back\n" << SPACE "[1] Dusty Mauve\n" << SPACE "[2] Deep Sage\n" << SPACE "[3] Stormy Blue\n" << SPACE "[4] Charcoal Lavender\n" << SPACE "[5] Slate Gray\n" << SPACE "[6] Black" << endl; 
    cout << SPACE "Enter your choice (1-5): ";
    //cin >> colorChoice;
    
    input = _getch();
    int colorChoice = tawgako(input);
    

    if(colorChoice < 0 || colorChoice > 6 || colorChoice == -1) {
        cout << "Invalid choice. " << endl;
        //backgroundColor = "0";
        Pause(700);
        cin.clear();
        STATUS = SELECT_BACKGROUNDCOLOR;
        return;
    }

    cin.clear();

	if(colorChoice == 0) {
    	STATUS = SETTINGS;
    	return;
	}

    switch(colorChoice) {
        case 1: backgroundColor = "5"; break; // Dusty Mauve
        case 2: backgroundColor = "2"; break; // Deep Sage
        case 3: backgroundColor = "3"; break; // Stormy Blue
        case 4: backgroundColor = "6"; break; // Charcoal Lavender
        case 5: backgroundColor = "7"; break; // Muted Teal
        case 6: backgroundColor = "0"; break;
        default: backgroundColor = "0"; break;
    }

    finalCombined = colo + backgroundColor + boardColor;
    charColor = finalCombined.c_str();
    system(charColor);
    string filename = LOADING_FILE;
    saveSettings(boardColor, backgroundColor, playerSymbols, ENABLE_SOUNDS, filename);
    cout << endl << SPACE "Background color changed! Please wait a moment..." << endl;

    Pause(700);
    STATUS = SELECT_BACKGROUNDCOLOR;
}


bool isDrawPossible() {
    char p1 = playerSymbols[0];
    char p2 = playerSymbols[1];

    if (checkWinner() != -1) {
        return false;
    }
    
    for (int i = 0; i < 3; i++) {
        
        if ((board[i][0] != p1 && board[i][0] != p2) ||
            (board[i][1] != p1 && board[i][1] != p2) ||
            (board[i][2] != p1 && board[i][2] != p2)) {
            return false;  // A win is still possible
        }
        
        if ((board[0][i] != p1 && board[0][i] != p2) ||
            (board[1][i] != p1 && board[1][i] != p2) ||
            (board[2][i] != p1 && board[2][i] != p2)) {
            return false;  
        }
    }

    if ((board[0][0] != p1 && board[0][0] != p2) ||
        (board[1][1] != p1 && board[1][1] != p2) ||
        (board[2][2] != p1 && board[2][2] != p2)) {
        return false;  
    }

    if ((board[0][2] != p1 && board[0][2] != p2) ||
        (board[1][1] != p1 && board[1][1] != p2) ||
        (board[2][0] != p1 && board[2][0] != p2)) {
        return false;  
    }

    return true;  
}

void clearKeyboardBuffer() {
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

void SettingsMenu() { // Not complete, but some progress made
    system("cls");
    TicTacToeArt();
	
	string soundtext;
	if(ENABLE_SOUNDS == 1)
		soundtext = "ON";
	else
		soundtext = "OFF";

    cout << "\n" SPACE "Settings Menu" << endl;
    cout << "" SPACE "1. Change Player Symbols" << endl;
    cout << "" SPACE "2. Change Board Color" << endl;
    cout << "" SPACE "3. Change Background " << endl;
    cout << "" SPACE "4. Sounds: " << soundtext << endl;
    cout << "" SPACE "5. Exit Settings" << endl;
    cout << "" SPACE "6. Reset Settings" << endl;

    
    string input;
    input = _getch();
    
    bool isValid = true;
    for(char ch : input) {
    	if(!isdigit(ch)) {
    		isValid = false;
		}
	}
	
	if(isValid == false) {
		cout << "" SPACE2 "Invalid choice. Please enter a valid choice." << endl;

	    cin.clear();
	   
	    Pause(800);
	    STATUS=SETTINGS;
	    return;
	}
	
	int choice;
	stringstream(input) >> choice;

    if(choice < 1 || choice > 6 || cin.fail()) {

	
	    cout << "" SPACE2 "Invalid choice. Please enter a valid choice." << endl;

	    cin.clear();
	
	    Pause(800);
	    STATUS=SETTINGS;
	    return;
	}

    switch (choice) {
        case 1: ChangePlayerSymbols(); break;
        case 2: STATUS = SELECT_BOARDCOLOR; break;
        case 3: STATUS = SELECT_BACKGROUNDCOLOR; break;
        case 4: {
        	if(ENABLE_SOUNDS) {
        		cout << SPACE2 "Sounds have now been disabled!";
        		PlaySound(NULL, NULL, 0);
        		ENABLE_SOUNDS = 0;
        		Pause(1000);
        		STATUS=SETTINGS;
        	}
        	else {
        		cout << SPACE2 "Sounds are now enabled!";
        		ENABLE_SOUNDS = 1;
        		Pause(1000);
        		STATUS=SETTINGS;
			}
        	break;
    	}
    	case 5: DisplayMenu(); break;
        case 6: ResetDisplaySettings(); break;
    }
}


void DisplayMenu() {
		
	system("cls");
	TicTacToeArt();
	if(ENABLE_SOUNDS)
		PlaySound(TEXT("resources/gamemusic-6082.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	string art = R"(                                             
▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
▐ ▀█    █▀█ █   █▀█ █ █   █▀▀ █▀█ █▄█ █▀▀▌
▐  █    █▀▀ █   █▀█  █    █ █ █▀█ █ █ █▀▀▌
▐ ▀▀▀   ▀   ▀▀▀ ▀ ▀  ▀    ▀▀▀ ▀ ▀ ▀ ▀ ▀▀▀▌
▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌                    
  
▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
▐ ▀▀▄   █ █ █▀█ █ █   ▀█▀ █▀█   █▀█ █   █▀█ █ █▌
▐ ▄▀    █▀█ █ █ █▄█    █  █ █   █▀▀ █   █▀█  █ ▌
▐ ▀▀▀   ▀ ▀ ▀▀▀ ▀ ▀    ▀  ▀▀▀   ▀   ▀▀▀ ▀ ▀  ▀ ▌
▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌

▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
▐ ▀▀█   █▀▄ █▀▀ █ █ █▀▀ █   █▀█ █▀█ █▀▀ █▀▄ █▀▀▌
▐  ▀▄   █ █ █▀▀ ▀▄▀ █▀▀ █   █ █ █▀▀ █▀▀ █▀▄ ▀▀█▌
▐ ▀▀    ▀▀  ▀▀▀  ▀  ▀▀▀ ▀▀▀ ▀▀▀ ▀   ▀▀▀ ▀ ▀ ▀▀▀▌
▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌
  
▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
▐ █ █   █▀▀ █▀▀ ▀█▀ ▀█▀ ▀█▀ █▀█ █▀▀ █▀▀▌
▐  ▀█   ▀▀█ █▀▀  █   █   █  █ █ █ █ ▀▀█▌
▐   ▀   ▀▀▀ ▀▀▀  ▀   ▀  ▀▀▀ ▀ ▀ ▀▀▀ ▀▀▀▌
▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌

▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌
▐ █▀▀   █▀▀ █ █ ▀█▀ ▀█▀▌
▐ ▀▀▄   █▀▀ ▄▀▄  █   █ ▌
▐ ▀▀    ▀▀▀ ▀ ▀ ▀▀▀  ▀ ▌
▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌        
	)";

	istringstream stream(art);
    string line;
    
    while (getline(stream, line)) {
    	ios init(NULL);
    	init.copyfmt(cout);
    	
        std::cout << SPACE << line << std::endl; // Adjust 50 for alignment
        
        cout.copyfmt(init);
    }

	string input;
	cout << endl;

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	input = _getch();
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

		
	bool isValid = true;
    for (char ch : input) {
        if (!isdigit(ch)) {
            isValid = false;
            break;
        }
    }
    
    if(isValid == false) {
		cin.clear();
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		STATUS = MAINMENU;
		return;
	}
	
	int status;
    stringstream(input) >> status;
    
    if(status < 1 || status > 5 || cin.fail()) {
		cin.clear();
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
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

        board[row][col] = playerSymbols[currentPlayer-1];
       
    	if(ENABLE_SOUNDS)
			PlaySound(TEXT("resources/mark.wav"), NULL, SND_FILENAME | SND_ASYNC);

        return true;
    }
    return false;
}

int tawgako(string input) {
	bool isValid = true;
	
	for(char ch : input) {
		if(!isdigit(ch)) {
			return -1;
		}
	}
	
	int num;
	stringstream(input) >> num;
	return num;
}

//[----------------------------------------------------------------------------] 
