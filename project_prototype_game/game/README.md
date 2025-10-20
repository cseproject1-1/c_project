# Pacman Game Collection in C

This repository contains multiple versions of a Pacman game implemented in C, ranging from basic to advanced implementations.

## Game Versions

### 1. Basic Pacman (`pacman.c`)
A simple console-based Pacman game with:
- Basic maze navigation
- Dot collection
- Ghost avoidance
- Score tracking
- WASD controls

### 2. Simplified Pacman (`pacman_simplified.c`)
A more portable version without platform-specific functions:
- Menu system
- Multiple lives
- Power pills
- Fruit collection
- Text-based interface

### 3. Advanced Pacman (`pacman_advanced.c`)
Enhanced version with improved features:
- Color support (Windows)
- Animated characters
- Multiple levels
- High score system
- Improved ghost AI
- Power-up mechanics

### 4. Enhanced Pacman (`pacman_enhanced.c`)
Advanced implementation with sophisticated gameplay:
- 5 unique level designs
- Four distinct ghosts with different behaviors
- Frightened mode for ghosts
- Fruit spawning system
- Difficulty settings
- Comprehensive scoring system

### 5. Ultimate Pacman (`pacman_final.c`)
The most advanced version with complete features:
- Ghost personality system (Blinky, Pinky, Inky, Clyde)
- Scatter and chase modes
- Eaten ghost mechanics
- Progressive difficulty
- Advanced collision detection
- Complete game state management
- Settings and customization

## Features Overview

### Core Gameplay
- Classic Pacman movement and controls
- Dot and power pill collection
- Ghost avoidance and eating mechanics
- Lives and scoring system
- Level progression

### Advanced Mechanics
- Ghost AI with multiple behavior modes
- Power-up and frightened states
- Fruit collection bonuses
- Dynamic difficulty adjustment
- High score tracking

### Visual Elements
- Text-based rendering
- Character animation
- Color support (platform dependent)
- Multiple maze designs
- Game state displays

## How to Compile and Run

Note: Some versions use platform-specific functions and may require specific compilation settings.

```bash
# Basic compilation
gcc -o pacman pacman_final.c

# Run the game
./pacman
```

## Game Controls

- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **Q** - Quit Game
- **Enter** - Confirm selections

## Game Elements

- **C** - Pacman
- **G** - Ghost (normal)
- **X** - Ghost (frightened)
- **.** - Dot (10 points)
- **O** - Power Pill (50 points)
- **F** - Fruit (100 points)

## Development Notes

All versions are designed to demonstrate different aspects of C programming:
- Basic file I/O and data structures
- Game loop implementation
- State machine design
- AI pathfinding algorithms
- Memory management

## Troubleshooting

If you encounter compilation errors related to missing headers:
1. Ensure you have a C compiler installed (GCC, Clang, etc.)
2. Some versions require Windows-specific headers for color and console functions
3. For cross-platform compatibility, use the simplified versions

## Future Enhancements

Possible improvements for future versions:
- Sound effects and music
- Save/load game state
- Network multiplayer
- Graphics library integration
- Mobile platform ports