//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Pseudo Op Table
//      Notes: "Library" referenced in assembler.cpp
//      Written By: Peter Boyer
//      Edited By: The Dougars
//
//  \*-------------------------------------------------------------------*/


#ifndef PSEUDOOPTABLE_H
#define PSEUDOOPTABLE_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "CT/Partial_Map/Kernel_3_C.h"
#include "CI/Text/Are_In_Order_1.h"
#include "CT/Array/Kernel_1_C.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class Array_Of_Pseudo_Data:
   instantiates
      Array_Kernel_1_C <Text>
{};

concrete_instance 
class Pseudo_Data:
    instantiates
Partial_Map_Kernel_3_C <Text, Array_Of_Pseudo_Data, Text_Are_In_Order_1>
{};

concrete_instance
class Pseudo_Op_Table
{
private:
    Pseudo_Data table;
    void initialize();

public:
    Pseudo_Op_Table();
    Integer getLength(Text &);
    Text getFormat(Text &);
    Boolean check(Text &);

};
///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void Pseudo_Op_Table::initialize()
{
    object Array_Of_Pseudo_Data data;
    Text temp;
    
    //---------------------------ORI
    data.Set_Bounds(1,2);
    data[1]="0";
    data[2]="--";
    temp="ORI";
    self.table.Define(temp,data);

    //---------------------------END
    data.Set_Bounds(1,2);
    data[1]="0";
    data[2]="--";
    temp="END";
    self.table.Define(temp,data);

    //---------------------------EQU
    data.Set_Bounds(1,2);
    data[1]="0";
    data[2]="--";
    temp="EQU";
    self.table.Define(temp,data);

    //---------------------------NMD
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="NMD";
    self.table.Define(temp,data);

    //---------------------------CCD
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="CCD";
    self.table.Define(temp,data);

    //---------------------------RES
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="RES";
    self.table.Define(temp,data);

    //---------------------------RET
    data.Set_Bounds(1,2);
    data[1]="0";
    data[2]="--";
    temp="RET";
    self.table.Define(temp,data);

    //---------------------------GTC
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="GTC";
    self.table.Define(temp,data);
	
    //---------------------------PTC
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="PTC";
    self.table.Define(temp,data);
    
  //---------------------------ENT
    data.Set_Bounds(1,2);
    data[1]="0";
    data[2]="--";
    temp="EXT";
    self.table.Define(temp,data);
	
    //---------------------------EXT
    data.Set_Bounds(1,2);
    data[1]="1";
    data[2]="--";
    temp="ENT";
    self.table.Define(temp,data);

}
///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Pseudo_Op_Table::Pseudo_Op_Table()
{
    initialize();
}

//----------------------------------------------------------------------

Integer Pseudo_Op_Table::getLength(Text &t)
{
    object Text siz,t_temp;
    object Array_Of_Pseudo_Data data, data_copy;
    t_temp=t;
    self.table.Undefine(t,t_temp,data);
    siz=data[1];
    self.table.Define(t_temp,data);
    object Integer out=To_Integer(siz);
    return out;
}
    
//---------------------------------------------------------------------


Text Pseudo_Op_Table::getFormat(Text& t)
{
    object Text form,form_2,t_temp;
    object Array_Of_Pseudo_Data data, data_copy;
    t_temp=t;
    self.table.Undefine(t,t_temp,data);
    form=data[2];
    self.table.Define(t_temp,data);
    return form; 
}
    
//---------------------------------------------------------------------

Boolean Pseudo_Op_Table::check(Text &t)
{
    return self.table.Is_Defined(t);
}

//---------------------------------------------------------------------

#endif
