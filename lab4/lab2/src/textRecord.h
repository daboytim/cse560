//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Text Record
//
//  \*-------------------------------------------------------------------*/

#ifndef TEXT_RECORD_H
#define TEXT_RECORD_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "hexAddress.h"
#include "instruction.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class TextRecord
{
private:
    HexAddress address;
    Instruction data;
    void initialize(Text &);
public:
    TextRecord() {}
    TextRecord(Text &);
    ~TextRecord() {}
    void setData(Text &);
    HexAddress getAddress();
    Instruction getData();
};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void TextRecord::initialize(Text &t)
{
    Text tmp;
    t.Split(t.Length()-5, tmp);
    data.setData(tmp);

    t.Split(t.Length()-2, tmp);
    address.setAddress(tmp);
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

TextRecord::TextRecord(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

void TextRecord::setData(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

HexAddress TextRecord::getAddress()
{
    return address;
}

//----------------------------------------------------------------------

Instruction TextRecord::getData()
{
    return data;
}

//----------------------------------------------------------------------

#endif
