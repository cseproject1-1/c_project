#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

// Game settings
#define WIDTH 40
#define HEIGHT 20
#define INITIAL_SPEED 200000

// Snake and fruit structures
typedef struct {
    int x, y;
} Point;

typedef struct {
    Point body[WIDTH * HEIGHT];
    int length;
    int direction; // 0: up, 1: right, 2: down, 3: left
} Snake;

typedef struct {
    Point pos;
    bool eaten;
    int just_eaten; // For visual feedback
} Fruit;

// Function prototypes
void setup();
void draw(Snake *snake, Fruit *fruit, int score, int high_score, bool paused);
void input(Snake *snake, bool *paused, bool *gameOver);
void logic(Snake *snake, Fruit *fruit, int *score, bool *gameOver);
void generate_fruit(Fruit *fruit, Snake *snake);
void show_welcome_screen();
void show_game_over_screen(int score, int high_score);
int load_high_score();
void save_high_score(int score);

int main() {
    show_welcome_screen();
    
    int high_score = load_high_score();
    
    while (1) { // Main game loop
        setup();

        Snake snake = {.length = 1, .direction = 1};
        snake.body[0] = (Point){WIDTH / 2, HEIGHT / 2};

        Fruit fruit = {.just_eaten = 0};
        generate_fruit(&fruit, &snake);

        int score = 0;
        bool gameOver = false;
        bool paused = false;
        int speed = INITIAL_SPEED;

        while (!gameOver) {
            draw(&snake, &fruit, score, high_score, paused);
            input(&snake, &paused, &gameOver);
            if (!paused) {
                logic(&snake, &fruit, &score, &gameOver);
            }
            // Increase speed as snake grows (difficulty progression)
            int current_speed = speed - (snake.length * 5000);
            if (current_speed < 50000) current_speed = 50000; // Minimum speed
            usleep(current_speed);
            // Brief pause for visual feedback if fruit was just eaten
            if (fruit.just_eaten) {
                usleep(50000);
            }
        }

        if (score > high_score) {
            high_score = score;
            save_high_score(high_score);
        }
        show_game_over_screen(score, high_score);
        
        int ch;
        do {
            ch = getch();
            if (ch == 'q' || ch == 'Q') {
                endwin();
                return 0;
            }
        } while (ch != 'r' && ch != 'R');
        endwin(); // Clean up ncurses before restart
    }
    
    return 0;
}

void setup() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100); // Set a timeout for getch()
    srand(time(NULL));
}

void draw(Snake *snake, Fruit *fruit, int score, int high_score, bool paused) {
    clear();

    for (int i = 0; i < WIDTH + 2; i++) {
        mvprintw(0, i, "#");
        mvprintw(HEIGHT + 1, i, "#");
    }
    for (int i = 0; i < HEIGHT + 1; i++) {
        mvprintw(i, 0, "#");
        mvprintw(i, WIDTH + 1, "#");
    }

    // Draw snake head and body with different characters
    mvprintw(snake->body[0].y, snake->body[0].x, "@"); // Head
    for (int i = 1; i < snake->length; i++) {
        mvprintw(snake->body[i].y, snake->body[i].x, "o"); // Body
    }

    // Draw fruit with visual feedback when just eaten
    if (fruit->just_eaten) {
        mvprintw(fruit->pos.y, fruit->pos.x, "+"); // Show a + where fruit was eaten
        fruit->just_eaten = 0; // Reset the flag
    } else {
        mvprintw(fruit->pos.y, fruit->pos.x, "*");
    }

    mvprintw(HEIGHT + 2, 2, "Score: %d", score);
    mvprintw(HEIGHT + 2, WIDTH - 15, "High Score: %d", high_score);
    mvprintw(HEIGHT + 3, 2, "Press 'P' to pause/resume");
    
    // Show pause indicator if game is paused
    if (paused) {
        mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "PAUSED");
    }

    refresh();
}

void input(Snake *snake, bool *paused, bool *gameOver) {
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            if (snake->direction != 2) snake->direction = 0;
            break;
        case KEY_RIGHT:
            if (snake->direction != 3) snake->direction = 1;
            break;
        case KEY_DOWN:
            if (snake->direction != 0) snake->direction = 2;
            break;
        case KEY_LEFT:
            if (snake->direction != 1) snake->direction = 3;
            break;
        case 'p':
        case 'P':
            *paused = !(*paused);
            break;
        case 'q':
        case 'Q':
            *gameOver = true;
            break;
    }
}

void logic(Snake *snake, Fruit *fruit, int *score, bool *gameOver) {
    Point new_head = snake->body[0];
    switch (snake->direction) {
        case 0: new_head.y--; break;
        case 1: new_head.x++; break;
        case 2: new_head.y++; break;
        case 3: new_head.x--; break;
    }

    if (new_head.x <= 0 || new_head.x >= WIDTH + 1 || new_head.y <= 0 || new_head.y >= HEIGHT + 1) {
        *gameOver = true;
        return;
    }

    for (int i = 0; i < snake->length; i++) {
        if (new_head.x == snake->body[i].x && new_head.y == snake->body[i].y) {
            *gameOver = true;
            return;
        }
    }

    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = new_head;

    if (new_head.x == fruit->pos.x && new_head.y == fruit->pos.y) {
        snake->length++;
        (*score)++;
        fruit->just_eaten = 1; // Set just eaten flag for visual feedback
        generate_fruit(fruit, snake);
    }
}

void generate_fruit(Fruit *fruit, Snake *snake) {
    bool on_snake;
    do {
        on_snake = false;
        fruit->pos.x = (rand() % WIDTH) + 1;
        fruit->pos.y = (rand() % HEIGHT) + 1;
        fruit->just_eaten = 0; // Reset just eaten flag
        for (int i = 0; i < snake->length; i++) {
            if (fruit->pos.x == snake->body[i].x && fruit->pos.y == snake->body[i].y) {
                on_snake = true;
                break;
            }
        }
    } while (on_snake);
}

void show_welcome_screen() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();
    
    int height, width;
    getmaxyx(stdscr, height, width);
    
    mvprintw(height / 2 - 6, width / 2 - 10, "SNAKE GAME");
    mvprintw(height / 2 - 4, width / 2 - 15, "=========================");
    mvprintw(height / 2 - 2, width / 2 - 12, "Use arrow keys to move");
    mvprintw(height / 2 - 1, width / 2 - 12, "Press 'P' to pause/resume");
    mvprintw(height / 2, width / 2 - 12, "Press 'Q' to quit");
    mvprintw(height / 2 + 1, width / 2 - 12, "Eat fruits (*) to grow");
    mvprintw(height / 2 + 3, width / 2 - 15, "=========================");
    mvprintw(height / 2 + 5, width / 2 - 12, "Press any key to start");
    refresh();
    getch();
    endwin();
}

void show_game_over_screen(int score, int high_score) {
    clear();
    
    int height, width;
    getmaxyx(stdscr, height, width);
    
    mvprintw(height / 2 - 3, width / 2 - 7, "GAME OVER!");
    mvprintw(height / 2 - 1, width / 2 - 8, "Your score: %d", score);
    mvprintw(height / 2, width / 2 - 9, "High score: %d", high_score);
    mvprintw(height / 2 + 2, width / 2 - 16, "Press 'R' to restart or 'Q' to quit.");
    refresh();
}

int load_high_score() {
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) {
        return 0;
    }
    int high_score;
    fscanf(file, "%d", &high_score);
    fclose(file);
    return high_score;
}

void save_high_score(int score) {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", score);
        fclose(file);
    }
}