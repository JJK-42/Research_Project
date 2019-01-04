/* 
 * File:   HexState.h
 * Author: mirsoleimanisa
 *
 * Created on 6 november 2014, 16:23
 * 
 * Modified by J Koschny
 * Added operator==
 * Added hash function
 * 
 * Made several things static for increased efficiency.
 */
#include "game/hex/Utilities.h"


#ifndef HEXSTATE_H
#define	HEXSTATE_H

struct BItem{
    int val;
    int rank;
    int parent;
    bool top;
    bool bottom;
    bool left;
    bool right;
};

const int N_PIECES = 2;
const int MAX_SIZE = 64;

class HexGameState {
public:
    HexGameState();
    HexGameState(int d);
    HexGameState(const HexGameState& orig);
    //HexGameState(const HexGameState* orig);
    HexGameState& operator=(const HexGameState& orig);
    HexGameState& operator =(HexGameState&& orig);
    virtual ~HexGameState();
    void Reset();
    void SetMove(int move);
    int GetPlyJM();
    int GetMoves(vector<int>& moves);
    void SetPlayoutMoves(vector<int>& moves);
    int GetPlayoutMoves(vector<int>& moves);   
    
    float GetResult(int plyjm);
    bool IsTerminal();
    void Print() const;
    void PrintToFile(char* fileName);
    void PrintDSet();
    void PrintDSet2(int pos);

    void MakeBoard();
    void MakeEdges(int row, int col, vector<int>& eList);
    float GetValue();
    int MoveValue();
    void ClearBoard();
    void ClearStone(int pos);
    int PutStone(int pos);
    
    void Evaluate();
    float EvaluateBoard(int plyjm, int direction);
    
    //dijkstra method
    float EvaluateDSet(int plyjm, int direction);
    float EvaluateDijkstra(int plyjm, int direction);
    
    //disjoint set method
    void MakeSet(int pos);
    void ClearSet(int pos);
    void UpdateSet();
    int Find(int pos);
    void Union(int pos1, int pos2);

        void UndoMoves(int beg);
        int GetMoveCounter();
        int UndoMove(int m);
    //int CurrIndicator();
    
    //Added
    bool operator==(const HexGameState& other) const;
    size_t get_hash() const;
    int get_val(int pos) const;
    
		static void init_zobrist(int seed);
		static bool hash_is_initialised();
    
private:
		/** Array containing the bit strings for zobrist */
		static uint64_t hash_strings[N_PIECES][MAX_SIZE];
		static bool hash_initialised;
		static bool edges_initialised;
		
    static int dim;
    static int size;
    
    static vector< vector<int> > edges;
		
		uint64_t hash;
    vector<BItem> dsboard;      //the board for using disjoint set data structure
    vector<int> lefPos;
    int pjm;
    int moveCounter;
    float _bReward;
    float _wReward;

};

#endif	/* HEXSTATE_H */

