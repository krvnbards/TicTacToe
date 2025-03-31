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

char marker[] = {'O', 'X'};

char playerSymbols[2] = {'O', 'X'}; // changed marker to this para sa SettingsMenu()


int firstPlayer = 2;


// Game Settings
int GAME_MODE = -1; // 1 = PvP 2 = PvE
int GAME_DIFFICULTY = 0;
string PlayerName[2];
int GAME_ROUNDS = -1;
int currentRound = 1;
int playerScore[2];
int autoPause = 0; // Pause = 0 Pause = 1


string boardColor = "7"; // Default white
string backgroundColor = "0"; // Default black background
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

void PrintCharDelay ();
void PrintCharDelay2 ();
void PrintAnimation ();

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

void aiRandomMove();
int evaluate();
bool availableMoves();
int minimax(int depth, bool isMaximizing);
void aiMiniMax();

//[-----------------------------------------------------------------------------]
int main() {
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

				cout << setw(50) << "\n\n" SPACE "Enter name for Player 1: ";
				cin >> PlayerName[0];

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

				cout << setw(50) << "\n\n" SPACE "Enter name for Player 2: ";
				cin >> PlayerName[1];

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

void aiRandomMove() {
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
		Sleep(ms); // 3-second delay
	    //cin.clear();
	    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
	
	//if(currentPlayer == 1 && GAME_MODE == 1)
	
	for(int i = 0; i < 9; i++) 
	{
		if(GAME_MODE == 2 && currentPlayer == 2) {
			cout << endl <<  setw(21) << PlayerName[currentPlayer-1] << " (" << playerSymbols[currentPlayer-1] << ") turn to move.. please wait.." << endl;
			Pause(1500);
			if(GAME_DIFFICULTY == 1)
				aiRandomMove();
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

		if (slot < 1 || slot > 9 || !placeMarker(slot) || cin.fail()) {
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
	cout << "\n\n";
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
    					{(SPACE SPACE2 "Motto:  Do it scared, do it tired, do it broke, do it alone, do it unsure, but do it\n"
                    			  SPACE SPACE2 "        anyways. Because we think we have forever to chase our dreams\n"
                      		          SPACE SPACE2 "        but today is all we really have and you'll never get it back.\n"
					  SPACE SPACE2 "        and you'll never get it back. No regrets!\n")} 
					};  

			const string DEVDESCRIPTION2[] = { 
						        {(SPACE SPACE2 "Age:    20")},	
							{(SPACE SPACE2 "Gender: Male")}, 
							{(SPACE SPACE2 "Motto:  Just do it!\n")} };

			const string DEVDESCRIPTION3[] = { 
							{(SPACE SPACE2 "Age:    20")},	
							{(SPACE SPACE2 "Gender: Male")}, 
							{(SPACE SPACE2 "Motto:  Live intentionally!\n")} };
											  
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
    Pause(2000);
    STATUS=SETTINGS;
}

void ChangeBoardColor() { // if other color (light gray, etc.) na ang background, then e change ang board color, ma reset to black ang background

	system("cls");
	TicTacToeArt();       // tarungon ra nya na nako - jules
    int colorChoice;
    cout << endl << setw(55) << "Choose a board color:" << endl;
    cout << setw(43) << "[0] Black" << endl << setw(42) << "[1] Blue" << endl << setw(43) << "[2] Green" << endl << setw(42) << "[3] Aqua" << endl << setw(41) << "[4] Red" << endl << setw(44) << "[5] Purple" << endl << setw(45) << "[6] Yellow " << endl << setw(48) << "[7] Light Gray" << endl << setw(49) << "[8] Light Green" << endl << setw(48) << "[9] Light Aqua" << endl << setw(49) << "[10] Light Red " << endl;
    cout << setw(60) << "Enter your choice (1-10): ";
    cin >> colorChoice;

    if(colorChoice < 0 || colorChoice > 10 || cin.fail()) {
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
        case 1: boardColor = "1"; break;
        case 2: boardColor = "a"; break;
        case 3: boardColor = "3"; break;
        case 4: boardColor = "4"; break;
        case 5: boardColor = "5"; break;
        case 6: boardColor = "6"; break;
        case 7: boardColor = "8"; break;
        case 8: boardColor = "a"; break;
        case 9: boardColor = "b"; break;
        case 10: boardColor = "c"; break;
        default: boardColor = "0"; break;
    }

    cout << "\nBoard color updated!" << endl;
    finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
    system(charColor);
    Pause(3000);
    STATUS=SETTINGS;
}


void ChangeBackgroundColor() { // not the final colors, tingali napay mas nice na color e add
	system("cls");
	TicTacToeArt();
	int colorChoice;
    cout << endl << setw(55) << "Choose a board color:" << endl;
    cout << setw(43) << "[0] Black" << endl << setw(42) << "[1] Blue" << endl << setw(43) << "[2] Green" << endl << setw(42) << "[3] Aqua" << endl << setw(41) << "[4] Red" << endl << setw(44) << "[5] Purple" << endl << setw(45) << "[6] Yellow " << endl << setw(48) << "[7] Light Gray" << endl << setw(49) << "[8] Light Green" << endl << setw(48) << "[9] Light Aqua" << endl << setw(49) << "[10] Light Red " << endl;
    cout << setw(60) << "Enter your choice (1-10): ";
    cin >> colorChoice;

    if(colorChoice < 1 || colorChoice > 10 || cin.fail()) {
    	//cin.clear();
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
	    case 1: backgroundColor = "1"; break;
	    case 2: backgroundColor = "a"; break;
	    case 3: backgroundColor = "3"; break;
	    case 4: backgroundColor = "4"; break;
	    case 5: backgroundColor = "5"; break;
	    case 6: backgroundColor = "6"; break;
	    case 7: backgroundColor = "8"; break;
	    case 8: backgroundColor = "a"; break;
	    case 9: backgroundColor = "b"; break;
	    case 10: backgroundColor = "c"; break;
    }

    finalCombined = colo + backgroundColor + boardColor;
	charColor = finalCombined.c_str();
    system(charColor);
    cout << "\nBackground color changed!" << endl;

    Pause(700);
    STATUS=SETTINGS;
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
