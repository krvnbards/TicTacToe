#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

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
string PlayerName[2];
int GAME_ROUNDS = -1;
int currentRound = 1;
int playerScore[2];


string boardColor = "0a"; // Default green
string backgroundColor = "0"; // Default black background
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

int checkWinner();


//[-----------------------------------------------------------------------------]

/* sometimes dili ma end ang round bisag nadaug na ang isa ka player,
   kailangan pun on ang nine boxes sa board para ma win. 
*/

int main() {
	system("color 0a");
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
			case GAME_ENTER_NAME1: {
				system("cls");
				TicTacToeArt();
				
				cout << "\n\nEnter name for Player 1: ";
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
				
				cout << "\n\nEnter name for Player 2: ";
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
				system("pause");
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
	            	cout << setw(55) << "\nNote: Inputting No will bring you back to the main menu." << endl;
	            	cout << setw(55) << "Do you want to play again? (Y/N): ";
	            	cin >> choice;
	            	
	            	if(choice == 'Y' || choice == 'y') {
	            		ResetGameSettings();
	            		clearBoard();
		           		StartGame();
		           		STATUS = GAME_STARTED;
						system("pause");
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						break;	
					} 
					else if(choice == 'N' || choice == 'n') {
						cout << setw(45) << "Returning to main menu..." << endl;
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						STATUS = MAINMENU;
						system("pause");
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

void Winner(int winner) {
	system("cls");
	TicTacToeArt();
	
	cout << endl << setw(40) << PlayerName[winner] << " has won the overall rounds with a score of " << playerScore[winner] << "!" << endl;
	
	cout << setw(55) << "SCORE BREAKDOWN" << endl;
	cout << setw(50) << PlayerName[0] << " - " << playerScore[0] << endl;
	cout << setw(50) << PlayerName[1] << " - " << playerScore[1] << endl;
	
	STATUS=GAME_END;
	//system("pause");
}

void roundEnd(int winner) {
	
	cout << setw(40) << PlayerName[winner] << " wins the round!" << endl;
	currentRound++;
	playerScore[winner]++;
	
	if (playerScore[0] > playerScore[1] && (playerScore[0] > GAME_ROUNDS/2)) {
		Winner(0);
		//clearBoard();
		return;
	}
    else if (playerScore[1] > playerScore[0] && (playerScore[0] > GAME_ROUNDS/2)) {
    	Winner(1);
    	//clearBoard();
    	return;
	}
    //else if(playerScore[1] == playerScore[0] && currentRound == GAME_ROUNDS) cout << "The game ends in a tie!\n";
	
	clearBoard();
	system("pause");
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

  _____ _     _____         _____          
 |_   _(_) __|_   _|_ _  __|_   _|__   ___ 
   | | | |/ __|| |/ _` |/ __|| |/ _ \ / _ \
   | | | | (__ | | (_| | (__ | | (_) |  __/
   |_| |_|\___||_|\__,_|\___||_|\___/ \___|
                                                                                                                                                                                            
	)";
	
	istringstream stream(TicTacToeArt);
    string line;
    while (getline(stream, line)) {
        std::cout << std::setw(70) << line << std::endl; // Adjust 50 for alignment
    }
	
	cout << setw(55) << "C++ 2025" << endl;
	
	cout << "________________________________________________________________________________________________________________________\n";
}

void StartGame() {
	
	DisplayBoard();
	if(firstPlayer == 1)
		currentPlayer = 2;
	else
		currentPlayer = 1;
	
	int slot;
	firstPlayer = currentPlayer;
	for(int i = 0; i < 9; i++) {
		cout << endl <<  setw(21) << PlayerName[currentPlayer-1] << " (" << playerSymbols[currentPlayer-1] << ") turn to play, place to mark slot (1-9): "; 
		cin >> slot;
		
		if (slot < 1 || slot > 9 || !placeMarker(slot) || cin.fail()) {
            cout << setw(39) << "Invalid move! Try again.\n";
            cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
            i--; // Retain the turn
            system("pause");
            continue;
        }
        
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        
        DisplayBoard();
        
        int winner = checkWinner();
        if(winner != 0) {
        	roundEnd(winner-1);
        	return;
		}
		
		if (isDrawPossible()) {
			clearBoard();
			//StartGame();
            cout << "This round ends in a draw! No more possible wins.\n";
            //system("pause");
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
		cout << "\n[0] Back\n[1] Player vs Player\n[2] Player vs Computer\n\nEnter your choice: ";
		cin >> GAME_MODE;
		
		if(GAME_MODE == 0) break;
		
		if(GAME_MODE < 1 || GAME_MODE > 2 || cin.fail()) {
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
	
	STATUS = GAME_ENTER_NAME1;

}

int checkWinner() {
    
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && (board[i][0] != ' ' && board[i][1] != ' ' && board[i][2] != ' ')) return currentPlayer;
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && (board[0][i] != ' ' && board[1][i] != ' ' && board[2][i] != ' ')) return currentPlayer;
    }
    
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && (board[0][0] != ' ' && board[1][1] != ' ' && board[1][1] != ' ' && board[2][2] != ' ')) return currentPlayer;
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && (board[0][2] != ' ' && board[1][1] != ' ' && board[1][1] != ' ' && board[2][2] != ' ')) return currentPlayer;
    
    return 0; 
}

void SelectRounds() {
	while(GAME_ROUNDS < 0) {
		system("cls");
		TicTacToeArt();
		
		
		cout << setw(50) << "\n[1] 3 Rounds\n[2] 5 Rounds\n[3] 7 Rounds\n(Soon more rounds/custom)\nEnter your choice: ";
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
	cout << "\n\tHow to Play Tic-Tac-Toe" << endl;
    cout << "\n1. The game is played on a 3x3 grid." << endl;
    cout << "2. Player 1 will be \"X\" and Player 2 (or Computer) will be \"O\"." << endl;
    cout << "3. Players take turns placing their mark (X or O) in an empty spot." << endl;
    cout << "4. The first player to get three of their marks in a row (vertically, horizontally, or diagonally) wins the round." << endl;
    cout << "5. If all 9 spaces are filled and no player has three in a row, the round is a draw." << endl;
    cout << "6. The game continues until all selected rounds are completed." << endl;

    cout << "\nControls:" << endl;
    cout << "- Enter the number (1 to 9) corresponding to the space you want to mark." << endl;
    cout << "- Invalid moves (selecting an already occupied space) will result on giving you a chance to take another move." << endl;

    cout << "\nHave fun and enjoy the game!\n" << endl;
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
    cout << "\nThank you for playing Tic-Tac-Toe!" << endl;
    cout << "\nExiting the game..." << endl;
    system("pause");
	STATUS = MAINMENU;
	exit(0);
	system("cls");
}

void ChangePlayerSymbols() { // Change symbol feature, Will check later if ok najud siya
	system("cls");
	TicTacToeArt();
    cout << "\nEnter symbol for Player 1: ";
    
    cin >> playerSymbols[0];
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    cout << "\nEnter symbol for Player 2: ";
    cin >> playerSymbols[1];
    
    
    cout << "Player symbols updated!" << endl;
    system("pause");
    SettingsMenu();
}

void ChangeBoardColor() { // if other color (light gray, etc.) na ang background, then e change ang board color, ma reset to black ang background

	system("cls");
	TicTacToeArt();       // tarungon ra nya na nako - jules
    int colorChoice;
    cout << "\nChoose a board color:" << endl;
    cout << "\n1. Blue \n2. Green \n3. Aqua \n4. Red \n5. Purple \n6. Yellow \n7. Light Gray \n8. Light Green \n9. Light Aqua \n10. Light Red " << endl;
    cout << "Enter your choice (1-10): ";
    cin >> colorChoice;
    
    if(colorChoice < 1 || colorChoice > 10 || cin.fail()) {
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch(colorChoice) {
        case 1: boardColor = "01"; break;
        case 2: boardColor = "0a"; break;
        case 3: boardColor = "03"; break;
        case 4: boardColor = "04"; break;
        case 5: boardColor = "05"; break;
        case 6: boardColor = "06"; break;
        case 7: boardColor = "08"; break;
        case 8: boardColor = "0a"; break;
        case 9: boardColor = "0b"; break;
        case 10: boardColor = "0c"; break;
        default: cout << "Invalid choice. Defaulting to green." << endl; boardColor = "0a"; break;
    }

    cout << "\nBoard color updated!" << endl;
    system(("color " + backgroundColor + boardColor.substr(1)).c_str());
    system("pause");
    SettingsMenu();
}


void ChangeBackgroundColor() { // not the final colors, tingali napay mas nice na color e add
	system("cls");
	TicTacToeArt();
	int choice;
    cout << "\n1. Light Gray\n2. Light Aqua\n3. Light Red\n4. Reset to Default (Black)\n";
    cout << "Enter your choice: ";
    cin >> choice;
    
     switch (choice) {
        case 1:
            cout << "\033[47m"; // Light Gray Background
            system("cls");
            break;
        case 2:
            cout << "\033[106m"; // Light Aqua Background
            system("cls");
            break;
        case 3:
            cout << "\033[101m"; // Light Red Background
            system("cls");
            break;
        case 4:
            cout << "\033[40m"; // Black Background (Reset)
            system("cls");
            break;
        default:
            cout << "Invalid choice." << endl;
            return;
    }
    TicTacToeArt();
    cout << "\nBackground color changed!" << endl;
    system("pause");
    SettingsMenu();
}

bool isDrawPossible() {
    char p1 = playerSymbols[0];
    char p2 = playerSymbols[1];

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

void SettingsMenu() { // Not complete, but some progress made
    system("cls");
    TicTacToeArt();
    int choice;

    cout << "\nSettings Menu" << endl;
    cout << "1. Change Player Symbols" << endl;
    cout << "2. Change Board Color" << endl;
    cout << "3. Change Background " << endl;
    cout << "4. Exit Settings" << endl;
    cout << "\nEnter your choice: ";
    cin >> choice;
    
    if(choice < 1 || choice > 4 || cin.fail()) {
    	STATUS = MAINMENU;
    	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	    STATUS = MAINMENU;
	    cout << "Invalid choice. Returning to Main Menu..." << endl;
	    system("pause");
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
	
	cout << R"(	
 _         ___ _                 ___                     
/ |       / _ | | __ _ _   _    / _ \__ _ _ __ ___   ___ 
| |      / /_)| |/ _` | | | |  / /_\/ _` | '_ ` _ \ / _ \
| |     / ___/| | (_| | |_| | / /_\| (_| | | | | | |  __/
|_|     \/    |_|\__,_|\__, | \____/\__,_|_| |_| |_|\___|
                       |___/                             
	)";
	
	cout << R"(	
 ____                              _____              ___ _             
|___ \      /\  /\_____      __   /__   \___         / _ | | __ _ _   _ 
  __) |    / /_/ / _ \ \ /\ / _____ / /\/ _ \ _____ / /_)| |/ _` | | | |
 / __/    / __  | (_) \ V  V |_____/ / | (_) |_____/ ___/| | (_| | |_| |
|_____|   \/ /_/ \___/ \_/\_/      \/   \___/      \/    |_|\__,_|\__, |
                                                                |___/ 
	)";
	
	cout << R"(                                            
 _____      ___               _                           
|___ /     /   \_____   _____| | ___  _ __   ___ _ __ ___ 
  |_ \    / /\ / _ \ \ / / _ | |/ _ \| '_ \ / _ | '__/ __|
 ___) |  / /_/|  __/\ V |  __| | (_) | |_) |  __| |  \__ \
|____/  /___,' \___| \_/ \___|_|\___/| .__/ \___|_|  |___/
                                     |_|                  
	)";
	 
	cout << R"(
 _  _       __      _   _   _                 
| || |     / _\ ___| |_| |_(_)_ __   __ _ ___ 
| || |_    \ \ / _ | __| __| | '_ \ / _` / __|
|__   _|   _\ |  __| |_| |_| | | | | (_| \__ \
   |_|     \__/\___|\__|\__|_|_| |_|\__, |___/
                                    |___/     
	)"; 
	
	cout << R"(
 ____       __      _ _   
| ___|     /____  _(_| |_ 
|___ \    /_\ \ \/ | | __|
 ___) |  //__  >  <| | |_ 
|____/   \__/ /_/\_|_|\__|
                          
	)";
	 
	 
	//cout << "[1] Play Game\n[2] How to Play\n[3] Developers\n[4] Settings\n[5] Exit\n\n" << endl;
	cout << "Enter choice: ";
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