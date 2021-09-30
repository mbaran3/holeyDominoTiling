#include <iostream>
#include <vector>
#include <utility>
#include <iterator>

class Board{

    using col = std::vector<int>::iterator;
    using row = std::vector<std::vector<int>>::iterator;

public:

    int squaresLeft;
    std::pair<col, col> bads;

    Board(int dim_x, int dim_y, int forbid1_x, int forbid1_y,
                int forbid2_x, int forbid2_y)
                :_board(dim_x, (std::vector<int>(dim_y, 0))),
                _bad1(forbid1_x, forbid1_y),
                _bad2(forbid2_x, forbid2_y),
                squaresLeft((dim_x * dim_y)-2)
                {
                    bads = getBad();
                    _board[forbid1_x][forbid1_y] = 1;
                    _board[forbid2_x][forbid2_y] = 1;
                }
    void placeDominoDown(col x, row y){
        int xIndex = (x - y->begin());
        *x = 1;
        y += 1;
        *(y->begin() + xIndex ) = 1;
        squaresLeft -= 2;
    }
    void removeDominoDown(col x, row y){
        int xIndex = (x - y->begin()) - 1;
        *x = 0;
        y += 1;
        *(y->begin() + xIndex) = 0;
        squaresLeft += 2;
    }
    void placedominoSide(col x){
        *x = 1;
        *x = 1; 
        squaresLeft -= 2;
    }
    void removeDominoSide(col x){
        *x = 0;
        x += 1;
        *x = 0; 
        squaresLeft += 2;
    }
    row begin(){
        return _board.begin();
    }
    row end(){
        return _board.end();
    }
    int getIndex(col x){
        return x - _board.begin()->begin();
    }
private:

    std::pair<col, col> getBad(){
        auto bad1 = (_board.begin() + _bad1.second)->begin() + _bad1.first;
        auto bad2 = (_board.begin() + _bad2.second)->begin() + _bad2.first;
        return {bad1, bad2};
    }

    std::vector<std::vector<int>> _board;
    std::pair<int, int> _bad1;
    std::pair<int, int> _bad2;


};
int hdtCount_recurse(Board myBoard){

    if(myBoard.squaresLeft == 0)
        return 1;
    if(myBoard.squaresLeft == 1)
        return 0;
    int total = 0;
    for(auto row = myBoard.begin(); row != myBoard.end(); ++row){
        for(auto col = row->begin(); col != row->end(); ++col){
            
            if(*col == 0){
                if(col+1 != row->end() && row+1 != myBoard.end() && 
                (*col+1 != 1)
                && (*((row+1)->begin() + myBoard.getIndex(col)) != 1))
                {
                    

                    myBoard.placeDominoDown(col, row);
                    total += hdtCount_recurse(myBoard);
                    myBoard.removeDominoDown(col, row);
                 
                    myBoard.placedominoSide(col);
                    total += hdtCount_recurse(myBoard);
                    myBoard.removeDominoSide(col);

                }
                else if(col+1 != row->end() && row+1 == myBoard.end()
                &&(*col+1 != 1) ){
                    myBoard.placedominoSide(col);
                    total += hdtCount_recurse(myBoard);
                    myBoard.removeDominoSide(col);
                }
                else if(col+1 == row->end() && row+1 != myBoard.end()
                &&((*((row+1)->begin() + myBoard.getIndex(col)) != 1))){
                    myBoard.placeDominoDown(col, row);
                    total += hdtCount_recurse(myBoard);
                    myBoard.removeDominoDown(col, row);
                }
            }
        }
    }
    return total;
}
int hdtCount(int dim_x, int dim_y, int forbid1_x, int forbid1_y, 
            int forbid2_x, int forbid2_y){

    Board myBoard(dim_x, dim_y, forbid1_x, 
                forbid1_y, forbid2_x, forbid2_y);

    return hdtCount_recurse(myBoard);
}