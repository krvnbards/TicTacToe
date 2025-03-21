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
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}
};

// Game Settings
int GAME_MODE = -1; // 1 = PvP 2 = PvE
string PlayerName[2];
int GAME_ROUNDS = -1;

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

void How_To_Play();
void DevelopersPage();
void ExitGame();
void DevelopersArt();

//[-----------------------------------------------------------------------------]
int main() {
	system("color 0a");
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
				
				cout << "Enter name for Player 1: ";
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
				
				cout << "Enter name for Player 2: ";
				cin >> PlayerName[0];
				
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
				DisplayBoard();
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
			case EXITGAME: {
                ExitGame();
                break;
            }
		}
	} cout << "END OF LOOP MAIN";
	return 1;
}

//[----------------------------FUNCTION DEFINITIONS----------------------------]

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
	
	cout << setw(70) << "Developed by Kervin, Jules, Kendrick (2025)" << endl;
	
	cout << "________________________________________________________________________________________________________________________\n";
}

void SelectMode() {
	while(GAME_MODE < 1) {
		system("cls");
		TicTacToeArt();
		cout << "[1] Player vs Player\n[2] Player vs Computer\nEnter your choice: ";
		cin >> GAME_MODE;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		if(GAME_MODE < 1 || GAME_MODE > 2 || cin.fail()) {
			GAME_MODE = -1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
	}
	
	STATUS = GAME_ENTER_NAME1;

}

void SelectRounds() {
	while(GAME_ROUNDS < 0) {
		system("cls");
		TicTacToeArt();
		
		
		cout << setw(50) << "[1] 3 Rounds\n[2] 5 Rounds\n[3] 7 Rounds\n(Soon more rounds/custom)\nEnter your choice: ";
		cin >> GAME_ROUNDS;
		
		if(GAME_ROUNDS < 1 || GAME_ROUNDS > 3 || cin.fail()) {
			GAME_ROUNDS = -1;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		
		switch(GAME_ROUNDS) {
			case 1: GAME_ROUNDS = 3; break;
			case 2: GAME_ROUNDS = 5; break;
			case 3: GAME_ROUNDS = 7; break;
		}
		
		STATUS = GAME_STARTED;
	}
}

void DisplayBoard() {
	cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
	cout << "====================================" << endl;
	cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
	cout << "====================================" << endl;
	cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl;
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
		cout << "Return to main menu!" << endl;
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
	cout << "\n\t\tHow to Play Tic-Tac-Toe" << endl;
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
 ____                            _____              ___ _             
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
	
	switch(status) {
		case 1: STATUS = GAME_SELECT_MODE; break;
		case 2: STATUS = HOW_TO_PLAY; break;
		case 3: STATUS = DEVELOPERS; break;
		case 4: STATUS = SETTINGS; break;
		case 5: STATUS = EXITGAME; break;
	}
}
//[----------------------------------------------------------------------------]
