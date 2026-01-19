#include "DancingLink.h"

using namespace std;
using namespace DL;

void DancingLinkEngineBase::DL_Init(DL::DancingLink& dl, const int& row, const int& col){
    dl.Row_Number_Now = dl.Row_Number_All = row;
    dl.Col_Number_Now = dl.Col_Number_All = col;

    dl.DL_Header.Col = 0;
    dl.DL_Header.Row = 0;

    //initial col header
    dl.Col_Header_Table = new Cell[col + 1];

    for (int i = 1; i < col; i++){
        dl.Col_Header_Table[i].Right = &dl.Col_Header_Table[i + 1];
    }
    for (int i = 2; i <= col; i++){
        dl.Col_Header_Table[i].Left = &dl.Col_Header_Table[i - 1];
    }
    for (int i = 1; i <= col; i++){
        UD_self(dl.Col_Header_Table[i]);
        dl.Col_Header_Table[i].Children_Number = 0;
        dl.Col_Header_Table[i].weight = 0;
        dl.Col_Header_Table[i].highest_priority = std::numeric_limits<int>::max();
        dl.Col_Header_Table[i].Col = i;
    }

    dl.DL_Header.Right = &dl.Col_Header_Table[1];
    dl.DL_Header.Left = &dl.Col_Header_Table[col];
    dl.Col_Header_Table[col].Right = &dl.DL_Header;
    dl.Col_Header_Table[1].Left = &dl.DL_Header;

    //initial row header
    dl.Row_Header_Table = new Cell[row + 1];
    for (int i = 1; i < row; i++)
        dl.Row_Header_Table[i].Down = &dl.Row_Header_Table[i + 1];
    
    for (int i = 2; i <= row; i++)
        dl.Row_Header_Table[i].Up = &dl.Row_Header_Table[i - 1];

    for (int i = 1; i <= row; i++){
        LR_self(dl.Row_Header_Table[i]);
        dl.Row_Header_Table[i].Children_Number = 0;
        dl.Row_Header_Table[i].Row = i;
    }

    dl.DL_Header.Down = &dl.Row_Header_Table[1];
    dl.DL_Header.Up = &dl.Row_Header_Table[row];
    dl.Row_Header_Table[row].Down = &dl.DL_Header;
    dl.Row_Header_Table[1].Up = &dl.DL_Header;
}

void DancingLinkEngineBase::Cell_Insert(DL::DancingLink& dl, const int& row, const int& col){
    Cell* r = &dl.Row_Header_Table[row];

    //当第 col 列已经有元素了， 则不再插入，直接退出！
    for (Cell* i = r->Right; i != r; i = i->Right){
        if (i->Col == col){
            return;
        }
    }

    Cell* c = new Cell;
    c->Row = (int)row;
    c->Col = (int)col;

    assert(row > 0);
    assert(col > 0);

    dl.Col_Header_Table[col].Children_Number++;
    c->Up = dl.Col_Header_Table[col].Up;
    c->Up->Down = c;

    dl.Col_Header_Table[col].Up = c;
    c->Down = &dl.Col_Header_Table[col];

    dl.Row_Header_Table[row].Children_Number++;
    c->Right = &dl.Row_Header_Table[row];
    c->Left = dl.Row_Header_Table[row].Left;
    c->Left->Right = c;
    dl.Row_Header_Table[row].Left = c;
}

// cell operation 
void DancingLinkEngineBase::UD_self(DL::Cell& c){
    c.Up = &c;
    c.Down = &c;
}

void DancingLinkEngineBase::LR_self(DL::Cell& c){
    c.Left = c.Right = &c;
}

void DancingLinkEngineBase::UD_remove(DL::Cell& c){
    if (c.InDLX){
        c.Up->Down = c.Down;
        c.Down->Up = c.Up;
        c.InDLX = false;
    }
}

void DancingLinkEngineBase::LR_remove(DL::Cell& c){
    if (c.InDLX){
        c.Left->Right = c.Right;
        c.Right->Left = c.Left;
        c.InDLX = false;
    }
}

void DancingLinkEngineBase::LR_recover(DL::Cell& c){
    if (c.InDLX = false){
        c.Left->Right = &c;
        c.Right->Left = &c;
        c.InDLX = true;
    }
}

void DancingLinkEngineBase::UD_recover(DL::Cell& c){
    if (c.InDLX = false){
        c.Up->Down = &c;
        c.Down->Up = &c;
        c.InDLX = true;
    }
}

//col row operation
void DancingLinkEngineBase::Remove_Single_Row(DL::DancingLink& dl, const int& row){
    Cell* r = &dl.Row_Header_Table[row];
    UD_remove(*r);
    r->Children_Number = 0;

    //delete row element neighbor's up down link
    for (Cell* j = r->Right; j != r; j = j->Right){
        if (j->InDLX){
            UD_remove(*j);
            dl.Col_Header_Table[j->Col].Children_Number--;
        }
    }
}

void DancingLinkEngineBase::Remove_Single_Col(DL::DancingLink& dl, const int& col){
    Cell* c = &dl.Col_Header_Table[col];
    LR_remove(*c);
    c->Children_Number == 0;
    for (Cell* j = c->Down; j != c; j = j->Down){
        if (j->InDLX){
            dl.Row_Header_Table[j->Row].Children_Number--;
            LR_remove(*j);
        }
    }
}

void DancingLinkEngineBase::Remove_Rows_Cols(DL::DancingLink& dl, const std::set<int>& row_set, const std::set<int>& col_set){
    std::set<int>::iterator it;
    for (it = row_set.begin(); it != row_set.end(); it++)
        Remove_Single_Row(dl, *it);
    
    for (it = col_set.begin(); it != col_set.end(); it++){
        if (*it == 0)
            continue;
        else{
            //remove col header's left right pointer
            LR_remove(dl.Col_Header_Table[*it]);
        }
    }
}

void DancingLinkEngineBase::Recover_Single_Row(DL::DancingLink& dl, const int& row){
    Cell* f = &dl.Row_Header_Table[row];
    UD_recover(*f);
    for (Cell* i = f->Right; i != f; i = i->Right){
        if (!i->InDLX){
            UD_recover(*i);
            dl.Col_Header_Table[i->Col].Children_Number++;
        }
    }
}

void DancingLinkEngineBase::Recover_Rows_Cols(DL::DancingLink& dl, const std::set<int>& row_set, const std::set<int>& col_set){
    std::set<int>::iterator it;
    for (it = row_set.begin(); it != row_set.end(); it++)
        Recover_Single_Row(dl. *it);
    for (it = col_set.begin(); it != col_set.end(); it++)
        LR_recover(dl.Col_Header_Table[*it]);
}

void DancingLinkEngineBase::Select_All_Rows_Cols(DL::DancingLink& dl, int& target_row, std::set<int>& row_set, std::set<int>& col_set){
    Cell* r = &dl.Row_Header_Table[target_row];

    //FIX ME;
    //will delete header
    //col_set.insert(r->Right->Col)
    //row_set.insert(target_row);

    for (Cell* i = r->Right; i != r; i = i->Right){
        col_set.insert(i->Col);
        for (Cell* j = dl.Col_Header_Table[i->Col].Down; j != &dl.Col_Header_Table[i->Col]; j = j->Down){
            row_set.insert(j->Row);
        }
    }
}

