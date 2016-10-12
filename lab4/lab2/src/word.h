//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : Word
//
//  \*-------------------------------------------------------------------*/

#ifndef WORD_H
#define WORD_H 1

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance
class Word
{
private:
    Text data; //maximum of 5 hex characters; left justified
    void initialize(Text &);
    Text toHex(Integer);
public:
    Word();
    Word(Text &);
    Word(Integer &);
    ~Word() {}
    void setData(Text &);
    void setData(Integer);
    Text toHex();
    Integer toDecimal();
    void increment(int=4);
    void Clear();
    friend Word operator++(Word &op)
    {
	op.increment();
    }
    Word operator&=(Word &op)
    {
	Text self_data = self.toHex();
	Text op_data = op.toHex();
	self.initialize(op_data);
	op.initialize(self_data);
    }
};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void Word::initialize(Text &t)
{
    Text t_copy = t;
    self.data = t;
}

//----------------------------------------------------------------------

Text Word::toHex(Integer i)
{
    Integer tmp = i;
    Text data;
    Character c;
    
//First Character
    if (tmp / 65536 > 0)
    {
	if (tmp / 65536 == 1)
	{
	    c = '1';
	}
	else if (tmp / 65536 == 2)
	{
	    c = '2';
	}
	else if (tmp / 65536 == 3)
	{
	    c = '3';
	}
	else if (tmp / 65536 == 4)
	{
	    c = '4';
	}
	else if (tmp / 65536 == 5)
	{
	    c = '5';
	}
	else if (tmp / 65536 == 6)
	{
	    c = '6';
	}
	else if (tmp / 65536 == 7)
	{
	    c = '7';
	}
	else if (tmp / 65536 == 8)
	{
	    c = '8';
	}
	else if (tmp / 65536 == 9)
	{
	    c = '9';
	}
	else if (tmp / 65536 == 10)
	{
	    c = 'A';
	}
	else if (tmp / 65536 == 11)
	{
	    c = 'B';
	}
	else if (tmp / 65536 == 12)
	{
	    c = 'C';
	}
	else if (tmp / 65536 == 13)
	{
	    c = 'D';
	}
	else if (tmp / 65536 == 14)
	{
	    c = 'E';
	}
	else
	{
	    c = 'F';
	}
	data.Add(data.Length(), c);
	tmp = tmp % 65536;
    }
    else
    {
	c = '0';
	data.Add(data.Length(), c);
    }
    
//Second Character
    if (tmp / 4096 > 0)
    {
	if (tmp / 4096 == 1)
	{
	    c = '1';
	}
	else if (tmp / 4096 == 2)
	{
	    c = '2';
	}
	else if (tmp / 4096 == 3)
	{
	    c = '3';
	}
	else if (tmp / 4096 == 4)
	{
	    c = '4';
	}
	else if (tmp / 4096 == 5)
	{
	    c = '5';
	}
	else if (tmp / 4096 == 6)
	{
	    c = '6';
	}
	else if (tmp / 4096 == 7)
	{
	    c = '7';
	}
	else if (tmp / 4096 == 8)
	{
	    c = '8';
	}
	else if (tmp / 4096 == 9)
	{
	    c = '9';
	}
	else if (tmp / 4096 == 10)
	{
	    c = 'A';
	}
	else if (tmp / 4096 == 11)
	{
	    c = 'B';
	}
	else if (tmp / 4096 == 12)
	{
	    c = 'C';
	}
	else if (tmp / 4096 == 13)
	{
	    c = 'D';
	}
	else if (tmp / 4096 == 14)
	{
	    c = 'E';
	}
	else
	{
	    c = 'F';
	}
	data.Add(data.Length(), c);
	tmp = tmp % 4096;
    }
    else
    {
	c = '0';
	data.Add(data.Length(), c);
    }
    
    
//Third Character
    if (tmp / 256 > 0)
    {
	if (tmp / 256 == 1)
	{
	    c = '1';
	}
	else if (tmp / 256 == 2)
	{
	    c = '2';
	}
	else if (tmp / 256 == 3)
	{
	    c = '3';
	}
	else if (tmp / 256 == 4)
	{
	    c = '4';
	}
	else if (tmp / 256 == 5)
	{
	    c = '5';
	}
	else if (tmp / 256 == 6)
	{
	    c = '6';
	}
	else if (tmp / 256 == 7)
	{
	    c = '7';
	}
	else if (tmp / 256 == 8)
	{
	    c = '8';
	}
	else if (tmp / 256 == 9)
	{
	    c = '9';
	}
	else if (tmp / 256 == 10)
	{
	    c = 'A';
	}
	else if (tmp / 256 == 11)
	{
	    c = 'B';
	}
	else if (tmp / 256 == 12)
	{
	    c = 'C';
	}
	else if (tmp / 256 == 13)
	{
	    c = 'D';
	}
	else if (tmp / 256 == 14)
	{
	    c = 'E';
	}
	else
	{
	    c = 'F';
	}
	data.Add(data.Length(), c);
	tmp = tmp % 256;
    }
    else
    {
	c = '0';
	data.Add(data.Length(), c);
    }
    
    
//Fourth Character
    if (tmp / 16 > 0)
    {
	if (tmp / 16 == 1)
	{
	    c = '1';
	}
	else if (tmp / 16 == 2)
	{
	    c = '2';
	}
	else if (tmp / 16 == 3)
	{
	    c = '3';
	}
	else if (tmp / 16 == 4)
	{
	    c = '4';
	}
	else if (tmp / 16 == 5)
	{
	    c = '5';
	}
	else if (tmp / 16 == 6)
	{
	    c = '6';
	}
	else if (tmp / 16 == 7)
	{
	    c = '7';
	}
	else if (tmp / 16 == 8)
	{
	    c = '8';
	}
	else if (tmp / 16 == 9)
	{
	    c = '9';
	}
	else if (tmp / 16 == 10)
	{
	    c = 'A';
	}
	else if (tmp / 16 == 11)
	{
	    c = 'B';
	}
	else if (tmp / 16 == 12)
	{
	    c = 'C';
	}
	else if (tmp / 16 == 13)
	{
	    c = 'D';
	}
	else if (tmp / 16 == 14)
	{
	    c = 'E';
	}
	else
	{
	    c = 'F';
	}
	data.Add(data.Length(), c);
	tmp = tmp % 16;
    }
    else
    {
	c = '0';
	data.Add(data.Length(), c);
    }
    
    
//Fifth Character
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
    else
    {
	c = 'F';
    }
    data.Add(data.Length(), c);

    return data;
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

Word::Word()
{
    Text t="-----";
    initialize(t);
}

//----------------------------------------------------------------------

Word::Word(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

Word::Word(Integer &i)
{
    Text t = toHex(i);
    initialize(t);
}

//----------------------------------------------------------------------

void Word::setData(Text &t)
{
    initialize(t);
}

//----------------------------------------------------------------------

void Word::setData(Integer i)
{
    Text t = toHex(i);
    initialize(t);
}

//----------------------------------------------------------------------

Text Word::toHex()
{
    return self.data;
}

//----------------------------------------------------------------------

Integer Word::toDecimal()
{
    Integer s=0;
    for(Integer i=0; i<self.data.Length(); i++)
    {
	s *= 16;
	if(48 <= self.data[i] and self.data[i] <= 57)
	{
	    s += To_Integer(self.data[i])-48;
	}
       	else if(65 <= self.data[i] and self.data[i] <= 70)
	{
	    s += To_Integer(self.data[i])-55;
	}
	else if(97 <= self.data[i] and self.data[i] <= 102)
	{
	    s += To_Integer(self.data[i])-87;
	}
    }
    return s;
}

//----------------------------------------------------------------------

void Word::increment(int i)
{
    // if(48 <= self.data[i] and self.data[i] < 57)
//     {
// 	self.data[i] = To_int(self.data[i]) + 1;
//     }
//     else if(self.data[i] == 57)
//     {
// 	self.data[i] == 65;
//     }
//     else if(65 <= self.data[i] and self.data[i] < 70)
//     {
// 	self.data[i] = To_int(self.data[i]) + 1;
//     }
//     else if(self.data[i] == 70)
//     {
// 	self.data[i] == 65;
//     }
//     else if(97 <= self.data[i] and self.data[i] < 102)
//     {
// 	self.data[i] = To_int(self.data[i]) + 1;
//     }
//     else if(self.data[i] == 102)
//     {
// 	self.data[i] == 48;
// 	if(i > 0)
// 	    increment(i-1);
//     }

    object Integer tmp = self.toDecimal();
    tmp = tmp + 1;
    self.setData(tmp);
}

//----------------------------------------------------------------------

void Word::Clear()
{
    self.data = "-----";
}

//----------------------------------------------------------------------

#endif
