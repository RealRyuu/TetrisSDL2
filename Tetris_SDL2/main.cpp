#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>

using namespace std;

extern "C" {
	//#ifdef BIT64
	//#include"./sdl64/include/SDL.h"
	//#include"./sdl64/include/SDL_main.h"
	//#else
#include<SDL.h>
#include<SDL_main.h>
	//#endif
}

#define BLOCK_SIZE			30 // wszystkie obrazki sa w rozdzielczosci 30x30px
#define BLOCKS_SCREEN_WIDTH	BLOCK_SIZE*12 // 12 = 10 klockow w grze + 2 klocki krawedziowe
#define SCREEN_WIDTH		BLOCKS_SCREEN_WIDTH + 250 // ekran klockow + ekran pomocniczy dla zapisu wynikow, punktow, poziomu
#define SCREEN_HEIGHT		BLOCK_SIZE*22 // 22 = 20 klockow w grze + dolne klocki krawedziowe + gorna krawedz
#define SPEED_DOWN			0.01 // predkosc przesuwu w dol - 1 pixel / SPEED_DOWN (czas, po ktorym klocek przesunie sie o 1 pixel w sekundach)
#define SPEED_HORIZONTAL	0.001 // predkosc przesuwu w lewo lub prawo - 1 pixel / SPEED_HORIZONTAL (czas, po ktorym klocek przesunie sie o 1 pixel w sekundach)
#define MAX_LVL				10 // maksymalny level gry
#define LVL_UP_TIME			30 // czas, po ktorym nastepuje przyspieszenie w sekuncach
#define SPEED_UP			0.2 // wartosc o jaka zwieksza sie mnoznik predkosci
#define FALL_SPEED			10 //  mnoznik predkosci podczas upadku po nacisnieciu strzalki w dol


// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostok¹ta o d³ugoœci boków l i k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

int block_error_control(SDL_Surface *Blue_block, SDL_Surface *Red_block, SDL_Surface *Brown_block, SDL_Surface *Navy_block, SDL_Surface *Green_block, SDL_Surface *Yellow_block, SDL_Surface *Pink_block, SDL_Surface *Grey_block, SDL_Surface *screen, SDL_Surface *charset, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *scrtex)
{
	if (Blue_block == NULL) {
		printf("SDL_LoadBMP(Blue_block.bmp.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Pink_block == NULL) {
		printf("SDL_LoadBMP(Pink_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Yellow_block == NULL) {
		printf("SDL_LoadBMP(Yellow_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Brown_block == NULL) {
		printf("SDL_LoadBMP(Brown_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Navy_block == NULL) {
		printf("SDL_LoadBMP(Navy_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Green_block == NULL) {
		printf("SDL_LoadBMP(Green_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Red_block == NULL) {
		printf("SDL_LoadBMP(Red_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	else if (Grey_block == NULL) {
		printf("SDL_LoadBMP(Grey_block.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
}

void falling_block(int type, SDL_Surface *screen, SDL_Surface*charset, SDL_Surface *Blue_block, SDL_Surface *Red_block, SDL_Surface *Brown_block, SDL_Surface *Navy_block, SDL_Surface *Green_block, SDL_Surface *Yellow_block, SDL_Surface *Pink_block, int x[], double distance[])
{
	for (int i = 0; i < 4; i++)
	{
		switch (type)
		{
		case 'I': DrawSurface(screen, Blue_block, x[i], distance[i]);
			break;
		case 'T': DrawSurface(screen, Pink_block, x[i], distance[i]);
			break;
		case 'O': DrawSurface(screen, Yellow_block, x[i], distance[i]);
			break;
		case 'L': DrawSurface(screen, Brown_block, x[i], distance[i]);
			break;
		case 'J': DrawSurface(screen, Navy_block, x[i], distance[i]);
			break;
		case 'S': DrawSurface(screen, Green_block, x[i], distance[i]);
			break;
		case 'Z': DrawSurface(screen, Red_block, x[i], distance[i]);
			break;
		}
	}
}

void draw_map(char map[22][12], SDL_Surface *screen, SDL_Surface *Blue_block, SDL_Surface *Red_block, SDL_Surface *Brown_block, SDL_Surface *Navy_block, SDL_Surface *Green_block, SDL_Surface *Yellow_block, SDL_Surface *Pink_block, SDL_Surface *Grey_block)
{
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (map[i][j] == 'I')
			{
				DrawSurface(screen, Blue_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == 'T')
			{
				DrawSurface(screen, Pink_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == 'O')
			{
				DrawSurface(screen, Yellow_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == 'L')
			{
				DrawSurface(screen, Brown_block, BLOCK_SIZE * j + 15, 30 * i + 15);
			}
			if (map[i][j] == 'J')
			{
				DrawSurface(screen, Navy_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == 'S')
			{
				DrawSurface(screen, Green_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == 'Z')
			{
				DrawSurface(screen, Red_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
			if (map[i][j] == '|')
			{
				DrawSurface(screen, Grey_block, BLOCK_SIZE * j + 15, BLOCK_SIZE * i + 15);
			}
		}
	}
}

void przypisz_typ(int *type, char types[])
{
	switch (*type)
	{
	case 1: *type = types[1];
		break;
	case 2: *type = types[2];
		break;
	case 3: *type = types[3];
		break;
	case 4: *type = types[4];
		break;
	case 5: *type = types[5];
		break;
	case 6: *type = types[6];
		break;
	case 7: *type = types[7];
		break;
	}
}

void set_cords(int type, int x[], double distance[])
{
	if (type == 'I')
	{
		for (int i = 0; i < 4; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 15;
			distance[i] = 30 * i + 15;
		}
	}
	else if (type == 'T') {
		for (int i = 0; i < 3; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 - BLOCK_SIZE + i * 30 + 15;
			distance[i] = 0 + 15;
		}
		x[3] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 15;
		distance[3] = 30 + 15;
	}
	else if (type == 'O') {
		for (int i = 0; i < 2; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 30 * i + 15;
			distance[i] = 0 + 15;
		}
		for (int i = 0; i < 2; i++)
		{
			x[i + 2] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 30 * i + 15;
			distance[i + 2] = 30 + 15;
		}
	}
	else if (type == 'L') {
		for (int i = 0; i < 3; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 15;
			distance[i] = 30 * i + 15;
		}
		x[3] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + BLOCK_SIZE + 15;
		distance[3] = 60 + 15;
	}
	else if (type == 'J') {
		for (int i = 0; i < 3; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + BLOCK_SIZE + 15;
			distance[i] = 30 * i + 15;
		}
		x[3] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + 15;
		distance[3] = 60 + 15;
	}
	else if (type == 'S') {
		for (int i = 0; i < 2; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + i * 30 + 15;
			distance[i] = 0 + 15;
		}
		for (int i = 0; i < 2; i++)
		{
			x[i + 2] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 - BLOCK_SIZE + i * 30 + 15;
			distance[i + 2] = 30 + 15;
		}
	}
	else if (type == 'Z') {
		for (int i = 0; i < 2; i++)
		{
			x[i] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 - BLOCK_SIZE + i * 30 + 15;
			distance[i] = 0 + 15;
		}
		for (int i = 0; i < 2; i++)
		{
			x[i + 2] = (BLOCKS_SCREEN_WIDTH - 2 * BLOCK_SIZE) / 2 + i * 30 + 15;
			distance[i + 2] = 30 + 15;
		}
	}
}

void rotate(int type, char types[], int x[], double distance[], char map[22][12])
{
	if (type != types[3])
	{
		bool reverse = false;
		int g[4] = { x[0], x[1], x[2], x[3] };
		int h[4] = { distance[0], distance[1], distance[2], distance[3] };
		for (int i = 0; i < 4; i++)
		{
			int temp = x[i];
			g[i] = (temp - g[2]) * cos(M_PI / 2) - (round(h[i]) - round(h[2])) * sin(M_PI / 2) + g[2];
			h[i] = (temp - g[2]) * sin(M_PI / 2) + (round(h[i]) - round(h[2])) * cos(M_PI / 2) + round(h[2]);
			continue;
		}
		for (int i = 0; i < 4; i++)
		{
			if ((map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'I' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'I') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'T' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'T') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'L' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'L') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'J' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'J') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'O' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'O') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'Z' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'Z') || (map[(int)(round(h[i]) - 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'S' || map[(int)(round(h[i]) + 15) / BLOCK_SIZE][g[i] / BLOCK_SIZE] == 'S')) reverse = true;
		}
		if (reverse == true)
			for (int i = 0; i < 4; i++)
			{
				int temp = g[i];
				g[i] = (temp - g[2]) * cos(-M_PI / 2) - (round(h[i]) - round(h[2])) * sin(-M_PI / 2) + g[2];
				h[i] = (temp - g[2]) * sin(-M_PI / 2) + (round(h[i]) - round(h[2])) * cos(-M_PI / 2) + round(h[2]);
				continue;
			}
		if (reverse == false)
		{
			for (int i = 0; i < 4; i++)
			{
				while (g[i] > 11 * BLOCK_SIZE)
					for (int j = 0; j < 4; j++)
					{
						g[j] -= BLOCK_SIZE;
					}
				while (g[i] < BLOCK_SIZE)
					for (int j = 0; j < 4; j++)
					{
						g[j] += BLOCK_SIZE;
					}
				while (h[i] > SCREEN_HEIGHT - BLOCK_SIZE)
				{
					for (int j = 0; j < 4; j++)
					{
						h[j] -= BLOCK_SIZE;
					}
				}
			}
		}
		for (int i = 0; i < 4; i++)
		{
			while (h[i] < 15)
			{
				for (int j = 0; j < 4; j++)
				{
					h[j] += 30;
				}
			}
		}
		for (int j = 0; j < 4; j++)
		{
			x[j] = g[j];
			distance[j] = h[j];
		}
	}
}
void horizontal_movement(bool *move_right, bool *move_left, bool *horizontal_move_start, double *movement_timer_vertical, int x[])
{
	if (*move_right == true)
	{
		if (*movement_timer_vertical > SPEED_HORIZONTAL)
		{
			for (int i = 0; i < 4; i++)
				x[i] += 1;
			*horizontal_move_start = true;
			*movement_timer_vertical = 0;
		}
	}
	if (*move_left == true)
	{
		if (*movement_timer_vertical > SPEED_HORIZONTAL)
		{
			for (int i = 0; i < 4; i++)
				x[i] -= 1;
			*horizontal_move_start = true;
			movement_timer_vertical = 0;
		}
	}
}
void combo(int *combo_count, int *points, bool *super_combo, int lvl)
{
	if (*combo_count > 0)
	{
		if (*combo_count == 1)
		{
			*points += 100 * (lvl + 1);
			*super_combo = false;
		}
		else if (*combo_count == 2)
		{
			*points += 200 * (lvl + 1);
			*super_combo = false;
		}
		else if (*combo_count == 3)
		{
			*points += 400 * (lvl + 1);
			*super_combo = false;
		}
		else if (*combo_count == 4)
		{
			if (*super_combo == true)
			{
				*points += 1200 * (lvl + 1);
				*super_combo = false;
			}
			else
			{
				*points += 800 * (lvl + 1);
				*super_combo = true;
			}
		}
		*combo_count = 0;
	}
}
int clear_map(char map[22][12])
{
	for (int i = 0; i < 21; i++)
	{
		for (int j = 1; j < 11; j++)
		{
			map[i][j] = 0;
		}
	}
	return 1;
}
void clear_line(char map[22][12], int *combo_count)
{
	for (int i = 0; i < 21; i++)
	{
		bool remove = false;
		for (int j = 0; j < 12; j++)
		{
			if (map[i][j] != 0)
				remove = true;
			else
			{
				remove = false;
				break;
			}
		}
		if (remove == true)
		{
			*combo_count += 1;
			for (int k = i; k > 0; k--)
				for (int g = 1; g < 11; g++)
				{
					map[k][g] = map[k - 1][g];
				}
		}

	}
}
void lvl_up(time_t *start_time, int *temp_time_start, int *lvl, double *speed_multiplier, bool *level_up)
{
	time(start_time);
	*temp_time_start = *start_time;
	*lvl += 1;
	*speed_multiplier += SPEED_UP;
	*level_up = false;
}
void draw_box(char map[22][12])
{
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i == 21 || j == 0 || j == 11)
				map[i][j] = '|';
		}
	}
}
void msges(bool pause, bool game_end, char text[], SDL_Surface *screen, SDL_Surface *charset)
{
	if (pause)
	{
		sprintf(text, "Pauza");
		DrawString(screen, BLOCKS_SCREEN_WIDTH / 2 - BLOCK_SIZE, SCREEN_HEIGHT / 2, text, charset);
	}
	if (game_end)
	{
		sprintf(text, "Koniec gry!");
		DrawString(screen, BLOCKS_SCREEN_WIDTH / 2 - BLOCK_SIZE, SCREEN_HEIGHT / 2, text, charset);
		sprintf(text, "Aby rozpoczac nowa gre - wcisnij 'n'");
		DrawString(screen, BLOCKS_SCREEN_WIDTH / 2 - BLOCK_SIZE, SCREEN_HEIGHT / 2 + 10, text, charset);
		sprintf(text, "Aby wyjsc - wcisnij 'ESC'");
		DrawString(screen, BLOCKS_SCREEN_WIDTH / 2 - BLOCK_SIZE, SCREEN_HEIGHT / 2 + 20, text, charset);

	}
}
void new_block(char map[22][12], double distance[], int x[], int *type, bool *stop, char types[], bool *fall, double *speed_multiplier, int lvl)
{
	for (int i = 0; i < 4; i++)
	{
		map[(int)distance[i] / BLOCK_SIZE][x[i] / BLOCK_SIZE] = *type;
	}
	*stop = false;
	*type = rand() % 7 + 1;
	przypisz_typ(type, types);
	set_cords(*type, x, distance);
	*fall = false;
	*speed_multiplier = 1 + lvl*SPEED_UP;
}

void stop_at_multiple(int x[], bool *move_right, bool *move_left, bool *horizontal_move_start, double *movement_timer_horizontal)
{
	if (((x[0] - 15) % 30 == 0))
	{
		*move_right = false;
		*move_left = false;
		*movement_timer_horizontal = 0;
		*horizontal_move_start = false;
	}
}
bool contact(char map[22][12], int x[], double distance[])
{
	for (int i = 0; i < 4; i++)
	{
		if (map[(int)floor((distance[i] + 15) / BLOCK_SIZE)][x[i] / BLOCK_SIZE] != 0)
		{
			return true;
			break;
		}
	}
	return false;
}
void save(char map[22][12], int x[], double distance[], int points, int lvl, double speed_multiplier, FILE *save_file)
{
	save_file = fopen("save.bin", "w+");
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 12; j++)
		{
			int temp = map[i][j];
			fputc(temp, save_file);
		}
	for (int i = 0; i < 4; i++)
	{
		fputc((int)distance[i], save_file);
	}
	for (int i = 0; i < 4; i++)
	{
		fputc((int)x[i], save_file);
	}
	fputc(points, save_file);
	fputc(lvl, save_file);
	fputc(speed_multiplier, save_file);
	fclose(save_file);
}
void load(char map[22][12], int x[], double distance[], int *points, int *lvl, double *speed_multiplier, FILE *save_file)
{
	save_file = fopen("save.bin", "r");
	for (int i = 0; i < 22; i++)
		for (int j = 0; j < 12; j++)
		{
			int temp = fgetc(save_file);
			if (temp == '\n' || temp == EOF) break;
			map[i][j] = temp;
		}
	for (int i = 0; i < 4; i++)
		distance[i] = fgetc(save_file);
	for (int i = 0; i < 4; i++)
		x[i] = fgetc(save_file);
	*points = fgetc(save_file);
	*lvl = fgetc(save_file);
	*speed_multiplier = fgetc(save_file);
	fclose(save_file);
}
// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc, type, lvl, temp_time_start, temp_time_end, time_delta, combo_count, points;
	double delta, worldTime, fpsTimer, fps, speed_multiplier, movement_timer_down, movement_timer_horizontal, time_diff;;
	bool move_right = false, move_left = false, stop = false, FULL_SCREEN = false, horizontal_move_start = false, super_combo = false, level_up = false, game_end = false, fall = false, pause = false;
	char types[8] = { '-', 'I', 'T', 'O', 'L', 'J', 'S', 'Z' };
	time_t lvl_timer;
	int x[4] = {};
	double distance[4] = {};
	time_t start_time;
	FILE *save_file;
	save_file = fopen("save.bin", "w+");
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *Blue_block;
	SDL_Surface *Red_block;
	SDL_Surface *Brown_block;
	SDL_Surface *Navy_block;
	SDL_Surface *Green_block;
	SDL_Surface *Yellow_block;
	SDL_Surface *Pink_block;
	SDL_Surface *Grey_block;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
	//	                                 &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Tetris");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int szary = SDL_MapRGB(screen->format, 0x54, 0x54, 0x54);
	char map[22][12] = {};

	draw_box(map);

	t1 = SDL_GetTicks();
	time(&start_time);
	temp_time_start = start_time;
	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	speed_multiplier = 1;
	combo_count = 0;
	points = 0;
	lvl = 0;
	movement_timer_down = 0;
	movement_timer_horizontal = 0;
	srand(time(NULL));
	Blue_block = SDL_LoadBMP("./Blue_block.bmp");
	Grey_block = SDL_LoadBMP("./Grey_block.bmp");
	Red_block = SDL_LoadBMP("./Red_block.bmp");
	Pink_block = SDL_LoadBMP("./Pink_block.bmp");
	Yellow_block = SDL_LoadBMP("./Yellow_block.bmp");
	Navy_block = SDL_LoadBMP("./Navy_block.bmp");
	Brown_block = SDL_LoadBMP("./Brown_block.bmp");
	Green_block = SDL_LoadBMP("./Green_block.bmp");
	block_error_control(Blue_block, Red_block, Brown_block, Navy_block, Green_block, Yellow_block, Pink_block, Grey_block, screen, charset, window, renderer, scrtex);
	type = rand() % 7 + 1;
	przypisz_typ(&type, types);
	set_cords(type, x, distance);
	while (!quit) {
		if (!pause)
		{
			// w tym momencie t2-t1 to czas w milisekundach,
			// jaki uplyna³ od ostatniego narysowania ekranu
			// delta to ten sam czas w sekundach
			if (!game_end)
			{
				t2 = SDL_GetTicks();
				time(&lvl_timer);
				temp_time_end = lvl_timer;
				time_delta = temp_time_end - temp_time_start;
			}
			delta = (t2 - t1) * 0.001;
			t1 = t2;
			worldTime += delta;
			movement_timer_down += delta;
			if (!game_end)
				if ((time_delta > LVL_UP_TIME || level_up) && lvl < MAX_LVL) lvl_up(&start_time, &temp_time_start, &lvl, &speed_multiplier, &level_up);

			for (int i = 1; i < 11; i++)
				if (map[0][i] != 0)
				{
					game_end = true;
				}
			if (move_right == true || move_left == true)
				movement_timer_horizontal += delta;
			horizontal_movement(&move_right, &move_left, &horizontal_move_start, &movement_timer_horizontal, x);
			if (movement_timer_down > SPEED_DOWN)
			{
				for (int i = 0; i < 4; i++)
				{
					distance[i] += speed_multiplier * 1;
				}
				movement_timer_down = 0;
			}
			stop = contact(map, x, distance);
			if (stop == true)
				new_block(map, distance, x, &type, &stop, types, &fall, &speed_multiplier, lvl);
			if (!game_end)
			{
				clear_line(map, &combo_count);
				combo(&combo_count, &points, &super_combo, lvl);
			}

			if (horizontal_move_start == true)
				stop_at_multiple(x, &move_right, &move_left, &horizontal_move_start, &movement_timer_horizontal);
		}

		SDL_FillRect(screen, NULL, czarny);

		if (!game_end)
		{
			draw_map(map, screen, Blue_block, Red_block, Brown_block, Navy_block, Green_block, Yellow_block, Pink_block, Grey_block);
			falling_block(type, screen, charset, Blue_block, Red_block, Brown_block, Navy_block, Green_block, Yellow_block, Pink_block, x, distance);
		}

		DrawRectangle(screen, 380, 16, 200, 50, czarny, szary);
		sprintf(text, "Liczba punktow: %d", points);
		DrawString(screen, BLOCKS_SCREEN_WIDTH + 40, 32, text, charset);
		sprintf(text, "Lvl: %d", lvl);
		DrawString(screen, BLOCKS_SCREEN_WIDTH + 40, 42, text, charset);
		msges(pause, game_end, text, screen, charset);
		//	DrawScreen(screen, plane, ship, charset, worldTime, delta, vertSpeed);

		// naniesienie wyniku rysowania na rzeczywisty ekran
		//		SDL_Flip(screen);

		fpsTimer += delta;
		if (fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
		};

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		//		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		// obs³uga zdarzeñ (o ile jakieœ zasz³y)
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				else if (event.key.keysym.sym == SDLK_q)
				{
					if (FULL_SCREEN == true)
					{
						SDL_FillRect(screen, NULL, 0x000000);
						SDL_DestroyTexture(scrtex);
						SDL_RenderClear(renderer);
						SDL_DestroyWindow(window);
						rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
							&window, &renderer);
						FULL_SCREEN = false;
					}
					else
					{
						SDL_FillRect(screen, NULL, 0x000000);
						SDL_DestroyTexture(scrtex);
						SDL_RenderClear(renderer);
						SDL_DestroyWindow(window);
						rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
						FULL_SCREEN = true;
					}
					if (rc != 0) {
						SDL_Quit();
						printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
						return 1;
					};

					SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
					SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

					SDL_SetWindowTitle(window, "Tetris");

					scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
						SDL_TEXTUREACCESS_STREAMING,
						SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				else if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_UP)
				{
					if (!move_left && !move_right && !fall && !pause) rotate(type, types, x, distance, map);
				}
				else if (event.key.keysym.sym == SDLK_LEFT)
				{
					if (!fall && !pause)
					{
						for (int i = 0; i < 4; i++) {
							if (map[(int)(round(distance[i]) - 12) / BLOCK_SIZE][x[i] / BLOCK_SIZE - 1] == 0 && map[(int)(round(distance[i]) + 12) / BLOCK_SIZE][x[i] / BLOCK_SIZE - 1] == 0)
							{
								move_left = true;
							}
							else
							{
								move_left = false;
								break;
							}
						}
					}
				}
				else if (event.key.keysym.sym == SDLK_RIGHT)
				{
					if (!fall && !pause)
					{
						for (int i = 0; i < 4; i++)
							if (map[(int)(round(distance[i]) - 12) / BLOCK_SIZE][x[i] / BLOCK_SIZE + 1] == 0 && map[(int)(round(distance[i]) + 12) / BLOCK_SIZE][x[i] / BLOCK_SIZE + 1] == 0)
							{
								move_right = true;
							}
							else
							{
								move_right = false;
								break;
							}
					}
				}
				else if (event.key.keysym.sym == SDLK_n)
				{
					clear_map(map);
					lvl = 0;
					points = 0;
					speed_multiplier = 1;
					type = rand() % 7 + 1;
					przypisz_typ(&type, types);
					set_cords(type, x, distance);
					super_combo = false;
					game_end = false;
				}
				else if (event.key.keysym.sym == SDLK_s)
				{
					level_up = true;
				}
				else if (event.key.keysym.sym == SDLK_l)
				{
					load(map, x, distance, &points, &lvl, &speed_multiplier, save_file);
				}
				else if (event.key.keysym.sym == SDLK_p)
				{
					if (pause == true) pause = false;
					else pause = true;
				}
				else if (event.key.keysym.sym == SDLK_z)
				{
					save(map, x, distance, points, lvl, speed_multiplier, save_file);
				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{
					if (!pause)
					{
						fall = true;
						speed_multiplier = FALL_SPEED;
					}
				}
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};
		};
		frames++;
	};

	// zwolnienie powierzchni
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};
