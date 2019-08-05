#include "stdafx.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
//
#include <windows.h>
//

#include <iostream>

#include <sys/timeb.h>

#include "globals.h"

void ShowHelp();
void SetUp();
void xboard();

int board_color[64] = 
{
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1
};

int LoadDiagram(char* file,int);
void CloseDiagram();

FILE *diagram_file;
char fen_name[256];

int flip = 0;

int computer_side;
int player[2];

int fixed_time;
int fixed_depth;
int max_time;
int start_time;
int stop_time;
int max_depth;
int turn = 0;

void PrintResult();
void NewGame();
void SetMaterial();

extern int move_start,move_dest;

int main()
{
printf("\n");
printf("Bills Basic Chess Engine\n");
printf("Version 1.0, 5/5/19\n");
printf("Copyright 2019 Bill Jordan \n");
printf("\n");
printf("\"help\" displays a list of commands.\n");
printf("\n");

char s[256];
char sFen[256];
char sText[256];

int m;
int turns=0;
int t;
int lookup;

double nps;

fixed_time = 0;

SetUp();

while(true)
{
if (side == computer_side) 
{  
	player[side] = 1;
	think();
	turns++;
	
	currentkey = GetKey();
	currentlock = GetLock();
	lookup = LookUp(side);
	/*
	if(lookup != 0)
	{
	   printf("\n lookup %d ",lookup);
	   Alg(hash_start,hash_dest);printf("\n");
	}
	else
	{	
		move_start = move_start;//
		move_dest = move_dest;//
		printf("(no legal moves)\n");
		computer_side = EMPTY;
		DisplayBoard();
		Gen();
		continue;
	} 
	*/
	if(move_start != 0 || move_dest != 0)
	{
	   hash_start = move_start;
	   hash_dest = move_dest;
	   Alg(hash_start,hash_dest);printf("\n");
	}
	else
	{	
		printf("(no legal moves)\n");
		computer_side = EMPTY;
		DisplayBoard();
		Gen();
		continue;
	} 

	printf("\n hash %d ",hashpositions[0]);
	printf(" hash %d ",hashpositions[1]);
	printf(" collisions %d ",collisions);
	printf("\n");
	collisions = 0;
	
	printf("Computer's move: %s\n", MoveString(hash_start,hash_dest,0));printf("\n");
	MakeMove(hash_start,hash_dest);

	SetMaterial();
	
	t = GetTime() - start_time;
	printf("\nTime: %d ms\n", t);
    if(t>0)
      nps = (double)nodes / (double)t;
    else
      nps=0;
	nps *= 1000.0;

	printf("\nNodes per second: %d\n", (int)nps);
	ply = 0;

	first_move[0] = 0;
	Gen();
	PrintResult();		
    printf(" turn "); printf("%d",turn++);
	DisplayBoard();
	continue;
}
	printf("Enter move or command> ");
	if (scanf("%s", s) == EOF)
	return 0;

	if (!strcmp(s, "d")) 
	{
		DisplayBoard();
		continue;
	}
	if (!strcmp(s, "f")) 
	{
		flip = 1 - flip;
		DisplayBoard();
		continue;
	}
	if (!strcmp(s, "go"))
	{
	computer_side = side;
	continue;
	}
	if (!strcmp(s, "help")) 
	{
		ShowHelp();
		continue;
	}
	if (!strcmp(s, "moves")) 
	{     
		printf("Moves \n");
		move *g;
		for (int i = 0; i < first_move[1]; ++i)
		{
			g = &move_list[i];
			printf("%s",MoveString(move_list[i].start,move_list[i].dest,move_list[i].promote));
			printf("\n");
		}
		continue;
	}
	if (!strcmp(s, "new")) 
	{
		NewGame();
		computer_side = EMPTY;
		continue;
	}
	if (!strcmp(s, "on") || !strcmp(s, "p")) 
	{
		computer_side = side;
		continue;
	}
	if (!strcmp(s, "off")) 
	{
		computer_side = EMPTY;
		continue;
	}
	if (!strcmp(s, "quit"))
	{
		printf("Program exiting\n");
		break;
	}
	if (!strcmp(s, "sb")) 
	{
		sFen[0] = 0;
		strcat_s(sFen,"c:\\bscp\\");//
		scanf("%s", sText);
		strcat_s(sFen,sText);
		strcat_s(sFen,".fen");
		LoadDiagram(sFen,1);
		continue;
	}
	if (!strcmp(s, "sd")) 
	{
		scanf("%d", &max_depth);
		max_time = 1 << 25;
		fixed_depth = 1;
		continue;
	}
	if (!strcmp(s, "st")) 
	{
		scanf("%d", &max_time);
		max_time *= 1000;
		max_depth = MAX_PLY;
		fixed_time = 1;
		continue;
	}
	if (!strcmp(s, "sw")) 
	{
		side = 1-side;
		xside = 1-xside;
		continue;
	}
	if (!strcmp(s, "undo")) 
	{
		if (!hply)
			continue;
		computer_side = EMPTY;
		TakeBack();
		ply = 0;
		if(first_move[0] != 0)
			first_move[0] = 0;
		Gen();
		continue;
	}
	if (!strcmp(s, "xboard")) 
	{
		xboard();
		break;
	}       

		ply = 0;
        first_move[0] = 0;
		Gen();
 		m = ParseMove(s);
		if (m == -1 || !MakeMove(move_list[m].start,move_list[m].dest))
		{
			printf("Illegal move. \n");
			printf(s);printf(" \n");
			MoveString(move_list[m].start,move_list[m].dest,move_list[m].promote);
			if (m == -1)printf(" m = -1 \n");
		}
		if(game_list[hply].promote >0 && (row[move_list[m].dest]==0 || row[move_list[m].dest]==7))
		{
			RemovePiece(xside,Q,move_list[m].dest);
			if(s[4]=='n' || s[4]=='N')
				AddPiece(xside,N,move_list[m].dest);
			else if(s[4]=='b' || s[4]=='B')
				AddPiece(xside,B,move_list[m].dest);
			else if(s[4]=='r' || s[4]=='r')
				AddPiece(xside,R,move_list[m].dest); 
			else AddPiece(xside,Q,move_list[m].dest);
		}
	}
    Free();
	return 0;
}

int ParseMove(char *s)
{
	int start, dest, i;

	if (s[0] < 'a' || s[0] > 'h' ||
			s[1] < '0' || s[1] > '9' ||
			s[2] < 'a' || s[2] > 'h' ||
			s[3] < '0' || s[3] > '9')
		return -1;

    start = s[0] - 'a';
    start += ((s[1] - '0') - 1)*8;
    dest = s[2] - 'a';
    dest += ((s[3] - '0') - 1)*8;

	for (i = 0; i < first_move[1]; ++i)
		if (move_list[i].start == start && move_list[i].dest == dest)
        {
			return i;
	    }
	return -1;
}
/* DisplayBoard() displays the board */
void DisplayBoard()
{
	//
	HANDLE hConsole;
hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int text = 15;

	int flip = 0;
	int i;
	int x=0;

	printf("\n8 ");
    if(flip==0)
    {
	for (int j = 0; j < 64; ++j)
    {
        i = Flip[j];
		        {
    	switch (color[i])
        {
			case EMPTY:
			if(board_color[i]==0)
				text = 127;
			else
				text = 34;
			SetConsoleTextAttribute(hConsole, text);

				printf("  ");
				SetConsoleTextAttribute(hConsole, 15);
				break;
			case 0:
				if(board_color[i]==0)
					text = 126;
				else
					text = 46;
				SetConsoleTextAttribute(hConsole, text);
				printf(" %c", piece_char[board[i]]);
			SetConsoleTextAttribute(hConsole, 15);
				break;

			case 1:
				if(board_color[i]==0)
					text = 112;
				else
					text = 32;
				SetConsoleTextAttribute(hConsole, text);
				printf(" %c", piece_char[board[i]] + ('a' - 'A'));
				SetConsoleTextAttribute(hConsole, 15);
				break;

			default:
				printf(" %d.",color[i]);
				break;
				
		}
		if((color[i]==0 || color[i]==1) && board[i]==6)
		if(x==0)
			printf(" %d",color[i]);
        else
            printf("%d ",color[i]);
		if(board[i]<0 || board[i]>6)
		if(x==0)
			printf(" %d.",board[i]);
        else
            printf("%d ",board[i]);
        }
		if ((j + 1) % 8 == 0 && j != 63)
			printf("\n%d ", row[i]);
	}
	printf("\n\n   a b c d e f g h\n\n");
    }
 
     if(flip==1)
    {
 	for (int j = 0; j < 64; ++j) {
        i = 63-Flip[j];
		switch (color[i]) 
		{
			case EMPTY:
				printf(" .");
				break;
			case 0:
				printf(" %c", piece_char[board[i]]);
				break;
			case 1:
				printf(" %c", piece_char[board[i]] + ('a' - 'A'));
				break;
		}
		if ((j + 1) % 8 == 0 && row[i] != 7)
			printf("\n%d ",  row[j]+2);//7-
	}
	printf("\n\n   h g f e d c b a\n\n");
    }
	//*/
}

/* xboard() is a substitute for main() that is XBoard
   and WinBoard compatible. See the following page for details:
   http://www.research.digital.com/SRC/personal/mann/xboard/engine-intf.html */

void xboard()
{
	int computer_side;
	char line[256], command[256];
	int m;
	int post = 0;
	int analyze = 0;
	int lookup;

	signal(SIGINT, SIG_IGN);
	printf("\n");
	NewGame();
	fixed_time = 0;
	computer_side = EMPTY;
    
	while(true) 
	{
		fflush(stdout);
		if (side == computer_side) 
		{
			think();
			SetMaterial();
			Gen();
			currentkey = GetKey();
			currentlock = GetLock();
			lookup = LookUp(side);

			if(move_start != 0 || move_dest != 0)
			{
				hash_start = move_start;
				hash_dest = move_dest;
			}
			else
				printf(" lookup=0 ");

			move_list[0].start = hash_start;
            move_list[0].dest = hash_dest;
			printf("move %s\n", MoveString(hash_start,hash_dest,0));
	
			MakeMove(hash_start,hash_dest);
  
			ply = 0;
			Gen();
			PrintResult();
			continue;
		}
		if (!fgets(line, 256, stdin))
			return;
		if (line[0] == '\n')
			continue;
		sscanf(line, "%s", command);
		if (!strcmp(command, "xboard"))
			continue;
		if (!strcmp(command, "new")) 
		{
			NewGame();
			computer_side = 1;
			continue;
		}
		if (!strcmp(command, "quit"))
			return;
		if (!strcmp(command, "force")) 
		{
			computer_side = EMPTY;
			continue;
		}
		if (!strcmp(command, "white")) 
		{
			side = 0;
			xside = 1;
			Gen();
			computer_side = 1;
			continue;
		}
		if (!strcmp(command, "black")) 
		{
			side = 1;
			xside = 0;
			Gen();
			computer_side = 0;
			continue;
		}
		if (!strcmp(command, "st")) 
		{
			sscanf(line, "st %d", &max_time);
			max_time *= 1000;
			max_depth = MAX_PLY;
			fixed_time = 1;
			continue;
		}
		if (!strcmp(command, "sd")) 
		{
			sscanf(line, "sd %d", &max_depth);
			max_time = 1 << 25;
			continue;
		}
		if (!strcmp(command, "time")) 
		{
			sscanf(line, "time %d", &max_time);
			if(max_time < 200)
			  max_depth = 1;
			else
			{
				max_time /= 2;
				max_depth = MAX_PLY;
			}
			continue;
		}
		if (!strcmp(command, "otim")) 
		{
			continue;
		}
		if (!strcmp(command, "go")) 
		{
			computer_side = side;
			continue;
		}
		if (!strcmp(command, "random")) 
			continue;
		if (!strcmp(command, "level")) 
			continue;
		if (!strcmp(command, "hard")) 
			continue;
		if (!strcmp(command, "easy")) 
			continue;
		if (!strcmp(command, "hint")) 
		{
			think();
			currentkey = GetKey();
			currentlock = GetLock();
			lookup = LookUp(side);
			if(hash_start==0 && hash_dest==0)
				continue;
			printf("Hint: %s\n", MoveString(hash_start,hash_dest,0));
			continue;
		}
		if (!strcmp(command, "undo")) 
		{
			if (!hply)
				continue;
			TakeBack();
			ply = 0;
			Gen();
			continue;
		}
		if (!strcmp(command, "remove")) 
		{
			if (hply < 2)
				continue;
			TakeBack();
			TakeBack();
			ply = 0;
			Gen();
			continue;
		}
		if (!strcmp(command, "post")) 
		{
			post = 2;
			continue;
		}
		if (!strcmp(command, "nopost")) 
		{
			post = 0;
			continue;
		}

		first_move[0] = 0;
		Gen();

		m = ParseMove(line);
		if (m == -1 || !MakeMove(move_list[m].start,move_list[m].dest))
			printf("Error (unknown command): %s\n", command);
		else 
		{
			ply = 0;
 			Gen();
			PrintResult();
		}
	}
}

void PrintResult()
{
	int i;
    int flag=0;

	SetMaterial();
	Gen();
	for (i = 0; i < first_move[1]; ++i)
		if (MakeMove(move_list[i].start,move_list[i].dest))
        {
			TakeBack();
            flag=1;
			break;
		}

    if(pawn_mat[0]==0 && pawn_mat[1]==0 && piece_mat[0]<=300 && piece_mat[1]<=300)
    {
		printf("1/2-1/2 {Stalemate}\n");

		NewGame();
		computer_side = EMPTY;
		return;
    }
	if (i == first_move[1] && flag==0)
    {
		Gen();
        printf(" end of game ");
	 
		if (Attack(xside,kingloc[side]))
        {
			if (side == 0)
			{
				printf("0-1 {Black mates}\n");
			}
			else
			{
				printf("1-0 {White mates}\n");
			}
		}
		else
		{
			printf("1/2-1/2 {Stalemate}\n");
		}
		NewGame();
		computer_side = EMPTY;
	}
	else if (reps() >= 3)
	{
		printf("1/2-1/2 {Draw by repetition}\n");
		NewGame();
		computer_side = EMPTY;
	}
	else if (fifty >= 100)
	{
		printf("1/2-1/2 {Draw by fifty move rule}\n");
		NewGame();
		computer_side = EMPTY;
	}
	if(turn>300)
	{
		printf("1/2-1/2 {>300 moves}\n");
		NewGame();
		computer_side = EMPTY;
		return;
	}
}

int reps()
{
int r = 0;

for (int i = hply - 1; i >= hply-fifty; i-=2)
	if (game_list[i].hash == currentkey && game_list[i].lock == currentlock)
		++r;
return r;
}

int LoadDiagram(char* file,int num)
{
int x,n=0;
static int count=1;
char ts[200];

diagram_file = fopen(file, "r");
if (!diagram_file)
{
	printf("Diagram missing.\n");
	return -1;
}

strcpy_s(fen_name,file);

for(x=0;x<num;x++)
{
	fgets(ts, 256, diagram_file);
	if(!ts) break;
}

for(x=0;x<64;x++)
{
	board[x]=EMPTY;
	color[x]=EMPTY;
}
int c=0,i=0,j;

while(ts)
{
	if(ts[c]>='0' && ts[c]<='8')
		i += ts[c]-48;
	if(ts[c]=='\\')
		continue;
	j=Flip[i];

	switch(ts[c])
	{
		case 'K': board[j]=K; color[j]=0;i++;
		kingloc[0]=j;break;
		case 'Q': board[j]=Q;color[j]=0;i++;break;
		case 'R': board[j]=R; color[j]=0;i++;break;
		case 'B': board[j]=B; color[j]=0;i++;break;
		case 'N': board[j]=N; color[j]=0;i++;break;
		case 'P': board[j]=P; color[j]=0;i++;break;
		case 'k': board[j]=K; color[j]=1;i++;

				  kingloc[1]=j;break;
		case 'q': board[j]=Q;color[j]=1;i++;break;
		case 'r': board[j]=R; color[j]=1;i++;break;
		case 'b': board[j]=B; color[j]=1;i++;break;
		case 'n': board[j]=N; color[j]=1;i++;break;
		case 'p': board[j]=P; color[j]=1;i++;break;
	}
	c++;
	if(ts[c]==' ')
	  break;
	if(i>63)
	  break;
}
if(ts[c]==' ' && ts[c+2]==' ')
{
	if(ts[c+1]=='w')
	{
		side=0;xside=1;
	}
	if(ts[c+1]=='b')
	{
		side=1;xside=0;
	}
}

game_list[0].castle_q[0] = 0;
game_list[0].castle_q[1] = 0;
game_list[0].castle_k[0] = 0;
game_list[0].castle_k[1] = 0;

while(ts[c])
{
	switch(ts[c])
	{
		case '-': break;
		case 'K':if(board[E1]==5 && color[E1]==0) game_list[0].castle_q[0] = 1;break;
		case 'Q':if(board[E1]==5 && color[E1]==0) game_list[0].castle_q[1] = 1;break;
		case 'k':if(board[E8]==5 && color[E8]==1) game_list[0].castle_k[0] = 1;break;
		case 'q':if(board[E8]==5 && color[E8]==1) game_list[0].castle_k[1] = 1;break;
		default:break;
	}
c++;
}

CloseDiagram();
DisplayBoard();
NewPosition();
Gen();
printf(" diagram # %d \n",num+count);
count++;
if(side==0)
  printf("White to move\n");
else
  printf("Black to move\n");
printf(" %s \n",ts);
return 0;
}

void CloseDiagram()
{
if (diagram_file)
    fclose(diagram_file);
diagram_file = NULL;
}

void ShowHelp()
{	
printf("d - Displays the board.\n");	
printf("f - Flips the board.\n");
printf("go - Starts the engine.\n");
printf("help - Displays help on the commands.\n");					
printf("moves - Displays of list of possible moves.\n");
printf("new - Starts a new game .\n");
printf("off - Turns the computer player off.\n");
printf("on or p - The computer plays a move.\n");
printf("sb - Loads a fen diagram.\n");
printf("sd - Sets the search depth.\n");
printf("st - Sets the time limit per move in seconds.\n");
printf("sw - Switches sides.\n");
printf("quit - Quits the program.\n");
printf("undo - Takes back the last move.\n");
printf("xboard - Starts xboard.\n");	
}

void SetUp()
{
RandomizeHash();
FreeAllHash();
SetTables();
SetMoves();
InitBoard();	
Gen();
computer_side = EMPTY;
player[0] =0;
player[1] = 0;
max_time = 1 << 25;
max_depth = 4;
}

void NewGame()
{
InitBoard();
first_move[0] = 0;
turn = 0;
fifty = 0;
ply = 0;
hply = 0;
Gen();
}

void SetMaterial()
{
pawn_mat[0]=0;
pawn_mat[1]=0;
piece_mat[0]=0;
piece_mat[1]=0;
for(int x=0;x<64;x++)
{
	if(board[x]<6)
	{
		if(board[x]==5)
			kingloc[color[x]] = x;
		if(board[x]==0)
			pawn_mat[color[x]] += 100;
		else
			piece_mat[color[x]] += piece_value[board[x]];
	}
}
}

int GetTime()
{
struct timeb timebuffer;
ftime(&timebuffer);
return (timebuffer.time * 1000) + timebuffer.millitm;
}

char *MoveString(int start,int dest,int promote)
{
static char str[6];

char c;

if (promote > 0) {
	switch (promote) {
		case N:
			c = 'n';
			break;
		case B:
			c = 'b';
			break;
		case R:
			c = 'r';
			break;
		default:
			c = 'q';
			break;
	}
	sprintf_s(str, "%c%d%c%d%c",
			col[start] + 'a',
			row[start] + 1,
			col[dest] + 'a',
			row[dest] + 1,
			c);
}
else
	sprintf_s(str, "%c%d%c%d",
			col[start] + 'a',
			row[start] + 1,
			col[dest] + 'a',
			row[dest] + 1);
return str;
}