# Makefile for Pacman Game Collection

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

# Game versions
GAMES = pacman_simplified pacman_advanced pacman_enhanced pacman_final

# Default target
all: $(GAMES)

# Build targets
pacman_simplified: pacman_simplified.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

pacman_advanced: pacman_advanced.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

pacman_enhanced: pacman_enhanced.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

pacman_final: pacman_final.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Clean build files
clean:
	del *.exe *.o 2>nul || rm -f *.exe *.o 2>/dev/null || true

# Run targets
run-simple: pacman_simplified
	./pacman_simplified

run-advanced: pacman_advanced
	./pacman_advanced

run-enhanced: pacman_enhanced
	./pacman_enhanced

run-final: pacman_final
	./pacman_final

# Help target
help:
	@echo "Pacman Game Collection Makefile"
	@echo "=============================="
	@echo "Available targets:"
	@echo "  all          - Build all game versions"
	@echo "  clean        - Remove compiled files"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Individual game targets:"
	@echo "  pacman_simplified - Build simplified version"
	@echo "  pacman_advanced   - Build advanced version"
	@echo "  pacman_enhanced   - Build enhanced version"
	@echo "  pacman_final      - Build ultimate version"
	@echo ""
	@echo "Run targets:"
	@echo "  run-simple   - Build and run simplified version"
	@echo "  run-advanced - Build and run advanced version"
	@echo "  run-enhanced - Build and run enhanced version"
	@echo "  run-final    - Build and run ultimate version"

.PHONY: all clean help run-simple run-advanced run-enhanced run-final