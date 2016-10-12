//  /*--------------------------------------------------------*\
//  |   Main Program: Lab 2 Simulator
//  |*--------------------------------------------------------*|
//  |   Date:         19 January 2010
//  |   Author:       The Dougars
//  |   
//  \*--------------------------------------------------------*/

///-------------------------------------------------------------
/// Global Context ---------------------------------------------
///-------------------------------------------------------------

#include "RESOLVE_Foundation.h"
#include "w10_560_machine.h"

///-------------------------------------------------------------
/// Interface --------------------------------------------------
///-------------------------------------------------------------

global_function Boolean checkFiles(preserves Command_Line_Handler& cmdLine, alters Character_OStream& stdOut)
{
    for(Integer i=3; i<5; i++)
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
    for(Integer i=1; i<3; i++)
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

program_body main(end_user_command_line)
{
    object Command_Line_Handler cmdLine;
    object Character_OStream stdOut;

    stdOut.Open_External ("");

    cmdLine.Get_Command_Line(command_line_arguments);

    if(cmdLine.Number_Of_Arguments() != 5)
    {
	stdOut <<"Command line argument error\n";
        stdOut <<"\nUsage: % program-name 'executable-input' 'process-input' 'process-output' 'trace-output'\n";
        return 1;
    }

    if(not File_Is_Writeable("."))
    {
	stdOut <<"Error: The current directory is not writeable!\n";
	return 1;
    }

    for(Integer i=1; i<5; i++)
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
    stdOut <<"Creating machine...\n";
    W10_560_Machine machine (cmdLine[1], cmdLine[2], cmdLine[3], cmdLine[4]);
    stdOut <<"Starting machine...\n";
    machine.start();
   stdOut <<"Machine finished.\n";
    
}
