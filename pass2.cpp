void passTwo()
{
    objectOut <<"H" <<loadAddress <<segmentName <<beginAddress <<length <<'\n';
    
    Text line;
    Integer locationCounter;
    while(!intermediateIn.At_EOS())
    {
        intermediateIn >>line;
        Text operands;
	Text assembledLine;
	Character t = 'T';
	Text address = toHex(locationCounter);
	
	assembledLine.Add(0,t);
	assembledLine.Add(1,address[0]);
	assembledLine.Add(2,address[1]);
	
	Text op;
	op.Add(0,line[10]);
	op.Add(1,line[11]);
	if(line[12] != ' ')
	{
	    op.Add(2,line[12]);
	}

	Character theOp;
	theOp = machineTable.getOpcode(op);
	assembledLine.Add(3,theOp);
	
        for(Integer i=14; line[i] != ' '; i++)
        {
            operands.Add(operands.Length(), line[i]);
        }
	Text operand;
	operand = getNextOperand(operands);
	if(symbolTable.Check(operand))
	{
	    Text value = symbolTable.getValue(operand);
	    assembledLine.Add(4,value[0]);
	}
	else
	{
	    assembledLine.Add(4,operand[0]);
	}
	
        
       
        
        
        objectOut <<assenbledLine;
        listingOut <<assembledLine;
    }
}

Text getNextOperand(Text &t)
{
    Text op;
    Character c;
    while(t.Length() > 0)
    {
	t.Remove(0,c);
	if(c != ',' && c != '(' && c != ')')
	{
	    op.Add(op.Length(), c);
	}
	else
	{
	    break;
	}
    }
    return op;
}

Text toHex(Integer &i)
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
