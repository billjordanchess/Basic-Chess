#include "globals.h"

move *g;

int px[6] = {0,10,20,30,40,0};
int nx[6] = {-3,7,17,27,37,0};
int bx[6] = {-3,7,17,27,37,0};
int rx[6] = {-5,5,15,25,35,0};
int qx[6] = {-9,1,11,21,31,0};
int kx[6] = {0,10,20,30,40,0};

int ForwardSquare[2] = {8,-8};
int Double[2] = {16,-16};
int Left[2] = {7,-9};
int Right[2] = {9,-7};
int OtherSide[2] = {1,0};

int mc;

/*

Gen sees if an en passant capture or castling is possible.
It then loops through the board searching for pieces of one 
side and generates
moves for them.

*/
void Gen()
{
mc = first_move[ply];

GenEp();

GenCastle();
	
for(int x = 0;x < 64;x++)
{
	if(color[x] == side)
	{
		switch(board[x])
		{
		case P:
			GenPawn(x);
			break;
		case N:
			GenKnight(x);
			break;
		case B:
			GenBishop(x,NE);
			GenBishop(x,SE);
			GenBishop(x,SW);
			GenBishop(x,NW);
			break;
		case R:
			GenRook(x,NORTH);
			GenRook(x,EAST);
			GenRook(x,SOUTH);
			GenRook(x,WEST);
			break;
		case Q:
			GenQueen(x,NE);
			GenQueen(x,SE);
			GenQueen(x,SW);
			GenQueen(x,NW);
			GenQueen(x,NORTH);
			GenQueen(x,EAST);
			GenQueen(x,SOUTH);
			GenQueen(x,WEST);
			break;
		case K:
			GenKing(x);
			break;
		default:
			break;
		}
	}
}
first_move[ply + 1] = mc;
}
/*

GenEp looks at the last move played and sees if it is a double pawn move.
If so, it sees if there is an opponent pawn next to it.
If there is, it adds the en passant capture to the move list.
Note that sometimes two en passant captures may be possible.

*/
void GenEp()
{
int ep = game_list[hply - 1].dest;
	
if(board[ep] == 0 && abs(game_list[hply - 1].start - ep) == 16)
{
	if(col[ep] > 0 && color[ep-1]==side && board[ep-1]==P)
	{
		AddCapture(ep-1,ep+ForwardSquare[side],10);				
	}
	if(col[ep] < 7 && color[ep+1]==side && board[ep+1]==P)
	{
		AddCapture(ep+1,ep+ForwardSquare[side],10);
	}
}
}
/*

GenCastle generates a castling move if the King and Rook haven't moved and
there are no pieces in the way. Attacked squares are looked at in MakeMove. 

*/
void GenCastle()
{
if(side==0)
{
if(game_list[hply].castle_k[side])
{
	if(board[F1] == EMPTY && board[G1] == EMPTY)
	{
		AddMove(E1,G1);
	}
}
if(game_list[hply].castle_q[side])
{
	if(board[B1] == EMPTY && board[C1] == EMPTY && board[D1] == EMPTY)
	{
		AddMove(E1,C1);
	}
}
}
else
{

if(game_list[hply].castle_k[side])
{
	if(board[F8] == EMPTY && board[G8] == EMPTY)
	{
		AddMove(E8,G8);
	}
}
if(game_list[hply].castle_q[side])
{
	if(board[B8] == EMPTY && board[C8] == EMPTY && board[D8] == EMPTY)
	{
		AddMove(E8,C8);
	}
}
}
}
/*

GenPawn generates single and double pawn moves and pawn 
captures for a pawn. 

*/
void GenPawn(const int x)
{
	if(board[x+ForwardSquare[side]] == EMPTY)
	{
		AddMove(x,x + ForwardSquare[side]);
		if(rank[side][x]==1 && board[x + Double[side]] == EMPTY)
		{
			AddMove(x,x + Double[side]);
		}
	}
	if(col[x] > 0 && color[x + Left[side]] == OtherSide[side])
	{
		AddCapture(x,x + Left[side],px[board[x + Left[side]]]);
	}
	if(col[x] < 7 && color[x + Right[side]] == OtherSide[side])
	{
		AddCapture(x,x + Right[side],px[board[x + Right[side]]]);
	}
}
/*

GenKnight generates knight moves and captures by using the 
knight_moves look up
table created in init.cpp. 

*/
void GenKnight(const int sq)
{
int k = 0;
int sq2 = knight_moves[sq][k++];
while(sq2 > -1)
{
	if(color[sq2] == EMPTY)
		AddMove(sq,sq2);
	else if(color[sq2] == xside)
		AddCapture(sq,sq2,nx[board[sq2]]);
	sq2 = knight_moves[sq][k++];
}
}
/*

GenBishop generates bishop moves and 
captures for each diagonal. 

*/
void GenBishop(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,bx[board[sq]]);
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}
/*

GenRook generates straight moves and captures 
for each rank and file. 

*/
void GenRook(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,rx[board[sq]]);
		}
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}
/*

GenQueen generates queen moves and captures 
for line. 

*/
void GenQueen(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,qx[board[sq]]);
		}
		break;
	}
	AddMove(x,sq);
	sq = qrb_moves[sq][dir];
}

}
/*

GenKing generates king moves and captures by using the 
king_moves look up table created in init.cpp. 

*/
void GenKing(const int x)
{
int k = 0;
int sq = king_moves[x][k++];

while(sq > -1)
{
	if(color[sq] == EMPTY)
		AddMove(x,sq);
	else if(color[sq] == xside)
		AddCapture(x,sq,kx[board[sq]]);
	sq = king_moves[x][k++];
}

}
/*

AddMove adds the start and dest squares of a move to the movelist. 
The score is the history value.

*/
void AddMove(const int x,const int sq)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = history[x][sq];
	mc++;
}
/*

AddCapture adds the start and dest squares of a move to the 
movelist. 
CAPTURE_SCORE is added to the score so that captures will be 
looked at first.
The score is also added and will be used in move ordering.

*/
void AddCapture(const int x,const int sq,const int score)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = score + CAPTURE_SCORE;
	mc++;
}
/*

GenCaptures is very similar to Gen, except that only captures 
are being generated instead of all moves.

*/
void GenCaptures()
{
first_move[ply + 1] = first_move[ply];
mc = first_move[ply];//

for(int x = 0;x < 64;x++)
{
	if(color[x] == side)
	{
		switch(board[x])
		{
		case P:
			CapPawn(x);
			break;
		case N:
			CapKnight(x);
			break;
		case B:
			CapBishop(x,NE);
			CapBishop(x,SE);
			CapBishop(x,SW);
			CapBishop(x,NW);
			break;
		case R:
			CapRook(x,EAST);
			CapRook(x,SOUTH);
			CapRook(x,WEST);
			CapRook(x,NORTH);
			break;
		case Q:
			CapQueen(x,NE);
			CapQueen(x,SE);
			CapQueen(x,SW);
			CapQueen(x,NW);
			CapQueen(x,EAST);
			CapQueen(x,SOUTH);
			CapQueen(x,WEST);
			CapQueen(x,NORTH);
			break;
		case K:
			CapKing(x);
			break;
		default:
			break;
		}
	}
}
first_move[ply + 1] = mc;
}
/*

CapPawn generates pawn captures.

*/
void CapPawn(const int x)
{	

if(col[x] > 0 && color[x + Left[side]] == OtherSide[side])
{
	AddCapture(x,x + Left[side],px[board[x + Left[side]]]);
}
if(col[x] < 7 && color[x + Right[side]] == OtherSide[side])
{
	AddCapture(x,x + Right[side],px[board[x + Right[side]]]);
}

}
/*

CapKnight generates knight captures.

*/
void CapKnight(const int x)
{
int k = 0;
int sq = knight_moves[x][k++];
while(sq > -1)
{
	if(color[sq] == xside)
		AddCapture(x,sq,nx[board[sq]]);
	sq = knight_moves[x][k++];
}
}
/*

CapBishop generates bishop captures.

*/
void CapBishop(const int x,const int dir)
{
		
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,bx[board[sq]]);
		break;
	}
	sq = qrb_moves[sq][dir];
}

}
/*

CapRook generates rook captures.

*/
void CapRook(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,rx[board[sq]]);
		}
		break;
	}
	sq = qrb_moves[sq][dir];
}

}
/*

CapQueen generates queen captures.

*/
void CapQueen(const int x,const int dir)
{
int sq = qrb_moves[x][dir];
while(sq > -1)
{
	if(color[sq] != EMPTY)
	{
		if(color[sq] == xside)
		{
			AddCapture(x,sq,qx[board[sq]]);
		}
		break;
	}
	sq = qrb_moves[sq][dir];
}

}
/*

CapKing generates king captures.

*/
void CapKing(const int x)
{
	int k = 0;
	int sq = king_moves[x][k++];

	while(sq > -1)
	{
		if(color[sq] == xside)
			AddCapture(x,sq,kx[board[sq]]);
		sq = king_moves[x][k++];
	}
}
