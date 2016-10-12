//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : SYMBOL TABLE
//      Notes: "Read and write" from assembler.cpp
//      Written By: Peter Boyer
//      Edited By: The Dougars      
//
//
//  \*-------------------------------------------------------------------*/


#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "CT/Partial_Map/Kernel_3_C.h"
#include "CI/Text/Are_In_Order_1.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

// Actual symbol table is a Partial Map of Array
//Instantiations of components shown below
concrete_instance 
class Symbol_Data:
    instantiates
  Array_Kernel_1_C <Text>
{};

concrete_instance 
class Symbol_Template:
    instantiates
Partial_Map_Kernel_3_C <Text, Symbol_Data, Text_Are_In_Order_1>
{};

concrete_instance
class Symbol_Table
{
private:
    Symbol_Template table;

    
public:

    void addSymbol(Text&, Text&, Text&);
    void removeAny(Text&, Text&, Text&);
    void remove(Text&, Text&, Text&);
    Text getValue(Text &);
    Text getOther(Text &);
    Boolean check(Text &);
    Integer size();
};   


///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------



void Symbol_Table::addSymbol(Text &t, Text &t_1, Text &t_2)
{
    // error check
    Text temp=t, temp_1=t_1, temp_2=t_2;
    object Symbol_Data array;
    array.Set_Bounds(0,1);
    array[0]=temp_1;
    array[1]=temp_2;
    self.table.Define(temp,array);
}

//----------------------------------------------------------------------

void Symbol_Table::removeAny(Text &t, Text &t_1, Text &t_2)
{
    // error check
    object Symbol_Data array;
    self.table.Undefine_Any(t,array);
    t_1 = array[0];
    t_2 = array[1];
}

//----------------------------------------------------------------------

void Symbol_Table::remove(Text &t, Text &t_1, Text &t_2)
{
    // error check
    object Symbol_Data array;
    object Text temp_t;
    self.table.Undefine(t,temp_t,array);
    t_1 = array[0];
    t_2 = array[1];
}
//----------------------------------------------------------------------

Text Symbol_Table::getValue(Text &t)
{
    object Text temp,temp_1,temp_2;
    object Symbol_Data array;
    self.table.Undefine(t,temp,array);
    temp_1=array[0];
    table.Define(temp,array);
    return temp_1;
}

//---------------------------------------------------------------------

Text Symbol_Table::getOther(Text &t)
{
    object Text temp,temp_1;
    object Symbol_Data array;
    self.table.Undefine(t,temp,array);
    temp_1=array[1];
    table.Define(temp,array);
    return temp_1;
}
    
//---------------------------------------------------------------------

Boolean Symbol_Table::check(Text &t)
{
    return self.table.Is_Defined(t);
}

//---------------------------------------------------------------------

Integer Symbol_Table::size()
{
    return self.table.Size();
}

//---------------------------------------------------------------------

#endif
