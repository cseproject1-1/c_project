#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Use an enum for better readability of game states
typedef enum {
    STATE_EXIT,    // 0
    ROOM_MAIN,     // 1
    ROOM_LIBRARY,  // 2
    ROOM_HALLWAY,  // 3
    ROOM_SECRET    // 4
} GameState;

// A struct to hold the player's status
typedef struct {
    int hasKey;
    int hasGem;
} Player;

// Function Prototypes
void showIntroduction();
GameState handleMainRoom(Player *player);
GameState handleLibrary(Player *player);
GameState handleHallway(Player *player);
GameState handleSecretRoom(Player *player);
int getUserChoice();

int main() {
    Player player = {0, 0}; // Initialize player with no items
    GameState currentState = ROOM_MAIN;

    showIntroduction();

    while (currentState != STATE_EXIT) {
        switch (currentState) {
            case ROOM_MAIN:
                currentState = handleMainRoom(&player);
                break;
            case ROOM_LIBRARY:
                currentState = handleLibrary(&player);
                break;
            case ROOM_HALLWAY:
                currentState = handleHallway(&player);
                break;
            case ROOM_SECRET:
                currentState = handleSecretRoom(&player);
                break;
            default:
                printf("Error: Unknown game state!\n");
                currentState = STATE_EXIT;
                break;
        }
    }

    printf("\nThanks for playing!\n");
    return 0;
}

void showIntroduction() {
    printf("Welcome to the Grand Adventure!\n");
    printf("Your quest is to find the treasure and escape the castle.\n");
    printf("--------------------------------------------------------\n");
}

GameState handleMainRoom(Player *player) {
    printf("\nYou are in the main hall. It's large and cold.\n");
    printf("1. Go to the library.\n");
    printf("2. Go to the long hallway.\n");
    int choice = getUserChoice();
    if (choice == 1) return ROOM_LIBRARY;
    if (choice == 2) return ROOM_HALLWAY;
    printf("Invalid choice. Try again.\n");
    return ROOM_MAIN;
}

GameState handleLibrary(Player *player) {
    printf("\nYou are in the library. A large book rests on a stand.\n");
    if (player->hasKey == 0) {
        printf("1. Read the book.\n");
    }
    printf("2. Look for a secret passage.\n");
    printf("3. Return to the main hall.\n");
    int choice = getUserChoice();
    if (choice == 1 && player->hasKey == 0) {
        printf("You open the book and a rusty key falls out! You take it.\n");
        player->hasKey = 1;
    } else if (choice == 2) {
        if (player->hasKey == 1) {
            printf("You use the key on a hidden lock behind a bookshelf! A secret door opens!\n");
            return ROOM_SECRET;
        } else {
            printf("You pull on a book and hear a click, but nothing happens.\n");
        }
    } else if (choice == 3) {
        return ROOM_MAIN;
    } else {
        printf("Invalid choice. Try again.\n");
    }
    return ROOM_LIBRARY;
}

GameState handleSecretRoom(Player *player) {
    printf("\nYou've found a secret, dusty room! In the corner sits a small chest.\n");
    if (player->hasGem == 0) {
        printf("You open the chest and find a beautiful, glowing gem!\n");
        player->hasGem = 1;
    }
    printf("There is nothing else here. You return to the library.\n");
    return ROOM_LIBRARY;
}

GameState handleHallway(Player *player) {
    printf("\nYou are in a long, dark hallway. A large treasure chest is at the end.\n");
    if (player->hasGem == 1) {
        printf("You place the glowing gem into a slot on the chest. It opens!\n");
        printf("Inside is more gold than you can carry!\n");
        printf("*********************************\n");
        printf("** YOU WIN! Congratulations! **\n");
        printf("*********************************\n");
    } else {
        printf("The chest is sealed by magic. You cannot open it.\n");
        printf("A cold wind blows through the hall, and the door slams shut behind you.\n");
        printf("You are trapped forever. GAME OVER.\n");
    }
    return STATE_EXIT;
}

int getUserChoice() {
    char buffer[10];
    printf("Your choice: ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        return atoi(buffer);
    }
    return -1; // Return an invalid choice on error
}