//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Text Record
//
//  \*-------------------------------------------------------------------*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "word.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class Instruction
{
private:
    Text data; //maximum of 5 Characters; left justified
    void initialize(Text &);
public:
    Instruction() {}
    Instruction(Text &);
    ~Instruction() {}
    Text getData();
    void setData(Text &);
    Character getOp();
    Integer getR();
    Integer getX();
    Integer getS();
};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void Instruction::initialize(Text &t)
{
    self.data = t;
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Instruction::Instruction(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

Text Instruction::getData()
{
    return data;
}
    
//---------------------------------------------------------------------

void Instruction::setData(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

Character Instruction::getOp()
{
    //works only on data[0]
    return data[0];
}
//----------------------------------------------------------------------

Integer Instruction::getR()
{
    //works only on data[1]
    if(48 <= data[1] and data[1] <= 57)
    {
	return To_Integer(data[1])-48;
    }
    else if(65 <= data[1] and data[1] <= 70)
    {
	return To_Integer(data[1])-65;
    }
    else if(97 <= data[1] and data[1] <= 102)
    {
	return To_Integer(data[1])-97;
    }
}

//----------------------------------------------------------------------

Integer Instruction::getX()
{
    //works only on data[2];
    switch (To_int(data[2]))
    {
    case 48:
	return 0;
    case 52:
	return 1;
    case 56:
	return 2;
    case (67 or 99):
	return 3;
    }
}

//----------------------------------------------------------------------

Integer Instruction::getS()
{
    //works onln on data[3] and data[4]
    Integer s;
    for(Integer i=3; i<=4; i++)
    {
	s *= 16;
	if(48 <= data[i] and data[i] <= 57)
	{
	    s += To_Integer(data[i])-48;
	}
       	else if(65 <= data[i] and data[i] <= 70)
	{
	    s += To_Integer(data[i])-55;
	}
	else if(97 <= data[i] and data[i] <= 102)
	{
	    s += To_Integer(data[i])-87;
	}
    }
    return s;
}

//----------------------------------------------------------------------

#endif
