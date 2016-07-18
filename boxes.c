#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "boxes.h"

void initMap();
void drawMap();
int addLine(int x, int y, char dir, int player);
int checkBox(int index, int player);
int gameLoop(int startingPlayer);
int emptyGameLoop();
int parseInput(char* input, int currentPlayer);

/*
 * Arguments:
 * height - The height of the playing field.
 * width - The width of the playing field.
 * playercount - The number of players in the game.
 * [filename] -
 */
int main(int argc, char* argv[]) {

	/* Program started with incorrect number of arguments */
	if (argc != 4 && argc != 5) {
		fprintf(stderr, "Usage: boxes height width playercount [filename]\n");
		exit(1);
	}

	/* Variable declaration */
	
	height = atoi(argv[1]);
	width = atoi(argv[2]);
	playercount = atoi(argv[3]);

	/* Invalid grid dimensions */
	if (height <= 1 || height >= 1000 ||
		width <= 1 || width >= 1000) {
		fprintf(stderr, "Invalid grid dimensions\n");
		exit(2);
	}

	/* Invalid player count */
	if (playercount < 1 || playercount > 100) {
		fprintf(stderr, "Invalid player count\n");
		exit(3);
	}

	/* Code for loading */
	/* Error reading grid */

	/* Initialize playgrid */
	initMap();
	drawMap();
	if (emptyGameLoop() == -1) {
		fprintf(stderr, "End of user input\n");
		exit(6);
	}
}

int gameLoop(int startingPlayer) {
	char* inputBuffer = (char*)malloc(30*sizeof(char));
	int currentPlayer = startingPlayer;
	int result;

	/* Loop runs while there are still boxes left. */
	while (remainingBoxes > 0) {

		/* Prompt for player's input. */
		printf("%c> ", (char) (currentPlayer + 65));
		if (fgets(inputBuffer, 30, stdin) == NULL) {
			printf("EOF");
			return -1;
		}

		result = parseInput(inputBuffer, currentPlayer);
		if (result > -1) {
			/* Not an error. */
			drawMap();
		}
		if (result == 0) {
			/* Extra turn not earned. */
			if (++currentPlayer >= playercount) {
				currentPlayer = 0;
			}
		}
	}

	/* TODO: call function to calculate and identify winner. */

	free(inputBuffer);
	return 0;

	
}

/*
 * This function parses the input runs addLine and passes information
 *   back up. This handles if input is valid as that's checked in
 *   addLine.
 */
int parseInput(char* input, int currentPlayer) {
	char* col = (char*)malloc(20*sizeof(char));
	char* row = (char*)malloc(20*sizeof(char));
	char dir;
	int result;

	int i = 0;
	int currentChar = 0;

	while (input[i] != ' ' && input[i] != '\n'&& input[i] != '\0') {
		row[currentChar] = input[i];
		++i;
	}
	++i;
	currentChar = 0;
	while (input[i] != ' ' && input[i] != '\n'&& input[i] != '\0') {
		col[currentChar] = input[i];
		++i;
	}
	++i;
	if (input[i] != ' ' && input[i] != '\n'&& input[i] != '\0') {
		dir = input[i];
	} else {
		return -1;
	}

	result = addLine(atoi(row), atoi(col), dir, currentPlayer);

	free(col);
	free(row);

	return result;
}

int emptyGameLoop() {

	return gameLoop(0);
}

void initMap() {
	/*char* mapString;*/
	int col, row;
	mapWidth = width*2+2;
	mapHeight = height*2+1;
	remainingBoxes = width*height;
	playerScores = malloc(playercount*sizeof(int));
	mapString = malloc(mapWidth*mapHeight*sizeof(char));
	memset(mapString, ' ', mapWidth*mapHeight);
 
	for (row = 0; row < mapHeight; ++row) {
		mapString[row*mapWidth + mapWidth-1] = '\n';
	}
    
	for (row = 0; row < mapHeight; row += 2) {
		for (col = 0; col < mapWidth; col += 2) {
			mapString[row*mapWidth + col] = '+';
		}
	}

	mapString[mapWidth*mapHeight] = '\0';
}

void drawMap() {
	printf("%s", mapString);
}

/*
 * This function returns an int code, dependant on exit conditions:
 * -1: The error code. row, col, or dir is incorrect.
 *        player is handled by the program and should not error.
 * 0:  The pass turn code. A line has been placed, and the function ended.
 * 1-2:  The extra turn code. A line has been placed that completes a box.
 *        The player who placed that line should get another turn.
 *        The number represents how many boxes were filled in.
 *        That information is probably unneccesary, but easy to code.
 */
int addLine(int row, int col, char dir, int player) {
	int index = 0;
	int boxes = 0;

	if (row < 0 || row > width || col < 0 || col > height ||
	        (dir != 'h' && dir != 'v')) {
		return -1;
	}

	index = row * 2 * mapWidth;
	index += 2 * col;
	if (dir == 'h') {
		if (col == width || mapString[index+1] == '-') {
			return -1;
		}
		mapString[index+1] = '-';
		if (row < height) {
			boxes += checkBox(index+1+mapWidth, player);
		}
		if (row > 0) {
			boxes += checkBox(index+1-mapWidth, player);
		}
	} else {
		if (row == height || mapString[index+mapWidth] == '|') {
			return -1;
		}
		mapString[index+mapWidth] = '|';
		if (col < width) {
			boxes += checkBox(index+mapWidth+1, player);
		}
		if (col > 0) {
			boxes += checkBox(index+mapWidth-1, player);
		}
	}
	remainingBoxes -= boxes;
	/*playerScores[currentPlayer] += boxes;*/
	return boxes;
}


/*
 * This function returns an int code, depending on the status of the box
 *         being checked.
 * 0: This box is already owned OR this box is not owned and not surrounded.
 * 1: This box is newly surrounded.
 */
int checkBox(int index, int player) {
	int borders = 0;

	if (mapString[index] != '.' && mapString[index] != ' ') {
		return -1;
	}

	/* Count the borders */
	borders += (mapString[index-1] == '|');
	borders += (mapString[index+1] == '|');
	borders += (mapString[index-mapWidth] == '-');
	borders += (mapString[index+mapWidth] == '-');

	if (borders == 4) {
		mapString[index] = (char) (player + 65);
		return 1;
	}

	return 0;
}
