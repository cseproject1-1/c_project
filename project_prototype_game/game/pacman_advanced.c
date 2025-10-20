#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

#define BOARD_WIDTH 21
#define BOARD_HEIGHT 21
#define MAX_GHOSTS 4
#define MAX_NAME_LENGTH 20

// Game elements
#define WALL '#'
#define DOT '.'
#define EMPTY ' '
#define PACMAN 'C'
#define GHOST 'G'
#define POWER_PILL 'O'
#define FRUIT 'F'

// Game states
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_WIN 3

// Directions
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_NONE -1

// Colors
#define COLOR_RESET 7
#define COLOR_PACMAN 14
#define COLOR_GHOST 12
#define COLOR_WALL 9
#define COLOR_DOT 15
#define COLOR_POWER 10
#define COLOR_FRUIT 13

// Game structures
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
    int scared;
} Ghost;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    int level;
} Player;

typedef struct {
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    int dotsCount;
    int powerMode;
    int powerTimer;
    int score;
    int level;
    int fruitsEaten;
} Game;

// Global variables
Pacman pacman;
Ghost ghosts[MAX_GHOSTS];
Game game;
Player player;
int gameState = STATE_MENU;
int gameSpeed = 150;

// Predefined maze layouts
const char level1[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#.........#.........#",
    "#.###.#.###.#.###.##",
    "#...................#",
    "#.#.#.#####.#.#.####",
    "#.#.............#...#",
    "#.###############.#.#",
    "#.................#.#",
    "#########.#########.#",
    "        #.#         ",
    "#########.#.#########",
    "#.#.................#",
    "#.#.###############.#",
    "#...#.............#.#",
    "####.#.#####.#.#.####",
    "#...................#",
    "##.###.#.###.#.###.##",
    "#.........#.........#",
    "#####################"
};

// Function prototypes
void initGame();
void initMaze();
void initPacman();
void initGhosts();
void drawBoard();
void drawStats();
void processInput();
void updateGame();
void movePacman();
void moveGhosts();
int isValidMove(int x, int y);
void checkCollisions();
void checkPowerMode();
void spawnFruit();
void resetLevel();
void nextLevel();
void saveScore();
void loadScores();
void showMenu();
void setColor(int color);
void gotoxy(int x, int y);
void clearScreen();

int main() {
    srand((unsigned int)time(NULL));
    
    // Initialize game
    strcpy(player.name, "Player");
    player.score = 0;
    player.level = 1;
    
    initGame();
    
    while (1) {
        switch (gameState) {
            case STATE_MENU:
                showMenu();
                break;
            case STATE_PLAYING:
                clearScreen();
                drawBoard();
                drawStats();
                processInput();
                updateGame();
                Sleep(gameSpeed);
                break;
            case STATE_GAME_OVER:
                clearScreen();
                printf("GAME OVER!\n");
                printf("Final Score: %d\n", game.score);
                printf("Press any key to return to menu...\n");
                getch();
                gameState = STATE_MENU;
                break;
            case STATE_WIN:
                clearScreen();
                printf("YOU WIN!\n");
                printf("Final Score: %d\n", game.score);
                printf("Press any key to return to menu...\n");
                getch();
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
    game.fruitsEaten = 0;
    
    initMaze();
    initPacman();
    initGhosts();
}

void initMaze() {
    int i, j;
    game.dotsCount = 0;
    
    // Copy level layout
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            game.board[i][j] = level1[i][j];
            if (game.board[i][j] == DOT) {
                game.dotsCount++;
            }
        }
    }
    
    // Add power pills
    game.board[1][1] = POWER_PILL;
    game.board[1][BOARD_WIDTH-2] = POWER_PILL;
    game.board[BOARD_HEIGHT-2][1] = POWER_PILL;
    game.board[BOARD_HEIGHT-2][BOARD_WIDTH-2] = POWER_PILL;
    game.dotsCount -= 4; // Adjust dot count
}

void initPacman() {
    pacman.x = BOARD_WIDTH / 2;
    pacman.y = BOARD_HEIGHT / 2;
    pacman.direction = DIR_NONE;
    pacman.nextDirection = DIR_NONE;
    pacman.lives = 3;
}

void initGhosts() {
    int i;
    // Initialize ghosts in corners
    ghosts[0].x = 1;
    ghosts[0].y = 1;
    ghosts[0].direction = DIR_RIGHT;
    ghosts[0].active = 1;
    ghosts[0].scared = 0;
    
    ghosts[1].x = BOARD_WIDTH - 2;
    ghosts[1].y = 1;
    ghosts[1].direction = DIR_LEFT;
    ghosts[1].active = 1;
    ghosts[1].scared = 0;
    
    ghosts[2].x = 1;
    ghosts[2].y = BOARD_HEIGHT - 2;
    ghosts[2].direction = DIR_RIGHT;
    ghosts[2].active = 1;
    ghosts[2].scared = 0;
    
    ghosts[3].x = BOARD_WIDTH - 2;
    ghosts[3].y = BOARD_HEIGHT - 2;
    ghosts[3].direction = DIR_LEFT;
    ghosts[3].active = 1;
    ghosts[3].scared = 0;
}

void drawBoard() {
    int i, j;
    
    // Draw top border
    for (i = 0; i < BOARD_WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");
    
    // Draw board
    for (i = 0; i < BOARD_HEIGHT; i++) {
        printf("#"); // Left border
        for (j = 0; j < BOARD_WIDTH; j++) {
            // Check if Pacman is here
            if (i == pacman.y && j == pacman.x) {
                setColor(COLOR_PACMAN);
                printf("%c", PACMAN);
                setColor(COLOR_RESET);
            }
            // Check if any ghost is here
            else {
                int ghostHere = 0;
                int g;
                for (g = 0; g < MAX_GHOSTS; g++) {
                    if (ghosts[g].active && ghosts[g].x == j && ghosts[g].y == i) {
                        if (game.powerMode) {
                            setColor(COLOR_POWER);
                        } else {
                            setColor(COLOR_GHOST);
                        }
                        printf("%c", GHOST);
                        setColor(COLOR_RESET);
                        ghostHere = 1;
                        break;
                    }
                }
                
                if (!ghostHere) {
                    switch (game.board[i][j]) {
                        case WALL:
                            setColor(COLOR_WALL);
                            printf("%c", WALL);
                            setColor(COLOR_RESET);
                            break;
                        case DOT:
                            setColor(COLOR_DOT);
                            printf("%c", DOT);
                            setColor(COLOR_RESET);
                            break;
                        case POWER_PILL:
                            setColor(COLOR_POWER);
                            printf("%c", POWER_PILL);
                            setColor(COLOR_RESET);
                            break;
                        case FRUIT:
                            setColor(COLOR_FRUIT);
                            printf("%c", FRUIT);
                            setColor(COLOR_RESET);
                            break;
                        default:
                            printf("%c", game.board[i][j]);
                            break;
                    }
                }
            }
        }
        printf("#\n"); // Right border
    }
    
    // Draw bottom border
    for (i = 0; i < BOARD_WIDTH + 2; i++) {
        printf("#");
    }
    printf("\n");
}

void drawStats() {
    printf("\nScore: %d", game.score);
    printf("  Level: %d", game.level);
    printf("  Lives: ");
    int i;
    for (i = 0; i < pacman.lives; i++) {
        setColor(COLOR_PACMAN);
        printf("%c ", PACMAN);
        setColor(COLOR_RESET);
    }
    
    if (game.powerMode) {
        printf("  POWER MODE: %d", game.powerTimer);
    }
    
    printf("\nUse WASD to move, Q to quit\n");
}

void processInput() {
    if (kbhit()) {
        char key = getch();
        switch (key) {
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
                break;
        }
    }
}

void updateGame() {
    movePacman();
    moveGhosts();
    checkCollisions();
    checkPowerMode();
    
    // Check win condition
    if (game.dotsCount <= 0) {
        nextLevel();
    }
    
    // Occasionally spawn fruit
    if (rand() % 500 == 0) {
        spawnFruit();
    }
}

void movePacman() {
    int newX = pacman.x;
    int newY = pacman.y;
    int newDirection = pacman.direction;
    
    // Try next direction first
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
    
    // If next direction not valid, try current direction
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
    
    // Move if valid
    if (isValidMove(newX, newY)) {
        pacman.x = newX;
        pacman.y = newY;
        pacman.direction = newDirection;
        
        // Handle what's in the new cell
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
                game.powerTimer = 30;
                break;
            case FRUIT:
                game.score += 100;
                game.fruitsEaten++;
                game.board[pacman.y][pacman.x] = EMPTY;
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
        
        // Simple AI: 70% chance to continue in same direction, 30% to change
        if (rand() % 100 < 30) {
            // Choose random direction
            int possibleDirections[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
            int validDirections[4];
            int validCount = 0;
            
            // Check which directions are valid
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
            
            // Choose a valid direction if available
            if (validCount > 0) {
                newDirection = validDirections[rand() % validCount];
            }
        }
        
        // Move in chosen direction
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
        
        // Move if valid
        if (isValidMove(newX, newY)) {
            ghosts[i].x = newX;
            ghosts[i].y = newY;
            ghosts[i].direction = newDirection;
        }
    }
}

int isValidMove(int x, int y) {
    // Check boundaries
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return 0;
    }
    
    // Check if it's a wall
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
                // Eat the ghost
                game.score += 200;
                ghosts[i].active = 0;
                // Respawn ghost after a delay
                // In a more advanced version, we would implement a proper respawn system
            } else {
                // Lose a life
                pacman.lives--;
                if (pacman.lives <= 0) {
                    gameState = STATE_GAME_OVER;
                } else {
                    // Reset positions
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

void spawnFruit() {
    // Find an empty spot to place fruit
    int attempts = 0;
    while (attempts < 50) {
        int x = rand() % (BOARD_WIDTH - 2) + 1;
        int y = rand() % (BOARD_HEIGHT - 2) + 1;
        
        if (game.board[y][x] == EMPTY) {
            game.board[y][x] = FRUIT;
            return;
        }
        attempts++;
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
    player.level = game.level;
    game.score += 1000; // Bonus for completing level
    resetLevel();
    
    // Increase difficulty
    if (gameSpeed > 50) {
        gameSpeed -= 10;
    }
}

void saveScore() {
    // In a real implementation, this would save to a file
}

void loadScores() {
    // In a real implementation, this would load from a file
}

void showMenu() {
    clearScreen();
    printf("======= PACMAN =======\n");
    printf("1. Start Game\n");
    printf("2. High Scores\n");
    printf("3. Exit\n");
    printf("Choose an option: ");
    
    char choice = getch();
    switch (choice) {
        case '1':
            gameState = STATE_PLAYING;
            initGame();
            break;
        case '2':
            clearScreen();
            printf("High Scores\n");
            printf("-----------\n");
            printf("No scores yet!\n");
            printf("\nPress any key to continue...");
            getch();
            break;
        case '3':
            exit(0);
            break;
    }
}

void setColor(int color) {
    // Set text color (Windows specific)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y) {
    // Move cursor to position (Windows specific)
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearScreen() {
    // Clear screen (Windows specific)
    system("cls");
}