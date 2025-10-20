/*
 * Basic Pacman Game in C
 * Simple implementation for beginners
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 19
#define HEIGHT 19
#define WALL '#'
#define DOT '.'
#define EMPTY ' '
#define PACMAN 'C'
#define GHOST 'G'
#define POWER_PILL 'O'

/* Game board */
char board[HEIGHT][WIDTH];
int score = 0;
int dots = 0;
int powerMode = 0;
int powerTimer = 0;

/* Positions */
typedef struct {
    int x, y;
} Position;

Position pacman;
Position ghosts[4];

/* Directions */
typedef struct {
    int dx, dy;
} Direction;

Direction directions[4] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Up, Down, Left, Right

/* Function prototypes */
void initializeBoard();
void printBoard();
void movePacman(int dx, int dy);
void moveGhosts();
int isValidMove(int x, int y);
void checkCollision();
void updatePowerMode();

int main() {
    srand((unsigned int)time(NULL));
    initializeBoard();
    
    printf("Welcome to Basic Pacman!\n");
    printf("Use WASD keys to move. Eat all dots to win!\n");
    printf("Press any key to start...\n");
    getch();
    
    while (1) {
        system("cls"); // Clear screen
        printBoard();
        printf("Score: %d\n", score);
        printf("Dots remaining: %d\n", dots);
        
        if (dots == 0) {
            printf("Congratulations! You won!\n");
            break;
        }
        
        // Get user input
        if (kbhit()) {
            char key = getch();
            switch (key) {
                case 'w': case 'W':
                    movePacman(0, -1);
                    break;
                case 's': case 'S':
                    movePacman(0, 1);
                    break;
                case 'a': case 'A':
                    movePacman(-1, 0);
                    break;
                case 'd': case 'D':
                    movePacman(1, 0);
                    break;
                case 'q': case 'Q':
                    printf("Thanks for playing!\n");
                    exit(0);
            }
        }
        
        moveGhosts();
        checkCollision();
        updatePowerMode();
        
        Sleep(200); // Control game speed
    }
    
    return 0;
}

void initializeBoard() {
    int i, j;
    // Initialize the board with walls around the edges
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                board[i][j] = WALL;
            } else {
                board[i][j] = DOT;
                dots++;
            }
        }
    }
    
    // Create some walls inside
    for (i = 2; i < HEIGHT - 2; i += 4) {
        for (j = 2; j < WIDTH - 2; j += 4) {
            board[i][j] = WALL;
            board[i][j+1] = WALL;
            board[i+1][j] = WALL;
        }
    }
    
    // Place power pills
    board[2][2] = POWER_PILL;
    board[2][WIDTH-3] = POWER_PILL;
    board[HEIGHT-3][2] = POWER_PILL;
    board[HEIGHT-3][WIDTH-3] = POWER_PILL;
    dots -= 4; // Adjust dot count
    
    // Place pacman
    pacman.x = WIDTH / 2;
    pacman.y = HEIGHT / 2;
    board[pacman.y][pacman.x] = PACMAN;
    
    // Place ghosts
    ghosts[0].x = 1;
    ghosts[0].y = 1;
    ghosts[1].x = WIDTH - 2;
    ghosts[1].y = 1;
    ghosts[2].x = 1;
    ghosts[2].y = HEIGHT - 2;
    ghosts[3].x = WIDTH - 2;
    ghosts[3].y = HEIGHT - 2;
}

void printBoard() {
    int i, j;
    // Print the board with Pacman and ghosts
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            int isGhost = 0;
            int g;
            for (g = 0; g < 4; g++) {
                if (ghosts[g].x == j && ghosts[g].y == i) {
                    printf("%c", GHOST);
                    isGhost = 1;
                    break;
                }
            }
            
            if (!isGhost) {
                if (i == pacman.y && j == pacman.x) {
                    printf("%c", PACMAN);
                } else {
                    printf("%c", board[i][j]);
                }
            }
        }
        printf("\n");
    }
}

void movePacman(int dx, int dy) {
    int newX = pacman.x + dx;
    int newY = pacman.y + dy;
    
    // Check if the move is valid
    if (isValidMove(newX, newY)) {
        // Restore the previous cell (dot or empty)
        if (board[pacman.y][pacman.x] != PACMAN) {
            board[pacman.y][pacman.x] = EMPTY;
        }
        
        pacman.x = newX;
        pacman.y = newY;
        
        // Handle what's in the new cell
        if (board[newY][newX] == DOT) {
            score += 10;
            dots--;
        } else if (board[newY][newX] == POWER_PILL) {
            score += 50;
            dots--;
            powerMode = 1;
            powerTimer = 20; // Power mode lasts for 20 moves
        }
        
        board[newY][newX] = PACMAN;
    }
}

void moveGhosts() {
    int i;
    for (i = 0; i < 4; i++) {
        // Simple ghost AI - random movement
        int dir = rand() % 4;
        int newX = ghosts[i].x + directions[dir].dx;
        int newY = ghosts[i].y + directions[dir].dy;
        
        // Check if the move is valid
        if (isValidMove(newX, newY)) {
            ghosts[i].x = newX;
            ghosts[i].y = newY;
        }
    }
}

int isValidMove(int x, int y) {
    // Check boundaries
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return 0;
    }
    
    // Check if it's a wall
    if (board[y][x] == WALL) {
        return 0;
    }
    
    return 1;
}

void checkCollision() {
    int i;
    for (i = 0; i < 4; i++) {
        if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
            if (powerMode) {
                // Eat the ghost
                score += 100;
                // Reset ghost position
                ghosts[i].x = (i % 2 == 0) ? 1 : WIDTH - 2;
                ghosts[i].y = (i < 2) ? 1 : HEIGHT - 2;
            } else {
                // Game over
                system("cls");
                printf("Game Over! A ghost caught you.\n");
                printf("Final Score: %d\n", score);
                exit(0);
            }
        }
    }
}

void updatePowerMode() {
    if (powerMode) {
        powerTimer--;
        if (powerTimer <= 0) {
            powerMode = 0;
        }
    }
}