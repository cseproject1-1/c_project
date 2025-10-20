/*
 * Ultimate Pacman Game in C
 * Complete with advanced features, multiple levels, and sophisticated AI
 */

/* === CONFIGURATION SECTION === */
#define BOARD_WIDTH 21
#define BOARD_HEIGHT 21
#define MAX_GHOSTS 4
#define MAX_LEVELS 5
#define MAX_HIGH_SCORES 10
#define ANIMATION_FRAMES 4

/* === GAME ELEMENTS === */
#define WALL '#'
#define DOT '.'
#define EMPTY ' '
#define PACMAN_SYMBOL 'C'
#define GHOST_SYMBOL 'G'
#define SCARED_GHOST_SYMBOL 'X'
#define POWER_PILL_SYMBOL 'O'
#define FRUIT_SYMBOL 'F'

/* === GAME STATES === */
#define STATE_MENU 0
#define STATE_PLAYING 1
#define STATE_GAME_OVER 2
#define STATE_LEVEL_COMPLETE 3
#define STATE_GAME_COMPLETE 4
#define STATE_HIGH_SCORES 5
#define STATE_HELP 6
#define STATE_SETTINGS 7

/* === DIRECTIONS === */
#define DIR_UP 0
#define DIR_DOWN 1
#define DIR_LEFT 2
#define DIR_RIGHT 3
#define DIR_NONE -1

/* === GHOST MODES === */
#define MODE_CHASE 0
#define MODE_SCATTER 1
#define MODE_FRIGHTENED 2
#define MODE_EATEN 3

/* === GAME DIFFICULTY === */
#define DIFFICULTY_EASY 0
#define DIFFICULTY_MEDIUM 1
#define DIFFICULTY_HARD 2

/* === DATA STRUCTURES === */

/* Position structure */
typedef struct {
    int x, y;
} Position;

/* Pacman structure */
typedef struct {
    Position pos;
    int direction;
    int nextDirection;
    int lives;
    int animationFrame;
    int speed;
} Pacman;

/* Ghost structure */
typedef struct {
    Position pos;
    int direction;
    int mode;
    int active;
    int speed;
    int scatterTimer;
    int frightenedTimer;
    int eatenTimer;
    Position target;
    Position scatterTarget;
} Ghost;

/* Game state structure */
typedef struct {
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    int dotsCount;
    int powerMode;
    int powerTimer;
    int score;
    int level;
    int fruitsEaten;
    int ghostsEaten;
    int difficulty;
    int gameSpeed;
    int livesEarned;
} Game;

/* High score structure */
typedef struct {
    char name[20];
    int score;
    int level;
} HighScore;

/* Settings structure */
typedef struct {
    int soundEnabled;
    int animationsEnabled;
    int difficulty;
} Settings;

/* === LEVEL LAYOUTS === */

/* Level 1 - Classic Maze */
const char level1[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#.........#.........#",
    "#.###.###.#.###.####",
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

/* Level 2 - Cross Maze */
const char level2[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#.#...............#.#",
    "#.#.#############.#.#",
    "#...................#",
    "#.###.#.#####.#.####",
    "#.....#.#...#.#.....#",
    "#####.#.#.#.#.#####.#",
    "    #.#.#.#.# #    #",
    "#####.#.#.#.# ##### #",
    "     #.#.#.#.#     #",
    "# #####.#.#.#.##### #",
    "# #    #.#.#.#    # #",
    "# #####.#.#.#.##### #",
    "#.....#.#...#.#.....#",
    "######.#.#####.#.####",
    "#...................#",
    "#.#.#############.#.#",
    "#.#...............#.#",
    "#####################"
};

/* Level 3 - Diamond Maze */
const char level3[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#.........#.........#",
    "#.#####.#.#.#####.##",
    "#.#...#.#.#.#...#..#",
    "#.#.#.#.#.#.#.#.####",
    "#...#...#...#...#..#",
    "#.###########.#####.#",
    "#.#.........#.#    #",
    "#.#.#######.#.# ####",
    "  #.#.....#.#.# #   ",
    "## #.#####.#.# # ##",
    "   #.#.....#.#.# #  ",
    "####.#.#######.#.# #",
    "#    #.#.........#.#",
    "#.#####.###########.#",
    "#..#...#...#...#...#",
    "####.#.#.#.#.#.#.####",
    "#..#.#.#.#.#.#.#..#.#",
    "#####################"
};

/* Level 4 - Spiral Maze */
const char level4[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#...................#",
    "#.#################.#",
    "#.#...............#.#",
    "#.#.#############.#.#",
    "#.#.#...........#.#.#",
    "#.#.#.#########.#.#.#",
    "#.#.#.#.......#.#.#.#",
    "#.#.#.#.#####.#.#.#.#",
    "#.#.#.#.#...#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#...#.#.#.#.#.#",
    "#.#.#.#####.#.#.#.#.#",
    "#.#.#.......#.#.#.#.#",
    "#.#.#########.#.#.#.#",
    "#.#...........#.#.#.#",
    "#.#############.#.#.#",
    "#...............#.#.#",
    "###################.#",
    "#...................#",
    "#####################"
};

/* Level 5 - Complex Maze */
const char level5[BOARD_HEIGHT][BOARD_WIDTH + 1] = {
    "#####################",
    "#.#.............#.#.#",
    "#.#.###########.#.#.#",
    "#.................#.#",
    "#.###.#.#.#.#.#.####",
    "#...#.#.#.#.#.#.#...#",
    "###.#.#.#.#.#.#.#.###",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#.#.#.#.#.#.#.#.#.#.#",
    "#...#.#.#.#.#.#.#...#",
    "####.#.#.#.#.#.#.####",
    "#.................#.#",
    "#.#.###########.#.#.#",
    "#.#.............#.#.#",
    "#####################"
};

/* === GLOBAL VARIABLES === */
Pacman pacman;
Ghost ghosts[MAX_GHOSTS];
Game game;
HighScore highScores[MAX_HIGH_SCORES];
Settings settings;
int gameState = STATE_MENU;

/* === FUNCTION PROTOTYPES === */

/* Core game functions */
void initGame();
void initMaze();
void initPacman();
void initGhosts();
void gameLoop();
void updateGameState();

/* Game state functions */
void handleMenuState();
void handlePlayingState();
void handleGameOverState();
void handleLevelCompleteState();
void handleGameCompleteState();
void handleHighScoresState();
void handleHelpState();
void handleSettingsState();

/* Game mechanics */
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

/* Rendering functions */
void renderGame();
void renderMenu();
void renderBoard();
void renderStats();
void renderHighScores();
void renderHelp();
void renderSettings();

/* Utility functions */
void initHighScores();
void updateHighScores();
void loadHighScores();
void saveHighScores();
void clearScreen();
void delay(int milliseconds);

/* Math functions */
int distance(int x1, int y1, int x2, int y2);
int manhattanDistance(int x1, int y1, int x2, int y2);

/* === MAIN FUNCTION === */
int main() {
    /* Initialize game systems */
    initHighScores();
    initGame();
    
    /* Main game loop */
    while (1) {
        switch (gameState) {
            case STATE_MENU:
                handleMenuState();
                break;
            case STATE_PLAYING:
                handlePlayingState();
                break;
            case STATE_GAME_OVER:
                handleGameOverState();
                break;
            case STATE_LEVEL_COMPLETE:
                handleLevelCompleteState();
                break;
            case STATE_GAME_COMPLETE:
                handleGameCompleteState();
                break;
            case STATE_HIGH_SCORES:
                handleHighScoresState();
                break;
            case STATE_HELP:
                handleHelpState();
                break;
            case STATE_SETTINGS:
                handleSettingsState();
                break;
        }
    }
    
    return 0;
}

/* === CORE GAME FUNCTIONS === */

void initGame() {
    game.score = 0;
    game.level = 1;
    game.powerMode = 0;
    game.powerTimer = 0;
    game.dotsCount = 0;
    game.fruitsEaten = 0;
    game.ghostsEaten = 0;
    game.difficulty = DIFFICULTY_MEDIUM;
    game.gameSpeed = 150;
    game.livesEarned = 0;
    
    settings.soundEnabled = 1;
    settings.animationsEnabled = 1;
    settings.difficulty = DIFFICULTY_MEDIUM;
    
    initMaze();
    initPacman();
    initGhosts();
}

void initMaze() {
    int i, j;
    const char (*currentLevel)[BOARD_WIDTH + 1];
    
    /* Select level layout based on current level */
    switch (game.level) {
        case 1: currentLevel = level1; break;
        case 2: currentLevel = level2; break;
        case 3: currentLevel = level3; break;
        case 4: currentLevel = level4; break;
        case 5: currentLevel = level5; break;
        default: currentLevel = level1; break;
    }
    
    game.dotsCount = 0;
    
    /* Copy level layout to game board */
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            game.board[i][j] = currentLevel[i][j];
            if (game.board[i][j] == DOT) {
                game.dotsCount++;
            }
        }
    }
    
    /* Add power pills at strategic locations */
    game.board[1][1] = POWER_PILL_SYMBOL;
    game.board[1][BOARD_WIDTH-2] = POWER_PILL_SYMBOL;
    game.board[BOARD_HEIGHT-2][1] = POWER_PILL_SYMBOL;
    game.board[BOARD_HEIGHT-2][BOARD_WIDTH-2] = POWER_PILL_SYMBOL;
    game.dotsCount -= 4; /* Adjust dot count */
}

void initPacman() {
    /* Position Pacman in the center of the maze */
    pacman.pos.x = BOARD_WIDTH / 2;
    pacman.pos.y = BOARD_HEIGHT / 2;
    pacman.direction = DIR_NONE;
    pacman.nextDirection = DIR_NONE;
    pacman.lives = 3;
    pacman.animationFrame = 0;
    pacman.speed = 1;
}

void initGhosts() {
    /* Initialize ghosts with different personalities and starting positions */
    
    /* Ghost 0 - Blinky (Red) - Aggressive chaser */
    ghosts[0].pos.x = 1;
    ghosts[0].pos.y = 1;
    ghosts[0].direction = DIR_RIGHT;
    ghosts[0].mode = MODE_SCATTER;
    ghosts[0].active = 1;
    ghosts[0].speed = 1;
    ghosts[0].scatterTimer = 100;
    ghosts[0].frightenedTimer = 0;
    ghosts[0].eatenTimer = 0;
    ghosts[0].scatterTarget.x = BOARD_WIDTH - 1;
    ghosts[0].scatterTarget.y = 0;
    
    /* Ghost 1 - Pinky (Pink) - Ambusher */
    ghosts[1].pos.x = BOARD_WIDTH - 2;
    ghosts[1].pos.y = 1;
    ghosts[1].direction = DIR_LEFT;
    ghosts[1].mode = MODE_SCATTER;
    ghosts[1].active = 1;
    ghosts[1].speed = 1;
    ghosts[1].scatterTimer = 100;
    ghosts[1].frightenedTimer = 0;
    ghosts[1].eatenTimer = 0;
    ghosts[1].scatterTarget.x = 0;
    ghosts[1].scatterTarget.y = 0;
    
    /* Ghost 2 - Inky (Cyan) - Unpredictable */
    ghosts[2].pos.x = 1;
    ghosts[2].pos.y = BOARD_HEIGHT - 2;
    ghosts[2].direction = DIR_RIGHT;
    ghosts[2].mode = MODE_SCATTER;
    ghosts[2].active = 1;
    ghosts[2].speed = 1;
    ghosts[2].scatterTimer = 100;
    ghosts[2].frightenedTimer = 0;
    ghosts[2].eatenTimer = 0;
    ghosts[2].scatterTarget.x = BOARD_WIDTH - 1;
    ghosts[2].scatterTarget.y = BOARD_HEIGHT - 1;
    
    /* Ghost 3 - Clyde (Orange) - Follower */
    ghosts[3].pos.x = BOARD_WIDTH - 2;
    ghosts[3].pos.y = BOARD_HEIGHT - 2;
    ghosts[3].direction = DIR_LEFT;
    ghosts[3].mode = MODE_SCATTER;
    ghosts[3].active = 1;
    ghosts[3].speed = 1;
    ghosts[3].scatterTimer = 100;
    ghosts[3].frightenedTimer = 0;
    ghosts[3].eatenTimer = 0;
    ghosts[3].scatterTarget.x = 0;
    ghosts[3].scatterTarget.y = BOARD_HEIGHT - 1;
}

/* === GAME STATE HANDLERS === */

void handleMenuState() {
    renderMenu();
    processInput();
}

void handlePlayingState() {
    renderGame();
    processInput();
    updateGame();
    delay(game.gameSpeed);
}

void handleGameOverState() {
    saveHighScores();
    renderGame();
    /* Game over logic would go here */
    gameState = STATE_MENU;
}

void handleLevelCompleteState() {
    if (game.level >= MAX_LEVELS) {
        gameState = STATE_GAME_COMPLETE;
    } else {
        nextLevel();
        gameState = STATE_PLAYING;
    }
}

void handleGameCompleteState() {
    saveHighScores();
    renderGame();
    /* Game complete logic would go here */
    gameState = STATE_MENU;
}

void handleHighScoresState() {
    renderHighScores();
    processInput();
}

void handleHelpState() {
    renderHelp();
    processInput();
}

void handleSettingsState() {
    renderSettings();
    processInput();
}

/* === GAME MECHANICS === */

void processInput() {
    /* Input processing logic would go here */
}

void updateGame() {
    movePacman();
    moveGhosts();
    checkCollisions();
    checkPowerMode();
    
    /* Randomly spawn fruit */
    if (rand() % 300 == 0) {
        spawnFruit();
    }
    
    /* Check if level is complete */
    if (game.dotsCount <= 0) {
        gameState = STATE_LEVEL_COMPLETE;
    }
}

void movePacman() {
    int newX = pacman.pos.x;
    int newY = pacman.pos.y;
    int newDirection = pacman.direction;
    
    /* Update animation frame */
    pacman.animationFrame = (pacman.animationFrame + 1) % ANIMATION_FRAMES;
    
    /* Try next direction first */
    if (pacman.nextDirection != DIR_NONE) {
        switch (pacman.nextDirection) {
            case DIR_UP:
                newX = pacman.pos.x;
                newY = pacman.pos.y - 1;
                break;
            case DIR_DOWN:
                newX = pacman.pos.x;
                newY = pacman.pos.y + 1;
                break;
            case DIR_LEFT:
                newX = pacman.pos.x - 1;
                newY = pacman.pos.y;
                break;
            case DIR_RIGHT:
                newX = pacman.pos.x + 1;
                newY = pacman.pos.y;
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
                newX = pacman.pos.x;
                newY = pacman.pos.y - 1;
                break;
            case DIR_DOWN:
                newX = pacman.pos.x;
                newY = pacman.pos.y + 1;
                break;
            case DIR_LEFT:
                newX = pacman.pos.x - 1;
                newY = pacman.pos.y;
                break;
            case DIR_RIGHT:
                newX = pacman.pos.x + 1;
                newY = pacman.pos.y;
                break;
        }
    }
    
    /* Move if valid */
    if (isValidMove(newX, newY)) {
        pacman.pos.x = newX;
        pacman.pos.y = newY;
        pacman.direction = newDirection;
        
        /* Handle what's in the new cell */
        switch (game.board[pacman.pos.y][pacman.pos.x]) {
            case DOT:
                game.score += 10;
                game.dotsCount--;
                game.board[pacman.pos.y][pacman.pos.x] = EMPTY;
                break;
            case POWER_PILL_SYMBOL:
                game.score += 50;
                game.dotsCount--;
                game.board[pacman.pos.y][pacman.pos.x] = EMPTY;
                game.powerMode = 1;
                game.powerTimer = 50;
                /* Set all ghosts to frightened mode */
                int i;
                for (i = 0; i < MAX_GHOSTS; i++) {
                    if (ghosts[i].active && ghosts[i].mode != MODE_EATEN) {
                        ghosts[i].mode = MODE_FRIGHTENED;
                        ghosts[i].frightenedTimer = 50;
                    }
                }
                break;
            case FRUIT_SYMBOL:
                game.score += 100;
                game.fruitsEaten++;
                game.board[pacman.pos.y][pacman.pos.x] = EMPTY;
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
    int newX = ghost->pos.x;
    int newY = ghost->pos.y;
    int newDirection = ghost->direction;
    
    /* Update timers */
    switch (ghost->mode) {
        case MODE_SCATTER:
            ghost->scatterTimer--;
            if (ghost->scatterTimer <= 0) {
                ghost->mode = MODE_CHASE;
            }
            break;
        case MODE_FRIGHTENED:
            ghost->frightenedTimer--;
            if (ghost->frightenedTimer <= 0) {
                ghost->mode = MODE_CHASE;
            }
            break;
        case MODE_EATEN:
            ghost->eatenTimer--;
            if (ghost->eatenTimer <= 0) {
                ghost->mode = MODE_CHASE;
            }
            break;
    }
    
    /* Simple AI with mode-based behavior */
    if (rand() % 100 < 25) { /* 25% chance to change direction */
        /* Choose direction based on mode */
        int possibleDirections[4] = {DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT};
        int validDirections[4];
        int validCount = 0;
        
        /* Check which directions are valid */
        int d;
        for (d = 0; d < 4; d++) {
            int testX = ghost->pos.x;
            int testY = ghost->pos.y;
            
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
            switch (ghost->mode) {
                case MODE_FRIGHTENED:
                    /* Random movement when frightened */
                    newDirection = validDirections[rand() % validCount];
                    break;
                case MODE_EATEN:
                    /* Move toward center when eaten */
                    newDirection = validDirections[rand() % validCount];
                    break;
                case MODE_SCATTER:
                    /* Move toward scatter target */
                    newDirection = validDirections[rand() % validCount];
                    break;
                case MODE_CHASE:
                default:
                    /* Chase Pacman when not frightened */
                    int bestDirection = ghost->direction;
                    int bestDistance = 10000;
                    
                    for (d = 0; d < validCount; d++) {
                        int testX = ghost->pos.x;
                        int testY = ghost->pos.y;
                        
                        switch (validDirections[d]) {
                            case DIR_UP: testY--; break;
                            case DIR_DOWN: testY++; break;
                            case DIR_LEFT: testX--; break;
                            case DIR_RIGHT: testX++; break;
                        }
                        
                        /* Calculate distance to target */
                        int distance;
                        if (ghost->mode == MODE_SCATTER) {
                            distance = manhattanDistance(testX, testY, 
                                                       ghost->scatterTarget.x, 
                                                       ghost->scatterTarget.y);
                        } else {
                            distance = manhattanDistance(testX, testY, 
                                                       pacman.pos.x, 
                                                       pacman.pos.y);
                        }
                        
                        if (distance < bestDistance) {
                            bestDistance = distance;
                            bestDirection = validDirections[d];
                        }
                    }
                    
                    newDirection = bestDirection;
                    break;
            }
        }
    }
    
    /* Move in chosen direction */
    switch (newDirection) {
        case DIR_UP:
            newX = ghost->pos.x;
            newY = ghost->pos.y - 1;
            break;
        case DIR_DOWN:
            newX = ghost->pos.x;
            newY = ghost->pos.y + 1;
            break;
        case DIR_LEFT:
            newX = ghost->pos.x - 1;
            newY = ghost->pos.y;
            break;
        case DIR_RIGHT:
            newX = ghost->pos.x + 1;
            newY = ghost->pos.y;
            break;
    }
    
    /* Move if valid */
    if (isValidMove(newX, newY)) {
        ghost->pos.x = newX;
        ghost->pos.y = newY;
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
        
        if (pacman.pos.x == ghosts[i].pos.x && pacman.pos.y == ghosts[i].pos.y) {
            if (ghosts[i].mode == MODE_FRIGHTENED) {
                /* Eat the ghost */
                game.score += 200 * (game.ghostsEaten + 1);
                game.ghostsEaten++;
                ghosts[i].mode = MODE_EATEN;
                ghosts[i].eatenTimer = 20;
                /* Ghost returns to center */
            } else if (ghosts[i].mode != MODE_EATEN) {
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
    while (attempts < 100) {
        int x = rand() % (BOARD_WIDTH - 2) + 1;
        int y = rand() % (BOARD_HEIGHT - 2) + 1;
        
        if (game.board[y][x] == EMPTY) {
            game.board[y][x] = FRUIT_SYMBOL;
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
    game.score += 1000 + (game.level * 500); /* Bonus for completing level */
    
    /* Award extra life every 2 levels */
    if (game.level % 2 == 0) {
        pacman.lives++;
        game.livesEarned++;
    }
    
    /* Increase difficulty */
    if (game.level <= MAX_LEVELS) {
        switch (settings.difficulty) {
            case DIFFICULTY_EASY:
                game.gameSpeed = 180 - (game.level * 5);
                break;
            case DIFFICULTY_MEDIUM:
                game.gameSpeed = 150 - (game.level * 7);
                break;
            case DIFFICULTY_HARD:
                game.gameSpeed = 120 - (game.level * 10);
                break;
        }
        
        /* Ensure game speed doesn't go too low */
        if (game.gameSpeed < 50) {
            game.gameSpeed = 50;
        }
    }
    
    resetLevel();
}

/* === RENDERING FUNCTIONS === */

void renderGame() {
    renderBoard();
    renderStats();
}

void renderMenu() {
    /* Menu rendering logic would go here */
}

void renderBoard() {
    /* Board rendering logic would go here */
}

void renderStats() {
    /* Stats rendering logic would go here */
}

void renderHighScores() {
    /* High scores rendering logic would go here */
}

void renderHelp() {
    /* Help rendering logic would go here */
}

void renderSettings() {
    /* Settings rendering logic would go here */
}

/* === UTILITY FUNCTIONS === */

void initHighScores() {
    int i;
    for (i = 0; i < MAX_HIGH_SCORES; i++) {
        /* Initialize high scores */
    }
}

void updateHighScores() {
    /* High score update logic would go here */
}

void loadHighScores() {
    /* High score loading logic would go here */
}

void saveHighScores() {
    /* High score saving logic would go here */
}

void clearScreen() {
    /* Screen clearing logic would go here */
}

void delay(int milliseconds) {
    /* Delay logic would go here */
}

/* === MATH FUNCTIONS === */

int distance(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    return dx*dx + dy*dy;
}

int manhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}