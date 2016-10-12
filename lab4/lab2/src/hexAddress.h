//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Hex Address
//
//  \*-------------------------------------------------------------------*/

#ifndef HEX_ADDRESS_H
#define HEX_ADDRESS_H 1

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class HexAddress
{
private:
    Text address;
    Text toHex(Integer &);
public:
    HexAddress() {}
    HexAddress(Text &);
    HexAddress(Integer &);
    ~HexAddress() {}
    void increment(int=1);
    Text getAddress();
    void setAddress(Text &);
    void setAddress(Integer &);
    Integer toDecimal();
    Boolean Is_Equal_To(HexAddress &);
    friend HexAddress operator++(HexAddress &op)
    {
	op.increment();
    }
};

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Text HexAddress::toHex(Integer &i)
{
    Text data;
    Character c;
    Integer tmp=i/16;
    
    for(int j=1; j>=0; j--)
    {
       	if (tmp == 0)
	{
	    c = '0';
	}
	else if (tmp == 1)
	{
	    c = '1';
	}
	else if (tmp == 2)
	{
	    c = '2';
	}
	else if (tmp == 3)
	{
	    c = '3';
	}
	else if (tmp == 4)
	{
	    c = '4';
	}
	else if (tmp == 5)
	{
	    c = '5';
	}
	else if (tmp == 6)
	{
	    c = '6';
	}
	else if (tmp == 7)
	{
	    c = '7';
	}
	else if (tmp == 8)
	{
	    c = '8';
	}
	else if (tmp == 9)
	{
	    c = '9';
	}
	else if (tmp == 10)
	{
	    c = 'A';
	}
	else if (tmp == 11)
	{
	    c = 'B';
	}
	else if (tmp == 12)
	{
	    c = 'C';
	}
	else if (tmp == 13)
	{
	    c = 'D';
	}
	else if (tmp == 14)
	{
	    c = 'E';
	}
	else if (tmp == 15)
	{
	    c = 'F';
	}
	data.Add(data.Length(), c);
	tmp=i%16;
    }
    return data;
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

HexAddress::HexAddress(Text &t)
{
    if(t.Length() > 2)
    {
	t.Split(t.Length()-2, address);
    }
    else
    {
	address = t;
    }
}

//----------------------------------------------------------------------

HexAddress::HexAddress(Integer &i)
{
    address = toHex(i);
}

//----------------------------------------------------------------------

void HexAddress::increment(int i)
{
    if(48 <= address[i] and address[i] < 57)
    {
	address[i] = To_int(address[i]) + 1;
    }
    else if(address[i] == 57)
    {
	address[i] == 65;
    }
    else if(65 <= address[i] and address[i] < 70)
    {
	address[i] = To_int(address[i]) + 1;
    }
    else if(address[i] == 70)
    {
	address[i] == 65;
    }
    else if(97 <= address[i] and address[i] < 102)
    {
	address[i] = To_int(address[i]) + 1;
    }
    else if(address[i] == 102)
    {
	address[i] == 48;
	if(i > 0)
	    increment(i-1);
    }
}

//----------------------------------------------------------------------

Text HexAddress::getAddress()
{
    return address;
}

//----------------------------------------------------------------------

void HexAddress::setAddress(Text &address)
{
    self.address = address;
}

//----------------------------------------------------------------------

Integer HexAddress::toDecimal()
{
    Integer s;
    for(Integer i=0; i<2; i++)
    {
	s *= 16;
	if(48 <= self.address[i] and self.address[i] <= 57)
	{
	    s += To_Integer(self.address[i])-48;
	}
       	else if(65 <= self.address[i] and self.address[i] <= 70)
	{
	    s += To_Integer(self.address[i])-55;
	}
	else if(97 <= self.address[i] and self.address[i] <= 102)
	{
	    s += To_Integer(self.address[i])-87;
	}
    }
    return s;
}

//----------------------------------------------------------------------

#endif
