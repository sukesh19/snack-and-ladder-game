#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <iomanip>
#include <string>
#include <cstdlib>

class SnakesAndLadders {
private:
    std::map<int, int> snakes;    // head -> tail
    std::map<int, int> ladders;   // bottom -> top
    std::vector<int> players;     // player positions
    std::vector<std::string> playerNames;
    int numPlayers;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dice;

public:
    SnakesAndLadders(int num_players) : numPlayers(num_players), rng(std::random_device{}()), dice(1, 6) {
        // Initialize players at position 0 (before start)
        players.resize(numPlayers, 0);
        playerNames.resize(numPlayers);
        
        // Get player names
        for (int i = 0; i < numPlayers; i++) {
            std::cout << "Enter name for Player " << (i + 1) << ": ";
            std::cin >> playerNames[i];
        }
        
        // Initialize snakes (head -> tail)
        snakes = {
            {16, 6}, {47, 26}, {49, 11}, {56, 53}, {62, 19}, 
            {64, 60}, {87, 24}, {93, 73}, {95, 75}, {98, 78}
        };
        
        // Initialize ladders (bottom -> top)
        ladders = {
            {1, 38}, {4, 14}, {9, 21}, {21, 42}, {28, 84}, 
            {36, 44}, {51, 67}, {71, 91}, {80, 100}
        };
    }
    
    int rollDice() {
        return dice(rng);
    }
    
    void displayBoard() {
        std::cout << "\n=== SNAKES AND LADDERS BOARD ===\n\n";
        
        // Display board from top to bottom (100 to 1)
        for (int row = 9; row >= 0; row--) {
            // Print row numbers
            for (int col = 0; col < 10; col++) {
                int square;
                if (row % 2 == 1) { // Odd rows go right to left
                    square = row * 10 + (10 - col);
                } else { // Even rows go left to right
                    square = row * 10 + col + 1;
                }
                
                // Check if any player is on this square
                std::string playerMarker = "";
                for (int i = 0; i < numPlayers; i++) {
                    if (players[i] == square) {
                        playerMarker += "P" + std::to_string(i + 1);
                    }
                }
                
                if (playerMarker.empty()) {
                    std::cout << std::setw(8) << square;
                } else {
                    std::cout << std::setw(8) << (std::to_string(square) + "(" + playerMarker + ")");
                }
            }
            std::cout << "\n\n";
        }
        
        // Display snakes and ladders legend
        std::cout << "\n=== SNAKES (Head -> Tail) ===\n";
        for (auto& snake : snakes) {
            std::cout << snake.first << " -> " << snake.second << "  ";
        }
        
        std::cout << "\n\n=== LADDERS (Bottom -> Top) ===\n";
        for (auto& ladder : ladders) {
            std::cout << ladder.first << " -> " << ladder.second << "  ";
        }
        std::cout << "\n\n";
    }
    
    void displayPlayerPositions() {
        std::cout << "=== PLAYER POSITIONS ===\n";
        for (int i = 0; i < numPlayers; i++) {
            std::cout << playerNames[i] << " (P" << (i + 1) << "): Position " << players[i] << "\n";
        }
        std::cout << "\n";
    }
    
    bool movePlayer(int playerIndex, int diceRoll) {
        int oldPosition = players[playerIndex];
        int newPosition = oldPosition + diceRoll;
        
        // Check if player would go beyond 100
        if (newPosition > 100) {
            std::cout << playerNames[playerIndex] << " rolled " << diceRoll 
                      << " but cannot move beyond 100. Staying at position " << oldPosition << "\n";
            return false;
        }
        
        players[playerIndex] = newPosition;
        std::cout << playerNames[playerIndex] << " moved from " << oldPosition 
                  << " to " << newPosition << "\n";
        
        // Check for ladders
        if (ladders.find(newPosition) != ladders.end()) {
            int ladderTop = ladders[newPosition];
            players[playerIndex] = ladderTop;
            std::cout << "🪜 LADDER! " << playerNames[playerIndex] 
                      << " climbs from " << newPosition << " to " << ladderTop << "!\n";
            newPosition = ladderTop;
        }
        
        // Check for snakes
        if (snakes.find(newPosition) != snakes.end()) {
            int snakeTail = snakes[newPosition];
            players[playerIndex] = snakeTail;
            std::cout << "🐍 SNAKE! " << playerNames[playerIndex] 
                      << " slides down from " << newPosition << " to " << snakeTail << "!\n";
        }
        
        // Check for win condition
        if (players[playerIndex] == 100) {
            return true; // Player wins
        }
        
        return false;
    }
    
    void playGame() {
        std::cout << "\n🎮 WELCOME TO SNAKES AND LADDERS! 🎮\n";
        std::cout << "Players: ";
        for (int i = 0; i < numPlayers; i++) {
            std::cout << playerNames[i] << " ";
        }
        std::cout << "\n";
        
        int currentPlayer = 0;
        bool gameWon = false;
        
        while (!gameWon) {
            displayBoard();
            displayPlayerPositions();
            
            std::cout << "=== " << playerNames[currentPlayer] << "'s turn ===\n";
            std::cout << "Press Enter to roll the dice...";
            std::cin.ignore();
            std::cin.get();
            
            int diceRoll = rollDice();
            std::cout << playerNames[currentPlayer] << " rolled: " << diceRoll << "\n";
            
            gameWon = movePlayer(currentPlayer, diceRoll);
            
            if (gameWon) {
                displayBoard();
                std::cout << "🏆 CONGRATULATIONS! " << playerNames[currentPlayer] 
                          << " WINS THE GAME! 🏆\n";
                break;
            }
            
            // Move to next player (unless rolled a 6)
            if (diceRoll != 6) {
                currentPlayer = (currentPlayer + 1) % numPlayers;
            } else {
                std::cout << "🎲 " << playerNames[currentPlayer] 
                          << " rolled a 6 and gets another turn!\n";
            }
            
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            
            // Clear screen (works on most systems)
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }
    }
};

int main() {
    std::cout << "🐍🪜 SNAKES AND LADDERS GAME 🪜🐍\n";
    std::cout << "==================================\n";
    
    int numPlayers;
    do {
        std::cout << "Enter number of players (2-4): ";
        std::cin >> numPlayers;
        if (numPlayers < 2 || numPlayers > 4) {
            std::cout << "Please enter a number between 2 and 4.\n";
        }
    } while (numPlayers < 2 || numPlayers > 4);
    
    SnakesAndLadders game(numPlayers);
    
    std::cout << "\n=== GAME RULES ===\n";
    std::cout << "1. Roll dice to move your piece\n";
    std::cout << "2. Land on ladder bottom to climb up\n";
    std::cout << "3. Land on snake head to slide down\n";
    std::cout << "4. Roll 6 to get an extra turn\n";
    std::cout << "5. First to reach 100 wins!\n";
    std::cout << "6. Must roll exact number to reach 100\n\n";
    
    game.playGame();
    
    std::cout << "\nThank you for playing! 🎮\n";
    return 0;
}
