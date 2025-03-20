#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;



//[--------------------------VARIABLES---------------------------]

// Gamit ta ug char diri para inig cout kay char mo gawas
// Plano pud nako nga naay option nga unsay character or symbol ang ganahan for player
char board[3][3] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}
};

enum stat_list {
	
	MAINMENU = 1,
	HOW_TO_PLAY,
	DEVELOPERS
};

enum stat_list STATUS = MAINMENU;
//[--------------------------------------------------------------]

//[----------------------------FUNCTION DECLARATIONS----------------------------]
void DisplayMenu();
//[-----------------------------------------------------------------------------]


int main() {
	system("color 0a");
	while(1) {
		switch(STATUS) {
			case MAINMENU: {
				DisplayMenu();
				break;
			}
		}
	}
	return 1;
}

//[----------------------------FUNCTION DEFINITIONS----------------------------]
void DisplayMenu() {
	system("cls");
	
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
        std::cout << std::setw(50) << line << std::endl; // Adjust 50 for alignment
    }
	
	cout << "================================================================================================\n";
	
	int status;
	
	cout << R"(	
 ___        ______ __                   _______                       
|_   |      |   __ \  |.---.-.--.--.    |     __|.---.-.--------.-----.
 _|  |_     |    __/  ||  _  |  |  |    |    |  ||  _  |        |  -__|
|______|    |___|  |__||___._|___  |    |_______||___._|__|__|__|_____|
                             |_____|                                   

	)";
	
	cout << R"(	

 ______      _______                       _______               ______ __              
|__    |    |   |   |.-----.--.--.--._____|_     _|.-----.______|   __ \  |.---.-.--.--.
|    __|    |       ||  _  |  |  |  |______||   |  |  _  |______|    __/  ||  _  |  |  |
|______|    |___|___||_____|________|       |___|  |_____|      |___|  |__||___._|___  |
                                                                                 |_____|

	)";
	 
	//cout << "[1] Play Game\n[2] How to Play\n[3] Developers\n[4] Settings\n[5] Exit\n\n" << endl;
	cout << "Enter action: ";
	while(cin >> status) {
		
	}
}
//[----------------------------------------------------------------------------]