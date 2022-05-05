#pragma once
#include"Func.h"
#include"Timer.h"
#include<fstream>
#include<iomanip>
#include <filesystem>
#include <Windows.h>
#include<MMSystem.h>


bool elemTaile(point p1, point p2, int j)
{
	if (j == 0) return false;
	else return (p1.x == p2.x && p1.y == p2.y);
}


template<class T>
int kanibal(const T* a, int n, T key, bool (*metchod)(T, T, int))
{
	if (n == 0) return -1;
	for (size_t i = 0; i < n; i++)
	{
		if (metchod(key, a[i], i))
			return i;

	}
	return -1;
}

bool fruit_in_snake_Tail(point p1, point p2)
{
	return (p1.x == p2.x && p1.y == p2.y);
}

template<class T>
int lineSearch(const T* a, int n, T key, bool(*metchod)(T, T))
{
	for (size_t i = 0; i < n; i++)
		if (metchod(a[i], key)) return i;

	return -1;

}


struct snake
{
	// ������� ����
	int row = 26;			// y 
	int col = 52;			// x



	point fruit;			// ������� ������� �� ��������� x � y
	point pos;				// ������� ������ ������ �� ��������� x � y


	bool Game_Status;		// ������ ����. false - ���� ���������, true - ���� ������������

	int dir;				// ����������� ������. 1 - �����, 2 - ����, 3 - ������, 4 - ������


	int score;				// ����
	int eaten_fruit = 0;	// ���������� ��������� �������
	float time = 0;				// ����� ���� (� ��������)

	point* tail = nullptr;	// ������ �� ����� ���������� ���� ������
	int size_tail = 0;		// ����������� ���� ������

	int speed = 65;			// �������� ������������ ������ (����� ��������, ��� ������, ��� �������)
	int balance_speed;		// ���������������� ��������(���-��� � ������� ������ ������ �� ������� � � ���� ������ ������ � ��� ����)

	// ����� ������	(����,������, �������� ������, ������)
	int colorField  = 4;
	int colorBorder = 7;
	int colorSnake  = 1;
	int colorFruit  = 14;


	// ���������� ��� ��������� ���������� �������������� ������
	int direction_UP    = 72;
	int direction_DOWN  = 80;
	int direction_LEFT  = 75;
	int direction_RIGHT = 77;
	int EXIT            = 27;

	int game_mode = 1;  // ������� ����� (0 - "� �������", 1 - "������������ ����")

	// ��������� ���� (������������ � ������ "���������� ����")
	int tmp_score;


	// ������� ������������� ���� ��� � ����� ���� (����� ����������)
	// ������� �������
	void end_of_game()
	{

		delete[]tail;
		tail = nullptr;
		size_tail = 0;
	}

	// ������� �������������� ���������� ����
	void setup()
	{
		Game_Status = true;

		dir = 4;

		fruit = { rand() % (col - 2) + 1, rand() % (row - 2) + 1};

		pos = { col / 2, row / 2 };

		score = 0;
		eaten_fruit = 0;
		time = 0;
		size_tail = 0;
	}

	// ��������� ���������� ���� 
	void printField()
	{
		system("cls");


		for (size_t i = 0; i < col; i++)
		{
			SetColor(7, colorBorder);
			gotoxy(i, 0);
			cout << ' ';
			SetColor(7, 0);
		}


		for (size_t i = 1; i < row; i++)
		{
			for (size_t j = 0; j < col; j++)
			{
				if (j == 0 || j == col - 1)
				{
					SetColor(7, colorBorder);
					gotoxy(j, i);
					cout << ' ';
					SetColor(7, 0);
				}
				else
				{
					SetColor(7, colorField);
					gotoxy(j, i);
					cout << ' ';
					SetColor(7, 0);
				}

			}
		}

		for (size_t i = 0; i < col; i++)
		{
			SetColor(7, colorBorder);
			gotoxy(i, row - 1);
			cout << ' ';
			SetColor(7, 0);

		}


		gotoxy(fruit.x, fruit.y);
		SetColor(0, colorFruit);
		cout << ' ';
		SetColor(7, 0);

		gotoxy(col + 5, 0);
		SetColor(Yellow, 0);
		cout << "INFO";
		SetColor(Green, 0);

		gotoxy(col + 2, 2);
		cout << "����: " << score;

		gotoxy(col + 2, 4);
		SetColor(11, 0);
		cout << "�������� �������: " << eaten_fruit;

		gotoxy(col + 2, 6);
		SetColor(13, 0);
		cout << "�����: " << time << " c��";

		SetColor(10, 0);
		gotoxy(1, 27);
		cout << "����� ����: ";
		switch (game_mode)
		{
		case 0: cout << "\"�����������\""; break;
		case 1: cout << "\"���������� ����\""; break;
		}


		SetColor(8, 0);
		gotoxy(35, 27);
		cout << "������� ����: ";
		switch (speed)
		{
		case 300: cout << "��������";	  break;
		case 200: cout << "������";		  break;
		case  65: cout << "����� ������"; break;
		case  30: cout << "����������";	  break;
		}

		SetColor(7, 0);
	}


	void input()
	{
		if (_kbhit())
		{
			int c = _getch();


			if (c == EXIT)
			{
				Game_Status = false;
				return;
			}
			else
			{
				// � ��������������� ����������� ���� ������ ���������(���� ���� �����)!!!
				if (direction_UP == c) if (dir != 2 || (size_tail == 0)) dir = 4;
				if (direction_DOWN == c) if (dir != 4 || (size_tail == 0)) dir = 2;
				if (direction_LEFT == c) if (dir != 3 || (size_tail == 0)) dir = 1;
				if (direction_RIGHT == c) if (dir != 1 || (size_tail == 0)) dir = 3;
			}
		}
	}

	bool logic()
	{
		ConsoleCursorStatus(false);
		switch (dir)
		{
		case 1:
			pos.x--;
			break;
		case 2:
			pos.y++;
			break;
		case 3:
			pos.x++;
			break;
		case 4:
			pos.y--;
			break;
		default:
			break;
		}


		switch (game_mode)
		{
		case 0:
			if ((pos.y >= row - 1 || pos.y <= 0) || (pos.x >= col - 1 || pos.x <= 0))
				return false;
		case 1:
			if ((pos.y >= row - 1 || pos.y <= 0) || (pos.x >= col - 1 || pos.x <= 0))
			{
				if (pos.y == row - 1) { pos.y = 1; }
				if (pos.y == 0)		  { pos.y = row - 2; }
				if (pos.x == 0)		  { pos.x = col - 2; }
				if (pos.x == col - 1) { pos.x = 1; }
			}
			break;
		default:
			break;
		}

		addElem(tail, size_tail, pos, 0);

		if (pos.x == fruit.x && pos.y == fruit.y)
		{
			point new_head = pos;

			switch (dir)
			{
			case 1:
				new_head.x--;
				break;
			case 2:
				new_head.y++;
				break;
			case 3:
				new_head.x++;
				break;
			case 4:
				new_head.y--;
				break;
			default:
				break;
			}

			addElem(tail, size_tail, new_head);
			gotoxy(new_head.x, new_head.y);
			SetColor(Green, colorSnake);
			cout << ' ';


			do {


				fruit = { rand() % (col - 2) + 1, rand() % (row - 2) + 1 };
				if (game_mode == 0) tmp_score = 10;
				else
				{
					if (game_mode == 1)
					{
						do
						{
							colorFruit = rand() % (16) + 1;
						}while (colorFruit == colorField || colorFruit == colorSnake || colorFruit == colorBorder);

						switch (colorFruit)
						{
						case 0:case 1:case 2:case 3:case 4:
							tmp_score = 50;
							break;
						case 5:case 6:case 7:case 8:case 9:
							tmp_score = 100;
							break;
						case 10:case 11:case 12:case 13:case 14:
							tmp_score = 200;
							break;
						case 15:case 16:
							tmp_score = 250;
							break;
						}
					}
				}

			} while (lineSearch(tail, size_tail, fruit, fruit_in_snake_Tail) != -1);

			gotoxy(fruit.x, fruit.y);
			SetColor(7, colorFruit);
			cout << ' ';
			SetColor(7, 0);

			score += tmp_score;
			eaten_fruit++;

			gotoxy(col + 2, 2);
			SetColor(Green, 0);
			cout << "����: " << score;

			gotoxy(col + 2, 4);
			SetColor(11, 0);
			cout << "�������� �������: " << eaten_fruit;
		}

		switch (dir)
		{
		case 2: case 4: balance_speed = speed * 2;
		case 3: case 1: balance_speed = speed;
		}

		gotoxy(tail[0].x, tail[0].y);
		SetColor(Green, colorSnake);
		cout << ' ';
		SetColor(0, colorField);
		Sleep(balance_speed);

		if (kanibal(tail, size_tail, tail[0], elemTaile) != -1) return false;

		gotoxy(tail[size_tail - 1].x, tail[size_tail - 1].y);
		cout << ' ';
		SetColor(7, 0);
		delElem(tail, size_tail, size_tail - 1);
		return true;
	}
	void start()
	{
		char game_over[] = "Game Over";

		setup();

		printField();
		while (Game_Status == true)
		{
			Timer t;

			input();
			if (Game_Status == false) break;
			Game_Status = logic();

			time += t.elapsed();
			gotoxy(col + 2, 6);
			SetColor(13, 0);
			cout << fixed << setprecision(2);
			cout << "�����: " << time << " c��";
		}


		for (size_t i = 0; i < 40; i++)
		{
			for (size_t j = 0; j < 80; j++)
			{
				gotoxy(j, i);
				cout << " ";
			}

		}

		system("cls");
		gotoxy(20, 13);

		SetColor(Red, 7);
		for (size_t i = 0; i < 9; i++)
		{
			cout << game_over[i] << " ";
			Sleep(100);
		}
		SetColor(7, 0);

		cout << endl << endl;

		end_of_game();

		system("pause");

	}

	void save_color_conf()
	{
		ofstream save("Color_config.bin", ios::binary);

		save.write((char*)&colorField, sizeof(int));
		save.write((char*)&colorBorder, sizeof(int));
		save.write((char*)&colorSnake, sizeof(int));
		save.write((char*)&colorFruit, sizeof(int));


		save.close();
	}

	void load_color_config()
	{
		ifstream load("Color_config.bin", ios::binary);

		if (load.is_open())
		{
			load.read((char*)&colorField, sizeof(int));
			load.read((char*)&colorBorder, sizeof(int));
			load.read((char*)&colorSnake, sizeof(int));
			load.read((char*)&colorFruit, sizeof(int));
		}
		load.close();
	}


	void select_color()
	{
		load_color_config();
		
		Menu m;
		Menu2D select_memu;
		vector<string> t = {"��������� ������", "������� ���������"};

		vector<string> ColorsField_t =
		{
			"< ���� ����: ������ >", "< ���� ����: c���� >", "< ���� ����: ������� >", "< ���� ����: ������� >","< ���� ����: ������� >","< ���� ����: ����������","< ���� ����: �����-������ >",
			"< ���� ����: ����� >","< ���� ����: �����-����� >","< ���� ����: ��������� >", "< ���� ����: ������-������� >","< ���� ����: ��������� >","< ���� ����: ������-������� >","< ���� ����: ������-���������� >",
			"< ���� ����: ���� ������ >","< ���� ����: ����� >"
		};

		vector<string> ColorsBorder_t =
		{
			"< ���� �������: ������ >", "< ���� �������: c���� >", "< ���� �������: ������� >", "< ���� �������: ������� >","< ���� �������: ������� >","< ���� �������: ����������","< ���� �������: �����-������ >",
			"< ���� �������: ����� >","< ���� �������: �����-����� >","< ���� �������: ��������� >", "< ���� �������: ������-������� >","< ���� �������: ��������� >","< ���� �������: ������-������� >","< ���� ����: ������-���������� >",
			"< ���� �������: ���� ������ >","< ���� �������: ����� >"
		};

		vector<string> ColorsSnake_t =
		{
		"< ���� ������: ������ >", "< ���� ������: c���� >", "< ���� ������: ������� >", "< ���� ������: ������� >","< ���� ������: ������� >","< ���� ������: ����������","< ���� ������: �����-������ >",
		"< ���� ������: ����� >","< ���� ������: �����-����� >","< ���� ������: ��������� >", "< ���� ������: ������-������� >","< ���� ������: ��������� >","< ���� ������: ������-������� >","< ���� ������: ������-���������� >",
		"< ���� ������: ���� ������ >","< ���� ������: ����� >"
		};

		vector<string> ColorsFruit_t =
		{
		"< ���� �������: ������ >", "< ���� �������: c���� >", "< ���� �������: ������� >", "< ���� �������: ������� >","< ���� �������: ������� >","< ���� �������: ����������","< ���� �������: �����-������ >",
		"< ���� �������: ����� >","< ���� �������: �����-����� >","< ���� �������: ��������� >", "< ���� �������: ������-������� >","< ���� �������: ��������� >","< ���� �������: ������-������� >","< ���� �������: ������-���������� >",
		"< ���� �������: ���� ������ >","< ���� �������: ����� >"
		};

		vector<vector<string>> res_m = { ColorsField_t , ColorsBorder_t, ColorsSnake_t , ColorsFruit_t };
		
		while (true)
		{
			Clear;
			int* pos;
			int start_pos[] = { colorField, colorBorder, colorSnake, colorFruit };
			gotoxy(24, 10);
			SetColor(Yellow, 0);
			cout << "��������� ������";
			SetColor(7, 0);
			int	ind = m.select_vertical(t, 24, 12);


			switch (ind)
			{
			case 0:
				Clear
				pos = select_memu.select_vertical(res_m, 20, 12, start_pos);

				colorField = pos[0];
				colorBorder = pos[1];
				colorSnake = pos[2];
				colorFruit = pos[3];

				save_color_conf();
				SetColor(7, 0);
				break;
			case 1:
				Clear;
				SetColor(Yellow, 0);
				gotoxy(20, 9);
				cout << "������� �������� ���������";
				SetColor(7, 0);


				gotoxy(21, 11);
				cout << "���� �������� ���� - ";
				SetColor(0, colorField);
				cout << " " << endl;
				SetColor(7, 0);


				gotoxy(21, 13);
				cout << "���� ������ - ";
				SetColor(0, colorBorder);
				cout << " " << endl;
				SetColor(7, 0);

				gotoxy(21, 15);
				cout << "���� ������ - ";
				SetColor(0, colorSnake);
				cout << " " << endl;
				SetColor(7, 0);

				gotoxy(21, 17);
				cout << "���� ������� - ";
				SetColor(0, colorFruit);
				cout << " " << endl;
				SetColor(7, 0);

				gotoxy(18, 20);
				Pause
				break;
			case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}
				
		}
	

	}

	void miniASCIt(int num)
	{
		switch (num)
		{
		case 134: cout << "F12";			  break;
		case 133: cout << "F11";			  break;
		case  72: cout << "c�������� �����";  break;
		case  80: cout << "c�������� ����";   break;
		case  75: cout << "c�������� �����";  break;
		case  77: cout << "c�������� ������"; break;
		case  27: cout << "ESC";			  break;
		case  32: cout << "������";			  break;
		case  13: cout << "Enter";            break;
		case   8: cout << "backspace";		  break;
		case   9: cout << "Tab";			  break;
		case  96: cout << "�";				  break;
		case   0: cout << "F1-F10";			  break;
		case   82: cout << "Insert(INS)";	  break;
		case   71: cout << "Home(HM)";		  break;
		case   83: cout << "DEL";			  break;
		case   79: cout << "END";			  break;
		default: cout << (char)num;			  break;
		}
	}

	void snake_control()
	{
		Menu m;
		vector<string> text_s = {
								"�������� ���������� ��� �������� ������",
								"�������� ���������� ��� �������� �����",
								"�������� ���������� ��� �������� ������",
								"�������� ���������� ��� �������� �����",
								"����� �� ����",
								"������� ���������",
							   };

		while (true)
		{
			load_control_config();
			Clear
				int ind = m.select_vertical(text_s, 16, 12);
			int c, k = 0;


			switch (ind)
			{
			case 0:
				Clear
					gotoxy(14, 12);
				SetColor(Yellow, 0);
				cout << "���������� ������� ��� �������� ������";
				SetColor(7, 0);
				do
				{
					if (k == 1)
					{
						gotoxy(14, 12);
						SetColor(Red, 0);
						cout << "���� ����������, ���� �� ������ ���������!!!";
						SetColor(7, 0);
					}
					gotoxy(14, 14);
					cout << "������� ����� ������� ����� ���������: ";
					c = _getch();
					if (c == 224) c = _getch();
					SetColor(Yellow, 0);
					miniASCIt(c);
					SetColor(7, 0);
					k++;
				} while (c == direction_RIGHT || c == direction_LEFT || c == direction_DOWN);
				direction_UP = c;
				save_control_config();
				gotoxy(14, 16);
				SetColor(Green, 0);
				cout << "������� ������� ���������";
				SetColor(7, 0);
				gotoxy(14, 18);
				Pause
					break;
			case 1:
				Clear
					gotoxy(14, 12);
				SetColor(Yellow, 0);
				cout << "���������� ������� ��� �������� �����";
				SetColor(7, 0);

				do
				{
					if (k == 1)
					{
						gotoxy(14, 12);
						SetColor(Red, 0);
						cout << "���� ����������, ���� �� ������ ���������!!!";
						SetColor(7, 0);
					}
					gotoxy(14, 14);
					cout << "������� ����� ������� ����� ���������: ";
					c = _getch();
					if (c == 224) c = _getch();
					SetColor(Yellow, 0);
					miniASCIt(c);
					SetColor(7, 0);
					k++;
				} while (c == direction_RIGHT || c == direction_LEFT || c == direction_UP);
				direction_DOWN = c;
				save_control_config();
				gotoxy(14, 16);
				SetColor(Green, 0);
				cout << "������� ������� ���������";
				SetColor(7, 0);
				gotoxy(14, 18);
				Pause
					break;
			case 2:
				Clear
					gotoxy(14, 12);
				SetColor(Yellow, 0);
				cout << "���������� ������� ��� �������� ������";
				SetColor(7, 0);

				do
				{
					if (k == 1)
					{
						gotoxy(14, 12);
						SetColor(Red, 0);
						cout << "���� ����������, ���� �� ������ ���������!!!";
						SetColor(7, 0);
					}
					gotoxy(14, 14);
					cout << "������� ����� ������� ����� ���������: ";
					c = _getch();
					if (c == 224) c = _getch();
					SetColor(Yellow, 0);
					miniASCIt(c);
					SetColor(7, 0);
					k++;
				} while (c == direction_UP || c == direction_LEFT || c == direction_DOWN);
				direction_RIGHT = c;
				save_control_config();
				gotoxy(14, 16);
				SetColor(Green, 0);
				cout << "������� ������� ���������";
				SetColor(7, 0);
				gotoxy(14, 18);
				Pause
					break;
			case 3:
				Clear
					gotoxy(14, 12);
				SetColor(Yellow, 0);
				cout << "���������� ������� ��� �������� �����";
				SetColor(7, 0);
				do
				{
					if (k == 1)
					{
						gotoxy(14, 12);
						SetColor(Red, 0);
						cout << "���� ����������, ���� �� ������ ���������!!!";
						SetColor(7, 0);
					}
					gotoxy(14, 14);
					cout << "������� ����� ������� ����� ���������: ";
					c = _getch();
					if (c == 224) c = _getch();
					SetColor(Yellow, 0);
					miniASCIt(c);
					SetColor(7, 0);
					k++;
				} while (c == direction_RIGHT || c == direction_UP || c == direction_DOWN);
				direction_LEFT = c;
				save_control_config();
				gotoxy(14, 16);
				SetColor(Green, 0);
				cout << "������� ������� ���������";
				SetColor(7, 0);
				gotoxy(14, 18);
				Pause
					break;
			case 4:
				Clear
					gotoxy(14, 12);
				cout << "���������� ������� ��� ������ �� ����";

				do
				{
					if (k == 1)
					{
						gotoxy(14, 12);
						SetColor(Red, 0);
						cout << "���� ����������, ���� �� ������ ���������!!!";
						SetColor(7, 0);
					}
					gotoxy(14, 14);
					cout << "������� ����� ������� ����� ���������: ";
					c = _getch();
					if (c == 224) c = _getch();
					SetColor(Yellow, 0);
					miniASCIt(c);
					SetColor(7, 0);
					k++;
				} while (c == direction_RIGHT || c == direction_UP || c == direction_DOWN || c == direction_LEFT);
				EXIT = c;
				save_control_config();
				gotoxy(14, 16);
				SetColor(Green, 0);
				cout << "������� ������� ���������";
				SetColor(7, 0);
				gotoxy(14, 18);
				Pause

					break;
			case 5:
				Clear
					gotoxy(15, 10);

				SetColor(Yellow, 0);
				cout << "�������� ������� �������� ����������";
				SetColor(7, 0);

				gotoxy(21, 12);
				cout << "������ - ";
				miniASCIt(direction_UP);

				gotoxy(21, 13);
				cout << "����� - ";
				miniASCIt(direction_DOWN);
				gotoxy(21, 14);

				cout << "����� - ";
				miniASCIt(direction_LEFT);
				gotoxy(21, 15);

				cout << "������ - ";
				miniASCIt(direction_RIGHT);
				gotoxy(21, 16);

				cout << "����� �� ���� - ";
				miniASCIt(EXIT);
				gotoxy(15, 20);
				Pause
					break;
			case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}
		}
	}

	void save_control_config()
	{
		ofstream save("Control_config.bin", ios::binary);

		save.write((char*)&direction_UP, sizeof(int));
		save.write((char*)&direction_DOWN, sizeof(int));
		save.write((char*)&direction_LEFT, sizeof(int));
		save.write((char*)&direction_RIGHT, sizeof(int));
		save.write((char*)&EXIT, sizeof(int));

		save.close();
	}


	void key_assignment()
	{
		Menu m;
		vector<string> t = { "���������� �� ����� ����"};


		while (true)
		{
			Clear
			gotoxy(22, 9);
			SetColor(Yellow, 0);
			cout << "��������� ����������";
			int ind = m.select_vertical(t, 20, 12);
			switch (ind)
			{
			case 0:
				snake_control();
				break;
			case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}


		}
	}

	void save_other_config()
	{
		ofstream save("other_config.bin", ios::binary);

		save.write((char*)&speed, sizeof(int));
		save.write((char*)&game_mode, sizeof(int));
		save.close();
	}
	void load_other_confing()
	{
		ifstream load("other_config.bin", ios::binary);

		if (load.is_open())
		{
			load.read((char*)&speed, sizeof(int));
			load.read((char*)&game_mode, sizeof(int));
		}
		load.close();
	}

	void game_modes()
	{
		Menu m;
		vector<string> t = { "�����������", "���������� ����..."};
		vector<string> t1 = { "����������", "���������� ��� �����"};

		while (true)
		{
			load_other_confing();
			Clear
			gotoxy(22, 8);
			SetColor(Green, 0);
			cout << "����� ������ ����";

			gotoxy(14, 10);
			SetColor(Yellow, 0);
			cout << "������� ����� ����� - ";
			switch (game_mode)
			{	
			case 0:
				cout << "�����������";
				break;
			case 1:
				cout << "���������� ����";
				break;
			default:
				break;
			}
			SetColor(7, 0);
			int ind = m.select_vertical(t, 22, 13);

			switch (ind)
			{
			case 0:
				while (true)
				{
					Clear
					gotoxy(20, 9);
					SetColor(Green, 0);
					cout << "����� \"�����������\"";

					int ind1 = m.select_vertical(t1, 20, 12);
					switch (ind1)
					{
					case 0:
						Clear
						game_mode = 0;
						save_other_config();

						gotoxy(20, 10);
						SetColor(Green, 0);
						cout << "����� ���� ������� ����������";
						SetColor(7, 0);
						gotoxy(18, 11);
						Pause
						return;
						break;
					case 1:
						Clear

						gotoxy(20, 8);
						SetColor(Green, 0);
						cout << "����� \"�����������\"";

						SetColor(Yellow, 0);
						gotoxy(12, 12);
						cout << "� ������ ������ ����, ����� ������";
						gotoxy(11, 13);
						cout << "������������� � �������� ����, ���� �������������";
						gotoxy(11, 15);
						Pause
						break;
					case -1:
						SetColor(7, 0);
						return;
						break;
					default:
						break;
					}

				}
				break;
			case 1:
				Clear
				while (true)
				{

					Clear
					
					gotoxy(20, 9);
					SetColor(Green, 0);
					cout << "����� \"���������� ����\"";
					int ind1 = m.select_vertical(t1, 22, 13);
					switch (ind1)
					{
					case 0:
						Clear
						game_mode = 1;
						save_other_config();
						gotoxy(18, 11);
						SetColor(Green, 0);
						cout << "����� ���� ������� ����������";
						SetColor(7, 0);
						gotoxy(18, 13);
						Pause
						return;
						break;
					case 1:
						Clear

						gotoxy(20, 11);
						SetColor(Green, 0);
						cout << "\"���������� ����\"";

						SetColor(Yellow, 0);
						gotoxy(13, 13);
						cout << "� ������ ������ ����, ����� ������";
						gotoxy(13, 14);
						cout << "������������� � �������� ����,";
						gotoxy(13, 15);
						cout << "��� ������� �� ���������������� ���� ����,";
						gotoxy(13, 16);
						cout << "��������� ������, ���� �� ������� �����";
						gotoxy(13, 18);
						Pause
						break;
					case -1:
						SetColor(7, 0);
						return;
						break;
					}
				}
			case -1:
				SetColor(7, 0);
				return;
				break;
			}

		}

	}
	void set_speed()
	{
		Menu m;
		vector<string> t = {
							"��������",
							"������", 
							"����� ������",
							"����������",
						   };

		while (true)
		{
			load_other_confing();
			
			Clear

			gotoxy(15, 8);
			SetColor(Yellow, 0);
			cout << "������� ������� ��������: ";
			
			switch(speed)
			{
			case 300: cout << "��������";	  break;
			case 200: cout << "������";		  break;
			case  65: cout << "����� ������"; break;
			case  30: cout << "����������";	  break;
			}
			SetColor(7, 0);

			int ind = m.select_vertical(t, 26, 10);

			switch (ind)
			{
			case 0:
				speed = 300;
				Clear
				gotoxy(18, 12);
				SetColor(Green, 0);
				cout << "��������� ������� ���������";
				SetColor(7, 0);
				gotoxy(18, 13);
				save_other_config();
				Pause
				break;
			case 1:
				speed = 200;
				Clear
					gotoxy(18, 12);
				SetColor(Green, 0);
				cout << "��������� ������� ���������";
				SetColor(7, 0);
				gotoxy(18, 13);
				save_other_config();
				Pause
				break;
			case 2:
				speed = 65;
				Clear
					gotoxy(18, 12);
				SetColor(Green, 0);
				cout << "��������� ������� ���������";
				SetColor(7, 0);
				gotoxy(18, 13);
				save_other_config();
				Pause
				break;
			case 3:
				speed = 30;
				Clear
					gotoxy(18, 12);
				SetColor(Green, 0);
				cout << "��������� ������� ���������";
				SetColor(7, 0);
				gotoxy(18, 13);
				save_other_config();
				Pause
				break;
			case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}

		}


	}
	void settings()
	{
		Menu m;
		vector<string> t = { 
							"������ ����",
							"����������",
							"��������� ������ ����",
							"�������� ����",
						   };

		while (true)
		{
			Clear

			SetColor(Yellow, 0);
			gotoxy(26, 8);
			cout << "���������";
			SetColor(7, 0);
			int d = m.select_vertical(t, 20, 10);

			switch (d)
			{
			case 0:
				game_modes();
				break;
			case 1:
				key_assignment();
				break;
			case 2:
				select_color();
				break;
			case 3:
				set_speed();
				break;
			case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}


		}


	}
	void load_control_config()
	{
		ifstream load("Control_config.bin", ios::binary);

		if (load.is_open())
		{
			load.read((char*)&direction_UP, sizeof(int));
			load.read((char*)&direction_DOWN, sizeof(int));
			load.read((char*)&direction_LEFT, sizeof(int));
			load.read((char*)&direction_RIGHT, sizeof(int));
			load.read((char*)&EXIT, sizeof(int));
		}
	}
	void menu()
	{
		Menu m;
		vector<string> text = {
								"������ ����",
								"���������",
								"����� �� ����"
							  };
		
		while (true)
		{
			
			system("cls");
			gotoxy(0, 4);
			SetColor(Red, 0);
			cout << "              **    **  ********  **        **            **      " << endl
				 << "             **    **  ********  **        **         **     **   " << endl
				 << "            ********  **        **        **         **      **  " << endl
				 << "           ********  ********  **        **         **      **  " << endl
				 << "          **    **  **        **        **         **      **  " << endl
				 << "         **    **  ********  ********  ********    **    **   " << endl
				 << "        **    **  ********  ********  ********       **      " << endl;
			SetColor(7, 0);
			
			int ind = m.select_vertical(text, 27, 13);
			switch (ind)
			{
			case 0:
				system("mode con cols=75 lines=30");
				load_control_config();
				load_color_config();
				load_other_confing();
				
				start();
				break;
			case 1:
				settings();
				break;
			case 2: case -1:
				SetColor(7, 0);
				return;
				break;
			default:
				break;
			}
		}




	}
};
