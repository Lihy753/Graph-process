#ifndef DANCINGLINK_H
#define DANCINGLINK_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <cmath>
#include <limits>
#include <assert.h>

namespace DL
{


class Cell{
    public:
        Cell()
            : Left(nullptr),
              Right(nullptr),
              Up(nullptr),
              Down(nullptr),
              Row(0),
              Col(0),
              weight(-1.0),
              highest_priority(std::numeric_limits<int>::max()),
              degree_in_graph(std::numeric_limits<int>::max()),
              InDLX(true),
              Children_Number(0){}

    Cell *Left, *Right, *Up, *Down;
    int Row;
    int Col;
    double weight;
    int highest_priority;
    int degree_in_graph;

    //flag for valid
    bool InDLX;

    int Children_Number;
    std::map<int, int> priority_list;
};

class DancingLink{
    public:
        DancingLink()
            : Row_Number_All(0),
              Col_Number_All(0),
              Row_Number_Now(0)
              Col_Number_Now(0){}
        ~DancingLink(){
            if (Row_Number_All != 0){
                for (int i = 0; i <= Row_Number_All; i++){
                    Cell* r = &Row_Header_Table[i];
                    if (r->Right == r)
                        continue;
                    Cell* head = r->Right;
                    Cell* t_pointer;
                    while (head->Right != r)
                    {
                        t_pointer = head->Right;
                        delete head;
                        head = t_pointer;
                    }
                    delete head;
                }

                delete[] Row_Header_Table;
                delete[] Col_Header_Table;
            }
        }

        void clear(){
            if (Row_Number_All != 0){
                // delete element cell
                for (int i = 1; i <= Row_Number_All; i++){
                    Cell* r = &Row_Header_Table[i];

                    if (r->Right == r)
                        continue;
                    Cell* head = r->Right;
                    Cell* t_pointer;
                    while (head->Right != r)
                    {
                        t_pointer = head->Right;
                        delete head;
                        head = t_pointer;
                    }
                    delete head;
                }

                delete[] Row_Header_Table;
                delete[] Col_Header_Table;
            }

            Row_Number_All = 0;
            Col_Number_All = 0;
            Row_Number_Now = 0;
            Col_Number_Now = 0;

            Row_Header_Table = nullptr;
            Col_Header_Table = nullptr;
        }

    public:
        Cell DL_Header;
        Cell* Row_Header_Table, Col_Header_Table;
        int Row_Number_All, Col_Number_All, Row_Number_Now, Col_Number_Now;
};

class DancingLinkEngineBase{
    public:
        void DL_Init(DL::DancingLink& dl, const int& row, const int& col);
        void Cell_Insert(DL::DancingLink& dl, const int& row, const int& col);

        // cell operation 
        void UD_self(DL::Cell& c);
        void LR_self(DL::Cell& c);
        void UD_remove(DL::Cell& c);
        void LR_remove(DL::Cell& c);
        void LR_recover(DL::Cell& c);
        void UD_recover(DL::Cell& c);

        //col row operation
        void Remove_Single_Row(DL::DancingLink& dl, const int& row);
        void Remove_Single_Col(DL::DancingLink& dl, const int& col);
        void Remove_Rows_Cols(DL::DancingLink& dl, const std::set<int>& row_set, const std::set<int>& col_set);
        void Recover_Single_Row(DL::DancingLink& dl, const int& row);
        void Recover_Rows_Cols(DL::DancingLink& dl, const std::set<int>& row_set, const std::set<int>& col_set);
        void Select_All_Rows_Cols(DL::DancingLink& dl, int& target_row, std::set<int>& row_set, std::set<int>& col_set);
}



} // namespace DL


#endif