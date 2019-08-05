#include "globals.h"

#define ISOLATED 20

int EvalPawn(const int x);
int EvalRook(const int s,const int x);
bool Pawns(const int s,const int file);
bool Pawns2(const int s,const int xs,const int start);

int queenside_pawns[2],kingside_pawns[2];

const int queenside_defence[2][64]=
{
{
	0, 0, 0, 0, 0, 0, 0, 0,
	8,10, 8, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
},
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 0, 0, 0,
	8,10, 8, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
}};

const int kingside_defence[2][64]=
{
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8,10, 8,
	0, 0, 0, 0, 0, 8, 6, 8,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 8, 8, 8,
	0, 0, 0, 0, 0, 0, 0, 0
},
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8, 6, 8,
	0, 0, 0, 0, 0, 8,10, 8,
	0, 0, 0, 0, 0, 0, 0, 0
}};
/*

Eval() is simple. Firstly it fetches the table scores which were updated whenever moves were
updated. 

It then looks up the pawn hash table. If the pawn position is not
already stored it evaluates the pawns and adds the scores to the pawn hash table.
It adds the pawn scores for each side.

It then adds a score for the king position depending on how much material the opponent has.
It adds a bonus for a pawn or piece in front of the King. (A very simple King defence score).

It turn returns the side to moves score minus the opponent's score.

There are plenty of things that could be added to the eval function.

*/
int Eval()
{
int score[2] = {0,0};
int queens[2] = {0,0};

queenside_pawns[0] = 0;
queenside_pawns[1] = 0;
kingside_pawns[0] = 0;
kingside_pawns[1] = 0;

for(int x = 0;x < 64;x++)
{
	if(color[x] != EMPTY)
	{
		score[color[x]] += square_score[color[x]][board[x]][x];
		if(board[x] == P)
		{
			score[color[x]] += EvalPawn(x);
		}
		else if(board[x] == R)
		{
			score[color[x]] += EvalRook(color[x],x);
		}
		else if(board[x] == Q)
		{
			queens[color[x]] = 1;
		}
	}
}
  if(queens[1]==0)
    score[0] += king_endgame[0][kingloc[0]];
  else
  {
	if(col[kingloc[0]]>3)
		score[0] += kingside_pawns[0];
	else
		score[0] += queenside_pawns[0];
  }

  if(queens[0]==0)
    score[1] += king_endgame[1][kingloc[1]];
   else
  {
	if(col[kingloc[0]]>3)
		score[1] += kingside_pawns[0];
	else
		score[1] += queenside_pawns[0];
  }

  return score[side] - score[xside];
}
/*

EvalPawn() evaluates each pawn and gives a bonus for passed pawns
and a minus for isolated pawns.

*/
int EvalPawn(const int x)
{
int score = 0;
int s = color[x];
int xs = OtherSide[s];

if(!Pawns2(s,xs,x))
{
	score += passed[s][x];
}
if( (col[x]==0 || !Pawns(s,col[x]-1)) && (col[x]==7 || !Pawns(s,col[x]+1)) )
	score -= ISOLATED;
kingside_pawns[s] += kingside_defence[s][x];
queenside_pawns[s] += queenside_defence[s][x];

return score;
}
/*

EvalRook() evaluates each rook and gives a bonus for being
on an open file or half-open file.
*/
int EvalRook(const int s,const int x)
{
int score = 0;
if(!Pawns(s,col[x]))
{
	score = 10;
	if(!Pawns(OtherSide[s],col[x]))
		score += 10;
}
return score;
}
/*

Pawns() searches for pawns on a file.
It is used to detect isolated pawns.

*/
bool Pawns(const int s,const int file)
{
for(int x = file + 8;x < A8; x += 8)
{
	if(board[x]==P && color[x]==s)
		return true;
}
return false;
}
/*

Pawns2() searches for pawns on a file beyond a square.
It is used to detect passed pawns.

*/
bool Pawns2(const int s,const int xs,const int start)
{
int x = start + ForwardSquare[s];
while(x>H2 && x<A7)
{
	if(board[x]==P && color[x]==xs)
		return true;
	if(col[x]>0 && board[x-1]==P && color[x-1]==xs)
		return true;
	if(col[x]<7 && board[x+1]==P && color[x+1]==xs)
		return true;
	x += ForwardSquare[s];
}
return false;
}