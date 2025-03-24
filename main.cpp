#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h>
#include <limits>
#include <cstdlib>
#include <ctime> 

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


string boardColor = "A"; // Default green
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

//[-----------------------------------------------------------------------------]

/* sometimes dili ma end ang round bisag nadaug na ang isa ka player,
   kailangan pun on ang nine boxes sa board para ma win.
*/

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
					cout << setw(50) << "\nSelect Computer Difficulty\n\n[1] Easy\n[2] Medium (not available)\n[3] Hard (not available)\nEnter your choice: " << endl;
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

				cout << setw(50) << "\n\nEnter name for Player 1: ";
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

				cout << setw(50) << "\n\nEnter name for Player 2: ";
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
						cout << setw(39) << "Returning to main menu... Please wait.." << endl;
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
			aiRandomMove();
			DisplayBoard();

	        int winner = checkWinner();
	        if(winner != -1) {
	        	roundEnd(winner);
	        	return;
			}
	
			if (isDrawPossible()) {
				clearBoard();
				//StartGame();
	            cout << setw(39) << "This round ends in a draw! No more possible wins.\n";
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
            cout << setw(39) << "This round ends in a draw! No more possible wins.\n";
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
		cout << "\n[0] Back\n[1] Player vs Player\n[2] Player vs Computer\n\nEnter your choice: ";
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
    Pause(2000);
    STATUS=SETTINGS;
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
    	cout << "Invalid choice. Defaulting to black." << endl;
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
        default: cout << "Invalid choice. Defaulting to green." << endl; boardColor = "0"; break;
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
	int choice;
    cout << "\nChoose a board color:" << endl;
    cout << "\n1. Blue \n2. Green \n3. Aqua \n4. Red \n5. Purple \n6. Yellow \n7. Light Gray \n8. Light Green \n9. Light Aqua \n10. Light Red " << endl;
    cout << "Enter your choice (1-10): ";
    cin >> choice;

    if(choice < 1 || choice > 10 || cin.fail()) {
    	//cin.clear();
    	cout << "Invalid choice. Defaulting to black." << endl;
    	Pause(700);
    	cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		STATUS = SETTINGS;
    	return;
	}

	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch(choice) {
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
	    default: cout << "Invalid choice. Defaulting to green." << endl; backgroundColor = "0"; break;
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

    cout << "\nSettings Menu" << endl;
    cout << "1. Change Player Symbols" << endl;
    cout << "2. Change Board Color" << endl;
    cout << "3. Change Background " << endl;
    cout << "4. Exit Settings" << endl;
    cout << "\nEnter your choice: ";
    cin >> choice;

    if(choice < 1 || choice > 4 || cin.fail()) {

	    //STATUS = MAINMENU;
	    cout << "Invalid choice. Please enter a valid choice." << endl;

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
  _   ____  _                ____                      
 / | |  _ \| | __ _ _   _   / ___| __ _ _ __ ___   ___ 
 | | | |_) | |/ _` | | | | | |  _ / _` | '_ ` _ \ / _ \
 | | |  __/| | (_| | |_| | | |_| | (_| | | | | | |  __/
 |_| |_|   |_|\__,_|\__, |  \____|\__,_|_| |_| |_|\___|
                    |___/                              
  ____    _   _                  _____           ____  _             
 |___ \  | | | | _____      __  |_   _|__       |  _ \| | __ _ _   _ 
   __) | | |_| |/ _ \ \ /\ / /____| |/ _ \ _____| |_) | |/ _` | | | |
  / __/  |  _  | (_) \ V  V /_____| | (_) |_____|  __/| | (_| | |_| |
 |_____| |_| |_|\___/ \_/\_/      |_|\___/      |_|   |_|\__,_|\__, |
                                                               |___/ 
  _____   ____                 _                           
 |___ /  |  _ \  _____   _____| | ___  _ __   ___ _ __ ___ 
   |_ \  | | | |/ _ \ \ / / _ \ |/ _ \| '_ \ / _ \ '__/ __|
  ___) | | |_| |  __/\ V /  __/ | (_) | |_) |  __/ |  \__ \
 |____/  |____/ \___| \_/ \___|_|\___/| .__/ \___|_|  |___/
                                      |_|                                                  
  _  _     ____       _   _   _                 
 | || |   / ___|  ___| |_| |_(_)_ __   __ _ ___ 
 | || |_  \___ \ / _ \ __| __| | '_ \ / _` / __|
 |__   _|  ___) |  __/ |_| |_| | | | | (_| \__ \
    |_|   |____/ \___|\__|\__|_|_| |_|\__, |___/
                                      |___/                                 
  ____    _____      _ _   
 | ___|  | ____|_  _(_) |_ 
 |___ \  |  _| \ \/ / | __|
  ___) | | |___ >  <| | |_ 
 |____/  |_____/_/\_\_|\__|
                           
							          
	)";

	istringstream stream(art);
    string line;
    
    while (getline(stream, line)) {
    	ios init(NULL);
    	init.copyfmt(cout);
    	
        std::cout << std::setw(25) << right << line << std::endl; // Adjust 50 for alignment
        
        cout.copyfmt(init);
    }


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