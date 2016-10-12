//  /*--------------------------------------------------------*\
//  |   Main Program: Lab 3 Assembler
//  |*--------------------------------------------------------*|
//  |   Date:         08 February 2010
//  |   Author:       The Dougars
//  |   Lab 4 Update  04 March 2010
//  |    -updates are marked with comments
//  \*--------------------------------------------------------*/

///-------------------------------------------------------------
/// Global Context ---------------------------------------------
///-------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "machine_table.h"
#include "pseudo_table.h"
#include "symbol_table.h"
#include "literal_table.h"
#include "intermediate_table.h"

///-------------------------------------------------------------
/// Interface --------------------------------------------------
///-------------------------------------------------------------

#define MAX_SYMBOLS 100
#define MAX_LITERALS 50
#define MAX_RECORDS 200
#define INT_UPPER 57
#define INT_LOWER 48
#define NOT_CAPS_UPPER 122
#define NOT_CAPS_LOWER 97
#define CAPS_LOWER 65
#define CAPS_UPPER 90
#define MAX_MEMORY 255

Character_OStream stdOut;
Character_IStream assemblySource;
Character_OStream objectOut;
Character_OStream listingFile;
Machine_Table machineTable;
Pseudo_Op_Table pseudoTable;
Symbol_Table symbolTable;
Literal_Table literalTable;
Integer locationCounter, beginAddress, recordCount;
Text executionBegin, segmentName, loadAddress, length;
Boolean isRelocatable, pass1Error;
Intermediate_Table interTable;
	    
//------------------------------CAPS--------------------------------
//getNextOperand()
//Written by: Derek Boytim,
//Edited by:The Dougars
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

//--------------------------------------------------------------
//representsAnInteger()
//Written by: Derek Boytim
//Edited by:The Dougars
Boolean representsAnInteger(Text &t)
{
    if(t.Length() == 0)
	return false;
    Integer i=0;
    if(t[i] == '-')
	i++;
    for(; i<t.Length(); i++)
    {
	if(t[i] < INT_LOWER || t[i] > INT_UPPER)
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
Integer toInteger(Text &t)
{
    Integer val=0,i=0;
    Boolean isNegative;
    if(t[i] == '-')
    {
	isNegative = true;
	i++;
    }
    for(; i<t.Length(); i++)
    {
       	val *= 10;
       	val += To_Integer(t[i])-48;
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
//toHexValue()
//Written by: Derek Boytim
//Edited by:The Dougars
Text toHexValue(Integer i) //converts i to a five hex character 2's
			   //complement value
{
    Text data;
    Character c;
    Boolean isNegative;
    if(i < 0)
    {
	isNegative = true;
	i *= -1;
    }
    Integer tmp;
    for(int j=0; j<5; j++)
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
    if(isNegative)//get 2's complement
    {
	Boolean firstOne = false;;
	for(int j=4; j>=0; j--)
	{
	    Character c = data[j];
	    if(!firstOne)
	    {
		if (c == '0')
		    c = '0';
		else if (c == '1')
		    c = 'F';
		else if (c == '2')
		    c = 'E';
		else if (c == '3')
		    c = 'D';
		else if (c == '4')
		    c = 'C';
		else if (c == '5')
		    c = 'B';
		else if (c == '6')
		    c = 'A';
		else if (c == '7')
		    c = '9';
		else if (c == '8')
		    c = '8';
		else if (c == '9')
		    c = '7';
		else if (c == 'A')
		    c = '6';
		else if (c == 'B')
		    c = '5';
		else if (c == 'C')
		    c = '4';
		else if (c == 'D')
		    c = '3';
		else if (c == 'E')
		    c = '2';
		else if (c == 'F')
		    c = '1';
	    }
	    else
	    {
		if (c == '0')
		    c = 'F';
		else if (c == '1')
		    c = 'E';
		else if (c == '2')
		    c = 'D';
		else if (c == '3')
		    c = 'C';
		else if (c == '4')
		    c = 'B';
		else if (c == '5')
		    c = 'A';
		else if (c == '6')
		    c = '9';
		else if (c == '7')
		    c = '8';
		else if (c == '8')
		    c = '7';
		else if (c == '9')
		    c = '6';
		else if (c == 'A')
		    c = '5';
		else if (c == 'B')
		    c = '4';
		else if (c == 'C')
		    c = '3';
		else if (c == 'D')
		    c = '2';
		else if (c == 'E')
		    c = '1';
		else if (c == 'F')
		    c = '0';
	    }
	    if(!firstOne && data[j] != '0')
	    {
		firstOne = true;
	    }
	    data[j] &= c;
	}
    }
    
    return data;
}

//----------------------------------------------------------------
//checkFiles()
//Written by: Derek Boytim
//Edited by:The Dougars
Boolean checkFiles(Command_Line_Handler& cmdLine,Character_OStream& stdOut)
{
    for(Integer i=2; i<4; i++)
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
	    if(!File_Is_Writeable(cmdLine[i]))
	    {
		stdOut << "Error: File '" << cmdLine[i] << "' is not writeable.\n";
		return false;
	    }
	}
    }
    for(Integer i=1; i<2; i++)
    {
	if( !File_Exists(cmdLine[i]))
	{
	    stdOut << "Error: File '" << cmdLine[i] << "' does not exist.\n";
	    return false;
	}
	if( !File_Is_Readable(cmdLine[i]))
	{
	    stdOut << "Error: File '" << cmdLine[i] << "' is not readable.\n";
	    return false;
	}
    }
    return true;
}

//--------------------------------------------------------------
//getLabel()
//Written by: Peter Boyer
//Edited by:The Dougars
Text getLabel(preserves Text& line)
{
    Text label;
    for(Integer i=0; i < 6; i++)
    {
	if(line[i] == ' ')
	    break;
	if((line[i]>= INT_LOWER and line[i] <= INT_UPPER) or
	   (line[i]>= CAPS_LOWER and line[i] <= CAPS_UPPER) or
           (line[i]>= NOT_CAPS_LOWER and line[i] <= NOT_CAPS_UPPER))
        {
	    label.Add(label.Length(), line[i]);
	}
	else
	{
	    listingFile<< "Error: "<<label<<" has incorrect label format\n";
	    return "";
	}
    }
    if(label.Length()>0)
    {
        if(! (label[0]>= CAPS_LOWER and label[0] <= CAPS_UPPER) and
           ! (label[0]>= NOT_CAPS_LOWER and label[0] <= NOT_CAPS_UPPER))
        {
	    listingFile<< "Error: "<<label<<" has incorrect label format\n";
	    return "";
        }
    }
    return label;
}

//--------------------------------------------------------------
//getOperation()
//Written by: Peter Boyer
//Edited by:The Dougars
Text getOperation(preserves Text& line)
{
    object Text operation;
    if(line.Length()>=10)
    {
	operation.Add(0,line[9]);
	operation.Add(1,line[10]);
	if(line[11] != ' ')
	    operation.Add(2,line[11]);
    }
    else
    {
        listingFile<< "Error: Missing operation\n";
    }
    return operation;
}

//--------------------------------------------------------------
//getOperands()
//Written by: Peter Boyer
//Edited by:The Dougars
Text getOperands(preserves Text& line)
{
    object Text operands;
    
    for(Integer i=13; i < line.Length(); i++)
    {
	if(line[i] == ' ')
	    break;
	operands.Add(operands.Length(), line[i]);
    }
    return operands;
}
//--------------------------------------------------------------
//getCCDOperands()
//Written by: Peter Boyer
//Edited by:The Dougars
Text getCCDOperands(preserves Text& line)//Operation will return two
					 //CCD operands 
{
    object Text operands;
    
    for(Integer i=13; i < 15; i++)
    {
	if (line.Length()>=15)
	    operands.Add(operands.Length(), line[i]);
	else
	    listingFile <<"Error: Operand on CCD line not two characters.\n";  
    }
    if(operands.Length()!=2)
    {
          listingFile <<"Error: Operand on CCD line not two characters.\n";
    }
    return operands;
}

//--------------------------------------------------------------
//putListingFileHeader()
//Written by: Derek Boytim
//Edited by:The Dougars
void putListingFileHeader()
{
    Text header0 = "+------------------------------------------------------------+\n";
    Text header1 = "|                         Listing                            |\n";
    Text header2 = "|------------------------------------------------------------|\n";
    Text header3 = "|   Object Code     |                 Source                 |\n";
    Text header4 = "|-------------------+----------------------------------------|\n";
    Text header5 = "| Loc  Data   Rel   | Rec   Label    OP    Operands/Comments |\n";
    Text header6 = "|(hex) (hex)        |(dec)                                   |\n";
    Text header7 = "|-------------------+----------------------------------------+\n";
    listingFile <<header0 <<header1 <<header2 <<header3 <<header4 <<header5 <<header6 <<header7;
}

//--------------------------------------------------------------
//pass1()
//Written by: Daren Hrelic and Peter Boyer
//Edited by:The Dougars
void pass1()
{
    
    //read in first non-comment line of source
    object Integer ENT_count=0, ENT_checker=0;;
    object Boolean firstOp=false, ENT_check=false;;
    object Text line, line2;
    if(!assemblySource.At_EOS())
    {
        assemblySource >> line;
	recordCount++;
	//check for non-empty source and non-comment
        while(!assemblySource.At_EOS() and line[0] == ';')
        {
	    assemblySource >> line;
	    recordCount++;
        }
    }
    else
    {
     	listingFile << "Error: Insufficient Number of Records in Source file.\n";
	pass1Error = true;
	
    }
//---Check for ORI, initialze global variables
    line2 = line;
    interTable.add_inter_line(line2);
    
    if( getOperation(line) != "ORI")
    {
        listingFile << "Error: Missing ORI operation in source file.\n";
	pass1Error = true;
	return;
    }
    else
    {
        object Text label, operand, value, info;
        label=getLabel(line);
	if(label.Length() == 0)
	{
	    listingFile <<"Error: A label must be provided on ORI line.\n";
	    pass1Error = true;
	}
        segmentName=label;
        operand=getOperands(line);
	object Integer val;
	if(operand=="")
	{
	    locationCounter=0;
	    beginAddress=0;
	    value="0";
	    loadAddress = "00";
	    info = "R";
	    isRelocatable = true;
	}
	else
	{
            value=getNextOperand(operand);
	    val=toInteger(value);
	    
	    if(val < 0)
	    {
		listingFile <<"Error: ORI operand not an integer.\n";
		pass1Error = true;
		return;
	    }
            loadAddress=toHexAddress(val);
            locationCounter=val;
	    beginAddress=val;
	    info = "A";
	}
        symbolTable.addSymbol(label,value,info);
    }
    
//----Next line read in
    if(!assemblySource.At_EOS())
    {
        do
	{
	    assemblySource >> line;
	    if(line.Length() == 0)
	    {
	       listingFile << "Error: Empty line in source file.\n";
	       pass1Error=true;
	       return; 
	    }
	    recordCount++;
	}while(line[0] == ';' and !assemblySource.At_EOS());
	recordCount++;
	line2=line;
	interTable.add_inter_line(line2);
    }
    else
    {
	listingFile << "Error: Insufficient records in source file.\n";
	pass1Error=true;
    }
    
//---main pass loop
    //break down line, including CCD
    Text label = getLabel(line),
	op = getOperation(line),
	operands;
    if(op=="CCD")
    {
	operands = getCCDOperands(line);
    }
    else
    {
        operands = getOperands(line);
    }
   
//--------UPDATE FOR LAB4---------------
    // External Sybol Modification
    while( op=="ENT" or op=="EXT" )
    {
        //Check for Symbol Errors
        if(label.Length()!=0)
	{
	    listingFile <<"Error:EXT and ENT Pseudo Ops cannot have a Label" <<'\n';
	    pass1Error=true;
            return;
	}
	if(operands.Length()==0)
	{
	    listingFile <<"Error:EXT and ENT Pseudo Ops must contain at least one operand."<<'\n';
	    pass1Error=true;
            return;
	}
	Integer COUNT=0;
	//Add EXT name to symbol table
        if(op=="EXT")
	{
	    while(operands.Length()!=0)
	    {
	        Text EXT_operand=getNextOperand(operands);
		Text filler="X";
		Text blank="--";
		symbolTable.addSymbol(EXT_operand,blank,filler);
		COUNT++;
	    }	    
        }
	else if(op=="ENT")
	{
            //Add ENT  &*symbolname to symbol table to avoid symbol confusion
	    while(operands.Length()!=0)
	    {
		object Text ENT_operand=getNextOperand(operands);
		Text filler="E";
		Text blank="00";
		Character z='&';
		ENT_operand.Add(0,z);
		symbolTable.addSymbol(ENT_operand,blank,filler);
		ENT_count++;
		ENT_checker++;
		COUNT++;
	    }	 
	}
        //Check to ensure there are not more than 4 operands
	if(COUNT>4)
	{
	    listingFile <<"Error:Neither EXT or ENT can contain more than four operands" <<'\n';
	    pass1Error=true;
            return;
	}
	//read in next line of input
        if(!assemblySource.At_EOS())
        {
            assemblySource >> line;
	    while(!assemblySource.At_EOS() and line[0] == ';')
            {
	    assemblySource >> line;
	    recordCount++;
            }
	    if(line.Length() == 0)
	    {
	        listingFile << "Error: Empty line in source file.\n";
	        pass1Error=true;
	        return;
	    }
	    recordCount++;
	    line2=line;
	    interTable.add_inter_line(line2);
	    label = getLabel(line);
	    op = getOperation(line);	    
	    if(op=="CCD")
	    {
		operands = getCCDOperands(line);
	    }
	    else
	    {
		operands = getOperands(line);
	    }

        }
        else
        {
	    listingFile << "Error: Insufficient records in source file.\n";
	    pass1Error=true;
        }
 	
    }
//-----Lab 4 Update End--------
     
    while(op != "END") //--------1
    {
	Boolean ENT_check=false;
	if(recordCount>=MAX_RECORDS)
	{
	    listingFile <<"Error: Number of source records exceeds 200.\n";
	    pass1Error=true;
	    return;
	}

	if(!machineTable.check(op) && !pseudoTable.check(op))
	{
	    listingFile <<"Error: Invalid operation \"" <<op <<"\".\n";
	    pass1Error=true;
	    return;
	}
	
//----Lab 4 update-----
	//---Ensure ENT and EXT are in beginning of records
	if(op=="ENT" or op =="EXT")
	{
	    listingFile <<"Error: ENT and EXT operations can only be preceded by the operation ORI."<<'\n';
	    pass1Error=true;
	    return;
	}
	//Check symbol table for symbol repetion or appearance of ENT
	Character z='&';
	Text label_ENT=label;
	label_ENT.Add(0,z);
	if(symbolTable.check(label_ENT) or symbolTable.check(label) )
	{

	    //----ENT Symbol setup---
	    Text temp_label=label_ENT;
	    Text value=symbolTable.getOther(temp_label);
	    if(value[0]=='E')
	    {
		Text t_1,t_2,E="E";
		symbolTable.remove(temp_label,t_1,t_2);
		object Text value_loc = toHexAddress(locationCounter);
		symbolTable.addSymbol(temp_label,value_loc,E);
		ENT_checker--;

	    }
	    else
	    {
	    listingFile<< "Error: Label \""<<label << "\" cannot be used more than once.\n";
	    pass1Error = true;
	    ENT_check=true;
	    }
	}
	
//-----Lab 4 Update End--------
        if(ENT_check==false)
	{
	    
	    if(op=="EQU")
	    {						   
		object Text operand = getNextOperand(operands), value, absRel;
		if(label.Length() == 0)
		{
		    listingFile <<"Error: No label on EQU line.\n";
		    pass1Error = true;
		    return;
		}
		if(operand.Length() == 0)
		{
		    listingFile <<"Error: No operand on EQU line.\n";
		    pass1Error = true;
		    return;
		}

		if(symbolTable.check(operand))
		{
		    value = symbolTable.getValue(operand);
		    absRel = symbolTable.getOther(operand);
		    symbolTable.addSymbol(label, value, absRel);
		}
		else
		{
		    if(representsAnInteger(operand))
		    {   
			value = toHexAddress(toInteger(operand));		       			absRel="A";
			symbolTable.addSymbol(label, operand, absRel);
		    }
		    else
		    {
			listingFile <<"Error: Operand on EQU line is not a symbol or an integer.\n";
			pass1Error = true;
			return;
		    }
		}
	    }
	    else if(label.Length() > 0)
	    {

		 object Text value = toHexAddress(locationCounter), r = "R";
		 symbolTable.addSymbol(label, value, r);
		 
	    }
	}

//----Check if machine_op or pseudo_op
	if (pseudoTable.check(op)) //------4
	{
	    if(op == "RES")
	    {
		Text operand = getNextOperand(operands);
		if(representsAnInteger(operand))
		{
		    locationCounter += toInteger(operand);
		}
		else
		{
		    listingFile <<"Error: Operand on RES line is not an integer.\n";
		    pass1Error = true;
		    return;
		}
	    }
	    else
	    {
		object Integer let=pseudoTable.getLength(op);
		locationCounter=locationCounter+let;
	    }
	}
	else if(machineTable.check(op))
	{
	    if(firstOp==false)
	    {
		executionBegin=toHexAddress(locationCounter);
		firstOp=true;
	    }
	    locationCounter++;
	}
	else
	{
	    listingFile<< "Error: "<<op <<" is an unknown operation.\n";
	}
//----Literals-------       
	object Text operand = getNextOperand(operands);//R field
	operand = getNextOperand(operands);//S field
	
	if (operand.Length()>0) //--------5
	{
	    if(operand[0] == '=')
	    {
		if(!literalTable.check(operand))
		{
		    object Character trash = ' ';
		    object Text value=operand, address="empty";
		    value.Remove(0,trash);
		    literalTable.addLiteral(operand, address, value);
		}
	    }
	}

//read in next line
	if(!assemblySource.At_EOS())
	{
	    do
	    {
	        assemblySource >> line;
		if(line.Length() == 0)
		{
		   listingFile << "Error: Empty line in source file.\n";
		   pass1Error=true;
		   return; 
		}
		recordCount++;
	    }while(line[0] == ';' and !assemblySource.At_EOS());
	    
	    line2=line;
	    interTable.add_inter_line(line2);
	}
	else
	{
	    listingFile << "Error: Missing END operation in source file.\n";
	    pass1Error=true;
	    break;
	}
	label = getLabel(line),
	op = getOperation(line);
	if(op=="CCD")
	{
	    operands = getCCDOperands(line);
	}
	else
	{  
            operands = getOperands(line);
	}
	
	if(op == "END" && label.Length() != 0)
	{
	    listingFile << "Error: Label on END line.\n";
	    pass1Error=true;
	    break;
	}
	if(op == "ORI")
	{
	    listingFile << "Error: Multiple ORI lines.\n";
	    pass1Error=true;
	    break;
	}
    }
    
//set address of literal in hex, right now literals are placed

    if(line.Length()>=13)
    {
	Text operand = getOperands(line);
	if(symbolTable.check(operand))
	{
	    if(isRelocatable && symbolTable.getOther(operand) == "A")
	    {
		listingFile <<"Error: Program is relocatable; symbol on END line must be relative.\n";
		pass1Error = true;
	    }
	    else
	    {
		executionBegin = symbolTable.getValue(operand);
	    }
	}
	else
	{

	    if(operand.Length()>0)
	    {
	    executionBegin = toHexAddress(toInteger(operand));
	    }
	    
	}
    }
    
    if(literalTable.size()>=MAX_LITERALS)
    {
	listingFile<< "Error: Number of Literals exceeds " <<MAX_LITERALS <<'\n';
	pass1Error=true;
    }
    
    Literal_Table tmpTable;
    while(literalTable.size()>0)
    {
	object Text t_1, t_2, t_3;
	literalTable.removeAny(t_1,t_2,t_3);
	t_2=toHexAddress(locationCounter);
	tmpTable.addLiteral(t_1,t_2,t_3);
	locationCounter++;
    }
    while(tmpTable.size()>0)
    {
	object Text t_1, t_2, t_3;
	tmpTable.removeAny(t_1,t_2,t_3);
	literalTable.addLiteral(t_1,t_2,t_3);
    }

//Check for location counter exceeding memory bounds
    if(locationCounter>MAX_MEMORY)
    {
        listingFile<< "Error:Location Counter outside of" <<MAX_MEMORY <<" bound."<<'\n';
	pass1Error=true;
    }
    object Integer len=locationCounter-beginAddress;
    length=toHexAddress(len);

//Check for max records and max symbols
    if(recordCount>=MAX_RECORDS)
    {
	listingFile<< "Error: Number of Source Records exceeds " <<MAX_RECORDS <<'\n';
	pass1Error=true;
    }
    //Ensure that additional table elements dont effect table size restriction
    if((symbolTable.size()-ENT_count)>=MAX_SYMBOLS)
    {
	listingFile<< "Error: Number of Symbols exceeds " <<MAX_SYMBOLS <<'\n';
	pass1Error=true;
    }
    if(ENT_checker!=0)
    {
	listingFile<< "Error: ENT symbol not used in source file " <<'\n';
	pass1Error=true;
    }
}
//--------------------------------------------------------------
//pass2()
//Written by: Derek Boytim and Chris Fenner
//Edited by:The Dougars
void pass2()
{
    Integer numRecords;
    putListingFileHeader();

    //output header record
    locationCounter=beginAddress;
    
    while(segmentName.Length() < 6)
       segmentName.Add(segmentName.Length(), ' ');

    objectOut <<"H" <<executionBegin <<segmentName <<loadAddress <<length;
    if(isRelocatable)
	objectOut <<'M';
    objectOut<<'\n';
    Text line;
    while(interTable.inter_size()>0)//Change: Continue removing until
				    //intertable is empty
    {
//-----Lab 4 update--------------------
	//----Create EXT flag------
	Boolean EXT_flag=false;
//-----Lab 4 update end----------------	    
	
	line = interTable.get_inter_line();//Change: get line from
					   //inter table
	if(line.Length() == 0 || line[0] == ';') //skip commented lines
	    continue;
	
        Text op, operands, assembledLine;
	//get op from line
	op = getOperation(line);

	//get operands from line
	if(op=="CCD")
	{
	    operands = getCCDOperands(line);
	}
	else
	{  
            operands = getOperands(line);
	}

	
//-----Lab 4 update--------------------
	//----Create ENT record------
	if(op=="ENT")
	{
	    while(operands.Length()>0)
	    {
		assembledLine.Clear();
		Text temp_op=getNextOperand(operands);
		Text oper=temp_op, i_value,loc_val, temp_loc_val;
		Character z='&';
		temp_op.Add(0,z);
		symbolTable.remove(temp_op,i_value,temp_loc_val);
		Integer loc=toInteger(i_value);
	        loc_val =toHexAddress(loc);
	        assembledLine.Add(0,'E');
		temp_op.Remove(0,z);
		Integer j=1, limit=temp_op.Length()+1;
		while(j<7)
		{
	            if(j<limit)
	            {
		        Character c;
		        temp_op.Remove(0,c);
	                assembledLine.Add(j,c);
		    }
		    else
		    {
		        Character c=' ';
	                assembledLine.Add(j,c);
		    }
		    j++;
                }
		//assembledLine.Add(j,'=');
		assembledLine.Add(j,loc_val[0]);
		assembledLine.Add(j+1,loc_val[1]);
		objectOut <<assembledLine <<'\n';
		//Once line is assembled remove ENT symbol from table
		//Add E to "other" to ensure relocatability
		//&*Symbolname will be removed from table
		Text E;
		symbolTable.remove(oper,i_value,E);
		E="E";
		symbolTable.addSymbol(oper,i_value,E);
       	    }
	assembledLine.Clear();    
	}
	else
        {
//-----Lab 4 update end-----------	    
	    //add T and address to text record
	    Text address = toHexAddress(locationCounter);
	    assembledLine.Add(0,'T');
	    assembledLine.Add(1,address[0]);
	    assembledLine.Add(2,address[1]);
	    if(machineTable.check(op)) //---------MACHINE OP---------//
	    {
		if(operands.Length() == 0)
		{
		    listingFile <<"Error: No operands present with machine op.\n";
		    continue;
		}
	    
		//assemble the OP
		Character theOp;
		theOp = machineTable.getOpcode(op);
		assembledLine.Add(3,theOp);
		//assemble the R
		Text operand;
		operand = getNextOperand(operands);
		
//-----Lab 4 update-----------
		//--check for EXT symbol----
		if(symbolTable.check(operand))
		{
		    
		    Text value = symbolTable.getValue(operand);
		    Integer i_value=toInteger(value);
		    Text test=symbolTable.getOther(operand);
  
		    if(test[0]=='X')
		        {
			   
			    EXT_flag=true;
			    assembledLine.Add(8,'X');
			    Integer j=9,limit=operand.Length()+9;
   	                    for(j;j<limit;j++)
	                    {
				Character c;
				operand.Remove(0,c);
	                        assembledLine.Add(j,c);
                            }
		         
			}
                    if(i_value>=1 and i_value<=3)
		    {
		        assembledLine.Add(4,value[0]);
		    }
		    else
		    {
		      listingFile <<"Error: R field integer is out of range.\n";
			continue;
		    }
//-----Lab 4 update end-----------      
		    
		}
		else if(representsAnInteger(operand))
		{
		    if(toInteger(operand) < 0 || toInteger(operand) > 3)
		    {
			listingFile <<"Error: R field integer is out of range.\n";
			continue;
		    }

		    assembledLine.Add(4,operand[0]);
		}
	
		else
		{
		    listingFile <<"Error: R field is not a symbol or an integer.\n";
		    continue;
		}

		//initialize X
		assembledLine.Add(5,'0');

		//assemble the S
		operand = getNextOperand(operands);
		if(operand.Length() == 0)
		{
		    listingFile <<"Error: Not enough operands with machine op.\n";
		    continue;
		}
		else
		{  
		    if(operand[0] == '=')
		    {
			Text value = literalTable.getAddress(operand);
			//Text value = toHexAddress(toInteger(literal));
			assembledLine.Add(6,value[0]);
			assembledLine.Add(7,value[1]);
			if(isRelocatable)
			    assembledLine.Add(8,'M');
		    
		    }
		    else if(symbolTable.check(operand))
		    {
			Text value = symbolTable.getValue(operand);
			//value = toHexAddress(toInteger(value));
			assembledLine.Add(6,value[0]);
			assembledLine.Add(7,value[1]);
       			Text relative = symbolTable.getOther(operand);
//-----Lab 4 update-----------
		//--check for EXT symbol----
		        if(relative[0]=='X')
		        { 
			    EXT_flag=true;
			    Character def='0';
			    assembledLine[6]=def;
			    assembledLine[7]=def;
			    assembledLine.Add(8,'X');
			    Integer j=9,limit=operand.Length()+9;
	                    for(j;j<limit;j++)
	                    {
				Character c;
				operand.Remove(0,c);
	                        assembledLine.Add(j,c);
                            }
		        }
			
//-----Lab 4 update end-----------    
			else if(isRelocatable && (relative[0] == 'R' or relative[0]=='E'))
			{
      			    assembledLine.Add(8,'M');

			}
		    }
		    else if(representsAnInteger(operand))
		    {
			Integer integer = toInteger(operand);
			if(0 <= integer && integer <= 255)
			{
			    
			    Text value = toHexAddress(integer);
			    assembledLine.Add(6,value[0]);
			    assembledLine.Add(7,value[1]);

			    
			}
		    }
		    else
		    {
			listingFile <<"Error: Invalid S field operand.\n";
			continue;
		    }

		}

		//assemble the X
		operand = getNextOperand(operands);
		if(operand.Length() != 0)
		{
		    Character c;
		    if(symbolTable.check(operand))
		    {
			Text value = symbolTable.getValue(operand);
			assembledLine.Remove(5,c);
			assembledLine.Add(5,value[0]);
			Text relative = symbolTable.getOther(operand);
//-----Lab 4 update-----------
		//--check for EXT symbol----
		        if(relative[0]=='X')
		        {
			    
			    EXT_flag=true;
			    assembledLine.Add(8,'X');
			    Integer j=9,limit=operand.Length()+9;
	                    for(j;j<limit;j++)
	                    {
				Character c;
				operand.Remove(0,c);
	                        assembledLine.Add(j,c);
                            }
		         }
			
//-----Lab 4 update end-----------    
			
		    }
		    else if(operand == "0")
		    {
			//do nothing; already taken care of
		    }
		    else if(operand == "1")
		    {
			assembledLine.Remove(5,c);
			assembledLine.Add(5,'4');
		    }
		    else if(operand == "2")
		    {
			assembledLine.Remove(5,c);
			assembledLine.Add(5,'8');
		    }
		    else if(operand == "3")
		    {
			assembledLine.Remove(5,c);
			assembledLine.Add(5,'C');
		    }
		    else
		    {
			listingFile <<"Error: Invalid X field operand.\n";
			continue;
		    }
		  
		}
 
		objectOut <<assembledLine <<'\n';
		locationCounter++;
		numRecords++;
		Text lineTemp = assembledLine, spaces;
		if(numRecords < 10)
		    spaces = "     ";
		else if(numRecords < 100)
		    spaces = "    ";
		else
		    spaces = "   ";
		Character trash;
		lineTemp.Remove(0, trash);
		lineTemp.Add(2, ' ');
		lineTemp.Add(2, ' ');
		lineTemp.Add(2, ' ');

		while(lineTemp.Length() < 14)
		    lineTemp.Add(10, ' ');

		//--------------

		line.Add(12, ' ');
		line.Add(12, ' ');
		listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';

	    }
	    else if(pseudoTable.check(op)) //--------PSEUDO OP---------//
            {  
		//Assemble the Op
		Text operand = getNextOperand(operands);

		//Check for incorrect operand values 
		if(op == "NMD")
		{
		  
		    if(!representsAnInteger(operand))
		    {
			listingFile <<"Error: Operand on NMD line is not an integer.\n";
			continue;
		    }
		    Text value = toHexValue(toInteger(operand));
		    assembledLine.Add(3,value[0]);
		    assembledLine.Add(4,value[1]);
		    assembledLine.Add(5,value[2]);
		    assembledLine.Add(6,value[3]);
		    assembledLine.Add(7,value[4]);	    

		    objectOut <<assembledLine <<'\n';
		    locationCounter++;
		    numRecords++;

		    Text lineTemp = assembledLine, spaces;
		    if(numRecords < 10)
			spaces = "     ";
		    else if(numRecords < 100)
			spaces = "    ";
		    else
			spaces = "   ";

		    Character trash;
		    lineTemp.Remove(0, trash);
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    while(lineTemp.Length() < 14)
			lineTemp.Add(10, ' ');

		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';
		}
		else if(op == "CCD")
		{
		    if(operand.Length() < 2)
		    {
			listingFile <<"Error: Operand on CCD line not two characters.\n";
			continue;
		    }
		    Text char1 = toHexAddress(To_Integer(operand[0]));
		    Text char2 = toHexAddress(To_Integer(operand[1]));
		    assembledLine.Add(3,char1[0]);
		    assembledLine.Add(4,char1[1]);
		    assembledLine.Add(5,char2[0]);
		    assembledLine.Add(6,char2[1]);
		    assembledLine.Add(7,'0');

		    objectOut <<assembledLine <<'\n';
		    locationCounter++;
		    numRecords++;

		    Text lineTemp = assembledLine, spaces;
		    if(numRecords < 10)
			spaces = "     ";
		    else if(numRecords < 100)
			spaces = "    ";
		    else
			spaces = "   ";

		    Character trash;
		    lineTemp.Remove(0, trash);
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    while(lineTemp.Length() < 14)
			lineTemp.Add(10, ' ');

		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';
		}
		else if(op == "RES")
		{
		    Integer res;
		    if(symbolTable.check(operand))
		    {
			Text value = symbolTable.getValue(operand);
			res = toInteger(value);
			Text relative = symbolTable.getOther(operand);
//-----Lab 4 update-----------
		//--check for EXT symbol----
		        if(relative[0]=='X')
		        {
			    EXT_flag=true;
			    assembledLine.Add(8,'X');
			    Integer j=9,limit=operand.Length()+9;
	                    for(j;j<limit;j++)
	                    {
				Character c;
				operand.Remove(0,c);
	                        assembledLine.Add(j,c);
                            }
		        }
			
//-----Lab 4 update end-----------    
		    }
		    else if(representsAnInteger(operand))
		    {
			res = toInteger(operand);
		    }
		    else
		    {
			listingFile <<"Error: Operand on RES line is not a symbol or an integer.\n";
			continue;
		    }
		    if(0 <= res && res <= 255)
			locationCounter += res;
		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"|                   |       " <<line <<'\n';
		}
		else if(op == "RET")
		{
		    Integer reg = toInteger(operand);
		    assembledLine.Add(3,'C');
		    assembledLine.Add(4,'3');
		    if(reg == 0)
		    {
			assembledLine.Add(5,'0');
		    }
		    else if(reg == 1)
		    {
			assembledLine.Add(5,'4');
		    }
		    else if(reg == 2)
		    {
			assembledLine.Add(5,'8');
		    }
		    else if(reg == 3)
		    {
			assembledLine.Add(5,'C');
		    }
		    //Assemble S
		    assembledLine.Add(6,'0');
		    assembledLine.Add(7,'0');

		    objectOut <<assembledLine <<'\n';
		    locationCounter++;
		    numRecords++;
		
		    Text lineTemp = assembledLine, spaces;
		    if(numRecords < 10)
			spaces = "     ";
		    else if(numRecords < 100)
			spaces = "    ";
		    else
			spaces = "   ";

		    Character trash;
		    lineTemp.Remove(0, trash);
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    while(lineTemp.Length() < 14)
			lineTemp.Add(10, ' ');

		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';
		}
		else if(op == "GTC")
		{
		    Integer reg = toInteger(operand);
		    assembledLine.Add(3,'B');
		    assembledLine.Add(4,'1');
		    if(reg == 0)
		    {
			assembledLine.Add(5,'0');
		    }
		    else if(reg == 1)
		    {
			assembledLine.Add(5,'4');
		    }
		    else if(reg == 2)
		    {
			assembledLine.Add(5,'8');
		    }
		    else if(reg == 3)
		    {
			assembledLine.Add(5,'C');
		    }
		    //Assemble S
		    assembledLine.Add(6,'0');
		    assembledLine.Add(7,'0');

		    objectOut <<assembledLine <<'\n';
		    locationCounter++;
		    numRecords++;
		
		    Text lineTemp = assembledLine, spaces;
		    if(numRecords < 10)
			spaces = "     ";
		    else if(numRecords < 100)
			spaces = "    ";
		    else
			spaces = "   ";

		    Character trash;
		    lineTemp.Remove(0, trash);
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    while(lineTemp.Length() < 14)
			lineTemp.Add(10, ' ');

		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';
		}
		else if(op == "PTC")
		{
		    Integer reg = toInteger(operand);
		    assembledLine.Add(3,'B');
		    assembledLine.Add(4,'3');
		    if(reg == 0)
		    {
			assembledLine.Add(5,'0');
		    }
		    else if(reg == 1)
		    {
			assembledLine.Add(5,'4');
		    }
		    else if(reg == 2)
		    {
			assembledLine.Add(5,'8');
		    }
		    else if(reg == 3)
		    {
			assembledLine.Add(5,'C');
		    }
		    //Assemble S
		    assembledLine.Add(6,'0');
		    assembledLine.Add(7,'0');

		    objectOut <<assembledLine <<'\n';
		    locationCounter++;
		    numRecords++;
		
		    Text lineTemp = assembledLine, spaces;
		    if(numRecords < 10)
			spaces = "     ";
		    else if(numRecords < 100)
			spaces = "    ";
		    else
			spaces = "   ";

		    Character trash;
		    lineTemp.Remove(0, trash);
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    lineTemp.Add(2, ' ');
		    while(lineTemp.Length() < 14)
			lineTemp.Add(10, ' ');

		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"| " <<lineTemp <<"    | " <<numRecords <<spaces <<line <<'\n';
		}
		else if(op == "ORI")
		{
		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"|                   |       " <<line <<'\n';
		}
		else if(op == "END")
		{
		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"|                   |       " <<line <<'\n';
		}
		else if(op == "EQU")
		{
		    line.Add(12, ' ');
		    line.Add(12, ' ');
		    listingFile <<"|                   |       " <<line <<'\n';
		}
	    }

        }
    }
}

//--------------------------------------------------------------
//putLiterals()
//Written by: Derek Boytim
//Edited by:The Dougars
void putLiterals()
{
    //append literal table to end of object file
    while(literalTable.size() > 0)
    {
	Text name, address, value, record="T",hex;
	Integer i;
	literalTable.removeAny(name, address, value);
	i=To_Integer(value);
	hex=toHexValue(i);
	record.Add(1, address[0]);
	record.Add(2, address[1]);
	record.Add(3, hex[0]);
	record.Add(4, hex[1]);
	record.Add(5, hex[2]);
	record.Add(6, hex[3]);
	record.Add(7, hex[4]);
	objectOut <<record <<'\n';
    }
}

//--------------------------------------------------------------
//assemble()
//Written by: Derek Boytim
//Edited by:The Dougars
void assemble()
{
    stdOut<<'\n';
    stdOut<<"Begin Assembly \n";
    stdOut<<"... \n";
    pass1();
    if(!pass1Error)
    {	
	pass2();
	putLiterals();
    }
    else
    {
	stdOut<<"Assembler Code Error. Please Check Listing File."<<'\n';
    }	    
}

//--------------------------------------------------------------
//main()
//Written by: Derek Boytim
//Edited by:The Dougars
program_body main(end_user_command_line)
{
    stdOut.Open_External ("");

    object Command_Line_Handler cmdLine;

    cmdLine.Get_Command_Line(command_line_arguments);

    if(cmdLine.Number_Of_Arguments() != 4)
    {
	stdOut <<"Command line argument error\n";
        stdOut <<"\nUsage: % program-name 'assembly-language-source' 'object-output' 'listing-output'\n";
        return 1;
    }

    if(not File_Is_Writeable("."))
    {
	stdOut <<"Error: The current directory is not writeable!\n";
	return 1;
    }

    for(Integer i=1; i<4; i++)
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

    assemblySource.Open_External(cmdLine[1]);
    objectOut.Open_External(cmdLine[2]);
    listingFile.Open_External(cmdLine[3]);
    
    assemble();

    assemblySource.Close_External();
    objectOut.Close_External();
    listingFile.Close_External();
    stdOut<<"Assembler Finished \n";
    
}
