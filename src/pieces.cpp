int main() {

  enum PieceType {
    Empty = 0,
    Pawn = 1,
    Knight = 2,
    Bishop = 3,
    Rook = 4,
    Queen = 5,
    King = 6,

    White = 0,
    Black = 8,
  };

  int Square[64] = {Empty};

  Square[0] = Rook | White;
  Square[1] = Knight | White;
  Square[2] = Bishop | White;
  Square[3] = King | White;
  Square[4] = Queen | White;
  Square[5] = Bishop | White;
  Square[6] = Knight | White;
  Square[7] = Rook | White;

  Square[8] = Pawn | White;
  Square[9] = Pawn | White;
  Square[10] = Pawn | White;
  Square[11] = Pawn | White;
  Square[12] = Pawn | White;
  Square[13] = Pawn | White;
  Square[14] = Pawn | White;
  Square[15] = Pawn | White;

  Square[48] = Rook | Black;
  Square[49] = Knight | Black;
  Square[50] = Bishop | Black;
  Square[51] = King | Black;
  Square[52] = Queen | Black;
  Square[53] = Bishop | Black;
  Square[54] = Knight | Black;
  Square[55] = Rook | Black;

  Square[56] = Pawn | Black;
  Square[57] = Pawn | Black;
  Square[58] = Pawn | Black;
  Square[59] = Pawn | Black;
  Square[60] = Pawn | Black;
  Square[61] = Pawn | Black;
  Square[62] = Pawn | Black;
  Square[63] = Pawn | Black;
}
