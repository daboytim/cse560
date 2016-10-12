//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : LITERAL TABLE
//      Notes: Read and Write Table for reference in assembler.cpp
//             Usage of Resolve Partial Map and Array
//      Written By: Peter Boyer
//      Edited By: The Dougars
//  \*-------------------------------------------------------------------*/


#ifndef LITERALTABLE_H
#define LITERALTABLE_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "CT/Partial_Map/Kernel_3_C.h"
#include "CI/Text/Are_In_Order_1.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

// Actual Literal Table is a Partial Map of Array
//Instantiations of components shown below
concrete_instance 
class Literal_Data:
    instantiates
  Array_Kernel_1_C <Text>
{};

concrete_instance 
class Literal_Template:
    instantiates
Partial_Map_Kernel_3_C <Text, Literal_Data, Text_Are_In_Order_1>
{};

concrete_instance
class Literal_Table
{
private:
    Literal_Template table;
    
public:
    void addLiteral(Text &, Text &, Text &);
    Text getAddress(Text &);
    Text getValue(Text &);
    void removeAny(Text &, Text &, Text &);
    Integer size();
    Boolean check(Text &);
    
};

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------


void Literal_Table::addLiteral(Text &t, Text &t_1, Text &t_2)
{
    //           0       1
    // (Name), Address,Value
    // error check
    Text name=t, address=t_1, value=t_2;
    object Literal_Data array;
    array.Set_Bounds(0,1);
    array[0]=address;
    array[1]=value;
    self.table.Define(name,array);
}

//----------------------------------------------------------------------

Text Literal_Table::getAddress(Text &t)
{
    object Text name,address;
    object Literal_Data array;
    self.table.Undefine(t,name,array);
    address=array[0];
    table.Define(name,array);
    return address;
}
//----------------------------------------------------------------------

Text Literal_Table::getValue(Text &t)
{
    object Text name,value;
    object Literal_Data array;
    self.table.Undefine(t,name,array);
    value=array[1];
    table.Define(name,array);
    return value;
}   

//---------------------------------------------------------------------

void Literal_Table::removeAny(Text &name, Text &address, Text &value)
{
    Literal_Data array;
    self.table.Undefine_Any(name, array);
    address = array[0];
    value = array[1];
}

//---------------------------------------------------------------------

Integer Literal_Table::size()
{
    return self.table.Size();
}

//---------------------------------------------------------------------
Boolean Literal_Table::check(Text &t)
{
    return self.table.Is_Defined(t);
}

#endif
