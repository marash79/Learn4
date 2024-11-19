// Learn4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _USE_MATH_DEFINES
#include <iostream>
#include "SDL.h"

SDL_Window* win=NULL;
SDL_Renderer* ren=NULL;
////for ordinary non-sdl calculation uncomment following lines:
//bool CoordsConvert(const int * maxX,const int * maxY, const int& xBase, const int& yBase, int& x, int& y){
// if (*maxX<0||*maxY<0||xBase<0||yBase<0) return false;/*check bad funcion call*/
////for SDL implementation
bool CoordsConvert(SDL_Window* window, const int& xBase, const int& yBase,int& x,int& y) {
	if (window == nullptr||x<0|y<0) return false;/*check bad funcion call*/
	int* maxX{ nullptr }, * maxY{ nullptr };
	SDL_GetWindowSize(window, maxX,maxY);/*get relative max windows coords (w+1,h+1)*/

	x = xBase + x;/*absolute x coordinates pre calculation*/
	y= yBase - y ;/*absolute Y coordinates pre calculation*/

	if (x<0 || y>*maxX || y<0 || y>*maxY) /*check relative coordinates in a window*/
	{
		return false;
	}
	return true;
}

int win_witdh = 800, win_height = 600;

void deInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	SDL_Quit();
	exit(error);
}

int init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{

		printf("COuldn't init SDL! Error %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}
	win = SDL_CreateWindow("This is my first CPP window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_witdh, win_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win == NULL) {
		printf("COuldn't create window! Error %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL) {
		printf("COuldn't create Renderer! Error %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}
}

double circle(double x, double x0, double y0, double rad)
{
	return sqrt(rad * rad - (x - x0)*(x - x0)) + y0;
}

void mathCoordsToSctreen(double x, double y, double scale, int centerx, int centery, int& sx, int& sy)
{
	sx = round(centerx + x * scale);
	sy = round(centery - y * scale);
}

int main(int argc, char* args[])
{
	init();
	
	double scale = 1.0;
	double x1, y1, x2, y2;
	int sx1, sy1, sx2, sy2;
	SDL_Rect rect{ 0,0,0,0 };
	int wanted_points = 3;
	bool rising = true;

	while (true)
	{

#pragma region DRAWING

		SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 200);
		int zeroX{ 100 }, zeroY{ 100 };
		int finalX, finalY;

		

		x1 = -100.0; x2 = 100.0;
		y1 = 0; y2 = 0;
		mathCoordsToSctreen(x1, y1, scale, win_witdh / 2, win_height / 2, sx1, sy1);
		mathCoordsToSctreen(x2, y2, scale, win_witdh / 2, win_height / 2, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);
		y1 = -100.0; y2 = 100.0;
		x1 = 0; x2 = 0;
		mathCoordsToSctreen(x1, y1, scale, win_witdh / 2, win_height / 2, sx1, sy1);
		mathCoordsToSctreen(x2, y2, scale, win_witdh / 2, win_height / 2, sx2, sy2);
		SDL_RenderDrawLine(ren, sx1, sy1, sx2, sy2);

		//for (double alpha = 0; alpha <= 360; alpha += 0.5)
		//{
		//	x1 = 100 * cos(alpha * M_PI / 180);
		//	y1 = 100 * sin(alpha * M_PI / 180);
		//	mathCoordsToSctreen(x1, y1, scale, win_witdh / 2, win_height / 2, sx1, sy1);
		//	SDL_RenderDrawPoint(ren, x1, y1);
		//	bar.x = sx1 - 1;
		//	bar.y = sy1 - 1;
		//	SDL_RenderFillRect(ren, &bar);
		//	y1 = -y1;
		//	mathCoordsToSctreen(x1, y1, scale, win_witdh / 2, win_height / 2, sx1, sy1);
		//	bar.x = sx1 - 1;
		//	bar.y = sy1 - 1;
		//	SDL_RenderFillRect(ren, &bar);

		//}

	

		int point_count = wanted_points;
		SDL_Point* points = (SDL_Point*)malloc(sizeof(SDL_Point) * (point_count + 1));

		/*CoordsConvert(win, zeroX, zeroY, finalX, finalY);

			SDL_RenderDrawLine(ren, zeroX -50, zeroY, zeroX +50, zeroY);
			SDL_RenderDrawLine(ren, zeroX, zeroY-50, zeroX, zeroY+50);

			SDL_RenderDrawRect(ren, &bar);*/
		float alpha = 0;
		for (int i = 0; i < point_count; i++)
		{
			alpha += 2 * M_PI / point_count;
			mathCoordsToSctreen(200 * cos(alpha), 200 * sin(alpha),scale, win_witdh / 2, win_height / 2, points[i].x, points[i].y);
		}
		points[point_count] = points[0];


		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		SDL_RenderDrawLines(ren, points, point_count + 1);
		


		SDL_RenderDrawRect(ren, &rect);
		free(points);

		if (rising)
			wanted_points++;
		else
			wanted_points--;
		if (rising && wanted_points > 100 || !rising && wanted_points <= 3)
			rising = !rising;
#pragma endregion
		
		
		SDL_RenderPresent(ren);

		SDL_Delay(20);
		
		
	}
	deInit(0);
	return 0;

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл

