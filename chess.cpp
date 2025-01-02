#include<iostream>
#include<unordered_set>
#include<vector>
using namespace std;
enum tag{WHITE,BLACK,NONE,WHITE_KING,BLACK_KING};
pair<int,int>WKing_pos,BKing_pos;
bool WLCastle,WRCastle,BLCastle,BRCastle;    //castle variable to make descision whether to castle or not.
void Display(vector<vector<pair<char,tag>>>grid)
{
    for(int i=0;i<grid.size();i++)
    {
        for(int j=0;j<grid[0].size();j++)
        {
            std::cout<<grid[i][j].first<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"---------------\n";
}
bool boundary_check(int row,int column) //true if valid
{
    return (
        row >=0 && row < 8 && column >=0 && column < 8
    );
}
class Blueprint
{
private:
    vector<int> x_dir = {2, 2, -2, -2, 1, 1, -1, -1};
    vector<int> y_dir = {1, -1, 1, -1, 2, -2, 2, -2};
    pair<int,int>WKing_pos,BKing_pos;

public:
    Blueprint()
    {
        WLCastle=BLCastle=WRCastle=BRCastle=true;
    }
    pair<int,int> pawn(vector<vector<pair<char,tag>>>grid,int row,int column,tag t) //move for the pawn. return the update move if valid.
    {
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        pair<int,int>move={row,column};
        grid[row][column].first='P';
        grid[row][column].second=t;
        unordered_set<int>moves; //used to store the valid set of the move.
        int dir=(t==WHITE)?-1:1;
        if(boundary_check(row+dir,column) && !isBlock(grid[row+dir][column].second) && !simulate_situation(grid,row+dir,column,{'P',t}))
        {
            moves.insert(encode(row+dir,column));
        }
        if(boundary_check(row+dir,column+1) && isCaptuare(grid,row+dir,column+1,t) &&!simulate_situation(grid,row+dir,column+1,{'P',t}))
        {
            moves.insert(encode(row+dir,column+1));
        }
        if(boundary_check(row+dir,column-1) && isCaptuare(grid,row+dir,column-1,t) &&!simulate_situation(grid,row+dir,column-1,{'P',t}))
        {
            moves.insert(encode(row+dir,column-1));
        }
        if((row==1 && t==BLACK)|| (row==6 && t==WHITE))
        {
           if (!isBlock(grid[row + dir][column].second) &&
                !isBlock(grid[row + 2 * dir][column].second) &&
                !simulate_situation(grid, row + 2 * dir, column, {'P', t})) {
                moves.insert(encode(row + 2 * dir, column));
            }
        }
        std::cout<<"States before\n";
        for(auto x:moves)
        {
            auto temp=decode(x);
            grid[temp.first][temp.second].first='P';
        }
        Display(grid);
        if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        }
        int a,b;
        std::cout<<"Enter row and column "; //asking for the modified move.
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            move={a,b};
        }
        moves.clear();
        return move;
    }
    pair<int,int> Knight(vector<vector<pair<char,tag>>>grid,int row,int column,tag t)
    {
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        pair<int,int>move={row,column};
        grid[row][column].first='K';
        grid[row][column].second=t;
        unordered_set<int>moves; //used to store the valid set of the move.
        for(int i=0;i<8;i++)
        {
            int rows=row+x_dir[i];
            int columns=column+y_dir[i];
            if(boundary_check(rows,columns)){
                
                if(!isBlock(grid[rows][columns].second) && !simulate_situation(grid,rows,columns,{'K',t})){
                    moves.insert(encode(rows,columns));
                }
                if(isCaptuare(grid,rows,columns,t) && !simulate_situation(grid,rows,columns,{'K',t})){
                    moves.insert(encode(rows,columns));
                }
            }
        }
        std::cout<<"States Before\n";
        for(auto x:moves)
        {
            auto temp=decode(x);
            grid[temp.first][temp.second].first='K';
        }
        Display(grid);
        if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        } 
        int a,b;
        std::cout<<"Enter row and column ";
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            move={a,b};
        }
        moves.clear();
        return move;
    }
    pair<int,int>King(vector<vector<pair<char,tag>>>grid,int row,int column,tag t){
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        grid[row][column]={'T',(t==WHITE)?WHITE_KING:BLACK_KING};
        unordered_set<int>moves;
        pair<int,int>move={row,column};
        for(int i=-1;i<2;i++)
        {
            for(int j=-1;j<2;j++)
            {
                if(i==0 && j==0) continue;
                if(boundary_check(row+i,column+j)){
                    if(t==WHITE) WKing_pos={row+i,column+j};
                    else BKing_pos={row+i,column+j};
                    if(!isBlock(grid[row+i][column+j].second) && !simulate_situation(grid,row+i,column+j,{'T',t}))
                        moves.insert(encode(row+i,column+j));
                    else if(isCaptuare(grid,row+i,column+j,t) && !simulate_situation(grid,row+i,column+j,{'T',t}))
                        moves.insert(encode(row+i,column+j));
                }
            }
        }
        //castle change logic!
        CastleChange(grid,t,row,column,moves);
        std::cout<<"States Before\n";
        for(auto x:moves)
        {
            auto temp=decode(x);
            grid[temp.first][temp.second].first='T';
        }
        Display(grid);
        if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        } 
        int a,b;
        std::cout<<"Enter row and column ";
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            move={a,b};
        }
        moves.clear();
        return move;

    }

    pair<int,int>Bishop(vector<vector<pair<char,tag>>>grid,int row,int column,tag t)
    {
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        grid[row][column]={'B',t};
        pair<int,int>move={row,column};
        unordered_set<int>moves;
        //right diagonal:
        for(int i=row-1,j=column+1;i >=0 && j < 8;i--,j++)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        for(int i=row+1,j=column+1;i <8 && j < 8;i++,j++)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        //left diagonal
        for(int i=row-1,j=column-1;i >=0 && j >=0 ;i--,j--)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        for(int i=row+1,j=column-1;i <8 && j >=0 ;i++,j--)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'B',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        std::cout<<"States Before\n";
        for(auto x:moves){
            auto temp=decode(x);
            grid[temp.first][temp.second].first='B';
        }
        Display(grid);
         if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        } 
        int a,b;
        std::cout<<"Enter row and column ";
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            move={a,b};
        }
        moves.clear();
        return move;
    }
    pair<int,int>Rook(vector<vector<pair<char,tag>>>grid,int row,int column,tag t)
    {
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        grid[row][column]={'R',t};
        pair<int,int>move={row,column};
        unordered_set<int>moves;
        //vertical move:
         for(int i=row-1;i >=0;i--)
        {
            if(!isBlock(grid[i][column].second) && !simulate_situation(grid,i,column,{'R',t})){
                moves.insert(encode(i,column));
            }
            else if(isCaptuare(grid,i,column,t) && !simulate_situation(grid,i,column,{'R',t})){
                moves.insert(encode(i,column));
                break;
            }
            else break;
        }
        for(int i=row+1;i < 8;i++)
        {
            if(!isBlock(grid[i][column].second) && !simulate_situation(grid,i,column,{'R',t})){
                moves.insert(encode(i,column));
            }
            else if(isCaptuare(grid,i,column,t) && !simulate_situation(grid,i,column,{'R',t})){
                moves.insert(encode(i,column));
                break;
            }
            else break;
        }
        //horizontal
        for(int i=column+1;i< 8; i++)
        {
            if(!isBlock(grid[row][i].second) && !simulate_situation(grid,row,i,{'R',t}))
            {
                moves.insert(encode(row,i));
            }
            else if(isCaptuare(grid,row,i,t) && !simulate_situation(grid,row,i,{'R',t})){
                moves.insert(encode(row,i));
                break;
            }
            else break;
        }
        for(int i=column-1;i>=0; i--)
        {
            if(!isBlock(grid[row][i].second) && !simulate_situation(grid,row,i,{'R',t}))
            {
                moves.insert(encode(row,i));
            }
            else if(isCaptuare(grid,row,i,t) && !simulate_situation(grid,row,i,{'R',t})){
                moves.insert(encode(row,i));
                break;
            }
            else break;
        }
        std::cout<<"States Before\n";
        for(auto x:moves)
        {
            auto temp=decode(x);
            grid[temp.first][temp.second].first='R';
        }
        Display(grid);
        if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        }         
        int a,b;
        std::cout<<"Enter row and column ";
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            if(column==0)
            {
                (t==WHITE)? WLCastle=false:BLCastle=false;
            }
            else if(column==7)
            {
                (t==WHITE)?WRCastle=false:BRCastle=false;
            }
            move={a,b};
        }
        moves.clear();
        return move;
    }
    pair<int,int>Queen(vector<vector<pair<char,tag>>>grid,int row,int column,tag t)
    {
        WKing_pos=::WKing_pos;//assign same as global variable value.
        BKing_pos=::BKing_pos;
        grid[row][column]={'Q',t};
        pair<int,int>move={row,column};
        unordered_set<int>moves;
        //vertical move:
         for(int i=row-1;i >=0;i--)
        {
            if(!isBlock(grid[i][column].second) && !simulate_situation(grid,i,column,{'Q',t})){
                moves.insert(encode(i,column));
            }
            else if(isCaptuare(grid,i,column,t) && !simulate_situation(grid,i,column,{'Q',t})){
                moves.insert(encode(i,column));
                break;
            }
            else break;
        }
        for(int i=row+1;i < 8;i++)
        {
            if(!isBlock(grid[i][column].second) && !simulate_situation(grid,i,column,{'Q',t})){
                moves.insert(encode(i,column));
            }
            else if(isCaptuare(grid,i,column,t) && !simulate_situation(grid,i,column,{'Q',t})){
                moves.insert(encode(i,column));
                break;
            }
            else break;
        }
        //horizontal
        for(int i=column+1;i< 8; i++)
        {
            if(!isBlock(grid[row][i].second) && !simulate_situation(grid,row,i,{'Q',t}))
            {
                moves.insert(encode(row,i));
            }
            else if(isCaptuare(grid,row,i,t) && !simulate_situation(grid,row,i,{'Q',t})){
                moves.insert(encode(row,i));
                break;
            }
            else break;
        }
        for(int i=column-1;i>=0; i--)
        {
            if(!isBlock(grid[row][i].second) && !simulate_situation(grid,row,i,{'Q',t}))
            {
                moves.insert(encode(row,i));
            }
            else if(isCaptuare(grid,row,i,t) && !simulate_situation(grid,row,i,{'Q',t})){
                moves.insert(encode(row,i));
                break;
            }
            else break;
        }
        //right diagonal:
        for(int i=row-1,j=column+1;i >=0 && j < 8;i--,j++)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        for(int i=row+1,j=column+1;i <8 && j < 8;i++,j++)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        //left diagonal
        for(int i=row-1,j=column-1;i >=0 && j >=0 ;i--,j--)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        for(int i=row+1,j=column-1;i <8 && j >=0 ;i++,j--)
        {
            if(!isBlock(grid[i][j].second) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
            }
            else if(isCaptuare(grid,i,j,t) && !simulate_situation(grid,i,j,{'Q',t})){
                moves.insert(encode(i,j));
                break;
            }
            else break;
        }
        std::cout<<"States Before\n";
        for(auto x:moves){
            auto temp=decode(x);
            grid[temp.first][temp.second].first='Q';
        }
        Display(grid);
        if(moves.empty()){
            std::cout<<"Block\n";
            return move;
        } 
        int a,b;
        std::cout<<"Enter row and column ";
        std::cin>>a>>b; 
        if(moves.count(encode(a,b))){
            move={a,b};
        }
        moves.clear();
        return move;

    }
    bool simulate_situation(vector<vector<pair<char,tag>>>grid,int row,int column,pair<char,tag>piece)
    {
        grid[row][column]=piece;
        bool temp=false;
        if(piece.first=='T')
        {
            auto [r,c]=(piece.second==WHITE)?BKing_pos:WKing_pos;
            for(int i=-1;i<2;i++)
            {
                for(int j=-1;j<2;j++)
                {
                    if(i==0 && j==0) continue;
                    if(boundary_check(r+i,c+j) && grid[r+i][c+j].first=='T' && r+i==row && c+j==column){
                        temp=true;
                        break;
                    }
                }
                if(temp==true) break;
            }
        }
        return temp || checkmate((piece.second==WHITE)?BLACK:WHITE,grid);
    }
    bool checkmate(tag T,vector<vector<pair<char,tag>>>grid)
    {
        //checking the horizontal ! condition queen and rook
        pair<int,int>King_pos=(T==WHITE)?BKing_pos:WKing_pos;
        for(int i=King_pos.second+1;i< 8;i++)
        {
            if(grid[King_pos.first][i].second==NONE) continue;
            if(grid[King_pos.first][i].second==T &&(grid[King_pos.first][i].first=='R' || grid[King_pos.first][i].first=='Q'))
                return true;
            else break;
        }
        for(int i=King_pos.second-1;i>=0;i--)
        {
            if(grid[King_pos.first][i].second==NONE) continue;
            if(grid[King_pos.first][i].second==T &&(grid[King_pos.first][i].first=='R' || grid[King_pos.first][i].first=='Q'))
                return true;
            else break;
        }
        //checking the vertical ! condition queen and rook.
        for(int i=King_pos.first+1;i < 8;i++)
        {
            if(grid[i][King_pos.second].second==NONE) continue;
            if(grid[i][King_pos.second].second==T && (grid[i][King_pos.second].first=='R' || grid[i][King_pos.second].first=='Q')) return true;
            else break;
        }
        for(int i=King_pos.first-1;i >= 0;i--)
        {
            if(grid[i][King_pos.second].second==NONE) continue;
            if(grid[i][King_pos.second].second==T && (grid[i][King_pos.second].first=='R' || grid[i][King_pos.second].first=='Q')) return true;
            else break;
        }
        //checking right diagonally ! condition queen and bishok!
        for(int i=King_pos.first-1,j=King_pos.second+1; i >=0 && j < 8;i--,j++)
        {
            if(grid[i][j].second==NONE) continue;
            if(grid[i][j].second==T && (grid[i][j].first=='B' || grid[i][j].first=='Q')) return true;
            else break;
        }
        for(int i=King_pos.first+1,j=King_pos.second+1; i < 8 && j < 8;i++,j++)
        {
            if(grid[i][j].second==NONE) continue;
            if(grid[i][j].second==T && (grid[i][j].first=='B' || grid[i][j].first=='Q')) return true;
            else break;
        }
        //checking left diagonally ! condition queen and bishok
        for(int i=King_pos.first-1,j=King_pos.second-1;i >=0 && j >=0;i--,j--)
        {
            if(grid[i][j].second==NONE) continue;
            if(grid[i][j].second==T && (grid[i][j].first=='B' || grid[i][j].first=='Q')) return true;
            else break;
        }
        for(int i=King_pos.first+1,j=King_pos.second-1;i < 8 && j >=0;i++,j--)
        {
            if(grid[i][j].second==NONE) continue;
            if(grid[i][j].second==T && (grid[i][j].first=='B' || grid[i][j].first=='Q')) return true;
            else break;
        }
        // for knight
        for(int i=0;i < 8;i++)
        {
            int row=King_pos.first+x_dir[i],column=King_pos.second+y_dir[i];
            if(boundary_check(row,column) && grid[row][column].second==T && grid[row][column].first=='K') return true;
        }
        //for pawn!
    int dir = (T == WHITE) ? 1 : -1;
    if ( (boundary_check(King_pos.first + dir, King_pos.second + 1) &&
        grid[King_pos.first + dir][King_pos.second + 1].second == T &&
        grid[King_pos.first + dir][King_pos.second + 1].first == 'P') ||
        (boundary_check(King_pos.first + dir, King_pos.second - 1) &&
        grid[King_pos.first + dir][King_pos.second - 1].second == T &&
        grid[King_pos.first + dir][King_pos.second - 1].first == 'P') ) {
        return true;
    }


        return false;
    }

   
private:
    void CastleChange(vector<vector<pair<char,tag>>>&grid,tag T,int row,int column,unordered_set<int>&moves)
    {
        if(T==WHITE)
        {
            WKing_pos=::WKing_pos;
            if(WLCastle==true)
            {
                for(int i=WKing_pos.second-1;i>0;i--)
                {
                    WKing_pos.second=i;
                    if(!isBlock(grid[7][i].second) && !simulate_situation(grid,7,i,{'T',WHITE})){
                        if(i==1) moves.insert(encode(7,2));
                    }
                    else break;
                }
            }
            WKing_pos=::WKing_pos;
            if(WRCastle==true)
            {
                for(int i=WKing_pos.second+1;i<7;i++)
                {
                    WKing_pos.second=i;
                    if(!isBlock(grid[7][i].second) && !simulate_situation(grid,7,i,{'T',WHITE})){
                        if(i==6) moves.insert(encode(7,6));
                    }
                    else break;
                }
            }
        }
        else if(T==BLACK){
            BKing_pos=::BKing_pos;
            if(BLCastle==true)
            {
                for(int i=BKing_pos.second-1;i>0;i--)
                {
                    BKing_pos.second=i;
                    if(!isBlock(grid[0][i].second) && !simulate_situation(grid,0,i,{'T',BLACK})){
                        if(i==1) moves.insert(encode(0,2));
                    }
                    else break;
                }
            }
            BKing_pos=::BKing_pos;
            if(BRCastle==true)
            {
                for(int i=BKing_pos.second+1;i<7;i++)
                {
                    BKing_pos.second=i;
                    if(!isBlock(grid[0][i].second) && !simulate_situation(grid,0,i,{'T',BLACK})){
                        if(i==6) moves.insert(encode(0,6));
                    }
                    else break;
                }
            }
        }
    }
    bool isBlock(tag t) //determines no piece in the block!
    {
        return t!=NONE;
    }
    bool isCaptuare(vector<vector<pair<char,tag>>>&grid,int row,int column,tag t) // if opponent then it can capture.
    {
        tag des=grid[row][column].second;
        return (des!=WHITE_KING && des!=BLACK_KING)&& des!=NONE && des != t;
    }
    int encode(int row,int column) //for space optimization the range would be 0 to 77 and these value are independently calculated so unique for every case
    {
        return row*10+column;
    }
    pair<int,int> decode(int a) // in acutual we don't need this just to test in c++ the on click will expected to give the indices.
    {
        int column=a %10;
        int row=a/10;
        return {row,column};
    }
}obj;
void Initialize_Board(vector<vector<pair<char,tag>>>&grid)
{
    string temp="RKBQTBKR";
    for(int i=0;i<8;i++)
    {
        grid[0][i]={temp[i],BLACK};
        grid[1][i]={'P',BLACK};
        grid[6][i]={'P',WHITE};
        grid[7][i]={temp[i],WHITE};
        if(temp[i]=='T'){
            grid[0][i].second=BLACK_KING;
            BKing_pos={0,i};
            WKing_pos={7,i};
            grid[7][i].second=WHITE_KING;
        }
    }
}
pair<int,int> pick_piece(char ch,int row,int column,tag T,vector<vector<pair<char,tag>>>grid,char& shift)
{
    pair<int,int>moves;
    switch(ch)
    {
        case 'P':
            moves=obj.pawn(grid,row,column,T);
            shift='P';
            if(moves.first==0 || moves.first==7) //promotion of pawn!.
            {
                std::cout<<"Choose between R,Q,B,K:";
                std::cin>>shift;
            }
            break;
        case 'K':
            moves=obj.Knight(grid,row,column,T);
            shift='K';
            break;
        case 'R':
            moves=obj.Rook(grid,row,column,T);
            shift='R';
            break;
        case 'Q':
            moves=obj.Queen(grid,row,column,T);
            shift='Q';
            break;
        case 'B':
            moves=obj.Bishop(grid,row,column,T);
            shift='B';
            break;
        case 'T':
            moves=obj.King(grid,row,column,T);
            shift='T';
            break;

    }
    return moves;
}
void Move(vector<vector<pair<char,tag>>>&grid,tag turn)
{
    int row,column;
    char ch;
    pair<int,int>move={-1,-1};
    bool notmodify=true;
    tag K=(turn==WHITE)?WHITE_KING:BLACK_KING;
    while(notmodify)
    {
        tag T= NONE;
        while(T != turn)
        {
            cin>>row>>column;
            if(boundary_check(row,column) && (grid[row][column].second==turn || grid[row][column].second==K)){
                T=turn;
                move=pick_piece(grid[row][column].first,row,column,turn,grid,ch);
            }
            else {
                std::cout<<"try 0-7 input\n";
            }
        }
        if(move.first!=row || move.second!=column) {
            notmodify=false;
        }
    }
    grid[row][column]={'.',NONE};
    grid[move.first][move.second]={ch,turn};
    if(ch=='T'){
        if(turn==WHITE){
            grid[move.first][move.second].second=WHITE_KING;
            WKing_pos=move;
            if((WLCastle ||WRCastle) && abs(column-move.second)==2){
                if(move.second==2){
                    grid[row][3]={'R',WHITE};
                    grid[row][0]={'.',NONE};
                }
                else{
                    grid[row][move.second-1]={'R',WHITE};
                    grid[row][7]={'.',NONE};
                }
            }
            WLCastle=WRCastle=false;
        }
        else{
            grid[move.first][move.second].second=BLACK_KING;
            BKing_pos=move;
             if((BLCastle ||BRCastle) && abs(column-move.second)==2){
                if(move.second==2){
                    grid[row][3]={'R',BLACK};
                    grid[row][0]={'.',NONE};
                }
                else{
                    grid[row][move.second-1]={'R',BLACK};
                    grid[row][7]={'.',NONE};
                }
            }
            BRCastle=BLCastle=false;
        }

    }
    Display(grid);
}
int main()
{
    vector<vector<pair<char,tag>>>grid(8,vector<pair<char,tag>>(8,{'.',NONE}));//testing current state is empty board.
    Initialize_Board(grid);
    Display(grid);
    while(true)
    {
        //White
        std::cout<<"Turn of White:\n";
        Move(grid,WHITE);
        if(obj.checkmate(WHITE,grid)){
            std::cout<<"checkmate\n";
            WLCastle=WRCastle=false;
        }
        std::cout<<"Turn of BLACK:\n";
        Move(grid,BLACK);
        if(obj.checkmate(BLACK,grid)){
             std::cout<<"checkmate\n";
            BRCastle=BLCastle=false;
        }
        //black
    }
    return 0;
}