#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;
#pragma warning(disable : 4996)

class TicTacToe
{
private:
	int **Grid;
	int size;
	int Save[2];

public:
	TicTacToe()           // Default Grid of size 3x3
	{
		this->size = 3;
		this->Grid = new int*[size];
		for (int i = 0, k = 1; i<size; i++)
		{
			this->Grid[i] = new int[size];
			for (int j = 0; j<size; j++, k++)
				this->Grid[i][j] = k;       //set grid entries from 1 to 9
		}
		Start();
	}

	void setTicTacToe(int size)        //Set grid of size n
	{
		this->size = size;
		this->Grid = new int*[size];
		for (int i = 0, k = 1; i<size; i++)
		{
			this->Grid[i] = new int[size];
			for (int j = 0; j<size; j++, k++)
				this->Grid[i][j] = k;       //set grid entries from 1 to n*n
		}
	}

	void setSize(int size)
	{
		this->size = size;
	}

	void Start()
	{
		cout << "Welcome to Tic-Tac-Toe \n";
		menu();
		cout << "\n Thanks for playing (^.^)\n";
	}

	void menu()
	{
	again:
		cout << "\n\nSelect an option.\n";
		cout << "\n 1.New Game\n 2.Load Game\n 3.Delete a Saved Game\n";
		cout << "Enter your choice : ";
		int op;
		cin >> op;

		if (op == 1)
			gameMode();

		else if (op == 2)
		{
			if (loadGame() == 0)
				goto again;
			else
				playGame(selectGame());
		}

		else if (op == 3)
		{
			if (loadGame() == 0)
				goto again;
			else
				deleteGame();
		}

		else
		{
			cin.clear();     //to avoid infinite loop in case input is character
			cin.ignore();
			cout << "\nInvalid option\n";
			goto again;
		}
	}

	void gameMode()
	{
		int op;
		cout << "\n\nSelect playing mode:\n";
	again:
		cout << "\n 1. Single Player (Human vs Computer)\n 2. Two Player (Human vs Human)\n";
		cout << "\nEnter your choice: ";
		cin >> op;

		if (op == 1 || op == 2)
			playGame(op);
		else
		{
			cin.clear();     //to avoid infinite loop in case input is character
			cin.ignore();
			cout << "\nInvalid location\n";
			cout << "\nSelect another location : ";
			goto again;
		}
	}

	int playGame(int op)
	{
		// if op = 1 computer mode. if op = 2 two player mode
		char player[] = { 'X', 'O' };
		int turn;

		if (op == 3)    // Game loaded
		{
			op = Save[0];
			turn = 1 - Save[1];

			displayGrid();
			goto Continue;
		}

		SelectGrid();
		this->setTicTacToe(size);
		this->displayGrid();
		turn = 1;

	Continue:
		turn = 1 - turn;
		if ((op == 2) || (player[turn] == 'X')) // player2 is not computer or it player1's turn
			cout << "Player " << player[turn] << " Select a location : ";

		if (setLocation(player[turn], op) == 1)  //Player press -1 to save
		{
			cout << "Do you want to Save?";
		again:
			cout << "\nPress S to save \nPress Q to quit.";
			char ch;
			cin >> ch;

			if (ch == 'S' || ch == 's')
			{
				save(turn, op);
				cout << "\nGame Saved\n";
				turn = 1 - turn;
				displayGrid();
				goto Continue;
			}

			else if (ch == 'Q' || ch == 'q')
			{
				playAgain();
				return 0;
			}
			else
			{
				cout << "\nInvalid option\n";
				goto again;
			}
		}

		if (!(op == 1 && player[turn] == 'X'))   //If not computer's turn
			displayGrid();

		if (Win(player[turn], op) || draw(player[turn], op))
		{
			playAgain();
			return 0;
		}

		else
			goto Continue;
	}

	void SelectGrid()
	{
		int option;
		cout << "\nSelect playing grid\t\tPress -1 to Save/Quit during game\n\n 1. 3 X 3\n 2. 4 X 4\n 3. 5 X 5\n\n";
	again:
		cout << "Enter your choice: ";
		cin >> option;

		if (option == 1)
			this->size = 3;
		else if (option == 2)
			this->size = 4;
		else if (option == 3)
			this->size = 5;
		else
		{
			cin.clear();    //to avoid infinite loop in case input is character
			cin.ignore();
			cout << "Invalid option.\n\n";
			goto again;
		}
	}

	void displayGrid()
	{
		for (int i = 0; i<size; i++)
			cout << " _____";
		cout << endl;
		for (int i = 0; i<size; i++)
		{
			for (int j = 0; j<size; j++)
				cout << "|     ";
			cout << '|' << endl << '|';
			for (int j = 0; j<size; j++)
			{
				cout << "  ";
				if (Grid[i][j] == 88)       //location is set to X
					cout << 'X' << ' ';
				else if (Grid[i][j] == 79)  //location is set to O
					cout << 'O' << ' ';
				else
					cout << Grid[i][j];
				if (Grid[i][j] <= 9)
					cout << ' ';
				cout << " " << '|';
			}
			cout << endl << '|';
			for (int j = 0; j<size; j++)
				cout << "_____|";
			cout << endl;
		}
	}

	int setLocation(char ch, int player2)
	{
		// Player2 = 1 for computer and 2 for human
		int location;
	again:

		if (player2 == 2 || ch == 'X')  //Either player2 is human
			cin >> location;            //or it is player1's turn
		else
			location = getLocation(ch);
		if (location>0 && location <= size*size)
		{
			if (alreadyFilled(location) == true)
			{
				cout << "\nAlready filled\n";
				cout << "\nSelect another location : ";
				goto again;
			}

			else
				for (int i = 0; i<size; i++)
					for (int j = 0; j<size; j++)
					{
						if (Grid[i][j] == location)
						{
							Grid[i][j] = ch;     //Place ch at selected location
							return 0;
						}
					}
		}
		else
		{

			if (location == -1)      //to quit during game
			{

				return 1;
			}

			if (location == -2)      //to quit during game
			{
				return 2;
			}

			else
			{
				cin.clear();     //to avoid infinite loop in case input is character
				cin.ignore();
				cout << "Invalid location\n";
				cout << "\nSelect another location : ";
				goto again;
			}
		}

		return 0;
	}

	int getLocation(char ch)
	{
		int location_;
		int countChar;

		srand(time(0));

		for (int j = 0; j<size; j++)  //Column win possible
		{
			countChar = 0;
			for (int i = 0; i<size - 1; i++)
			{
				if (Grid[i][j] == Grid[i + 1][j])
					countChar++;
				if (Grid[size - 1][j] == Grid[0][j])
					countChar++;
			}

			if (countChar == size - 2)
				for (int i = 0; i<size; i++)
				{
					if (!(Grid[i][j] == 88 || Grid[i][j] == 79))   //if any location in column is free, set it
					{
						location_ = Grid[i][j];
						return location_;
					}
				}
		}

		for (int i = 0; i<size - 1; i++)     //Main Diagonal win possible
		{
			if (Grid[i][i] == Grid[i + 1][i + 1])
				countChar++;
			if (Grid[0][0] == Grid[size - 1][size - 1])
				countChar++;

			if (countChar == size - 2)
				for (int i = 0; i<size; i++)
				{
					if (!(Grid[i][i] == 88 || Grid[i][i] == 79))   //if any location in diagonal is free, set it
					{
						location_ = Grid[i][i];
						return location_;
					}
				}
		}

		for (int i = 0; i<size; i++)  //row win possible
		{
			countChar = 0;
			for (int j = 0; j<size - 1; j++)
			{
				if (Grid[i][j] == Grid[i][j + 1])
					countChar++;
				if (Grid[i][size - 1] == Grid[j][0])
					countChar++;
			}

			if (countChar == size - 2)
				for (int j = 0; j<size; j++)
				{
					if (!(Grid[i][j] == 88 || Grid[i][j] == 79)) //if any location in row is free, set it
					{
						location_ = Grid[i][j];
						return location_;
					}
				}
		}

		countChar = 0;
		for (int i = 0, j = size - 1; i<size - 1; i++)    //Secondary Diagonal win possible
		{
			if (Grid[i][j - i] == Grid[i + 1][j - i - 1])
				countChar++;

			if (Grid[size - 1][0] == Grid[0][j])
				countChar++;

			if (countChar == size - 2 || countChar == size - 3)      //if possibility of winning
				for (int i = 0, j = size - 1; i<size; i++)
				{
					if (!(Grid[i][j - i] == 88 || Grid[i][j - i] == 79))   //if any location in diagonal is free, set it
					{
						location_ = Grid[i][j - i];
						return location_;
					}
				}
		}

	again:
		int maxRange = size*size;
		location_ = rand() % maxRange + 1;    //No better choice

		if (location_ > 0 && location_ <= (size*size))
		{
			if (!alreadyFilled(location_))
				return location_;
			else

				goto again;
		}
		return location_;
	}

	bool alreadyFilled(int location)
	{
		int alreadyfilled = 0;

		for (int i = 0; i<size; i++)
			for (int j = 0; j<size; j++)
			{
				if (Grid[i][j] == location)
					return false;
				else
					alreadyfilled++;
			}

		if (alreadyfilled == size*size)   //selected location already filled
			return true;
		else
			return false;
	}

	bool Win(char ch, int mode)
	{
		int countX = 0, countO = 0;
		for (int j = 0; j<size; j++)  //Column Win check
		{
			countX = 0, countO = 0;
			for (int i = 0; i<size; i++)
			{
				if (Grid[i][j] == 88)
					countX++;
				else if (Grid[i][j] == 79)
					countO++;
			}

			if (countX == size || countO == size)        // All Column entries are filled with X or O
			{
				winnerMsg(ch, mode);
				return true;
			}
		}

		for (int i = 0; i<size; i++)    // Row win check
		{
			countX = 0, countO = 0;
			for (int j = 0; j<size; j++)
			{
				if (Grid[i][j] == 88)
					countX++;
				else if (Grid[i][j] == 79)
					countO++;
			}

			if (countX == size || countO == size)        // All Row entries are filled with X or O
			{
				winnerMsg(ch, mode);
				return true;
			}
		}

		countX = 0, countO = 0;
		for (int i = 0; i<size; i++)     //Main Diagonal win check
		{
			if (Grid[i][i] == 88)
			{
				countX++;
			}

			else if (Grid[i][i] == 79)
			{
				countO++;
			}

			if (countX == size || countO == size)        // All diagonal entries are filled with X or O
			{
				winnerMsg(ch, mode);
				return true;
			}
		}

		countX = 0, countO = 0;
		for (int i = 0, j = size - 1; i<size; i++)    //Secondary Diagonal win check
		{
			if (Grid[i][j - i] == 88)
			{
				countX++;
			}

			else if (Grid[i][j - i] == 79)
			{
				countO++;
			}

			if (countX == size || countO == size)        // All diagonal entries are filled with X or O
			{
				winnerMsg(ch, mode);
				return true;
			}
		}

		return false;
	}

	void winnerMsg(char ch, int mode)
	{
		if (mode == 1)
		{
			if (ch == 'X')
			{
				displayGrid();
				cout << "\n\n   You Win (^.^) \n\n";
			}

			if (ch == 'O')
				cout << "\n\n   You Lose (-_-) \n\n";
		}

		else if (mode == 2)
		{
			cout << "\n\n   Player " << ch << " Wins \\o/ \n\n";
		}
	}

	bool draw(char ch, int mode)
	{
		for (int i = 0; i<size; i++)
			for (int j = 0; j<size; j++)
			{
				if (Grid[i][j] != 88 && Grid[i][j] != 79)    //not all entries are filled with X or O
					return false;
			}
		if (mode == 1)
		{
			if (ch == 'X')
				displayGrid();
		}
		cout << "\n\tMatch Draw (-_-) \n\n";          //no entry left
		return true;
	}

	int playAgain()
	{
		cout << "\n Want to play again [Y/N]: ";
		char ch;
		cin >> ch;
		if (ch == 'y' || ch == 'Y')
		{
			menu();
			return 0;
		}
		else if (ch == 'n' || ch == 'N')
			return 0;
		else
		{
			cout << " Invalid option.\n";
			cin.clear();        //to avoid infinite loop in case input is more than 1byte
			cin.ignore();
			playAgain();
		}
		return 0;
	}

	int Read_No_of_Line()
	{
		ifstream inf("SaveCount.txt");
		if (!inf)
			return 0;       //if file does not exists return 0
		int counter;
		inf >> counter;
		inf.close();

		return counter;
	}

	void write_No_of_Line(int counter)
	{
		ofstream outf("SaveCount.txt");
		outf << counter;
		outf.close();
	}

	void save(int turn, int mode)
	{
		int counter = 0;
		if (Read_No_of_Line() >= 1)
			counter = Read_No_of_Line();
		counter = counter + 1;
		write_No_of_Line(counter);

		ofstream out("Save.txt", ios::app);
		string Time = currentDateTime();

		out.write(reinterpret_cast<char *>(&counter), sizeof(int));
		out.write(reinterpret_cast<char *>(&mode), sizeof(int));
		out.write(reinterpret_cast<char *>(&turn), sizeof(int));
		out.write(reinterpret_cast<char *>(&size), sizeof(int));

		for (int i = 0; i<size; i++)
			for (int j = 0; j<size; j++)
				out.write(reinterpret_cast<char *>(&Grid[i][j]), sizeof(int));

		out.close();
	}

	int loadGame()
	{
		string date;
		int counter, Array[25], mode, turn, lines = Read_No_of_Line();
		cout << "\nSaved Games: " << lines << '\n';
		ifstream in("Save.txt");
		if (!in)
		{
			cout << "\n No saved game.";
			return 0;
		}
		in.read(reinterpret_cast<char *>(&counter), sizeof(int));
		in.read(reinterpret_cast<char *>(&Save[0]), sizeof(int));
		in.read(reinterpret_cast<char *>(&Save[1]), sizeof(int));
		in.read(reinterpret_cast<char *>(&size), sizeof(int));
		cout << "---> Slot " << counter << '\n';
		for (int j = 0; j<size*size; j++)
			in.read(reinterpret_cast<char*>(&Array[j]), sizeof(int));

		for (; counter<lines;)
		{
			in.read(reinterpret_cast<char *>(&counter), sizeof(int));
			cout << "---> Slot " << counter << '\n';
			in.read(reinterpret_cast<char *>(&mode), sizeof(int));
			in.read(reinterpret_cast<char *>(&turn), sizeof(int));
			in.read(reinterpret_cast<char *>(&size), sizeof(int));
			for (int l = 0; l<size*size; l++)
				in.read(reinterpret_cast<char*>(&Array[l]), sizeof(int));
		}
		in.close();
		return 1;
	}

	int selectGame()
	{
	again:
		cout << "\nSelect a Game to load : ";
		int op, lines;
		cin >> op;

		lines = Read_No_of_Line();
		if (op<1 || op>lines)
		{
			cin.clear();
			cin.ignore();
			cout << "\nInvalid option\n";
			goto again;
		}

		ifstream in("Save.txt");
		int counter, Array[25];
		string date;

		in.read(reinterpret_cast<char *>(&counter), sizeof(int));
		in.read(reinterpret_cast<char *>(&Save[0]), sizeof(int));
		in.read(reinterpret_cast<char *>(&Save[1]), sizeof(int));
		in.read(reinterpret_cast<char *>(&size), sizeof(int));
		for (int i = 0; i<size*size; i++)
			in.read(reinterpret_cast<char*>(&Array[i]), sizeof(int));

		while (lines != 0)
		{
			if (counter == op)
			{
				this->Grid = new int*[size];
				for (int i = 0, k = 0; i<size; i++)
				{
					this->Grid[i] = new int[size];
					for (int j = 0; j<size; j++, k++)
						this->Grid[i][j] = Array[k];       //set grid entries to loaded state
				}
				return 3;
			}
			in.read(reinterpret_cast<char *>(&counter), sizeof(int));
			in.read(reinterpret_cast<char *>(&Save[0]), sizeof(int));
			in.read(reinterpret_cast<char *>(&Save[1]), sizeof(int));
			in.read(reinterpret_cast<char *>(&size), sizeof(int));

			for (int i = 0; i<size*size; i++)
				in.read(reinterpret_cast<char*>(&Array[i]), sizeof(int));
			lines--;
		}
		in.close();
		return 0;
	}

	void deleteGame()
	{
	again:
		cout << "\nSelect a Game to Delete: \n";

		int op, counter, newLines, lines, Array[25];
		string date;
		cin >> op;

		lines = Read_No_of_Line();

		if (op > 0 && op <= lines)
		{
			newLines = lines - 1;
			write_No_of_Line(newLines);
		}
		else
		{
			cin.clear();
			cin.ignore();
			cout << "\nInvalid option\n";
			goto again;
		}
		ifstream in("Save.txt");
		ofstream temp("temp.dat");

		while (lines >= 0)
		{
			in.read(reinterpret_cast<char *>(&counter), sizeof(int));
			in.read(reinterpret_cast<char *>(&Save[0]), sizeof(int));
			in.read(reinterpret_cast<char *>(&Save[1]), sizeof(int));
			in.read(reinterpret_cast<char *>(&size), sizeof(int));

			for (int i = 0; i<size*size; i++)
				in.read(reinterpret_cast<char*>(&Array[i]), sizeof(int));

			if (op != counter)
			{
				if (op < counter)
				{
					counter = counter - 1;
				}

				temp.write(reinterpret_cast<char *>(&counter), sizeof(int));
				temp.write(reinterpret_cast<char *>(&Save[0]), sizeof(int));
				temp.write(reinterpret_cast<char *>(&Save[1]), sizeof(int));
				temp.write(reinterpret_cast<char *>(&size), sizeof(int));
				for (int i = 0; i<size*size; i++)
					temp.write(reinterpret_cast<char*>(&Array[i]), sizeof(int));
			}
			lines--;
		}

		in.close();
		temp.close();

		remove("Save.txt");
		rename("temp.dat", "Save.txt");
		cout << "\nDeleted Successfully!\n\n";

	}

	const std::string currentDateTime()
	{
		time_t now = time(0);  //current time
		struct tm Time;
		char String[80];

		Time = *localtime(&now);
		
		strftime(String, sizeof(String), "%d/%m/%Y%X", &Time);

		return String;
	}

	~TicTacToe()
	{
		for (int i = 0; i < size; i++)
			delete[] Grid[i];
		delete[] Grid;
	}
};

int main()
{
	system("Color F3");
	TicTacToe obj;
	return 0;
}
