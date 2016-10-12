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
    if(48 <= self.data[i] and self.data[i] < 57)
    {
	self.data[i] = To_int(self.data[i]) + 1;
    }
    else if(self.data[i] == 57)
    {
	self.data[i] == 65;
    }
    else if(65 <= self.data[i] and self.data[i] < 70)
    {
	self.data[i] = To_int(self.data[i]) + 1;
    }
    else if(self.data[i] == 70)
    {
	self.data[i] == 65;
    }
    else if(97 <= self.data[i] and self.data[i] < 102)
    {
	self.data[i] = To_int(self.data[i]) + 1;
    }
    else if(self.data[i] == 102)
    {
	self.data[i] == 48;
	if(i > 0)
	    increment(i-1);
    }
}

//----------------------------------------------------------------------

void Word::Clear()
{
    self.data = "-----";
}

//----------------------------------------------------------------------

#endif
