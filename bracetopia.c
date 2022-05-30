// File: bracetopia.c
//
// Description: Performs the Brace-topia program
// in either print mode or infinite mode with the given arguments.
//
// @author	Jack Schneider, jas1727
//

#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <getopt.h>
#include <time.h>

/// Prints the standard usage message for errors

void printUsageMessage() {
	fprintf( stderr, "usage:\nbracetopia [-h] [-t N]"
		       	" [-c N] [-d dim] [-s %%str] [-v %%vac]"
		       	" [-e %%end]\n");
}

/// Finds the happiness of the cell within the grid at the
/// given x and y coordinates
/// @param dimension the dimension of the grid
/// @param x the x coordinate of the cell
/// @param y the y coordinate of the cell
/// @param grid a table containing the cell
/// @return cellHappiness the happiness of the cell

double getCellHappiness(int dimension, int x, int y, char grid[][dimension]) {
	double cellHappiness;
	char cellAgent = grid[x][y];
	double sameAgentNum = 0;
	double difAgentNum = 0;
	if (y - 1 > 0 && x - 1 > 0 && grid[x-1][y-1] != '.') {
		if (grid[x-1][y-1] == cellAgent) {
			sameAgentNum++;
		} else {
			difAgentNum++;
		}
	}
        if (y - 1 > 0 && grid[x][y-1] != '.') {
                if (grid[x][y-1] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
        if (y - 1 > 0 && x + 1 < dimension && grid[x+1][y-1] != '.') {
                if (grid[x+1][y-1] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
        if (x - 1 > 0 && grid[x-1][y] != '.') {
                if (grid[x-1][y] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
        if (x + 1 < dimension && grid[x+1][y] != '.') {
                if (grid[x+1][y] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
        if (y + 1 < dimension && x - 1 > 0 && grid[x-1][y+1] != '.') {
                if (grid[x-1][y+1] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
	if (y + 1 < dimension && grid[x][y+1] != '.') {
                if (grid[x][y+1] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
	if (y + 1 < dimension && x + 1 < dimension && grid[x+1][y+1] != '.') {
                if (grid[x+1][y+1] == cellAgent) {
                        sameAgentNum++;
                } else {
                        difAgentNum++;
                }
        }
	if (sameAgentNum + difAgentNum == 0) {
		cellHappiness = 1;
	} else {
		cellHappiness = sameAgentNum / (sameAgentNum + difAgentNum);
	}
	return cellHappiness;
}

/// Performs moves within the grid and returns the number of moves
/// @param dimension the dimensions of the grid
/// @param grid the table to perform moves on
/// @param prefStrength the preference strength of cells in the table
/// @param movesInCycle the number of moves performed
/// @return movesInCycle the number of moves performed

int performMoves(int dimension, char grid[][dimension], int prefStrength, int movesInCycle) {
	for (int x = 0; x < dimension; x++) {
		for(int y = 0; y < dimension; y++) {
			if (getCellHappiness(dimension, x, y, grid) < (double) (prefStrength/100.00)) {
				if (y - 1 > 0 && x - 1 > 0 && grid[x-1][y-1] == '.') {
					grid[x-1][y-1] = grid[x][y];
					grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (y - 1 > 0 && grid[x][y-1] == '.') {
                			grid[x][y-1] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (y - 1 > 0 && x + 1 < dimension && grid[x+1][y-1] == '.') {
                			grid[x+1][y-1] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (x - 1 > 0 && grid[x-1][y] == '.') {
                			grid[x-1][y] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (x + 1 < dimension && grid[x+1][y] == '.') {
                			grid[x+1][y] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (y + 1 < dimension && x - 1 > 0 && grid[x-1][y+1] == '.') {
                			grid[x-1][y+1] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (y + 1 < dimension && grid[x][y+1] == '.') {
                			grid[x][y+1] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
				else if (y + 1 < dimension && x + 1 < dimension && grid[x+1][y+1] == '.') {
                			grid[x+1][y+1] = grid[x][y];
                                        grid[x][y] = '.';
					movesInCycle++;
        			}
			}
		}
	}
	return movesInCycle;
}

/// The main function for running the program
/// @param argc the number of arguments
/// @param argv the list of arguments

int main(int argc, char *argv[]) {

	// Initialize elements and their default values
	int dimension = 15;
	int prefStrength = 50;
	int vacancy = 20;
	int endline = 60;
	int cycleDelay = 900000;
	int maxCycle = 0;
	int usePrintMode = 0;

	// Interpret and possibly assign command line arguments
	int opt;
        int flagVal = 0;
        while ( (opt = getopt( argc, argv, "hd:s:v:e:c:t:") ) != -1 ) {
                switch ( opt ) {
			case 'h':
				fprintf( stderr, "usage:\nbracetopia [-h]"
					       	"[-t N] [-c N] [-d dim]"
					       	"[-s %%str] [-v %%vac] [-e %%end]\n"
						"Option      Default   Example   Description\n"
						"\'-h\'        NA        -h        print this usage message.\n"
						"\'-t N\'      900000    -t 5000   microseconds cycle delay.\n"
						"\'-c N\'      NA        -c4       count cycle maximum value.\n"
						"\'-d dim\'    15        -d 7      width and height dimension.\n"
						"\'-s %%str\'  50        -s 30     strength of preference.\n"
						"\'-v %%vac\'  20        -v30      percent vacancies.\n"
						"\'-e %%endl\' 60        -e75      percent Endline braces. Others want Newline.\n");
				return 0;
				break;
                        case 'd':
                                flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal > 39 || flagVal < 5) {
					fprintf(stderr, "dimension (%d) must be a value in [5...39]\n", flagVal);
					printUsageMessage();
					return( EXIT_FAILURE + 1 );
				}
				dimension = flagVal;
                                break;
                        case 's':
				flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal > 99 || flagVal < 1) {
                                        fprintf(stderr, "preference strength (%d) must be a value in [1...99]\n", flagVal);
                                        printUsageMessage();
                                        return( EXIT_FAILURE + 1 );
                                }
				prefStrength = flagVal;
                                break;
			case 'v':
				flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal > 99 || flagVal < 1) {
                                        fprintf(stderr, "vacancy (%d) must be a value in [1...99]\n", flagVal);
                                        printUsageMessage();
                                        return( EXIT_FAILURE + 1 );
                                }
				vacancy = flagVal;
				break;
			case 'e':
				flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal > 99 || flagVal < 1) {
					fprintf(stderr, "endline proportion (%d) must be a value in [1...99]\n", flagVal);
					printUsageMessage();
					return( EXIT_FAILURE + 1 );
				}
				endline = flagVal;
				break;
			case 'c':
				flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal < 0) {
					fprintf(stderr, "count (%d) must be a non-negative integer.\n", flagVal);
					printUsageMessage();
					return( EXIT_FAILURE + 1 );
				}
				usePrintMode = 1;
				maxCycle = flagVal;
				break;
			case 't':
				flagVal = (int)strtol( optarg, NULL, 10 );
				if (flagVal > 0) {
					cycleDelay = flagVal;
				}
				break;
                        default:
                                // prints when invalid flag
                                printUsageMessage();
                                return( EXIT_FAILURE );
                }
	}

	// implementation before running print mode or infinite mode
	// initializes the grid randomly with correct proportions
	char grid[dimension][dimension];
	char valueList[dimension*dimension];
	int numVacant = (dimension*dimension*vacancy)/100;
	int numEndline = (((dimension*dimension)-numVacant)*endline)/100;
	int vacantCount = 0;
        int endlineCount = 0;
	// assigns values of correct proportions to an array
	for (int count = 0; count < dimension*dimension; count++) {
		if(vacantCount < numVacant) {
			valueList[count] = '.';
			vacantCount++;
		} else if (endlineCount < numEndline) {
			valueList[count] = 'e';
			endlineCount++;
		} else {
			valueList[count] = 'n';
		}
	}
	// randomizes the array using Fisher-Yates algorithm
	int upper = (dimension*dimension) - 1;
	srand(41);
	for (int i = 0; i <= (dimension*dimension)-2; i++) {
		int lower = i;
		int j = (rand() % (upper - lower + 1)) + lower;
		char temp = valueList[i];
		valueList[i] = valueList[j];
		valueList[j] = temp;
	}
	// assigns array values to grid
	int gridCount = 0;
	for (int x = 0; x < dimension; x++) {
		for (int y = 0; y < dimension; y++) {
			grid[x][y] = valueList[gridCount];
			gridCount++;
		}
	}

        // run either print mode or infinite mode
	int cycleCounter = 0;
	int movesInCycle = 0;
	double teamHappiness = 0;
	double totalHappiness = 0;
	// run print mode
	if (usePrintMode) {

		for (int x = 0; x < dimension; x++) {
                        for (int y = 0; y < dimension; y++) {
				totalHappiness += getCellHappiness(dimension, x, y, grid);
                                printf("%c", grid[x][y]);
                        }
                        printf("\n");
                }
		teamHappiness = totalHappiness / (dimension*dimension);
                printf("cycle: %d \n", cycleCounter);
                printf("moves this cycle: %d \n", movesInCycle);
                printf("teams\' \"happiness\": %f \n", teamHappiness);
                printf("dim: %d, %%strength of preference: %d%%"
                                  ", %%vacancy: %d%%, %%end: %d%% \n"
                                  , dimension, prefStrength, vacancy, endline);

		movesInCycle = performMoves(dimension, grid, prefStrength, movesInCycle);
                cycleCounter++;
                usleep(cycleDelay);

		while (cycleCounter <= maxCycle) {

			totalHappiness = 0;
			for (int x = 0; x < dimension; x++) {
				for (int y = 0; y < dimension; y++) {
					totalHappiness += getCellHappiness(dimension, x, y, grid);
					printf("%c", grid[x][y]);
				}
				printf("\n");
			}
			teamHappiness = totalHappiness / (dimension*dimension);
			printf("cycle: %d \n", cycleCounter);
			printf("moves this cycle: %d \n", movesInCycle);
			printf("teams\' \"happiness\": %f \n", teamHappiness);
			printf("dim: %d, %%strength of preference: %d%%"
					", %%vacancy: %d%%, %%end: %d%% \n"
				       , dimension, prefStrength, vacancy, endline);	

			movesInCycle = 0;
			movesInCycle = performMoves(dimension, grid, prefStrength, movesInCycle);
			cycleCounter++;
			usleep(cycleDelay);

		}
		return 0;
	// run infinite mode
	} else {

		initscr();
		int numRows, numCols;
		getmaxyx(stdscr, numRows, numCols);
		move(numRows - 1, numCols / 4);
		refresh();

		move(0,0);
		for (int x = 0; x < dimension; x++) {
                        for (int y = 0; y < dimension; y++) {
                                totalHappiness += getCellHappiness(dimension, x, y, grid);
                                printw("%c", grid[x][y]);
                        }
                        printw("\n");
                }
                teamHappiness = totalHappiness / (dimension*dimension);
                printw("cycle: %d \n", cycleCounter);
                printw("moves this cycle: %d \n", movesInCycle);
                printw("teams\' \"happiness\": %f \n", teamHappiness);
                printw("dim: %d, %%strength of preference: %d%%"
                                  ", %%vacancy: %d%%, %%end: %d%% \n"
                                  , dimension, prefStrength, vacancy, endline);
		printw("Use Control-C to quit.");

		refresh();
                movesInCycle = performMoves(dimension, grid, prefStrength, movesInCycle);
                cycleCounter++;
                usleep(cycleDelay);

		while ( 1 ) {

			move(0,0);
			totalHappiness = 0;
			for (int x = 0; x < dimension; x++) {
                                for (int y = 0; y < dimension; y++) {
					totalHappiness += getCellHappiness(dimension, x, y, grid);
                                        printw("%c", grid[x][y]);
                                }
                                printw("\n");
                        }
			teamHappiness = totalHappiness / (dimension*dimension);
                        printw("cycle: %d \n", cycleCounter);
                        printw("moves this cycle: %d \n", movesInCycle);
                        printw("teams\' \"happiness\": %f \n", teamHappiness);
                        printw("dim: %d, %%strength of preference: %d%%"
                                        ", %%vacancy: %d%%, %%end: %d%% \n"
                                       , dimension, prefStrength, vacancy, endline);
			printw("Use Control-C to quit.");

			refresh();
			movesInCycle = 0;
			movesInCycle = performMoves(dimension, grid, prefStrength, movesInCycle);
			cycleCounter++;
			usleep(cycleDelay);
		}
		endwin();
		return 0;
	}	
}
