/*
 * Simplified Pacman Game in C
 * This version avoids platform-specific functions for better portability
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15
#define MAX_GHOSTS 3

/* Game elements */
#define WALL '#'
#define DOT '.'
#define EMPTY ' '
#define PACMAN 'P'
#define GHOST 'G'
#define POWER_PILL 'O'

/* Game states */
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_WIN 3

/* Directions */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_NONE -1

/* Game structures */
typedef struct {
    int x, y;
    int direction;
    int nextDirection;
    int lives;
} Pacman;

typedef struct {
    int x, y;
    int direction;
    int active;
} Ghost;

typedef struct {
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    int dotsCount;
    int powerMode;
    int powerTimer;
    int score;
    int level;
} Game;

/* Global variables */
Pacman pacman;
Ghost ghosts[MAX_GHOSTS];
Game game;
int gameState = STATE_MENU;

/* Predefined maze layout */
const char initialMaze[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###############",
    "#.............#",
    "#.###.#.###.##",
    "#.............#",
    "#.#.#.#####.#.#",
    "#.#...........#",
    "#.###########.#",
    "#.............#",
    "#.###########.#",
    "#.............#",
    "##.#####.#.####",
    "#.............#",
    "#.###.#.###.##",
    "#.............#",
    "###############"
};

/* Function prototypes */
void initGame();
void initMaze();
void initPacman();
void initGhosts();
void printBoard();
void printStats();
void updateGame();
void movePacman();
void moveGhosts();
int isValidMove(int x, int y);
void checkCollisions();
void checkPowerMode();
void resetLevel();
void nextLevel();

int main() {
    /* Initialize random seed */
    srand((unsigned int)time(NULL));
    
    /* Initialize game */
    initGame();
    
    printf("=== PACMAN GAME ===\n");
    printf("Rules:\n");
    printf("- Eat all dots to win\n");
    printf("- Avoid ghosts\n");
    printf("- Power pills let you eat ghosts\n");
    printf("- Press Enter to start...\n");
    getchar();
    
    /* Main game loop */
    while (1) {
        switch (gameState) {
            case STATE_MENU:
                printf("\n=== MENU ===\n");
                printf("1. Start Game\n");
                printf("2. Quit\n");
                printf("Enter choice: ");
                
                int choice;
                scanf("%d", &choice);
                
                if (choice == 1) {
                    gameState = STATE_PLAYING;
                    initGame();
                } else {
                    printf("Thanks for playing!\n");
                    return 0;
                }
                break;
                
            case STATE_PLAYING:
                printBoard();
                printStats();
                
                /* Get user input */
                printf("Move (WASD) or Q to quit: ");
                char input;
                scanf(" %c", &input);
                
                switch (input) {
                    case 'w': case 'W':
                        pacman.nextDirection = DIR_UP;
                        break;
                    case 's': case 'S':
                        pacman.nextDirection = DIR_DOWN;
                        break;
                    case 'a': case 'A':
                        pacman.nextDirection = DIR_LEFT;
                        break;
                    case 'd': case 'D':
                        pacman.nextDirection = DIR_RIGHT;
                        break;
                    case 'q': case 'Q':
                        gameState = STATE_MENU;
                        continue;
                }
                
                /* Update game state */
                updateGame();
                
                /* Check win condition */
                if (game.dotsCount <= 0) {
                    gameState = STATE_WIN;
                }
                break;
                
            case STATE_GAME_OVER:
                printf("\n*** GAME OVER ***\n");
                printf("Final Score: %d\n", game.score);
                gameState = STATE_MENU;
                break;
                
            case STATE_WIN:
                printf("\n*** YOU WIN! ***\n");
                printf("Final Score: %d\n", game.score);
                gameState = STATE_MENU;
                break;
        }
    }
    
    return 0;
}

void initGame() {
    game.score = 0;
    game.level = 1;
    game.powerMode = 0;
    game.powerTimer = 0;
    game.dotsCount = 0;
    
    initMaze();
    initPacman();
    initGhosts();
}

void initMaze() {
    int i, j;
    game.dotsCount = 0;
    
    /* Copy initial maze layout */
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            game.board[i][j] = initialMaze[i][j];
            if (game.board[i][j] == DOT) {
                game.dotsCount++;
            }
        }
    }
    
    /* Add power pills at corners */
    game.board[1][1] = POWER_PILL;
    game.board[1][BOARD_WIDTH-2] = POWER_PILL;
    game.board[BOARD_HEIGHT-2][1] = POWER_PILL;
    game.board[BOARD_HEIGHT-2][BOARD_WIDTH-2] = POWER_PILL;
    game.dotsCount -= 4; /* Adjust dot count */
}

void initPacman() {
    pacman.x = BOARD_WIDTH / 2;
    pacman.y = BOARD_HEIGHT / 2;
    pacman.direction = DIR_NONE;
    pacman.nextDirection = DIR_NONE;
    pacman.lives = 3;
}

void initGhosts() {
    /* Initialize ghosts in corners */
    ghosts[0].x = 1;
    ghosts[0].y = 1;
    ghosts[0].direction = DIR_RIGHT;
    ghosts[0].active = 1;
    
    ghosts[1].x = BOARD_WIDTH - 2;
    ghosts[1].y = 1;
    ghosts[1].direction = DIR_LEFT;
    ghosts[1].active = 1;
    
    ghosts[2].x = 1;
    ghosts[2].y = BOARD_HEIGHT - 2;
    ghosts[2].direction = DIR_RIGHT;
    ghosts[2].active = 1;
}

void printBoard() {
    int i, j;
    
    printf("\n");
    
    /* Print top border */
    for (j = 0; j < BOARD_WIDTH + 2; j++) {
        printf("#");
    }
    printf("\n");
    
    /* Print board */
    for (i = 0; i < BOARD_HEIGHT; i++) {
        printf("#"); /* Left border */
        for (j = 0; j < BOARD_WIDTH; j++) {
            /* Check if Pacman is here */
            if (i == pacman.y && j == pacman.x) {
                printf("%c", PACMAN);
            }
            /* Check if any ghost is here */
            else {
                int ghostHere = 0;
                int g;
                for (g = 0; g < MAX_GHOSTS; g++) {
                    if (ghosts[g].active && ghosts[g].x == j && ghosts[g].y == i) {
                        printf("%c", GHOST);
                        ghostHere = 1;
                        break;
                    }
                }
                
                if (!ghostHere) {
                    printf("%c", game.board[i][j]);
                }
            }
        }
        printf("#\n"); /* Right border */
    }
    
    /* Print bottom border */
    for (j = 0; j < BOARD_WIDTH + 2; j++) {
        printf("#");
    }
    printf("\n");
}

void printStats() {
    printf("\nScore: %d | Level: %d | Lives: %d", game.score, game.level, pacman.lives);
    if (game.powerMode) {
        printf(" | POWER MODE: %d", game.powerTimer);
    }
    printf("\n");
}

void updateGame() {
    movePacman();
    moveGhosts();
    checkCollisions();
    checkPowerMode();
}

void movePacman() {
    int newX = pacman.x;
    int newY = pacman.y;
    int newDirection = pacman.direction;
    
    /* Try next direction first */
    if (pacman.nextDirection != DIR_NONE) {
        switch (pacman.nextDirection) {
            case DIR_UP:
                newX = pacman.x;
                newY = pacman.y - 1;
                break;
            case DIR_DOWN:
                newX = pacman.x;
                newY = pacman.y + 1;
                break;
            case DIR_LEFT:
                newX = pacman.x - 1;
                newY = pacman.y;
                break;
            case DIR_RIGHT:
                newX = pacman.x + 1;
                newY = pacman.y;
                break;
        }
        
        if (isValidMove(newX, newY)) {
            newDirection = pacman.nextDirection;
        }
    }
    
    /* If next direction not valid, try current direction */
    if (newDirection == pacman.direction || pacman.nextDirection == DIR_NONE) {
        switch (pacman.direction) {
            case DIR_UP:
                newX = pacman.x;
                newY = pacman.y - 1;
                break;
            case DIR_DOWN:
                newX = pacman.x;
                newY = pacman.y + 1;
                break;
            case DIR_LEFT:
                newX = pacman.x - 1;
                newY = pacman.y;
                break;
            case DIR_RIGHT:
                newX = pacman.x + 1;
                newY = pacman.y;
                break;
        }
    }
    
    /* Move if valid */
    if (isValidMove(newX, newY)) {
        pacman.x = newX;
        pacman.y = newY;
        pacman.direction = newDirection;
        
        /* Handle what's in the new cell */
        switch (game.board[pacman.y][pacman.x]) {
            case DOT:
                game.score += 10;
                game.dotsCount--;
                game.board[pacman.y][pacman.x] = EMPTY;
                break;
            case POWER_PILL:
                game.score += 50;
                game.dotsCount--;
                game.board[pacman.y][pacman.x] = EMPTY;
                game.powerMode = 1;
                game.powerTimer = 20;
                break;
        }
    }
    
    pacman.nextDirection = DIR_NONE;
}

void moveGhosts() {
    int i;
    for (i = 0; i < MAX_GHOSTS; i++) {
        if (!ghosts[i].active) continue;
        
        int newX = ghosts[i].x;
        int newY = ghosts[i].y;
        int newDirection = ghosts[i].direction;
        
        /* Simple AI: 70% chance to continue in same direction, 30% to change */
        if (rand() % 100 < 30) {
            /* Choose random direction */
            int possibleDirections[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
            int validDirections[4];
            int validCount = 0;
            
            /* Check which directions are valid */
            int d;
            for (d = 0; d < 4; d++) {
                int testX = ghosts[i].x;
                int testY = ghosts[i].y;
                
                switch (possibleDirections[d]) {
                    case DIR_UP:
                        testY--;
                        break;
                    case DIR_DOWN:
                        testY++;
                        break;
                    case DIR_LEFT:
                        testX--;
                        break;
                    case DIR_RIGHT:
                        testX++;
                        break;
                }
                
                if (isValidMove(testX, testY)) {
                    validDirections[validCount++] = possibleDirections[d];
                }
            }
            
            /* Choose a valid direction if available */
            if (validCount > 0) {
                newDirection = validDirections[rand() % validCount];
            }
        }
        
        /* Move in chosen direction */
        switch (newDirection) {
            case DIR_UP:
                newX = ghosts[i].x;
                newY = ghosts[i].y - 1;
                break;
            case DIR_DOWN:
                newX = ghosts[i].x;
                newY = ghosts[i].y + 1;
                break;
            case DIR_LEFT:
                newX = ghosts[i].x - 1;
                newY = ghosts[i].y;
                break;
            case DIR_RIGHT:
                newX = ghosts[i].x + 1;
                newY = ghosts[i].y;
                break;
        }
        
        /* Move if valid */
        if (isValidMove(newX, newY)) {
            ghosts[i].x = newX;
            ghosts[i].y = newY;
            ghosts[i].direction = newDirection;
        }
    }
}

int isValidMove(int x, int y) {
    /* Check boundaries */
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return 0;
    }
    
    /* Check if it's a wall */
    if (game.board[y][x] == WALL) {
        return 0;
    }
    
    return 1;
}

void checkCollisions() {
    int i;
    for (i = 0; i < MAX_GHOSTS; i++) {
        if (!ghosts[i].active) continue;
        
        if (pacman.x == ghosts[i].x && pacman.y == ghosts[i].y) {
            if (game.powerMode) {
                /* Eat the ghost */
                game.score += 200;
                ghosts[i].active = 0;
            } else {
                /* Lose a life */
                pacman.lives--;
                if (pacman.lives <= 0) {
                    gameState = STATE_GAME_OVER;
                } else {
                    /* Reset positions */
                    initPacman();
                    initGhosts();
                }
                return;
            }
        }
    }
}

void checkPowerMode() {
    if (game.powerMode) {
        game.powerTimer--;
        if (game.powerTimer <= 0) {
            game.powerMode = 0;
        }
    }
}

void resetLevel() {
    initMaze();
    initPacman();
    initGhosts();
    game.powerMode = 0;
    game.powerTimer = 0;
}

void nextLevel() {
    game.level++;
    game.score += 1000; /* Bonus for completing level */
    resetLevel();
}