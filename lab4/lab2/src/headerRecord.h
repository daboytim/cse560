//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Header Record
//
//  \*-------------------------------------------------------------------*/

#ifndef HEADER_RECORD_H
#define HEADER_RECORD_H 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "hexAddress.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class HeaderRecord
{
private:
    Text name;
    HexAddress begin,load;
    Integer len;
    void initialize(Text &);
public:
    HeaderRecord(Text &);
    ~HeaderRecord() {}
    void setData(Text &);
    HexAddress getBeginAddress();
    Text getName();
    HexAddress getLoadAddress();
    Integer length();
};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void HeaderRecord::initialize(Text &t)
{
    Text tmp = t, a;

    tmp.Split(tmp.Length()-2, a);
    HexAddress val(a);
    len = val.toDecimal();

    tmp.Split(tmp.Length()-2, a);
    load.setAddress(a);

    tmp.Split(tmp.Length()-6, name);

    tmp.Split(tmp.Length()-2, a);
    begin.setAddress(a);
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

HeaderRecord::HeaderRecord(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

void HeaderRecord::setData(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

HexAddress HeaderRecord::getBeginAddress()
{
    return begin;
}

//----------------------------------------------------------------------

Text HeaderRecord::getName()
{
    return name;
}

//----------------------------------------------------------------------

HexAddress HeaderRecord::getLoadAddress()
{
    return load;
}

//----------------------------------------------------------------------

Integer HeaderRecord::length()
{
    return len;
}

//----------------------------------------------------------------------

#endif
