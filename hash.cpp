#include "globals.h"

U64 hash[2][6][64];
U64 lock[2][6][64];

U64 currentkey,currentlock;
U64 collisions;

const U64 MAXHASH =  5000000;
const U64 HASHSIZE = 5000000;

int hash_start,hash_dest;

/*

A hash table entry includes a lock and start and dest squares.

*/
struct hashp
{
U64 hashlock;
int start;
int dest;
int num;
};

hashp *hashpos[2];
unsigned int  hashpositions[2];
/*

RandomizeHash is called when the engine is started.
The whitehash, blackhash, whitelock and blacklock tables
are filled with random numbers.

*/
void RandomizeHash()
{
int p,x;
for(p=0;p<6;p++)
	for(x=0;x<64;x++)
	{
		hash[0][p][x] = Random(HASHSIZE);
		hash[1][p][x]= Random(HASHSIZE);
		lock[0][p][x]= Random(HASHSIZE);
		lock[1][p][x]= Random(HASHSIZE);
	}
	hashpos[0] = new hashp[MAXHASH];
hashpos[1] = new hashp[MAXHASH];
}
/*

Random() generates a random number up to the size of x.

*/
int Random(const int x)
{
return rand() % x;
}
/*

Free() Frees memory that was allocated to the hashpos pointers 

with new.

*/
void Free()
{
delete hashpos[0];
delete hashpos[1];
}
/*

FreeAllHash() empties the Hash Tables.

*/
void FreeAllHash()
{
hashpositions[0]=0;
hashpositions[1]=0;
}
/*
Adds an entry into the HashTable.
If that index is already being used, it simply overwrites it.

*/
void AddHash(const int s, const move m)
{
hashp* ptr = &hashpos[s][currentkey];
ptr->hashlock = currentlock;
ptr->start=m.start;
ptr->dest=m.dest;
}
/*

AddKey updates the current key and lock.
The key is a single number representing a position.
Different positions may map to the same key.
The lock is very similar to the key (its a second key), which 
is a different number
because it was seeded with different random numbers.
While the odds of several positions having the same key are 
very high, the odds of
two positions having the same key and same lock are very very 
low.

*/
void AddKey(const int s,const int p,const int x)
{
  currentkey ^= hash[s][p][x];
  currentlock ^= lock[s][p][x];
}
/*

GetLock gets the current lock from a position.

*/
U64 GetLock()
{
U64 loc=0;
for(int x=0;x<64;x++)
{
if(board[x]!=6)
    loc ^= lock[color[x]][board[x]][x];
}
return loc;
}
/*

GetKey gets the current key from a position.

*/
U64 GetKey()
{
U64 key=0;
for(int x=0;x<64;x++)
{
if(board[x]!=6)
    key ^= hash[color[x]][board[x]][x];
}
return key;
}
/*

Looks up the current position to see if it is in the HashTable.
If so, it fetches the move stored there.

*/
bool LookUp(const int s)
{
if(hashpos[s][currentkey].hashlock != currentlock)
{
  return false;
}

hash_start = hashpos[s][currentkey].start;
hash_dest = hashpos[s][currentkey].dest;
return true;
}

