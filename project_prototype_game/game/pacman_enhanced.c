/*
 * Enhanced Pacman Game in C
 * Advanced features and improved gameplay
 */

/* Game configuration */
#define BOARD_WIDTH 19
#define BOARD_HEIGHT 19
#define MAX_GHOSTS 4
#define MAX_LEVELS 3
#define MAX_HIGH_SCORES 5

/* Game elements */
#define WALL '#'
#define DOT '.'
#define EMPTY ' '
#define PACMAN_RIGHT 'C'
#define PACMAN_LEFT 'c'
#define PACMAN_UP 'u'
#define PACMAN_DOWN 'n'
#define GHOST_NORMAL 'G'
#define GHOST_SCARED 'X'
#define POWER_PILL 'O'
#define FRUIT_CHERRY 'h'
#define FRUIT_STRAWBERRY 's'
#define FRUIT_APPLE 'a'

/* Game states */
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_WIN 3
#define STATE_HIGH_SCORES 4
#define STATE_HELP 5

/* Directions */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_NONE -1

/* Ghost modes */
#define MODE_CHASE 0
#define MODE_SCATTER 1
#define MODE_FRIGHTENED 2

/* Game structures */
typedef struct {
    int x, y;
    int direction;
    int nextDirection;
    int lives;
    int animationFrame;
} Pacman;

typedef struct {
    int x, y;
    int direction;
    int mode;
    int active;
    int speed;
    int scatterTimer;
    int frightenedTimer;
} Ghost;

typedef struct {
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    int dotsCount;
    int powerMode;
    int powerTimer;
    int score;
    int level;
    int fruitsEaten;
    int ghostsEaten;
} Game;

typedef struct {
    char name[20];
    int score;
} HighScore;

/* Level layouts */
const char level1[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###################",
    "#........#........#",
    "#.####.#.#.####.##",
    "#.................#",
    "#.#.#.#####.#.#.####",
    "#.#.............#..#",
    "#.###############.#",
    "#.................#",
    "#########.#.########",
    "        #.#         ",
    "#########.#.########",
    "#.................#",
    "#.###############.#",
    "#..#.............#.#",
    "####.#.#####.#.#.####",
    "#.................#",
    "##.####.#.#.####.##",
    "#........#........#",
    "###################"
};

const char level2[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###################",
    "#.#.............#.#",
    "#.#.###########.#.#",
    "#.................#",
    "#.###.#.###.#.####",
    "#.....#.#...#.....#",
    "#####.#.#.#####.###",
    "    #.#.#.#     #  ",
    "#####.#.#.# ##### ##",
    "        #.#        ",
    "## #####.#.# ##### #",
    "  #     #.#.# #    ",
    "###.#####.#.#####.#",
    "#.....#...#.#.....#",
    "#.####.###.#.###.##",
    "#.................#",
    "#.#.###########.#.#",
    "#.#.............#.#",
    "###################"
};

const char level3[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "###################",
    "#.................#",
    "#.#####.#.#####.##",
    "#.#...#.#.#...#..#",
    "#.#.#.#.#.#.#.####",
    "#...#...#...#....#",
    "#.###########.####",
    "#.#.#.......#.#   ",
    "#.#.#########.# ###",
    "  #.....#.....# #  ",
    "###.#.#####.#.# # #",
    "   #.#.......#.# # ",
    "####.###########.#",
    "#....#...#...#...#",
    "####.#.#.#.#.#.####",
    "#..#.#...#.#...#.#",
    "##.#####.#.#####.#",
    "#.................#",
    "###################"
};

/* Global variables */
Pacman pacman;
Ghost ghosts[MAX_GHOSTS];
Game game;
HighScore highScores[MAX_HIGH_SCORES];
int gameState = STATE_MENU;
int currentLevelLayout = 0;

/* Function prototypes */
void initGame();
void initMaze();
void initPacman();
void initGhosts();
void initHighScores();
void saveHighScore();
void loadHighScores();
void printBoard();
void printStats();
void printMenu();
void printHighScores();
void printHelp();
void processInput();
void updateGame();
void movePacman();
void moveGhosts();
void ghostAI(int ghostIndex);
int isValidMove(int x, int y);
void checkCollisions();
void checkPowerMode();
void spawnFruit();
void resetLevel();
void nextLevel();
void updateHighScores();
int isWall(int x, int y);

int main() {
    /* Initialize game components */
    initHighScores();
    initGame();
    
    /* Main game loop */
    while (1) {
        switch (gameState) {
            case STATE_MENU:
                printMenu();
                processInput();
                break;
                
            case STATE_PLAYING:
                printBoard();
                printStats();
                processInput();
                updateGame();
                break;
                
            case STATE_GAME_OVER:
                saveHighScore();
                printBoard();
                printf("\n*** GAME OVER ***\n");
                printf("Final Score: %d\n", game.score);
                printf("Press Enter to continue...");
                getchar();
                gameState = STATE_MENU;
                break;
                
            case STATE_WIN:
                if (game.level >= MAX_LEVELS) {
                    saveHighScore();
                    printf("\n*** CONGRATULATIONS! YOU BEAT ALL LEVELS! ***\n");
                    printf("Final Score: %d\n", game.score);
                } else {
                    printf("\n*** LEVEL COMPLETE! ***\n");
                    printf("Score: %d\n", game.score);
                    nextLevel();
                    printf("Starting Level %d...\n", game.level);
                }
                printf("Press Enter to continue...");
                getchar();
                if (game.level < MAX_LEVELS) {
                    gameState = STATE_PLAYING;
                } else {
                    gameState = STATE_MENU;
                }
                break;
                
            case STATE_HIGH_SCORES:
                printHighScores();
                processInput();
                break;
                
            case STATE_HELP:
                printHelp();
                processInput();
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
    game.ghostsEaten = 0;
    
    initMaze();
    initPacman();
    initGhosts();
}

void initMaze() {
    int i, j;
    const char (*currentLevel)[BOARD_WIDTH + 1];
    
    /* Select level layout */
    switch (game.level) {
        case 1: currentLevel = level1; break;
        case 2: currentLevel = level2; break;
        case 3: currentLevel = level3; break;
        default: currentLevel = level1; break;
    }
    
    game.dotsCount = 0;
    
    /* Copy level layout */
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            game.board[i][j] = currentLevel[i][j];
            if (game.board[i][j] == DOT) {
                game.dotsCount++;
            }
        }
    }
    
    /* Add power pills */
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
    pacman.animationFrame = 0;
}

void initGhosts() {
    /* Initialize ghosts with different behaviors */
    ghosts[0].x = 1;
    ghosts[0].y = 1;
    ghosts[0].direction = DIR_RIGHT;
    ghosts[0].mode = MODE_SCATTER;
    ghosts[0].active = 1;
    ghosts[0].speed = 1;
    ghosts[0].scatterTimer = 100;
    ghosts[0].frightenedTimer = 0;
    
    ghosts[1].x = BOARD_WIDTH - 2;
    ghosts[1].y = 1;
    ghosts[1].direction = DIR_LEFT;
    ghosts[1].mode = MODE_SCATTER;
    ghosts[1].active = 1;
    ghosts[1].speed = 1;
    ghosts[1].scatterTimer = 100;
    ghosts[1].frightenedTimer = 0;
    
    ghosts[2].x = 1;
    ghosts[2].y = BOARD_HEIGHT - 2;
    ghosts[2].direction = DIR_RIGHT;
    ghosts[2].mode = MODE_SCATTER;
    ghosts[2].active = 1;
    ghosts[2].speed = 1;
    ghosts[2].scatterTimer = 100;
    ghosts[2].frightenedTimer = 0;
    
    ghosts[3].x = BOARD_WIDTH - 2;
    ghosts[3].y = BOARD_HEIGHT - 2;
    ghosts[3].direction = DIR_LEFT;
    ghosts[3].mode = MODE_SCATTER;
    ghosts[3].active = 1;
    ghosts[3].speed = 1;
    ghosts[3].scatterTimer = 100;
    ghosts[3].frightenedTimer = 0;
}

void initHighScores() {
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        strcpy(highScores[i].name, "None");
        highScores[i].score = 0;
    }
}

void saveHighScore() {
    updateHighScores();
}

void loadHighScores() {
    /* In a real implementation, this would load from a file */
}

void printBoard() {
    int i, j;
    
    /* Print top border */
    printf("\n+");
    for (j = 0; j < BOARD_WIDTH; j++) {
        printf("-");
    }
    printf("+\n");
    
    /* Print board */
    for (i = 0; i < BOARD_HEIGHT; i++) {
        printf("|"); /* Left border */
        for (j = 0; j < BOARD_WIDTH; j++) {
            /* Check if Pacman is here */
            if (i == pacman.y && j == pacman.x) {
                switch (pacman.direction) {
                    case DIR_RIGHT: printf("%c", PACMAN_RIGHT); break;
                    case DIR_LEFT: printf("%c", PACMAN_LEFT); break;
                    case DIR_UP: printf("%c", PACMAN_UP); break;
                    case DIR_DOWN: printf("%c", PACMAN_DOWN); break;
                    default: printf("%c", PACMAN_RIGHT); break;
                }
            }
            /* Check if any ghost is here */
            else {
                int ghostHere = 0;
                int g;
                for (g = 0; g < MAX_GHOSTS; g++) {
                    if (ghosts[g].active && ghosts[g].x == j && ghosts[g].y == i) {
                        if (ghosts[g].mode == MODE_FRIGHTENED) {
                            printf("%c", GHOST_SCARED);
                        } else {
                            printf("%c", GHOST_NORMAL);
                        }
                        ghostHere = 1;
                        break;
                    }
                }
                
                if (!ghostHere) {
                    printf("%c", game.board[i][j]);
                }
            }
        }
        printf("|\n"); /* Right border */
    }
    
    /* Print bottom border */
    printf("+");
    for (j = 0; j < BOARD_WIDTH; j++) {
        printf("-");
    }
    printf("+\n");
}

void printStats() {
    printf("\nScore: %d | Level: %d | Lives: ", game.score, game.level);
    int i;
    for (i = 0; i < pacman.lives; i++) {
        printf("C ");
    }
    
    if (game.powerMode) {
        printf("| POWER: %d", game.powerTimer);
    }
    
    printf("\nGhosts eaten: %d | Fruits eaten: %d\n", game.ghostsEaten, game.fruitsEaten);
}

void printMenu() {
    printf("\n======= PACMAN =======\n");
    printf("1. Start Game\n");
    printf("2. High Scores\n");
    printf("3. Help\n");
    printf("4. Exit\n");
    printf("Choose an option: ");
}

void printHighScores() {
    printf("\n===== HIGH SCORES =====\n");
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        printf("%d. %-15s %d\n", i+1, highScores[i].name, highScores[i].score);
    }
    printf("\nPress Enter to return to menu...");
}

void printHelp() {
    printf("\n======= HELP =======\n");
    printf("CONTROLS:\n");
    printf("  W - Move Up\n");
    printf("  A - Move Left\n");
    printf("  S - Move Down\n");
    printf("  D - Move Right\n");
    printf("  Q - Quit Game\n");
    printf("\nGAME ELEMENTS:\n");
    printf("  C - Pacman\n");
    printf("  G - Ghost\n");
    printf("  . - Dot (10 points)\n");
    printf("  O - Power Pill (50 points)\n");
    printf("  h - Cherry (100 points)\n");
    printf("\nOBJECTIVE:\n");
    printf("Eat all dots while avoiding ghosts.\n");
    printf("Power pills allow you to eat ghosts temporarily.\n");
    printf("\nPress Enter to return to menu...");
}

void processInput() {
    char input;
    scanf(" %c", &input);
    
    switch (gameState) {
        case STATE_MENU:
            switch (input) {
                case '1':
                    gameState = STATE_PLAYING;
                    initGame();
                    break;
                case '2':
                    gameState = STATE_HIGH_SCORES;
                    break;
                case '3':
                    gameState = STATE_HELP;
                    break;
                case '4':
                    exit(0);
                    break;
            }
            break;
            
        case STATE_PLAYING:
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
                    break;
            }
            break;
            
        case STATE_HIGH_SCORES:
        case STATE_HELP:
            if (input) {
                gameState = STATE_MENU;
            }
            break;
    }
}

void updateGame() {
    movePacman();
    moveGhosts();
    checkCollisions();
    checkPowerMode();
    
    /* Occasionally spawn fruit */
    if (rand() % 200 == 0) {
        spawnFruit();
    }
    
    /* Check win condition */
    if (game.dotsCount <= 0) {
        gameState = STATE_WIN;
    }
}

void movePacman() {
    int newX = pacman.x;
    int newY = pacman.y;
    int newDirection = pacman.direction;
    
    /* Animate Pacman */
    pacman.animationFrame = (pacman.animationFrame + 1) % 2;
    
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
                game.powerTimer = 30;
                /* Set all ghosts to frightened mode */
                int i;
                for (i = 0; i < MAX_GHOSTS; i++) {
                    if (ghosts[i].active) {
                        ghosts[i].mode = MODE_FRIGHTENED;
                        ghosts[i].frightenedTimer = 30;
                    }
                }
                break;
            case FRUIT_CHERRY:
            case FRUIT_STRAWBERRY:
            case FRUIT_APPLE:
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
        
        /* Move based on speed */
        if (rand() % ghosts[i].speed == 0) {
            ghostAI(i);
        }
    }
}

void ghostAI(int ghostIndex) {
    Ghost* ghost = &ghosts[ghostIndex];
    int newX = ghost->x;
    int newY = ghost->y;
    int newDirection = ghost->direction;
    
    /* Update timers */
    if (ghost->mode == MODE_SCATTER) {
        ghost->scatterTimer--;
        if (ghost->scatterTimer <= 0) {
            ghost->mode = MODE_CHASE;
        }
    } else if (ghost->mode == MODE_FRIGHTENED) {
        ghost->frightenedTimer--;
        if (ghost->frightenedTimer <= 0) {
            ghost->mode = MODE_CHASE;
        }
    }
    
    /* Simple AI with mode-based behavior */
    if (rand() % 100 < 20) { /* 20% chance to change direction */
        /* Choose direction based on mode */
        int possibleDirections[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
        int validDirections[4];
        int validCount = 0;
        
        /* Check which directions are valid */
        int d;
        for (d = 0; d < 4; d++) {
            int testX = ghost->x;
            int testY = ghost->y;
            
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
            
            /* Don't allow reversing unless in frightened mode */
            int reverseDirection = 0;
            switch (possibleDirections[d]) {
                case DIR_UP: reverseDirection = (ghost->direction == DIR_DOWN); break;
                case DIR_DOWN: reverseDirection = (ghost->direction == DIR_UP); break;
                case DIR_LEFT: reverseDirection = (ghost->direction == DIR_RIGHT); break;
                case DIR_RIGHT: reverseDirection = (ghost->direction == DIR_LEFT); break;
            }
            
            if (!reverseDirection || ghost->mode == MODE_FRIGHTENED) {
                if (isValidMove(testX, testY)) {
                    validDirections[validCount++] = possibleDirections[d];
                }
            }
        }
        
        /* Choose a valid direction if available */
        if (validCount > 0) {
            if (ghost->mode == MODE_FRIGHTENED) {
                /* Random movement when frightened */
                newDirection = validDirections[rand() % validCount];
            } else {
                /* Chase Pacman when not frightened */
                int bestDirection = ghost->direction;
                int bestDistance = 1000;
                
                for (d = 0; d < validCount; d++) {
                    int testX = ghost->x;
                    int testY = ghost->y;
                    
                    switch (validDirections[d]) {
                        case DIR_UP: testY--; break;
                        case DIR_DOWN: testY++; break;
                        case DIR_LEFT: testX--; break;
                        case DIR_RIGHT: testX++; break;
                    }
                    
                    /* Calculate distance to Pacman */
                    int dx = testX - pacman.x;
                    int dy = testY - pacman.y;
                    int distance = dx*dx + dy*dy;
                    
                    if (distance < bestDistance) {
                        bestDistance = distance;
                        bestDirection = validDirections[d];
                    }
                }
                
                newDirection = bestDirection;
            }
        }
    }
    
    /* Move in chosen direction */
    switch (newDirection) {
        case DIR_UP:
            newX = ghost->x;
            newY = ghost->y - 1;
            break;
        case DIR_DOWN:
            newX = ghost->x;
            newY = ghost->y + 1;
            break;
        case DIR_LEFT:
            newX = ghost->x - 1;
            newY = ghost->y;
            break;
        case DIR_RIGHT:
            newX = ghost->x + 1;
            newY = ghost->y;
            break;
    }
    
    /* Move if valid */
    if (isValidMove(newX, newY)) {
        ghost->x = newX;
        ghost->y = newY;
        ghost->direction = newDirection;
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
            if (ghosts[i].mode == MODE_FRIGHTENED) {
                /* Eat the ghost */
                game.score += 200 * (game.ghostsEaten + 1);
                game.ghostsEaten++;
                ghosts[i].active = 0;
                /* Respawn ghost after delay */
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
            /* Reset ghost modes */
            int i;
            for (i = 0; i < MAX_GHOSTS; i++) {
                if (ghosts[i].active && ghosts[i].mode == MODE_FRIGHTENED) {
                    ghosts[i].mode = MODE_CHASE;
                }
            }
        }
    }
}

void spawnFruit() {
    /* Find an empty spot to place fruit */
    int attempts = 0;
    while (attempts < 50) {
        int x = rand() % (BOARD_WIDTH - 2) + 1;
        int y = rand() % (BOARD_HEIGHT - 2) + 1;
        
        if (game.board[y][x] == EMPTY) {
            /* Choose random fruit type */
            char fruits[] = {FRUIT_CHERRY, FRUIT_STRAWBERRY, FRUIT_APPLE};
            game.board[y][x] = fruits[rand() % 3];
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
    game.score += 1000; /* Bonus for completing level */
    resetLevel();
}

void updateHighScores() {
    int i, j;
    HighScore newScore;
    strcpy(newScore.name, "Player");
    newScore.score = game.score;
    
    /* Insert new score in correct position */
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        if (game.score > highScores[i].score) {
            /* Shift lower scores down */
            for (j = MAX_HIGH_SCORES - 1; j > i; j--) {
                highScores[j] = highScores[j-1];
            }
            /* Insert new score */
            highScores[i] = newScore;
            break;
        }
    }
}

int isWall(int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return 1;
    }
    return (game.board[y][x] == WALL);
}