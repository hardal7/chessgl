#include <iostream>
#include <ostream>
char move1[] = "Rad1";
char move[] = "h3";

unsigned int piecePosition = 0;
unsigned int newPosition = 0;

int main() {
  if (sizeof(move) == 3) {
    std::cout << "Single pawn move!" << std::endl;
    move[0] == 'a'   ? piecePosition += 0
    : move[0] == 'b' ? piecePosition += 1
    : move[0] == 'c' ? piecePosition += 2
    : move[0] == 'd' ? piecePosition += 3
    : move[0] == 'e' ? piecePosition += 4
    : move[0] == 'f' ? piecePosition += 5
    : move[0] == 'g' ? piecePosition += 6
    : move[0] == 'h' ? piecePosition += 7
                     : piecePosition = 64;
    piecePosition += (8-((move[1] - '0') - 1))*8;
    std::cout << piecePosition;
  }
  return 0;
}
