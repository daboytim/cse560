//  /*--------------------------------------------------------*\
//  |   Main Program: Lab 4 Linker/Loader
//  |*--------------------------------------------------------*|
//  |   Date:         02 March 2010
//  |   Author:       The Dougars
//  |   
//  \*--------------------------------------------------------*/

///-------------------------------------------------------------
/// Global Context ---------------------------------------------
///-------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "symbol_table.h"

///-------------------------------------------------------------
/// Interface --------------------------------------------------
///-------------------------------------------------------------

#define ASCII_NUMERIC_LOW 48
#define ASCII_NUMERIC_HIGH 57
#define ASCII_UPPER_A 65
#define ASCII_UPPER_F 70
#define ASCII_LOWER_A 97
#define ASCII_LOWER_F 102

Command_Line_Handler cmdLine;
Character_OStream stdOut;
Character_OStream execOut;
Character_OStream errLog;
Integer IPLA, footprint;
Text executionBegin;
Symbol_Table extSymbolTable;

//--------------------------------------------------------------

//checkFiles()
//Written by: Derek Boytim
//Edited by: The Dougars
Boolean checkFiles(Command_Line_Handler& cmdLine,Character_OStream& stdOut)
{
    for(Integer i=1; i<3; i++)
    {
	if(File_Exists(cmdLine[i]))
	{
	    stdOut << "Warning: File '" << cmdLine[i] << "' already exist, overwrite?(y/n) ";
	    Character_IStream in;
	    Character y_n;
	    in.Open_External("");
	    in >> y_n;
	    if(y_n != 'y' and y_n != 'Y')
	    {
		return false;
	    }
	    if(not File_Is_Writeable(cmdLine[i]))
	    {
		stdOut << "Error: File '" << cmdLine[i] << "' is not writeable.\n";
		return false;
	    }
	}
    }
    for(Integer i=3; i<cmdLine.Number_Of_Arguments(); i++)
    {
	if(not File_Exists(cmdLine[i]))
	{
	    stdOut << "Error: File '" << cmdLine[i] << "' does not exist.\n";
	    return false;
	}
	if(not File_Is_Readable(cmdLine[i]))
	{
	    stdOut << "Error: File '" << cmdLine[i] << "' is not readable.\n";
	    return false;
	}
    }
    return true;
}

//--------------------------------------------------------------

//representsAnInteger()
//Written by: Derek Boytim
//Edited by:The Dougars
Boolean representsAnInteger(Text &value)
{
    if(value.Length() == 0)
	return false;
    Integer i=0;
    if(value[i] == '-')
	i++;
    for(; i<value.Length(); i++)
    {
	if(value[i] < ASCII_NUMERIC_LOW || value[i] > ASCII_NUMERIC_HIGH)
       	{
	    return false;
	}
    }
    return true;
}

//--------------------------------------------------------------

//representsHex()
//Written by: Derek Boytim
//Edited by:The Dougars
Boolean representsHex(Text &hex)
{
    if(hex.Length() == 0)
	return false;
    for(Integer i; i<hex.Length(); i++)
    {
	if(!(ASCII_NUMERIC_LOW <= hex[i] && hex[i] <= ASCII_NUMERIC_HIGH) &&
	   !(ASCII_UPPER_A <= hex[i] && hex[i] <= ASCII_UPPER_F) &&
	   !(ASCII_LOWER_A <= hex[i] && hex[i] <= ASCII_LOWER_F))
       	{
	    return false;
	}
    }
    return true;
}

//--------------------------------------------------------------

//toInteger()
//Written by: Derek Boytim
//Edited by:The Dougars
Integer toInteger(Text &value)
{
    Integer val=0,i=0;
    Boolean isNegative;
    if(value[i] == '-')
    {
	isNegative = true;
	i++;
    }
    for(; i<value.Length(); i++)
    {
       	val *= 10;
       	val += To_Integer(value[i])-ASCII_NUMERIC_LOW;
    }
    if(isNegative)
    {
	val *= -1;
    }
    return val;
}

//--------------------------------------------------------------

//toHexAddress()
//Written by: Derek Boytim
//Edited by:The Dougars
Text toHexAddress(Integer i) //converts i to a two hex char string
			     //not necessasarily an address
                             //requires 0 <= i <= 255
{
    Text data;
    Character c;
    Integer tmp;
    
    for(int j=0; j<2; j++)
    {
	tmp=i%16;
	if (tmp == 0)
	    c = '0';
	else if (tmp == 1)
	    c = '1';
	else if (tmp == 2)
	    c = '2';
	else if (tmp == 3)
	    c = '3';
	else if (tmp == 4)
	    c = '4';
	else if (tmp == 5)
	    c = '5';
	else if (tmp == 6)
	    c = '6';
	else if (tmp == 7)
	    c = '7';
	else if (tmp == 8)
	    c = '8';
	else if (tmp == 9)
	    c = '9';
	else if (tmp == 10)
	    c = 'A';
	else if (tmp == 11)
	    c = 'B';
	else if (tmp == 12)
	    c = 'C';
	else if (tmp == 13)
	    c = 'D';
	else if (tmp == 14)
	    c = 'E';
	else if (tmp == 15)
	    c = 'F';
	data.Add(0, c);
	i/=16;
    }
    return data;
}

//--------------------------------------------------------------

//toDecimal()
//Written by: Derek Boytim
//Edited by: The Dougars
Integer toDecimal(Text &hex) //converts a 2 hex character value to the
			     //decimal equivalent; hex >= 0
{
    Integer s;
    for(Integer i=0; i<hex.Length(); i++)
    {
	s *= 16;
	if(ASCII_NUMERIC_LOW <= hex[i] and hex[i] <= ASCII_NUMERIC_HIGH)
	{
	    s += To_Integer(hex[i])-ASCII_NUMERIC_LOW;
	}
       	else if(ASCII_UPPER_A <= hex[i] and hex[i] <= ASCII_UPPER_F)
	{
	    s += To_Integer(hex[i])-(ASCII_UPPER_A-10);
	}
	else if(ASCII_LOWER_A <= hex[i] and hex[i] <= ASCII_LOWER_F)
	{
	    s += To_Integer(hex[i])-(ASCII_LOWER_A-10);
	}
    }
    return s;
}

//--------------------------------------------------------------

//getSymbol()
//Written by: Derek Boytim
//Edited by: The Dougars
Text getSymbol(Text &line)
{
    Text symbol;
    for(Integer i=1; i<7; i++)
    {
	if(line[i] == ' ')
	    break;
	symbol.Add(symbol.Length(), line[i]);
    }
    return symbol;
}

//--------------------------------------------------------------

//getSymbolValue()
 //Written by: Derek Boytim
//Edited by: The Dougars
Text getSymbolValue(Text &line)
{
    Text value;
    for(Integer i=7; i<9; i++)
    {
	value.Add(value.Length(), line[i]);
    }
    return value;
}

//--------------------------------------------------------------

//getSegmentName()
//Written by: Derek Boytim
//Edited by: The Dougars
Text getSegmentName(Text &line)
{
    Text name;
    for(Integer i=3; i<9; i++)
    {
	if(line[i] == ' ')
	    break;
	name.Add(name.Length(), line[i]);
    }
    return name;
}

//--------------------------------------------------------------

//getSegmentLength()
//Written by: Derek Boytim
//Edited by: The Dougars
Text getSegmentLength(Text &line)
{
    Text length;
    for(Integer i=11; i<13; i++)
    {
	length.Add(length.Length(), line[i]);
    }
    return length;
}

//--------------------------------------------------------------

//pass0()
//Written by: Derek Boytim
//Edited by: The Dougars
Boolean pass0()
{
    //loop through obj inputs, find Main, compute footprint size, get IPLA
    Character_IStream objIn, stdIn;
    Text mainExecBegin;
    Boolean mainFound = false;
    
    for(Integer i=3; i<cmdLine.Number_Of_Arguments(); i++)
    {
	objIn.Open_External(cmdLine[i]);
	Text header, name, length;
	objIn >>header;
	
   	if(header.Length() < 14)//no M at end
	{
	    errLog <<"Error: Object file '" <<cmdLine[i] <<"' is not relocatable.\n";
	    return false;
	}
	name = getSegmentName(header);
	if(name == "Main")
	{
	    if(!mainFound)//initial value is false
		mainFound = true;
	    else//already set
	    {
		errLog <<"Error: More than one 'Main' object file provided.\n";
		return false;
	    }
	}

	length = getSegmentLength(header);
	footprint += toDecimal(length);
	
	objIn.Close_External();
    }

    if(!mainFound)
    {
	errLog <<"Error: No object files specify 'Main'.\n";
	return false;
    }

    if(footprint > 256)
    {
	errLog <<"Error: Footprint is too large: "<<footprint <<'\n';
	return false;
    }

    Text input;
    stdIn.Open_External("");
    do
    {
	stdOut <<"The footprint size is " <<footprint <<" words.\n";
	stdOut <<"Provide an appropriate IPLA (hex): ";
	stdIn >>input;
    }while(!representsHex(input) || !(0 <= toDecimal(input) && toDecimal(input) <= 255));
    stdIn.Close_External();
    IPLA = toDecimal(input);
    return true;
}

//--------------------------------------------------------------

//pass1()
//Written by: Derek Boytim
//Edited by: The Dougars
Boolean pass1()
{
    //loop through files, find E records/put in EST
    Character_IStream objFile;
    Integer PLA = IPLA;
    
    for(Integer i=3; i<cmdLine.Number_Of_Arguments(); i++)
    {
	objFile.Open_External(cmdLine[i]);
	Text line, length, progName;
	objFile >>line; //header

	progName = getSegmentName(line);
	if(!extSymbolTable.check(progName))//add program name to EST
	{
	    Text other, addr=toHexAddress(PLA);
	    extSymbolTable.addSymbol(progName, addr, other);
	}
	else
	{
	    errLog <<"Error: External symbol '" <<progName <<"' already defined.\n";
	    return false;
	}
	
	length = getSegmentLength(line);

	if(progName == "Main")//set address where execution should begin
	{
	    Text execBegin;
	    execBegin.Add(0, line[1]);
	    execBegin.Add(1, line[2]);
	    executionBegin = toHexAddress(toDecimal(execBegin) + PLA);
	}
	
	while(!objFile.At_EOS())//find entry symbol definitions
	{
	    objFile >>line;
	    if(line[0] == 'E')
	    {
		Text symbol, value, other;
		symbol = getSymbol(line);
		value = getSymbolValue(line);
		if(extSymbolTable.check(symbol))
		{
		    errLog <<"Error: External symbol '" <<symbol <<"' already defined.\n";
		    return false;
		}
		else
		{
		    value = toHexAddress(toDecimal(value) + PLA);
		    extSymbolTable.addSymbol(symbol, value, other);
		}
	    }
	    else if(line[0] == 'T') //quit on first T record
	    {
		break;
	    }
	}
	PLA += toDecimal(length);//update PLA for next program 
	objFile.Close_External();
    }
}

//--------------------------------------------------------------

//pass2()
//Written by: Derek Boytim
//Edited by: The Dougars
void pass2()
{
    //loop through files, build object file/replace ext symbols with
    //values

    Character_IStream objFile;
    Text line;
    Integer PLA=IPLA;
    
    //build header
    Text iplaAddr = toHexAddress(IPLA), footHex = toHexAddress(footprint);
    execOut <<'H' <<executionBegin <<"Main  " <<iplaAddr <<footHex <<'\n';
    
    for(Integer i=3; i<cmdLine.Number_Of_Arguments(); i++)//loop through object files
    {
	objFile.Open_External(cmdLine[i]);
	objFile >>line;

	Text length = getSegmentLength(line);
	
	while(!objFile.At_EOS())
	{
	    objFile >>line;
	    if(line[0] == 'E') //ignore E records
	    {
		continue;
	    }
	    else if(line[0] == 'T')//is a Text record
	    {
		//relocate record
		Text addr;
		addr.Add(0, line[1]);
		addr.Add(1, line[2]);
		addr = toHexAddress(toDecimal(addr) + PLA);
		line[1] = addr[0];
		line[2] = addr[1];
		
		if(line.Length() == 8) //line is complete; echo
		{
		    execOut <<line <<'\n';
		}
		else//fix S field value
		{
		    if(line[8] == 'M')
		    {
			Character trash;
			line.Remove(8, trash);//remove M
			Text addr;
			addr.Add(0, line[6]);
			addr.Add(1, line[7]);
			addr = toHexAddress(toDecimal(addr) + PLA);
			line[6] = addr[0];
			line[7] = addr[1];
		    }
		    else if(line[8] == 'X')
		    {
			Character c;
			line.Remove(8, c); //remove X
			Text symbol;
			while(line.Length() > 8)//remove symbol from end of record
			{
			    line.Remove(8, c);
			    symbol.Add(symbol.Length(), c);
			}
			if(extSymbolTable.check(symbol))
			{
			    Text addr, value = extSymbolTable.getValue(symbol);
			    addr.Add(0, line[6]);
			    addr.Add(1, line[7]);
			    addr = toHexAddress(toDecimal(addr) + toDecimal(value));
			    line[6] = addr[0];
			    line[7] = addr[1];
			}
			else
			{
			    errLog <<"Error: EXT symbol '" <<symbol <<"' not defined.\n";
			    return;
			}
		    }
		    execOut <<line <<'\n';
		}
	    }
	}
	PLA += toDecimal(length); //update PLA for next program
	objFile.Close_External();
    }
}

//--------------------------------------------------------------

//linker()
//Written by: Derek Boytim
//Edited by: The Dougars
void linker()
{
    if(pass0())
	if(pass1())
	    pass2();
}

//--------------------------------------------------------------

//main()
//Written by: Derek Boytim
//Edited by: The Dougars
program_body main(end_user_command_line)
{
    stdOut.Open_External ("");
    cmdLine.Get_Command_Line(command_line_arguments);

    if(cmdLine.Number_Of_Arguments() < 4)
    {
	stdOut <<"Command line argument error\n";
        stdOut <<"\nUsage: % program-name 'output-executable' 'output-error-log' 'obj-1' ... 'obj-n'\n";
        return 1;
    }

    if(not File_Is_Writeable("."))
    {
	stdOut <<"Error: The current directory is not writeable!\n";
	return 1;
    }

    for(Integer i=1; i<cmdLine.Number_Of_Arguments(); i++)
    {
	if(File_Is_A_Directory(cmdLine[i]))
	{
	    stdOut << "Error: Parameter '" << cmdLine[i] << "' is a directory.\n";
	    return 1;
	}
    }

    if(not checkFiles(cmdLine, stdOut))
    {
	stdOut <<"Program quit. Command parameter error!\n";
	return 1;
    }

    execOut.Open_External(cmdLine[1]);
    errLog.Open_External(cmdLine[2]);
    
    linker();

    execOut.Close_External();
    errLog.Close_External();
}
