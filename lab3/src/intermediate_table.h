//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Intermediate_Table
//      Notes: Intermediate file to bridge pass 1 to pass 2
//      Written By: Chris Fenner
//      Edited By: The Dougars
//
//
//  \*-------------------------------------------------------------------*/

#ifndef INTERMEDIATETABLE_H
#define INTERMEDIATETABLE_H 1

///-------------------------------------------------------------
/// Global Context ---------------------------------------------
///-------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "CT/Queue/Kernel_1a_C.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

//Instantiate the Record

concrete_instance
class Inter_Data:
    instantiates
        Queue_Kernel_1a_C <Text>
{};

concrete_instance
class Intermediate_Table
{
private:
    Inter_Data table;
    
public:
    Intermediate_Table();
    void add_inter_line(Text &);
    Text get_inter_line();
    Integer inter_size();
};

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Intermediate_Table::Intermediate_Table()
{
    
}

//----------------------------------------------------------------------

void Intermediate_Table::add_inter_line(Text &t)
{
    self.table.Enqueue(t);
}

//----------------------------------------------------------------------

Text Intermediate_Table::get_inter_line()
{
    Text t;
    self.table.Dequeue(t);
    
    return t;
}

//----------------------------------------------------------------------

Integer Intermediate_Table::inter_size()
{
    return self.table.Length();
}

//----------------------------------------------------------------------

#endif
