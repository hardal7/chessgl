#include <iostream>
int main() {
  std::string input;
  std::string move[2] = {"b8a6", "a6b8"};
  unsigned int i = 0;
  while (true) {
    getline(std::cin, input);
    if (input == "quit") {
      return 0;
    } else if (input == "uci") {
      std::cout << "id name fehmi" << std::endl;
      std::cout << "id author hardal" << std::endl;
      std::cout << "uciok" << std::endl;
    } else if (input == "isready") {
      std::cout << "readyok" << std::endl;
    } else if (input.find("position startpos") != std::string::npos) {
      // If no move is given then the engine is white
      std::cout << "bestmove " << move[i] << std::endl;
      i == 0 ? i++ : i--;
    }
  }
}
