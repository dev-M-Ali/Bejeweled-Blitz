#include <iostream>
#include <ctime>
#include <Windows.h>
#include <utility>
#include <thread>
#include <chrono>

using namespace std;

pair<int, int> swap1, swap2; //Pair variables that will be used to store the coordinates of the 2 swapped gems in order to reverse the swap if a match is not found, values are updated in the swap function
int TimeHasEnded = 0, Score=0;



/*
Summary:
The program has complete core functionality. Assigned shapes/gems to values 0 - 4 called ID of the shape/gem.
ID of fire/exploding gem = ID of basic gem + 10; Similarly, ID of lightning/destroyer gem = ID of basic gem + 20;
Upon finding a match, the program changes the value of the array locations to -1. This results in an explosion symbol in their place.
After this, the value of the array location becomes -2, which indicates that the explosion has been displayed and now elements should fall from above.
Integrated timer using multithreading.
*/




bool isCursorKeyPressed(int& whichKey)   //whichKey passed as reference.... 
{
	char key;
	key = GetAsyncKeyState(37);
	if (key == 1)
	{
		whichKey = 1;		// 1 if left key is pressed 
		return true;
	}
	key = GetAsyncKeyState(38);
	if (key == 1)
	{

		whichKey = 2;		// 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(39);
	if (key == 1)
	{

		whichKey = 3; // 3 if right key is pressed
		return true;
	}
	key = GetAsyncKeyState(40);
	if (key == 1)
	{

		whichKey = 4;   // 4 if down key is pressed
		return true;
	}
	key = GetAsyncKeyState(13);
	if (key == 1)
	{
		whichKey = 5;		// 5 if enter key is pressed 
		return true;
	}
	return false;
}
//Updated to also accept Enter Key
void myLine(int x1, int y1, int x2, int y2, int color) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(color, color, color)); //2 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}
void myRect(int x1, int y1, int x2, int y2, int R, int G, int B)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0)); //Fill color is passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));  //Fill color is black
	SelectObject(device_context, brush);
	Ellipse(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);


}
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}


void myThickLine(int x1, int y1, int x2, int y2) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 6, RGB(169, 169, 169)); //6 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}
//myThickLine is grey in colour
void myColouredLine(int x1, int y1, int x2, int y2, int R, int G, int B) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B)); //2 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}
void myHollowRect(int x1, int y1, int x2, int y2, int R, int G, int B)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = (HBRUSH)::GetStockObject(NULL_BRUSH);

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void mySquareJewel(int x1, int y1, int x2, int y2)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	SelectObject(device_context, pen);
	HBRUSH brush = (HBRUSH)::GetStockObject(NULL_BRUSH);


	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void myTriangleJewel(int x1, int y1, int x2, int y2)
{
	myColouredLine(x1, y2, x2, y2, 255, 0, 255);
	myColouredLine(x2, y2, (x1 + x2) / 2, y1, 255, 0, 255);
	myColouredLine((x1 + x2) / 2, y1, x1, y2, 255, 0, 255);
}
void myCircularJewel(int x1, int y1, int x2, int y2)
{
	myEllipse(x1, y1, x2, y2, 0, 255, 0);
	myEllipse(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, 0, 255, 0);
	myEllipse(x2, y1, (x1 + x2) / 2, (y1 + y2) / 2, 0, 255, 0);
	myEllipse(x1, y2, (x1 + x2) / 2, (y1 + y2) / 2, 0, 255, 0);
	myEllipse(x2, y2, (x1 + x2) / 2, (y1 + y2) / 2, 0, 255, 0);
}
void myCrystalJewel(int x1, int y1, int x2, int y2)
{
	myColouredLine((x1 + x2) / 2, y2, x1, (y1 + y2) / 2, 0, 255, 255);
	myColouredLine(x1, (y1 + y2) / 2, (x1 + x2) / 2, y1, 0, 255, 255);
	myColouredLine((x1 + x2) / 2, y1, (x1 + x2) / 2, y1, 0, 255, 255);
	myColouredLine((x1 + x2) / 2, y1, x2, (y1 + y2) / 2, 0, 255, 255);
	myColouredLine(x2, (y1 + y2) / 2, (x1 + x2) / 2, y2, 0, 255, 255);
}
void myTetraJewel(int x1, int y1, int x2, int y2)
{
	myRect(x1, y1, x2, y2, 255, 255, 255);
	myHollowRect(x1 + 5, y1 + 5, x1 + 25, y1 + 25, 255, 210, 0);
	myHollowRect(x1 + 5, y2 - 5, x1 + 25, y2 - 25, 255, 210, 0);
	myHollowRect(x2 - 5, y2 - 5, x2 - 25, y2 - 25, 255, 210, 0);
	myHollowRect(x2 - 5, y1 + 5, x2 - 25, y1 + 25, 255, 210, 0);
}
void myExplosion1(int x1, int y1, int x2, int y2)		 //x1, y1, x2 and y2 should be coordinates of the boundary square of the shape
{
	myEllipse(x1, y1, x2, y2, 255, 255, 255);
	myEllipse(x1 + 40, y1 + 40, x2 - 40, y2 - 40, 255, 215, 0);
}
void myFlame(int x1, int y1)
{

	//-10 is done to correct the position of the flame according to my board, did so using find and replace to save time

	//Dark red
	myColouredLine(x1 - 10 + 22, y1 + 70, x1 - 10 + 10, y1 + 45, 213, 0, 0);
	myColouredLine(x1 - 10 + 10, y1 + 45, x1 - 10 + 13, y1 + 23, 213, 0, 0);
	myColouredLine(x1 - 10 + 13, y1 + 23, x1 - 10 + 20, y1 + 12, 213, 0, 0);
	myColouredLine(x1 - 10 + 20, y1 + 12, x1 - 10 + 18, y1 + 7, 213, 0, 0);
	myColouredLine(x1 - 10 + 18, y1 + 7, x1 - 10 + 12, y1 + 3, 213, 0, 0);
	myColouredLine(x1 - 10 + 12, y1 + 3, x1 - 10 + 29, y1 + 5, 213, 0, 0);
	myColouredLine(x1 - 10 + 29, y1 + 5, x1 - 10 + 48, y1 + 15, 213, 0, 0);
	myColouredLine(x1 - 10 + 48, y1 + 15, x1 - 10 + 57, y1 + 25, 213, 0, 0);
	myColouredLine(x1 - 10 + 57, y1 + 25, x1 - 10 + 61, y1 + 50, 213, 0, 0);
	myColouredLine(x1 - 10 + 61, y1 + 50, x1 - 10 + 58, y1 + 57, 213, 0, 0);
	myColouredLine(x1 - 10 + 58, y1 + 57, x1 - 10 + 55, y1 + 63, 213, 0, 0);
	myColouredLine(x1 - 10 + 55, y1 + 63, x1 - 10 + 51, y1 + 70, 213, 0, 0);

	//Yellow
	myEllipse(x1 - 10 + 14, y1 + 35, x1 - 10 + 58, y1 + 70, 255, 193, 7);
	myEllipse(x1 - 10 + 17, y1 + 24, x1 - 10 + 55, y1 + 68, 255, 193, 7);

	//Gold
	myEllipse(x1 - 10 + 24, y1 + 24, x1 - 10 + 48, y1 + 70, 255, 235, 59);
}
void myElectric(int x1, int y1)
{
	//-25 is written in just to position lightning symbol correctly inside each shape, did so using find and replace to save time

	myColouredLine(x1 - 25 + 45, y1 - 25 + 31, x1 - 25 + 62, y1 - 25 + 31, 255, 239, 59);
	myColouredLine(x1 - 25 + 62, y1 - 25 + 31, x1 - 25 + 52, y1 - 25 + 44, 255, 239, 59);
	myColouredLine(x1 - 25 + 52, y1 - 25 + 44, x1 - 25 + 61, y1 - 25 + 44, 255, 239, 59);
	myColouredLine(x1 - 25 + 61, y1 - 25 + 44, x1 - 25 + 38, y1 - 25 + 69, 255, 239, 59);
	myColouredLine(x1 - 25 + 38, y1 - 25 + 69, x1 - 25 + 45, y1 - 25 + 47, 255, 239, 59);
	myColouredLine(x1 - 25 + 45, y1 - 25 + 47, x1 - 25 + 35, y1 - 25 + 47, 255, 239, 59);
	myColouredLine(x1 - 25 + 35, y1 - 25 + 47, x1 - 25 + 45, y1 - 25 + 31, 255, 239, 59);
}
void myRectG(int x1, int y1, int x2, int y2, int R, int G, int B) //myRect filled with green colour
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(0, 255, 0)); //Fill color is passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void loadingScreen()
{
	int flag = 1;
	system("color D0");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\tLoading...";
	Sleep(1000);
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\tBejeweled Blitz";
	Sleep(2000);
	cout << "\n\n\n\t\t\t\t\t\t\t\t\t\t\t     Press Enter Key to Start";
	while (flag)
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			flag = 0;
			Sleep(100);
		}
	}
	system("color 07");
	system("cls");
}
void fullscreen()
{
	keybd_event(VK_MENU, 0x38, 0, 0);
	keybd_event(VK_RETURN, 0x1c, 0, 0);
	keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);

}




//myFlame functions represent the flame gems
void myFlameSquareJewel(int x1, int y1, int x2, int y2)
{
	myFlame(x1, y1 - 10);
	mySquareJewel(x1, y1, x2, y2);

}
void myFlameTriangleJewel(int x1, int y1, int x2, int y2)
{
	myFlame(x1 + 10, y1);
	myTriangleJewel(x1, y1, x2, y2);
}
void myFlameCircularJewel(int x1, int y1, int x2, int y2)
{
	myCircularJewel(x1, y1, x2, y2);
	myFlame(x1 + 10, y1);
}
void myFlameCrystalJewel(int x1, int y1, int x2, int y2)
{
	myCrystalJewel(x1, y1, x2, y2);
	myFlame(x1 + 10, y1);
}
void myFlameTetraJewel(int x1, int y1, int x2, int y2)
{
	myTetraJewel(x1, y1, x2, y2);
	myFlame(x1 + 10, y1);
}



//myElectric functions represent the lightning gems
void myElectricSquareJewel(int x1, int y1, int x2, int y2)
{
	mySquareJewel(x1, y1, x2, y2);
	myElectric(x1, y1);
}
void myElectricTriangleJewel(int x1, int y1, int x2, int y2)
{
	myTriangleJewel(x1, y1, x2, y2);
	myElectric(x1 + 10, y1 + 20);
}
void myElectricCircularJewel(int x1, int y1, int x2, int y2)
{
	myCircularJewel(x1, y1, x2, y2);
	myElectric(x1 + 10, y1 + 10);
}
void myElectricCrystalJewel(int x1, int y1, int x2, int y2)
{
	myCrystalJewel(x1, y1, x2, y2);
	myElectric(x1 + 10, y1 + 10);
}
void myElectricTetraJewel(int x1, int y1, int x2, int y2)
{
	myTetraJewel(x1, y1, x2, y2);
	myElectric(x1 + 10, y1 + 10);
}



//Values of x co-relate with columns while values of y co-relate with rows
void RandomValueAssigner(int icons[][8])
{

	//To generate random values, we use the rand() function but the output of this function is only pseudo-random i.e. it will result in the same values each time the program is executed. Imagine as if the compiler has a huge array of random numbers, but it always gives u the first number, then the second one during the same execution every time the program is run.


	srand(time(NULL));


	//To avoid the pseudo-random numbers, we give the compiler a "seed" or starting index of the array. Once again, if this seed were a constant value, our generated values would still be pseudo-random. This is why we used the time() function whose output is a dynamic value. It returns the number of seconds passed since the start of Jan 1, 1970.
	

	int flag = 0;
b:	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			icons[i][j] = rand() % 5;
		}
	}

	//To randomize the array again if there are matching elements in the beginning
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			if (icons[i][j] == icons[i][j + 1] && icons[i][j] == icons[i][j + 2])
			{
				flag = 1;
			}
		}
	}
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (icons[i][j] == icons[i + 1][j] && icons[i][j] == icons[i + 2][j])
			{
				flag = 1;
			}
		}
	}
	if (flag == 1)
	{
		flag = 0;
		goto b;
	}
}
void DrawBoard()
{
	int i = 1;	//Will be used to Count how many Horizontal and Vertical Lines have been Printed yet.

	int x1, y1, x2, y2;
	x1 = x2 = 600;
	y1 = y2 = 220;

	while (i <= 9)
	{
		//Prints Horizontal Lines of Board/Grid
		myThickLine(x1, y1, x1 + 800, y1);
		y1 = y1 + 100;

		//Prints Vertical Lines of Board/Grid
		myThickLine(x2, y2, x2, y2 + 800);
		x2 = x2 + 100;

		i++;
	}

	gotoxy(0, 5);
	cout << "\t\t\t\t\t\t\t\t\t\t\t\tYour scored " << Score * 1000 << " points!";

}
bool UpdateBoard(int icons[][8])
{
	//Sleep(75);						//Uncomment this statement to make the falling affect more noticeable but it results in screen blinking

	int temp, flag = 0;					//Will use flag to identify if the whole board is free of empty spaces or not

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//i and j are multiplied by 100 because each box on the grid is 100 pixels wide and 100 pixels tall. +600 and +220 indicate the top left corner of the board. The other values added are just to increase or decrease the size of the gem so that they make look visually pleasing.
			//Drawing normal gems
			if (icons[i][j] == 0)
			{

				mySquareJewel(j * 100 + 25 + 600, i * 100 + 25 + 220, j * 100 + 75 + 600, i * 100 + 75 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 1)
			{

				myTriangleJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 2)
			{

				myCircularJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 3)
			{

				myCrystalJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 4)
			{

				myTetraJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == -1)			//-1 ID indicates that gems have been destroyed and cues the explosion animation
			{
				Sleep(50);
				Score++;
				myExplosion1(j * 100 + 25 + 600, i * 100 + 25 + 220, j * 100 + 75 + 600, i * 100 + 75 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
				Sleep(100);
				icons[i][j] = -2;				//-2 ID will indicate that these gems have been matched/deleted and are now ready for gems from above to fall down


				//Comment the next statement for a different effect


				myRect(j * 100 + 10 + 600, i * 100 + 10 + 220, j * 100 + 90 + 600, i * 100 + 90 + 220, 0, 0, 0); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == -2)			//-2 ID indicates that these gems have been matched/deleted and are now ready for gems from above to fall down
			{
				flag = 1;
				if (i == 0)
				{
					icons[i][j] = rand() % 5;
				}
				else
				{
					temp = icons[i][j];
					icons[i][j] = icons[i - 1][j];
					icons[i - 1][j] = temp;

					myRect(600, 220, 1400, 1020, 0, 0, 0); //To erase the upper gem before drawing it in the new slot, +5 and +95 are to position it inside the grid box

				}
			}

			//Drawing flame gems
			//ID of flame gems = (ID of normal gem) + 10;

			else if (icons[i][j] == 10)
			{

				myFlameSquareJewel(j * 100 + 25 + 600, i * 100 + 25 + 220, j * 100 + 75 + 600, i * 100 + 75 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 11)
			{

				myFlameTriangleJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 12)
			{

				myFlameCircularJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 13)
			{

				myFlameCrystalJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 14)
			{

				myFlameTetraJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}

			//Drawing lightning gems
			//ID of lightning gems = (ID of normal gem) + 20;

			else if (icons[i][j] == 20)
			{

				myElectricSquareJewel(j * 100 + 25 + 600, i * 100 + 25 + 220, j * 100 + 75 + 600, i * 100 + 75 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 21)
			{

				myElectricTriangleJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 22)
			{

				myElectricCircularJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 23)
			{

				myElectricCrystalJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
			else if (icons[i][j] == 24)
			{

				myElectricTetraJewel(j * 100 + 15 + 600, i * 100 + 15 + 220, j * 100 + 85 + 600, i * 100 + 85 + 220); //+600 in x and +220 y indicates the coordinates of top left corner of the board.
			}
		}
	}
	if (flag == 0)
		return false;		//Empty spaces are not present
	else
		return true;		//Empty spaces are present
}
void InputKeysAndUpdateSquare(int& row, int& col, bool& EnterKeyPressed)
{
	int whichkey = 0;
	isCursorKeyPressed(whichkey);

	if (whichkey == 1 && col != 0)//Left
	{
		col--;
		myRect(600, 220, 1400, 1020, 0, 0, 0);	
		myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 255, 255);
	}
	else if (whichkey == 2 && row != 0)	//Up
	{
		row--;
		myRect(600, 220, 1400, 1020, 0, 0, 0);
		myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 255, 255);
	}
	else if (whichkey == 3 && col != 7) //Right
	{
		col++;
		myRect(600, 220, 1400, 1020, 0, 0, 0);
		myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 255, 255);
	}
	else if (whichkey == 4 && row != 7) //Down
	{
		row++;
		myRect(600, 220, 1400, 1020, 0, 0, 0);
		myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 255, 255);
	}
	else if (whichkey == 5) //Enter Key
	{
		EnterKeyPressed = 1;
	}

	myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 255, 255);
}
void SwapGems(int icons[][8], int& row, int& col, bool& EnterKeyPressed)
{
	int temp;
	int DrcKey; //DirectionKey i.e. Alternate name for cursor keys 
	while (true)
	{
		myHollowRect((col * 100) + 10 + 600, (row * 100) + 10 + 220, (col * 100) + 90 + 600, (row * 100) + 90 + 220, 255, 0, 0); //Draws the select square in red to signify that it is ready to switch gems
		isCursorKeyPressed(DrcKey);
		if (DrcKey == 1 && col != 0) //Left
		{
			myRect(col * 100 + 5 + 600, row * 100 + 5 + 220, col * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			myRect((col - 1) * 100 + 5 + 600, row * 100 + 5 + 220, (col - 1) * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			temp = icons[row][col];
			icons[row][col] = icons[row][col - 1];
			icons[row][col - 1] = temp;
			EnterKeyPressed = 0;
			swap1.first = col;
			swap1.second = row;
			swap2.first = col - 1;
			swap2.second = row;
			DrcKey = 0;
			break;
		}
		else if (DrcKey == 2 && row != 0) //Up
		{
			myRect(col * 100 + 5 + 600, row * 100 + 5 + 220, col * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			myRect(col * 100 + 5 + 600, (row - 1) * 100 + 5 + 220, col * 100 + 95 + 600, (row - 1) * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			temp = icons[row][col];
			icons[row][col] = icons[row - 1][col];
			icons[row - 1][col] = temp;
			EnterKeyPressed = 0;
			swap1.first = col;
			swap1.second = row;
			swap2.first = col;
			swap2.second = row - 1;
			DrcKey = 0;
			break;
		}
		else if (DrcKey == 3 && col != 7) //Right
		{
			myRect(col * 100 + 5 + 600, row * 100 + 5 + 220, col * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			myRect((col + 1) * 100 + 5 + 600, row * 100 + 5 + 220, (col + 1) * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			temp = icons[row][col];
			icons[row][col] = icons[row][col + 1];
			icons[row][col + 1] = temp;
			EnterKeyPressed = 0;
			swap1.first = col;
			swap1.second = row;
			swap2.first = col + 1;
			swap2.second = row;
			DrcKey = 0;
			break;
		}
		else if (DrcKey == 4 && row != 7) //Down
		{
			myRect(col * 100 + 5 + 600, row * 100 + 5 + 220, col * 100 + 95 + 600, row * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			myRect(col * 100 + 5 + 600, (row + 1) * 100 + 5 + 220, col * 100 + 95 + 600, (row + 1) * 100 + 95 + 220, 0, 0, 0);	//To remove the previous shape that was drawn
			temp = icons[row][col];
			icons[row][col] = icons[row + 1][col];
			icons[row + 1][col] = temp;
			EnterKeyPressed = 0;
			swap1.first = col;
			swap1.second = row;
			swap2.first = col;
			swap2.second = row + 1;
			DrcKey = 0;
			break;
		}
	}
}
bool FindMatches(int icons[][8]) //Will return true or false to indicate if a match was found, used to undo a swap if no match was found
{
	int flag = 0;
	int ID;		//To store the ID of the gems in case a 3 match is found, then used to check if more IDs match
	for (int checkRow = 0; checkRow < 8; checkRow++)		//Checking for a lightning gem match in each row and exploding it
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol + 2] == icons[checkRow][checkCol] + 20 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol + 2] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow][j] = -1;
				}
			}
			else if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol + 1] == icons[checkRow][checkCol] + 20 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol + 1] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow][j] = -1;
				}
			}
			else if (icons[checkRow][checkCol + 1] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] + 20 && icons[checkRow][checkCol + 1] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow][j] = -1;
				}
			}
		}
	}
	for (int checkCol = 0; checkCol < 8; checkCol++) //Checking for a lightning gem match in each column and exploding it
	{
		for (int checkRow = 0; checkRow < 6; checkRow++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] && icons[checkRow + 2][checkCol] == icons[checkRow][checkCol] + 20 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow + 2][j] = -1;
				}
			}
			else if (icons[checkRow][checkCol] == icons[checkRow + 2][checkCol] && icons[checkRow + 1][checkCol] == icons[checkRow][checkCol] + 20 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow + 1][j] = -1;
				}
			}
			else if (icons[checkRow + 1][checkCol] == icons[checkRow + 2][checkCol] && icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] + 20 && icons[checkRow + 1][checkCol] != -2)
			{
				flag = 1;
				for (int i = 0; i < 8; i++)
				{
					icons[i][checkCol] = -1;
				}
				for (int j = 0; j < 8; j++)
				{
					icons[checkRow + 1][j] = -1;
				}
			}
		}
	}
	for (int checkRow = 2; checkRow < 8; checkRow++) //Checking for an L shaped match, ⊥ shaped match or ⅃ shaped match 
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] != -1 && icons[checkRow][checkCol] != -2) //ANCHOR for personal reference
			{
				ID = icons[checkRow][checkCol];
				if (icons[checkRow][checkCol] == icons[checkRow - 1][checkCol] && icons[checkRow][checkCol] == icons[checkRow - 2][checkCol])
				{
					icons[checkRow][checkCol] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol + 1] = icons[checkRow][checkCol + 2] = icons[checkRow - 1][checkCol] = icons[checkRow - 2][checkCol] = -1;
				}
				else if (icons[checkRow][checkCol + 1] == icons[checkRow - 1][checkCol + 1] && icons[checkRow][checkCol + 1] == icons[checkRow - 2][checkCol + 1])
				{
					icons[checkRow][checkCol + 1] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol] = icons[checkRow][checkCol + 2] = icons[checkRow - 1][checkCol + 1] = icons[checkRow - 2][checkCol + 1] = -1;
				}
				else if (icons[checkRow][checkCol + 2] == icons[checkRow - 1][checkCol + 2] && icons[checkRow][checkCol + 2] == icons[checkRow - 2][checkCol + 2])
				{
					icons[checkRow][checkCol + 2] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol] = icons[checkRow][checkCol + 1] = icons[checkRow - 1][checkCol + 2] = icons[checkRow - 2][checkCol + 2] = -1;

				}
			}
		}
	}
	for (int checkRow = 0; checkRow < 6; checkRow++) //Checking for an upside down L shaped match, upside down ⊥ shaped match or an upside down ⅃ shaped match
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] != -1 && icons[checkRow][checkCol] != -2) //ANCHOR for personal reference
			{
				ID = icons[checkRow][checkCol];
				if (icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] && icons[checkRow][checkCol] == icons[checkRow + 2][checkCol])
				{
					icons[checkRow][checkCol] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol + 2] = icons[checkRow][checkCol + 1] = icons[checkRow + 1][checkCol] = icons[checkRow + 2][checkCol] = -1;
				}
				else if (icons[checkRow][checkCol + 1] == icons[checkRow + 1][checkCol + 1] && icons[checkRow][checkCol + 1] == icons[checkRow + 2][checkCol + 1])
				{
					icons[checkRow][checkCol + 1] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol] = icons[checkRow][checkCol + 2] = icons[checkRow + 1][checkCol + 1] = icons[checkRow + 2][checkCol + 1] = -1;
				}
				else if (icons[checkRow][checkCol + 2] == icons[checkRow + 1][checkCol + 2] && icons[checkRow][checkCol + 2] == icons[checkRow + 2][checkCol + 2])
				{
					icons[checkRow][checkCol + 2] = ID + 20;
					flag = 1;
					icons[checkRow][checkCol] = icons[checkRow][checkCol + 1] = icons[checkRow + 1][checkCol + 2] = icons[checkRow + 2][checkCol + 2] = -1;
				}
			}
		}
	}
	for (int checkRow = 1; checkRow < 7; checkRow++) //Checking for ⊢ shaped match or its mirrored match
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] != -1 && icons[checkRow][checkCol] != -2)
			{
				if (icons[checkRow][checkCol] == icons[checkRow - 1][checkCol] && icons[checkRow][checkCol] == icons[checkRow + 1][checkCol])
				{
					flag = 1;
					ID = icons[checkRow][checkCol];
					icons[checkRow][checkCol + 1] = icons[checkRow][checkCol + 2] = icons[checkRow - 1][checkCol] = icons[checkRow + 1][checkCol] = -1;
					icons[checkRow][checkCol] = ID + 20;
				}
				else if (icons[checkRow][checkCol + 2] == icons[checkRow - 1][checkCol + 2] && icons[checkRow][checkCol] == icons[checkRow + 1][checkCol + 2])
				{
					flag = 1;
					ID = icons[checkRow][checkCol];
					icons[checkRow][checkCol] = icons[checkRow][checkCol + 1] = icons[checkRow - 1][checkCol + 2] = icons[checkRow + 1][checkCol + 2] = -1;
					icons[checkRow][checkCol + 2] = ID + 20;
				}
			}
		}
	}
	for (int checkRow = 0; checkRow < 8; checkRow++)		//Checking for a flame gem match in each row and exploding it
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol + 2] == icons[checkRow][checkCol] + 10 && icons[checkRow][checkCol] != -2)
			{
				icons[checkRow][checkCol] = -1;
				flag = 1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + i) >= 0 && (checkRow + i) < 8 && (checkCol + 2 + j) >= 0 && (checkCol + 2 + j) < 8)
						{
							icons[checkRow + i][checkCol + 2 + j] = -1;
						}
					}
				}
			}
			else if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol + 1] == icons[checkRow][checkCol] + 10 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + i) >= 0 && (checkRow + i) < 8 && (checkCol + 1 + j) >= 0 && (checkCol + 1 + j) < 8)
						{
							icons[checkRow + i][checkCol + 1 + j] = -1;
						}
					}
				}
			}
			else if (icons[checkRow][checkCol + 1] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] + 10 && icons[checkRow][checkCol + 1] != -2)
			{
				icons[checkRow][checkCol + 2] = -1;
				flag = 1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + i) >= 0 && (checkRow + i) < 8 && (checkCol + j) >= 0 && (checkCol + j) < 8)
						{
							icons[checkRow + i][checkCol + j] = -1;
						}
					}
				}
			}
		}
	}
	for (int checkCol = 0; checkCol < 8; checkCol++) //Checking for a flame gem match in each column and exploding it
	{
		for (int checkRow = 0; checkRow < 6; checkRow++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] && icons[checkRow + 2][checkCol] == icons[checkRow][checkCol] + 10 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				icons[checkRow][checkCol] = -1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + 2 + j) >= 0 && (checkRow + 2 + j) < 8 && (checkCol + i) >= 0 && (checkCol + i) < 8)
						{
							icons[checkRow + 2 + j][checkCol + i] = -1;
						}
					}
				}
			}
			else if (icons[checkRow][checkCol] == icons[checkRow + 2][checkCol] && icons[checkRow + 1][checkCol] == icons[checkRow][checkCol] + 10 && icons[checkRow][checkCol] != -2)
			{
				flag = 1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + 1 + j) >= 0 && (checkRow + 1 + j) < 8 && (checkCol + i) >= 0 && (checkCol + i) < 8)
						{
							icons[checkRow + 1 + j][checkCol + i] = -1;
						}
					}
				}
			}
			else if (icons[checkRow + 1][checkCol] == icons[checkRow + 2][checkCol] && icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] + 10 && icons[checkRow + 1][checkCol] != -2)
			{
				icons[checkRow][checkCol + 2] = -1;
				flag = 1;
				for (int i = -1; i <= 1; i++)
				{
					for (int j = -1; j <= 1; j++)
					{
						if ((checkRow + j) >= 0 && (checkRow + j) < 8 && (checkCol + i) >= 0 && (checkCol + i) < 8)
						{
							icons[checkRow + j][checkCol + i] = -1;
						}
					}
				}
			}
		}
	}
	for (int checkRow = 0; checkRow < 8; checkRow++)		//Checking for row matches and replacing them
	{
		for (int checkCol = 0; checkCol < 6; checkCol++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow][checkCol + 1] && icons[checkRow][checkCol] == icons[checkRow][checkCol + 2] && icons[checkRow][checkCol] != -2 && icons[checkRow][checkCol] != -1)
			{
				ID = icons[checkRow][checkCol];
				icons[checkRow][checkCol] = icons[checkRow][checkCol + 1] = icons[checkRow][checkCol + 2] = -1;
				flag = 1;
				if (ID == icons[checkRow][checkCol + 3] && checkCol < 5)
				{
					icons[checkRow][checkCol + 1] = ID + 10;						//If more than 3 gems match in a straight line, add a flame gem in place of icons[checkRow][checkCol + 1], ID + 10 is the ID of flame gem i.e. (ID of normal gem) + 10
					icons[checkRow][checkCol + 3] = -1;
					if (ID == icons[checkRow][checkCol + 4] && checkCol < 4)
					{
						icons[checkRow][checkCol + 1] = icons[checkRow][checkCol + 4] = -1;					//If more than 4 gems match in a straight line, add a lightning gem in place of icons[checkRow][checkCol + 1], ID + 20 is the ID of lightning gem i.e. (ID of normal gem) + 20 
						icons[checkRow][checkCol + 2] = ID + 20;
					}
				}
			}
		}
	}
	for (int checkCol = 0; checkCol < 8; checkCol++)		//Checking for column matches and replacing them
	{
		for (int checkRow = 0; checkRow < 6; checkRow++)
		{
			if (icons[checkRow][checkCol] == icons[checkRow + 1][checkCol] && icons[checkRow][checkCol] == icons[checkRow + 2][checkCol] && icons[checkRow][checkCol] != -2 && icons[checkRow][checkCol] != -1)
			{
				ID = icons[checkRow][checkCol];
				icons[checkRow][checkCol] = icons[checkRow + 1][checkCol] = icons[checkRow + 2][checkCol] = -1;
				flag = 1;
				if (ID == icons[checkRow + 3][checkCol] && checkRow < 5)
				{
					icons[checkRow + 1][checkCol] = ID + 10;					//If more than 3 gems match in a straight line, add a flame gem in place of icons[checkRow][checkCol], ID + 10 is the ID of flame gem i.e. (ID of normal gem) + 10
					icons[checkRow + 3][checkCol] = -1;
					if (ID == icons[checkRow + 4][checkCol] && checkRow < 4)
					{
						icons[checkRow + 1][checkCol] = icons[checkRow + 4][checkCol] = -1;				//If more than 4 gems match in a straight line, add a lightning gem in place of icons[checkRow][checkCol + 1], ID + 20 is the ID of lightning gem i.e. (ID of normal gem) + 20
						icons[checkRow + 2][checkCol] = ID + 20;
					}
				}
			}
		}
	}

	if (flag == 0)
		return false;
	else
		return true;
}
void finalFunction(int icons[][8])
{

	int row = 0, col = 0;		//Used to store the coordinates of the moving/select square

	bool EnterKeyPressed = 0;

	int temp;

	FindMatches(icons);

	while (true && TimeHasEnded==0)
	{


		DrawBoard();

		while (UpdateBoard(icons)); //Will prevent the program from moving on until the board is completely updated

		InputKeysAndUpdateSquare(row, col, EnterKeyPressed);

		FindMatches(icons);

		if (EnterKeyPressed == 1)
		{
			SwapGems(icons, row, col, EnterKeyPressed);				//Swap function with isCursorKeyPressed() inside to identify which adjacent gem to swap with

			UpdateBoard(icons);

			if (FindMatches(icons) == 0)						//Reversing a swap if a match is not found
			{
				UpdateBoard(icons);
				Sleep(400);
				temp = icons[swap1.second][swap1.first];
				icons[swap1.second][swap1.first] = icons[swap2.second][swap2.first];
				icons[swap2.second][swap2.first] = temp;
			}

			myRect(600, 220, 1400, 1020, 0, 0, 0);					//600, 220 represent coordinates of top left corner of the board

		}

	}

}
void myTimer(int seconds)
{
	int timer = time(NULL) + seconds;
	int SecLeft = seconds;

	int x1, y1, x2, y2;
	x1 = 375;  y1 = 0; x2 = 1250 + 375; y2 = 50;

	while (timer > time(NULL))
	{
		Sleep(50);

		myHollowRect(x1 - 1, y1, 1250 + 376, 51, 255, 213, 0);		//To draw a thick gold border around the time bar
		myHollowRect(x1 - 2, y1, 1250 + 377, 52, 255, 213, 0);
		myHollowRect(x1 - 3, y1, 1250 + 377, 53, 255, 213, 0);

		myRect(x1, y1, 1250 + 375, 50, 0, 0, 0);

		myRectG(x1, y1, x2, y2, 0, 255, 0);

		SecLeft = timer - time(NULL);

		x2 = (25 * SecLeft) + 375;

	}

	TimeHasEnded = 1;

}





int main()
{
	fullscreen();
	loadingScreen();
	Sleep(20);

	int icons[8][8];
	RandomValueAssigner(icons);
	myHollowRect(10 + 600, 10 + 220, 90 + 600, 90 + 220, 255, 255, 255);		//To Draw the Select/Moving Square for the first time
	int seconds = 50;

	std::thread Task1(finalFunction, icons);
	std::thread Task2(myTimer, seconds);

	Task1.join();
	Task2.join();

	system("cls");
	cout << "\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\tYour scored " << Score * 1000 << " points! Congratulations!" << endl << endl << endl;
	Sleep(400);

	system("pause");
	return 0;

}