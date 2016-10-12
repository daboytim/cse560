//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Machine_Table
//      Notes: "Library" for machine operation lookup in assembler
//      Written By: Peter Boyer
//      Edited By: The Dougars
//
//
//  \*-------------------------------------------------------------------*/


#ifndef MACHINETABLE_H
#define MACHINETABLE_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "CT/Array/Kernel_1_C.h"
#include "CT/Partial_Map/Kernel_3_C.h"
#include "CI/Text/Are_In_Order_1.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------
          
concrete_instance 
class Array_Of_Data:
    instantiates
  Array_Kernel_1_C <Text>
{};

concrete_instance 
class Table_Data:
    instantiates
Partial_Map_Kernel_3_C <Text, Array_Of_Data, Text_Are_In_Order_1>
{};

concrete_instance
class Machine_Table
{
private:
    Table_Data table;
    void initialize();

public:
    Machine_Table();
    Character getOpcode(Text &);
    Integer getLength(Text &);
    Text getFormat(Text &);
    Boolean check(Text &);

};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void Machine_Table::initialize()
{
    object Array_Of_Data data;
    Text temp;

    //---------------------------Op 0-LD
    data.Set_Bounds(1,3);
    data[1]="0";
    data[2]="1";
    data[3]="00000";
    temp="LD";
    self.table.Define(temp,data);

    //---------------------------Op 1-LDI
    data.Set_Bounds(1,3);
    data[1]="1";
    data[2]="1";
    data[3]="10000";
    temp="LDI";
    self.table.Define(temp,data);
    
     //---------------------------Op 2-ST
    data.Set_Bounds(1,3);
    data[1]="2";
    data[2]="1";
    data[3]="20000";
    temp="ST";
    self.table.Define(temp,data);

    //---------------------------Op 3-ADD
    data.Set_Bounds(1,3);
    data[1]="3";
    data[2]="1";
    data[3]="30000";
    temp="ADD";
    self.table.Define(temp,data);   
    
    //---------------------------Op 4-SUB
    data.Set_Bounds(1,3);
    data[1]="4";
    data[2]="1";
    data[3]="40000";
    temp="SUB";
    self.table.Define(temp,data);

    //---------------------------Op 5-MUL
    data.Set_Bounds(1,3);
    data[1]="5";
    data[2]="1";
    data[3]="50000";
    temp="MUL";
    self.table.Define(temp,data);
    
     //---------------------------Op 6-DIV
    data.Set_Bounds(1,3); 
    data[1]="6";
    data[2]="1";
    data[3]="60000";
    temp="DIV";
    self.table.Define(temp,data);

    //---------------------------Op 7-OR
    data.Set_Bounds(1,3);
    data[1]="7";
    data[2]="1";
    data[3]="70000";
    temp="OR";
    self.table.Define(temp,data);

     //---------------------------Op 8-AND
    data.Set_Bounds(1,3);
    data[1]="8";
    data[2]="1";
    data[3]="80000";
    temp="AND";
    self.table.Define(temp,data);

    //---------------------------Op 9-SHL
    data.Set_Bounds(1,3);
    data[1]="9";
    data[2]="1";
    data[3]="90000";
    temp="SHL";
    self.table.Define(temp,data);

    //---------------------------Op A-SHR
    data.Set_Bounds(1,3);
    data[1]="A";
    data[2]="1";
    data[3]="A0000";
    temp="SHR";
    self.table.Define(temp,data);

     //---------------------------Op B-IO
    data.Set_Bounds(1,3);
    data[1]="B";
    data[2]="1";
    data[3]="B0000";
    temp="IO";
    self.table.Define(temp,data);

    //---------------------------Op C-BR
    data.Set_Bounds(1,3);
    data[1]="C";
    data[2]="1";
    data[3]="C0000";
    temp="BR";
    self.table.Define(temp,data);

    //---------------------------Op D-BRZ
    data.Set_Bounds(1,3);
    data[1]="D";
    data[2]="1";
    data[3]="D0000";
    temp="BRZ";
    self.table.Define(temp,data);

    //---------------------------Op E-BRN
    data.Set_Bounds(1,3);
    data[1]="E";
    data[2]="1";
    data[3]="E0000";
    temp="BRN";
    self.table.Define(temp,data);

    //---------------------------Op F-BRS
    
    data.Set_Bounds(1,3);
    data[1]="F";
    data[2]="1";
    data[3]="F0000";
    temp="BRS";
    self.table.Define(temp,data);

 }


///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Machine_Table::Machine_Table()
{
    initialize();
}

//----------------------------------------------------------------------

Integer Machine_Table::getLength(Text &t)
{
    if (not self.table.Is_Defined(t))
    {
	return 9;
    }
    object Text temp,temp_1;
    object Array_Of_Data data;
    self.table.Undefine(t,temp,data);
    temp_1=data[2];
    self.table.Define(temp,data);
    object Integer out=To_Integer(temp_1);
    return out;
}
//---------------------------------------------------------------------

Character Machine_Table::getOpcode(Text &t)
{
    if (not self.table.Is_Defined(t))
    {
	return '-';
    }
    object Text temp,temp_1;
    object Array_Of_Data data;
    self.table.Undefine(t,temp,data);
    temp_1=data[1];
    self.table.Define(temp,data);
    object Character c=temp_1[0];
    return c;
      
}
    
//---------------------------------------------------------------------

Text Machine_Table::getFormat(Text& t)
{
    if (not self.table.Is_Defined(t))
    {
	return "Null";
    }
    object Text temp, temp_1;
    object Array_Of_Data data;
    self.table.Undefine(t,temp,data);
    temp_1=data[3];
    self.table.Define(temp,data);
    return temp_1; 
}
    
//---------------------------------------------------------------------

Boolean Machine_Table::check(Text &t)
{
    return self.table.Is_Defined(t);
}

//---------------------------------------------------------------------

#endif
