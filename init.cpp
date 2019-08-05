#include "globals.h"

int side,xside;
int fifty;
int ply,hply;

int nodes;

int board[64];
int color[64];
int kingloc[2];

int history[64][64];

int table_score[2] ;
int square_score[2][6][64];
int king_endgame[2][64];
int pawn_mat[2];
int piece_mat[2];
int passed[2][64];

int qrb_moves[64][9];
int knight_moves[64][9];
int king_moves[64][9];

move move_list[MOVE_STACK];
int first_move[MAX_PLY];

game game_list[GAME_STACK];

char piece_char[6] = 
{
	'P', 'N', 'B', 'R', 'Q', 'K'
};

int piece_value[6] = 
{
	100, 300, 300, 500, 900, 10000
};

int init_color[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1
};

int init_board[64] = 
{
	3, 1, 2, 4, 5, 2, 1, 3,
	0, 0, 0, 0, 0, 0, 0, 0,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6,
	0, 0, 0, 0, 0, 0, 0, 0,
	3, 1, 2, 4, 5, 2, 1, 3
};

const int col[64]=
{
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7,
	0, 1, 2, 3, 4, 5, 6, 7
};

const int row[64]=
{
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7
};

int Flip[64] = 
{
	 56,  57,  58,  59,  60,  61,  62,  63,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 32,  33,  34,  35,  36,  37,  38,  39,
	 24,  25,  26,  27,  28,  29,  30,  31,
	 16,  17,  18,  19,  20,  21,  22,  23,
	  8,   9,  10,  11,  12,  13,  14,  15,
	  0,   1,   2,   3,   4,   5,   6,   7
};

int pawn_score[64] = 
{
	  0,   0,   0,   0,   0,   0,   0,   0,
      0,   2,   4, -12, -12,   4,   2,   0,
      0,   2,   4,   4,   4,   4,   2,   0,
      0,   2,   4,   8,   8,   4,   2,   0,
      0,   2,   4,   8,   8,   4,   2,   0,
      4,   8,  10,  16,  16,  10,   8,   4,
    100, 100, 100, 100, 100, 100, 100, 100,
	  0,   0,   0,   0,   0,   0,   0,   0
};

int knight_score[64] = 
{
	-30, -20, -10,  -8,  -8, -10, -20, -30,
	-16, -6,   -2,   0,   0,   -2, -6, -16,
	-8,   -2,   4,   6,   6,   4,   -2, -8,
	-5,   0,   6,   8,   8,   6,   0, -5,
	-5,   0,   6,   8,   8,   6,   0, -5,
	-10,   -2,   4,   6,   6,   4,   -2, -10,
	-20, -10,   -2,   0,   0,   -2, -10, -20,
	-150, -20, -10, -5, -5, -10, -20, -150
};

int bishop_score[64] = 
{
	-10, -10, -12, -10, -10, -12, -10, -10,
	  0,   4,   4,   4,   4,   4,   4,   0,
	  2,   4,   6,   6,   6,   6,   4,   2,
	  2,   4,   6,   8,   8,   6,   4,   2,
	  2,   4,   6,   8,   8,   6,   4,   2,
	  2,   4,   6,   6,   6,   6,   4,   2,
	-10,   4,   4,   4,   4,   4,   4, -10,
	-10, -10, -10, -10, -10, -10, -10, -10
};

int rook_score[64] = 
{
	4, 4,  4,  6, 6,  4, 4, 4,
	0, 0,  0,  0, 0,  0, 0, 0,
	0, 0,  0,  0, 0,  0, 0, 0,
	0, 0,  0,  0, 0,  0, 0, 0,
	0, 0,  0,  0, 0,  0, 0, 0,
	0, 0,  0,  0, 0,  0, 0, 0,
	20, 20, 20, 20, 20, 20, 20, 20,
	10, 10, 10, 10, 10, 10, 10, 10
};

int queen_score[64] = 
{
    -10, -10,  -6,  -4,  -4,  -6, -10, -10,
	-10,   2,   2,   2,   2,   2,   2, -10,
	  2,   2,   2,   3,   3,   2,   2,   2,
	  2,   2,   3,   4,   4,   3,   2,   2,
	  2,   2,   3,   4,   4,   3,   2,   2,
	  2,   2,   2,   3,   3,   2,   2,   2,
	-10,   2,   2,   2,   2,   2,   2, -10,
	-10, -10,   2,   2,   2,   2, -10, -10
};

int king_score[64] = 
{
     20,  20,  20, -40,  10, -60,  20,  20,     
	 15,  20, -25, -30, -30, -45,  20,  15,   
	-48, -48, -48, -48, -48, -48, -48, -48,
    -48, -48, -48, -48, -48, -48, -48, -48,
    -48, -48, -48, -48, -48, -48, -48, -48,
    -48, -48, -48, -48, -48, -48, -48, -48,
    -48, -48, -48, -48, -48, -48, -48, -48,
    -48, -48, -48, -48, -48, -48, -48, -48
};

int king_endgame_score[64] = 
{
	  0,   8,  16,  18,  18,  16,  8,   0,
	  8,  16,  24,  32,  32,  24,  16,  8,
	 16,  24,  32,  40,  40,  32,  24,  16,
	 25,  32,  40,  48,  48,  40,  32,  25,
	 25,  32,  40,  48,  48,  40,  32,  25,
	 16,  24,  32,  40,  40,  32,  24,  16,
	 8,  16,  24,  32,  32,  24,  16,  8,
	  0,  8,  16,  18,  18,  16,  8,   0
};

int passed_score[64] = 
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	60,  60,  60,  60  ,60, 60, 60, 60,
	30, 30, 30, 30, 30, 30, 30, 30,
	15, 15, 15, 15,15, 15, 15, 15, 
	8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8,
	0, 0, 0, 0, 0, 0, 0, 0
};

int rank[2][64];
/*

SetTables fills the square_score tables, king_endgame tables and passed tables with the individual piece tables.
The value of each piece is added to the score for each square.
The board is flipped for the Black scores.

*/
void SetTables()
{

for(int x=0;x<64;x++)
{
    square_score[0][0][x] = pawn_score[x] + 100;
    square_score[0][1][x] = knight_score[x] + 300;
    square_score[0][2][x] = bishop_score[x] + 300;
	square_score[0][3][x] = rook_score[x] + 500;
    square_score[0][4][x] = queen_score[x] + 900;
    square_score[0][5][x] = king_score[x];

    square_score[1][0][x] = pawn_score[Flip[x]] + 100;
    square_score[1][1][x] = knight_score[Flip[x]] + 300;
    square_score[1][2][x] = bishop_score[Flip[x]] + 300;
	square_score[1][3][x] = rook_score[Flip[x]] + 500;
    square_score[1][4][x] = queen_score[Flip[x]] + 900;
	square_score[1][5][x] = king_score[Flip[x]];

	king_endgame[0][x] = king_endgame_score[x] - square_score[0][5][x];
	king_endgame[1][x] = king_endgame_score[x] - square_score[1][5][x];
	passed[0][x] = passed_score[Flip[x]];
	passed[1][x] = passed_score[x];
}

}
/*

Sets up variables for a new game.

*/
void InitBoard()
{
int x;

for (x = 0; x < 64; ++x) 
{
	color[x] = init_color[x];
	board[x] = init_board[x];
	rank[0][x] = row[x];
	rank[1][x] = 7 - row[x];
}

side = 0;
xside = 1;
fifty = 0;
ply = 0;
hply = 0;
first_move[0] = 0;
kingloc[0] = E1;
kingloc[1] = E8;
	
game_list[hply].castle_q[0] = 1;
game_list[hply].castle_q[1] = 1;
game_list[hply].castle_k[0] = 1;
game_list[hply].castle_k[1] = 1;
}
/*

NewPosition gets the board ready before the computer starts to think.

*/
void NewPosition()
{
	piece_mat[0] = pawn_mat[0] = table_score[0] = 0;
	piece_mat[1] = pawn_mat[1] = table_score[1] = 0;

	for(int i=0;i<64;i++)
	{
		if(board[i] < 6)
		{     
			AddPiece(color[i],board[i],i);
		}
	}
	currentkey = GetKey();
    currentlock = GetLock();
}
/*

Alg displays a move.

*/
void Alg(int a,int b)
{
Algebraic(a);
Algebraic(b);
}
/*

Algebraic displays a square.
e.g. 3 becomes col[3] + 96 which is ascii character 'd' and row[3]+1 which is '1'.
Passing 3 returns 'd1'.

*/
void Algebraic(int a)
{
	if(a<0 || a>63) return; 
	char c[2]="a";
	c[0] =  96+1+col[a];
	printf("%s%d",c,row[a]+1);
}
/*

SetMoves creates the lookup tables for Knights, line-pieces and Kings.
These will later be used to generate moves, captures and attacks.

*/
void SetMoves()
{
	int k=0;
	int y;
	nodes = 1;

for(int x=0;x<64;x++)
{
	k = 0;
	if(row[x]<6 && col[x]<7) 
		knight_moves[x][k++] =  x+17;
	if(row[x]<7 && col[x]<6) 
		knight_moves[x][k++] =  x+10;
	if(row[x]<6 && col[x]>0) 
		knight_moves[x][k++] =  x+15;
	if(row[x]<7 && col[x]>1) 
		knight_moves[x][k++] =  x+6;
	if(row[x]>1 && col[x]<7) 
		knight_moves[x][k++] =  x-15;
	if(row[x]>0 && col[x]<6) 
		knight_moves[x][k++] =  x-6;
	if(row[x]>1 && col[x]>0) 
		knight_moves[x][k++] =  x-17;
	if(row[x]>0 && col[x]>1) 
		knight_moves[x][k++] =  x-10;
	knight_moves[x][k] = -1;
}

for(int x=0;x<64;x++)
{
	k = 0;

	for(int z=0;z<8;z++)
	{
		qrb_moves[x][z] = -1;
	}
		
	if(col[x]>0) qrb_moves[x][WEST]=x-1;
	if(col[x]<7) qrb_moves[x][EAST]=x+1;
	if(row[x]>0) qrb_moves[x][SOUTH]=x-8;
	if(row[x]<7) qrb_moves[x][NORTH]=x+8;
	if(col[x]<7 && row[x]<7) qrb_moves[x][NE]=x+9;
	if(col[x]>0 && row[x]<7) qrb_moves[x][NW]=x+7;
	if(col[x]>0 && row[x]>0) qrb_moves[x][SW]=x-9;
	if(col[x]<7 && row[x]>0) qrb_moves[x][SE]=x-7;

    y=0;
	if(col[x]>0) 
		king_moves[x][y++]=x-1;
	if(col[x]<7) 
		king_moves[x][y++]=x+1;
	if(row[x]>0) 
		king_moves[x][y++]=x-8;
	if(row[x]<7) 
		king_moves[x][y++]=x+8;
	if(col[x]<7 && row[x]<7) 
		king_moves[x][y++]=x+9;
	if(col[x]>0 && row[x]<7) 
		king_moves[x][y++]=x+7;
	if(col[x]>0 && row[x]>0) 
		king_moves[x][y++]=x-9;
	if(col[x]<7 && row[x]>0) 
		king_moves[x][y++]=x-7;
	king_moves[x][y] = -1;
}
nodes = 0;
}
/*dont need below here*/
int done[100];
int GetBest(int ply);//
void ShowAll(int ply)
{

move *g;
DisplayBoard();
memset(done, 0, sizeof(done));

          printf(" ply ");
        printf("%d",ply);
        printf(" nodes ");
        printf("%d",nodes);
        printf(" side ");
        printf("%d",side);
        printf(" xside ");
        printf("%d",xside);
        printf("\n");
  
	 printf(" one %d ",first_move[ply]);
	  printf(" two %d ",first_move[ply+1]);
	  Alg(move_list[first_move[0]].start,move_list[first_move[0]].dest);
			
        printf("\n");
 
     int j;
  for(int i=first_move[ply];i<first_move[ply+1];i++)
  //     for(int i=first_move[ply+1];i<first_move[ply + 2];i++)
    {
        j = GetBest(ply);
        {
        //how dest display current line?
        g = &move_list[j];
        printf("%s",MoveString(move_list[j].start,move_list[j].dest,move_list[j].promote));
        printf(" ");
        printf(" score ");
        printf("%d",g->score);
        printf("\n");
        }
    }
 printf("\n");
 
  _getch();
}

int GetBest(int ply)
{
move *g;
int bestscore = -100000000;
int best = 0;
  for(int i=0;i<first_move[ply+1]-first_move[ply];i++)
  {
    if(done[i] == 1) continue;
    g = &move_list[first_move[ply] + i];
    if(g->start == 0 && g->dest == 0)
      continue;//
    if(g->score > bestscore)
    {
        bestscore= g->score;
        best = i;
    }
  }
  if(best<1000) done[best]=1;//1000?
  return first_move[ply]+best;
}