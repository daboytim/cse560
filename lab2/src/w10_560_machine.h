//  /*-------------------------------------------------------------------*\
//  |   Concrete Instance : W10-560 Machine
//
//  \*-------------------------------------------------------------------*/

#ifndef W10_560_MACHINE_H
#define W10_560_MACHINE 1

///---------------------------------------------------------------------
/// Global Context -----------------------------------------------------
///---------------------------------------------------------------------

#include "CT/Partial_Map/Kernel_3_C.h"
#include "CI/Integer/Are_In_Order_1.h"
#include "headerRecord.h"
#include "textRecord.h"
#include "hexAddress.h"
#include "word.h"
#include "instruction.h"

///---------------------------------------------------------------------
/// Interface ----------------------------------------------------------
///---------------------------------------------------------------------

concrete_instance 
class Partial_Map_Of_Memory :
    instantiates
Partial_Map_Kernel_3_C <Integer, Text, Integer_Are_In_Order_1>
{};


concrete_instance
class W10_560_Machine
{
private:
    Word r0, r1, r2, r3;//registers
    Word pc; //program counter
    Partial_Map_Of_Memory memory;
    Character_IStream executableIn;
    Character_IStream processIn;
    Character_OStream processOut;
    Character_OStream processTrace;
    Boolean started, finished;
    Text execInName;
    Text processInName;
    Text processOutName;
    Text traceOutName;
    void initialize();//initialize machine
    void finalize();//finalize machine
    Boolean loadExec();
    void Dump_Memory();
    void Dump_Registers();
    void incPc();//increment program counter
    void run();//fetch, execute cycle
    void op0(Instruction &);
    void op1(Instruction &);
    void op2(Instruction &);
    void op3(Instruction &);
    void op4(Instruction &);
    void op5(Instruction &);
    void op6(Instruction &);
    void op7(Instruction &);
    void op8(Instruction &);
    void op9(Instruction &);
    void opA(Instruction &);
    void opB(Instruction &);
    void opC(Instruction &);
    void opD(Instruction &);
    void opE(Instruction &);
    void opF(Instruction &);
public:
    W10_560_Machine(Text &, Text &, Text &, Text &);
    ~W10_560_Machine();
    void start();
};

///---------------------------------------------------------------------
/// Private Members ----------------------------------------------------
///---------------------------------------------------------------------

void W10_560_Machine::initialize()
{
    //-initialize memory
    Text t="-----";//20 bits
    for(Integer i=0; i<256; i++)
    {
	Text t_copy = t;
	Integer j = i;
	memory.Define(j, t_copy);
    }

    //open streams
    executableIn.Open_External(execInName);
    processIn.Open_External(processInName);
    processOut.Open_External(processOutName);
    processTrace.Open_External(traceOutName);
    
    //-initialize registers
    r0.setData(t);
    r1.setData(t);
    r2.setData(t);
    r3.setData(t);
}

//----------------------------------------------------------------------

void W10_560_Machine::finalize()
{
    //-close inputs/outputs
    executableIn.Close_External();
    processIn.Close_External();
    processOut.Close_External();
    processTrace.Close_External();
}

//----------------------------------------------------------------------

Boolean W10_560_Machine::loadExec()
{    
    //load executable; return false if error
    //load header

    object Text t;
    executableIn >> t;
    
     //check H** and len
    if (t[0] != 'H' or t.Length() != 13)
    {
        processOut <<"invalid contents: HeaderRecord Error\n";
        return false;
    }

    object HeaderRecord headerRecord(t);
    object Text tempname=headerRecord.getName();
    for (Integer j=0; j<6; j++)
    {
	if(not((tempname[j]>= 48 and tempname[j] <= 57) or
	       (tempname[j]>= 65 and tempname[j] <= 90) or
	       (tempname[j]>= 97 and tempname[j] <= 122)))
	{
	    processOut <<"invalid contents: HeaderRecord Error\n";
	    return false;
	}
    }
    Text bgAdd=(headerRecord.getBeginAddress()).getAddress();
    Text ldAdd=(headerRecord.getLoadAddress()).getAddress();
    
    if(bgAdd.Length()!=2 or ldAdd.Length()!=2)
    {
	processOut <<"invalid contents: HeaderRecord Error\n";
	return false;
    }
    //check  hex addresses
    for(Integer k=0; k<2; k++)
    {
	if(not((bgAdd[k]>= 48 and bgAdd[k] <= 57)or
	       (bgAdd[k]>= 65 and bgAdd[k] <= 70) or
	       (bgAdd[k]>= 97 and bgAdd[k] <= 102)or
	       (ldAdd[k]>= 48 and ldAdd[k] <= 57)or
	       (ldAdd[k]>= 65 and ldAdd[k] <= 70) or
	       (ldAdd[k]>= 97 and ldAdd[k] <= 102)))
	{
	     processOut <<"invalid contents: HeaderRecord Error\n";
	     return false;
	}
    }

    //set program counter
    Text pcText = headerRecord.getBeginAddress().getAddress();
    while(pcText.Length() < 5)
    {
	Character c='0';
	pcText.Add(0,c);
    }
    pc.setData(pcText); 
    //read in each command and store 
    while(!executableIn.At_EOS()) 
    {
	object Text temp;
	executableIn >> temp;
	
	if (temp[0]!='T'or temp.Length()!=8)
	{
	    processOut <<"invalid contents: TextRecord Error\n";
	    return false;
	}
	for(Integer k=1; k<8; k++)
        {
	    if(not((temp[k]>= 48 and temp[k] <= 57) or
		   (temp[k]>= 65 and temp[k] <= 70) or
		   (temp[k]>= 97 and temp[k] <= 102)))
	    {
	        processOut <<"invalid contents: TextRecord Error char\n";
	        return false;
	    }
        }
        //check for syntax errors
	object TextRecord textRecord(temp);
        object Integer addr = textRecord.getAddress().toDecimal();
	object Text tmpMem = memory[addr];
  
	//check for filled mem location
	if(tmpMem == "-----")
	{
	    Text tmp = textRecord.getData().getData();
	    memory[addr] = tmp;
	}
	else
        {
	    //no-op check
	    processOut << "invalid contents: Address already allocated\n";
	    object Text t_no_op="no_op";
	    //object Word no_op(t_no_op);
	    memory[addr] = t_no_op;
	}
    }
    return true;
}

//----------------------------------------------------------------------

void W10_560_Machine::incPc()
{
    ++pc;
}

//----------------------------------------------------------------------

void W10_560_Machine::run()
{
    
    Character_OStream std;
    std.Open_External("");
     //fetch execute cycle
    object Integer count; 
    while(!finished)
    {
	object Integer addr = pc.toDecimal();
	std <<"pc: " <<addr <<'\n';
	object Text inst_text=memory[addr];//fetch instruction
	std <<"in: " <<inst_text<<'\n';
	object Instruction m_code_inst(inst_text);
	object Character t_OP=m_code_inst.getOp();
	std <<"op: " <<t_OP <<'\n';
	
	incPc();//increment pc
	std <<"pc incremented\n";
	
	if(count==200)
	{
	    processOut <<"Execution halted, too many instructions.\n";
	    return;
	}
	if(inst_text=="no_op")
	{
	    processTrace << "No Operation\n";
	    continue;
	}
	else if(t_OP=='0')//exec instruction
	{
	    std <<"op0\n";
	    op0(m_code_inst);
        }
	else if(t_OP=='1')
	{
	    std <<"op1\n";
	    op1(m_code_inst);
	}
	else if(t_OP=='2')
	{
	    std <<"op2\n";
	    op2(m_code_inst);
	}
	else if(t_OP=='3')
	{
	    std <<"op3\n";
	    op3(m_code_inst);
	}
	else if(t_OP=='4')
	{
	    std <<"op4\n";
	    op4(m_code_inst);
	}
	else if(t_OP=='5')
	{
	    std <<"op5\n";
	    op5(m_code_inst);
	}
	else if(t_OP=='6')
	{
	    std <<"op6\n";
	    op6(m_code_inst);
	}
	else if(t_OP=='7')
	{
	    std <<"op7\n";
	    op7(m_code_inst);
	}
	else if(t_OP=='8')
	{
	    std <<"op8\n";
	    op8(m_code_inst);
	}
	else if(t_OP=='9')
	{
	    std <<"op9\n";
	    op9(m_code_inst);
	}
	else if(t_OP=='A')
	{
	    std <<"opA\n";
	    opA(m_code_inst);
	}
	else if(t_OP=='B')
	{
	    std <<"opB\n";
	    opB(m_code_inst);
	}
	else if(t_OP=='C')
	{
	    std <<"opC\n";
	    opC(m_code_inst);
	}
	else if(t_OP=='D')
	{
	    std <<"opD\n";
	    opD(m_code_inst);
	}
	else if(t_OP=='E')
	{
	    std <<"opE\n";
	    opE(m_code_inst);
	}
	else if(t_OP=='F')
	{
	    std <<"opF\n";
	    opF(m_code_inst);
	}
	count ++;
    }
}
//----------------------------------------------------------------------

void W10_560_Machine::Dump_Memory() //Print out memory
{
    processTrace << "           MEMORY" << '\n';
    processTrace << "  Address  "<<"|"<< "  Content" <<'\n';
    processTrace << "-------------------------" <<'\n';
    for(Integer i=0; i<256;i++)
    {
	HexAddress location (i);
	Text address = location.getAddress();
	Text contents = memory[i];
	processTrace <<"  "<< address << "  |  "<< contents <<'\n';
    }
    processTrace << "-------------------------" <<'\n';
}

//----------------------------------------------------------------------

void W10_560_Machine::Dump_Registers() //Print out Register
{   
    object Text reg0=r0.toHex();
    object Text reg1=r1.toHex();
    object Text reg2=r2.toHex();
    object Text reg3=r3.toHex();
    processTrace <<"            REGISTERS" << '\n';
    processTrace <<"|  r0   |" <<"  r1   |" <<"  r2   |" <<"  r3   |" <<'\n';
    processTrace <<"| " << reg0<<" | " <<reg1 <<" | " <<reg2 <<" | " <<reg3 <<" | " <<'\n';
}
    
    
//----------------------------------------------------------------------

void W10_560_Machine::op0(Instruction &inst) //LD
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if(s < 0 or 255 < s)
    {
	processOut <<"Address range error.\n";
	return;
    }
    
    //load to R
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text conttmp = memory[s];
    if(inst.getR() == 0)
    {
	r0.setData(conttmp);
	Text regtmp = r0.toHex();
	
	processTrace <<"LD 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(conttmp);
	Text regtmp = r1.toHex();
	
	processTrace <<"LD 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(conttmp);
	Text regtmp = r2.toHex();
	
	processTrace <<"LD 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(conttmp);
	Text regtmp = r3.toHex();
	
	processTrace <<"LD 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op1(Instruction &inst) //LDI
{
    //get value of S(X)
    Integer val = inst.getS();
    if(inst.getX() == 1)
    {
	val += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	val += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	val += r3.toDecimal();
    }
    //load to R
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    if(inst.getR() == 0)
    {
	r0.setData(val);
	Text regtmp = r0.toHex();

	processTrace <<"LDI 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(val);
	Text regtmp = r1.toHex();

	processTrace <<"LDI 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(val);
	Text regtmp = r2.toHex();

	processTrace <<"LDI 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(val);
	Text regtmp = r3.toHex();

	processTrace <<"LDI 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op2(Instruction &inst) //ST
{
    //get address of S(X)
    Integer addr = inst.getS();
    if(inst.getX() == 1)
    {
	addr += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	addr += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	addr += r3.toDecimal();
    }
    //get contents of R
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text regtmp;
    if(inst.getR() == 0)
    {
	regtmp = r0.toHex();
	processTrace <<"ST 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 0: " << regtmp<<'\n';
    }
    else if(inst.getR() == 1)
    {
	regtmp = r1.toHex();
	processTrace <<"ST 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 1: " << regtmp<<'\n';
    }
    else if(inst.getR() == 2)
    {
	regtmp = r2.toHex();
	processTrace <<"ST 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 2: " << regtmp<<'\n';
    }
    else if(inst.getR() == 3)
    {
	regtmp = r3.toHex();
	processTrace <<"ST 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 3: " << regtmp<<'\n';
    }
    //store
    memory[addr] = regtmp;
    processTrace <<"Memory[" <<addr <<"]: " << regtmp <<'\n';
}

//----------------------------------------------------------------------

void W10_560_Machine::op3(Instruction &inst) //ADD
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if(s < 0 or 255 < s)
    {
	processOut <<"Address range error.\n";
	return;
    }
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();
    //R+=val
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text conttmp = memory[s];
    if(inst.getR() == 0)
    {
	r0.setData(r0.toDecimal()+val);
	Text regtmp = r0.toHex();

	processTrace <<"ADD 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(r1.toDecimal()+val);
	Text regtmp = r1.toHex();

	processTrace <<"ADD 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(r2.toDecimal()+val);
	Text regtmp = r2.toHex();

	processTrace <<"ADD 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(r3.toDecimal()+val);
	Text regtmp = r3.toHex();

	processTrace <<"ADD 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op4(Instruction &inst) //SUB
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if(s < 0 or 255 < s)
    {
	processOut <<"Address range error.\n";
	return;
    }
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();
    //R-=val
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text conttmp = memory[s];

    if(inst.getR() == 0)
    {
	r0.setData(r0.toDecimal()-val);
	Text regtmp = r0.toHex();

	processTrace <<"SUB 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(r1.toDecimal()-val);
	Text regtmp = r1.toHex();

	processTrace <<"SUB 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(r2.toDecimal()-val);
	Text regtmp = r2.toHex();

	processTrace <<"SUB 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(r3.toDecimal()-val);
	Text regtmp = r3.toHex();

	processTrace <<"SUB 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op5(Instruction &inst) //MUL
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if(s < 0 or 255 < s)
    {
	processOut <<"Address range error.\n";
	return;
    }
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();
    //R*=val
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text conttmp = memory[s];
    if(inst.getR() == 0)
    {
	r0.setData(r0.toDecimal()*val);
	Text regtmp = r0.toHex();

	processTrace <<"MUL 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(r1.toDecimal()*val);
	Text regtmp = r1.toHex();

	processTrace <<"MUL 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(r2.toDecimal()*val);
	Text regtmp = r2.toHex();

	processTrace <<"MUL 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(r3.toDecimal()*val);
	Text regtmp = r3.toHex();

	processTrace <<"MUL 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op6(Instruction &inst) //DIV
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if(s < 0 or 255 < s)
    {
	processOut <<"Address range error.\n";
	return;
    }
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();
    if(val == 0)
    {
	processOut <<"Attempt to divide by zero.\n";
	return;
    }
    //R/=val
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    Text conttmp = memory[s];
    if(inst.getR() == 0)
    {
	r0.setData(r0.toDecimal()/val);
	Text regtmp = r0.toHex();

	processTrace <<"DIV 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 0: " << regtmp <<'\n';
    }
    else if(inst.getR() == 1)
    {
	r1.setData(r1.toDecimal()/val);
	Text regtmp = r1.toHex();

	processTrace <<"DIV 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(r2.toDecimal()/val);
	Text regtmp = r2.toHex();

	processTrace <<"DIV 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(r3.toDecimal()/val);
	Text regtmp = r3.toHex();

	processTrace <<"DIV 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
	processTrace <<"Register 3: " << regtmp <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op7(Instruction &inst) //OR R,S(X)
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 0)
    {
	s += r0.toDecimal();
    }
    else if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if (inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();

    //Convert contents at memory to binary
    object Integer div, rem;
    object Text val_bin;
    div = val / 2;
    rem = val % 2;
    while (div <= 0 and rem == 1)
    {
	object Character ch1 = '1', ch0 = '0';
	if (rem == 1)
	{
	    val_bin.Add(0, ch1);
	}
	else
	{
	    val_bin.Add(0, ch0);
	}
	rem = div % 2;
	div = div / 2;
    }
    while (val_bin.Length() < 20)
    {
	object Character tmp = '0';
	val_bin.Add(0, tmp);
    }

    //Convert Appropriate register to binary
    object Integer reg_int;
    if (inst.getR() == 0)
    {
	reg_int = r0.toDecimal();
    }
    else if (inst.getR() == 1)
    {
	reg_int = r1.toDecimal();
    }
    else if (inst.getR() == 2)
    {
	reg_int = r2.toDecimal();
    }
    else if (inst.getR() == 3)
    {
	reg_int = r3.toDecimal();
    }
    object Integer div2, rem2;
    object Text reg_bin;
    div2 = reg_int / 2;
    rem2 = reg_int % 2;
    while (div2 <= 0 and rem2 == 1)
    {
	object Character ch1 = '1', ch0 = '0';
	if (rem2 == 1)
	{
	    reg_bin.Add(0, ch1);
	}
	else
	{
	    reg_bin.Add(0, ch0);
	}
	rem2 = div2 % 2;
	div2 = div2 / 2;
    }
    while (reg_bin.Length() < 20)
    {
	object Character tmp = '0';
	reg_bin.Add(0, tmp);
    }

    //OR register and memory together
    object Integer count;
    while (reg_bin.Length() > 20)
    {
	if (reg_bin[count] == '1' or val_bin[count] == '1')
	{
	    reg_bin[count] = '1';
	}
	else
	{
	    reg_bin[count] = '0';
	}
	count ++;
    }

    //Convert ORed binary into decimal
    object Integer count2 = 19, two_power = 1, OR_total;
    if (reg_bin[count2] == '1')
    {
	OR_total ++;
    }
    while (count2 >= 0)
    {
	two_power = two_power * 2;
	if (reg_bin[count2] == '1')
	{
	    OR_total = OR_total + two_power;
	}
	count --;
    }

    //Store ORed hex in appropriate register, output results
    Integer temp_s = inst.getS();
    Integer temp_x = inst.getX();
    Text temp_mem = memory[s];
    if (inst.getR() == 0)
    {
	r0.setData(OR_total);
	processTrace <<"OR 0," << temp_s <<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r0.toHex();
	processTrace <<"Register 0: " << temp_r <<'\n';
    }
    else if (inst.getR() == 1)
    {
	r1.setData(OR_total);
	processTrace <<"OR 1," << temp_s <<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r1.toHex();
	processTrace <<"Register 1: " << temp_r <<'\n';
    }
    else if (inst.getR() == 2)
    {
	r2.setData(OR_total);
	processTrace <<"OR 2," << temp_s <<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r2.toHex();
	processTrace <<"Register 2: " << temp_r <<'\n';
    }
    else if (inst.getR() == 3)
    {
	r3.setData(OR_total);
	processTrace <<"OR 3," << temp_s <<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r3.toHex();
	processTrace <<"Register 3: " << temp_r <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op8(Instruction &inst) //AND R,S(X)
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 0)
    {
	s += r0.toDecimal();
    }
    else if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if (inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();

    //Convert contents at memory to binary
    object Integer div, rem;
    object Text val_bin;
    div = val / 2;
    rem = val % 2;
    while (div <= 0 and rem == 1)
    {
	object Character ch1 = '1', ch0 = '0';
	if (rem == 1)
	{
	    val_bin.Add(0, ch1);
	}
	else
	{
	    val_bin.Add(0, ch0);
	}
	rem = div % 2;
	div = div / 2;
    }
    while (val_bin.Length() < 20)
    {
	object Character tmp = '0';
	val_bin.Add(0, tmp);
    }

    //Convert Appropriate register to binary
    object Integer reg_int;
    if (inst.getR() == 0)
    {
	reg_int = r0.toDecimal();
    }
    else if (inst.getR() == 1)
    {
	reg_int = r1.toDecimal();
    }
    else if (inst.getR() == 2)
    {
	reg_int = r2.toDecimal();
    }
    else if (inst.getR() == 3)
    {
	reg_int = r3.toDecimal();
    }
    object Integer div2, rem2;
    object Text reg_bin;
    div2 = reg_int / 2;
    rem2 = reg_int % 2;
    while (div2 <= 0 and rem2 == 1)
    {
	object Character ch1 = '1', ch0 = '0';
	if (rem2 == 1)
	{
	    reg_bin.Add(0, ch1);
	}
	else
	{
	    reg_bin.Add(0, ch0);
	}
	rem2 = div2 % 2;
	div2 = div2 / 2;
    }
    while (reg_bin.Length() < 20)
    {
	object Character tmp = '0';
	reg_bin.Add(0, tmp);
    }

    //AND register and memory together
    object Integer count;
    while (count < 20)
    {
	if (reg_bin[count] == '1' and val_bin[count] == '1')
	{
	    reg_bin[count] = '1';
	}
	else
	{
	    reg_bin[count] = '0';
	}
	count ++;
    }

    //Convert ANDed binary into decimal
    object Integer count2 = 19, two_power = 1, AND_total;
    if (reg_bin[count2] == '1')
    {
	AND_total ++;
    }
    while (count2 >= 0)
    {
	two_power = two_power * 2;
	if (reg_bin[count2] == '1')
	{
	    AND_total = AND_total + two_power;
	}
	count --;
    }

    //Store ANDed hex in appropriate register
    Integer temp_s = inst.getS();
    Integer temp_x = inst.getX();
    Text temp_mem = memory[s];
    if (inst.getR() == 0)
    {
	r0.setData(AND_total);
	processTrace <<"AND 0," << temp_s<<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r0.toHex();
	processTrace <<"Register 0: " << temp_r <<'\n';
    }
    else if (inst.getR() == 1)
    {
	r1.setData(AND_total);
	processTrace <<"AND 1," << temp_s<<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
        Text   temp_r = r1.toHex();
	processTrace <<"Register 1: " << temp_r <<'\n';
    }
    else if (inst.getR() == 2)
    {
	r2.setData(AND_total);
	processTrace <<"AND 2," << temp_s<<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r2.toHex();
	processTrace <<"Register 2: " << temp_r <<'\n';
    }
    else if (inst.getR() == 3)
    {
	r3.setData(AND_total);
	processTrace <<"AND 3," << temp_s<<"(" << temp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << temp_mem <<'\n';
	Text temp_r = r3.toHex();
	processTrace <<"Register 3: " << temp_r <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::op9(Instruction &inst)
{
    Integer shift = inst.getS();
    if(inst.getX() == 1)
    {
	shift += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	shift += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	shift += r3.toDecimal();
    }

    if(shift < 0 or shift > 19)
    {
	processOut << "Error: Cannot shift more than 19 bits\n";
	return;
    }
    //Get contents from regiter R
    Integer val;
    if(inst.getR() == 0)
    {
	val = r0.toDecimal();
    }
    else if(inst.getR() == 1)
    {
	val = r1.toDecimal();
    }
    else if(inst.getR() == 2)
    {
	val = r2.toDecimal();
    }
    else if(inst.getR() == 3)
    {
	val = r3.toDecimal();
    }

    //Shift Register to the left by s bits
    while(shift > 0)
    {
	val *= 2;
	shift--;
    }

    //Place shifted number into register
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    if(inst.getR() == 0)
    {
	r0.setData(val);
	Text regtmp = r0.toHex();		

	processTrace <<"SHL 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 0: " << regtmp <<'\n';	
    }
    else if(inst.getR() == 1)
    {
	r1.setData(val);
	Text regtmp = r1.toHex();		

	processTrace <<"SHL 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(val);
	Text regtmp = r2.toHex();
	
	processTrace <<"SHL 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(val);
	Text regtmp = r3.toHex();	
	
	processTrace <<"SHL 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 3: " << regtmp <<'\n';
    }    
}

//----------------------------------------------------------------------

void W10_560_Machine::opA(Instruction &inst)
{
    Integer shift = inst.getS();
    if(inst.getX() == 1)
    {
	shift += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	shift += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	shift += r3.toDecimal();
    }
    
    if(shift < 0 or shift > 19)
    {
	processOut << "Error: Cannot shift more than 19 bits\n";
	return;
    }
    //Get contents from regiter R
    Integer val;
    if(inst.getR() == 0)
    {
	val = r0.toDecimal();
    }
    else if(inst.getR() == 1)
    {
	val = r1.toDecimal();
    }
    else if(inst.getR() == 2)
    {
	val = r2.toDecimal();
    }
    else if(inst.getR() == 3)
    {
	val = r3.toDecimal();
    }

    //Shift Register to the right by s bits
    while(shift > 0)
    {
	val /= 2;
	shift--;
    }

    //Place shifted number into register
    Integer stmp = inst.getS();
    Integer xtmp = inst.getX();
    if(inst.getR() == 0)
    {
	r0.setData(val);
	Text regtmp = r0.toHex();

	processTrace <<"SHR 0," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 0: " << regtmp <<'\n';	
    }
    else if(inst.getR() == 1)
    {
	r1.setData(val);
	Text regtmp = r1.toHex();		

	processTrace <<"SHR 1," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 1: " << regtmp <<'\n';
    }
    else if(inst.getR() == 2)
    {
	r2.setData(val);
	Text regtmp = r2.toHex();
	
	processTrace <<"SHR 2," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 2: " << regtmp <<'\n';
    }
    else if(inst.getR() == 3)
    {
	r3.setData(val);
	Text regtmp = r3.toHex();	

	processTrace <<"SHR 3," << stmp <<"(" << xtmp <<")\n";
	processTrace <<"Register 3: " << regtmp <<'\n';
    }       
}

//----------------------------------------------------------------------

void W10_560_Machine::opB(Instruction &inst)
{
	//Get R value to see which input/output procdure to run
	if(inst.getR() == 0)
	{
	    Integer pos,num;
	    Character byte,ch;
	    Text ins;
	    Boolean truth;
	    //read in process input file until first non separator and consume
	    while(!processIn.At_EOS() || byte <= 32 || byte >= 127)
	    {
		processIn.Read(byte);
	    }
	    //read in word up to next separator or end of line
	    while(!processIn.At_EOS() || byte > 32 || byte < 127)
	    {
		processIn.Read(byte);
		ins.Add(pos,byte);
		pos++;
	    }
	    //Consume next separator byte
	    processIn.Read(byte);
	    //See if first character in ins is a + or -
	    if(ins[0] == 43 or ins[0] == 45)
	    {
		ins.Remove(0,ch);
	    }
	    //check to see if rest is in  well formed decimal rep
	    truth = Can_Convert_To_Integer(ins);
	    num = To_Integer(ins);
	    if(truth == true and (0 < num and num <= 1048576))
	    {			
		//store into s
		num = To_Integer(ins);
		Integer s = inst.getS();
		if(inst.getX() == 1)
		{
		    s += r1.toDecimal();
		}
		else if(inst.getX() == 2)
		{
		    s += r2.toDecimal();
		}
		else if(inst.getX() == 3)
		{
		    s += r3.toDecimal();
		}
		if (s < 0 or 255 < s)
		{
		    processOut << "Address range error.\n";
		    return;
		}
		//Replace value at address S(X) with num
		memory[s].Clear();
		Word tmpWord (num);
		memory[s] = tmpWord.toHex();

		//output trace file
		Integer stmp = inst.getS();
		Integer xtmp = inst.getX();
		Text conttmp = memory[s];
		if(xtmp == 0)
		{
		    Text regtmp = r0.toHex();	

		    processTrace <<"IO 0," << stmp <<"(" << xtmp <<")\n";
		    processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		    processTrace <<"Register 0: " << regtmp <<'\n';
		}
		else if(xtmp == 1)
		{
		    Text regtmp = r1.toHex();	

		    processTrace <<"IO 0," << stmp <<"(" << xtmp <<")\n";
		    processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		    processTrace <<"Register 1: " << regtmp <<'\n';
		}
		else if(xtmp == 2)
		{
		    Text regtmp = r2.toHex();	

		    processTrace <<"IO 0," << stmp <<"(" << xtmp <<")\n";
		    processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		    processTrace <<"Register 2: " << regtmp <<'\n';
		}
		else if(xtmp == 3)
		{
		    Text regtmp = r3.toHex();	

		    processTrace <<"IO 0," << stmp <<"(" << xtmp <<")\n";
		    processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		    processTrace <<"Register 3: " << regtmp <<'\n';
		}
	    }	    
	}	
	else if(inst.getR() == 1)
	{
	    //Get the address of S(X) for the output trace file
	    Integer s = inst.getS();
	    if(inst.getX() == 1)
	    {
		s += r1.toDecimal();
	    }
	    else if(inst.getX() == 2)
	    {
		s += r2.toDecimal();
	    }
	    else if(inst.getX() == 3)
	    {
		s += r3.toDecimal();
	    }
	    if(s < 0 or 255 < s)
	    {
		processOut <<"Address range error.\n";
		return;
	    }
	    Character byte,tmp;
	    Text t;
	    Integer i;
	    //Get Hex value of register X
	    if(inst.getX() == 0)
	    {
		t = r0.toHex();
		i = r0.toDecimal();
	    }
	    if(inst.getX() == 1)
	    {
		t = r1.toHex();
		i = r1.toDecimal();		
	    }
	    if(inst.getX() == 2)
	    {
		t = r2.toHex();
		i = r1.toDecimal();		
	    }
	    if(inst.getX() == 3)
	    {
		t = r3.toHex();
		i = r1.toDecimal();		
	    }
	    //read in first byte of input stream
	    processIn.Read(byte);

	    
	    //Replace last 8 bits of register X with byte that was
	    //read in
	    //Convert the Hex characters to an integer
	    Character ch1,ch2;
	    Integer res,digit;
	    t.Remove(t.Length()-1,ch1);
	    t.Remove(t.Length()-1,ch2);
	    if(48 <= ch1 and ch1 <= 57)
	    {
		res = To_Integer(ch1) - 48;
	    }
	    else if (65 <= ch1 and ch1 <= 70)
	    {
		res = To_Integer(ch1) - 55;
	    }
	    else if(97 <= ch1 and ch1 <= 102)
	    {
		res = To_Integer(ch1) - 87;
	    }
	    res *= 16;
	    if(48 <= ch2 and ch2 <= 57)
	    {
		res += To_Integer(ch2) - 48;
	    }
	    else if (65 <= ch2 and ch2 <= 70)
	    {
		res += To_Integer(ch2) - 55;
	    }
	    else if(97 <= ch2 and ch2 <= 102)
	    {
		res += To_Integer(ch2) - 87;
	    }

	    //replace 8 LSB's with byte
	    i -= res;
	    digit = To_Integer(byte);
	    i += digit;
	    //put new data in register and output trace file
	    Integer stmp = inst.getS();
	    Integer xtmp = inst.getX();
	    Text conttmp = memory[s];
	    if(xtmp == 0)
	    {
		r0.setData(i);
		Text regtmp = r0.toHex();	

	        processTrace <<"IO 1," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';		
	    }
	    if(xtmp == 1)
	    {
		r1.setData(i);
		Text regtmp = r1.toHex();

	        processTrace <<"IO 1," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 1: " << regtmp <<'\n';			
	    }
	    if(xtmp == 2)
	    {
		r2.setData(i);
		Text regtmp = r2.toHex();	

	        processTrace <<"IO 1," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 2: " << regtmp <<'\n';			
	    }
	    if(xtmp == 3)
	    {
		r3.setData(i);
		Text regtmp = r3.toHex();	

	        processTrace <<"IO 1," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 3: " << regtmp <<'\n';				
	    }
	}
	else if(inst.getR() == 2)
	{
	    //Number will be represented in 20 bit 2's compl
            //get contents at S(X)
	    Integer s = inst.getS();
	    if(inst.getX() == 0)
	    {
		s += r0.toDecimal();
	    }
	    else if(inst.getX() == 1)
	    {
		s += r1.toDecimal();
	    }
	    else if(inst.getX() == 2)
	    {
		s += r2.toDecimal();
	    }
	    else if (inst.getX() == 3)
	    {
		s += r3.toDecimal();
	    }
	    if (s < 0 or 255 < s)
	    {
		processOut << "Address range error.\n";
		return;
	    }
	    Word tmpWord (memory[s]);
	    Integer val = tmpWord.toDecimal();
	    //See if val is positively or negatively signed
	    if(val <= 524287)
	    {
		processOut << val;
	    }
	    else
	    {
		val -= 104576;
		processOut << val;
	    }

	    //output trace file
	    Integer stmp = inst.getS();
	    Integer xtmp = inst.getX();
	    Text conttmp = memory[s];
	    if(xtmp == 0)
	    {
		Text regtmp = r0.toHex();

		processTrace <<"IO 2," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';
	    }
	    else if(xtmp == 1)
	    {
		Text regtmp = r1.toHex();

		processTrace <<"IO 2," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 1: " << regtmp <<'\n';
	    }
	    else if(xtmp == 2)
	    {
		Text regtmp = r2.toHex();

		processTrace <<"IO 2," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 2: " << regtmp <<'\n';
	    }
	    else if(xtmp == 3)
	    {
		Text regtmp = r3.toHex();

		processTrace <<"IO 2," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 3: " << regtmp <<'\n';
	    }    
	}
	else if(inst.getR() == 3)
	{
	    //Get the address of S(X) for the output trace file
	    Integer s = inst.getS();
	    if(inst.getX() == 1)
	    {
		s += r1.toDecimal();
	    }
	    else if(inst.getX() == 2)
	    {
		s += r2.toDecimal();
	    }
	    else if(inst.getX() == 3)
	    {
		s += r3.toDecimal();
	    }
	    if(s < 0 or 255 < s)
	    {
		processOut <<"Address range error.\n";
		return;
	    }	    
	    Integer res;
	    Character ch1,ch2;
	    Text t;
	    //Get contents of Register X
	    if(inst.getX() == 0)
	    {
		t = r0.toHex();
	    }
	    if(inst.getX() == 1)
	    {
		t = r1.toHex();
	    }
	    if(inst.getX() == 2)
	    {
		t = r2.toHex();
	    }
	    if(inst.getX() == 3)
	    {
		t = r3.toHex();
	    }
	    //Remove 8 most significant bits
	    t.Remove(0,ch1);
	    t.Remove(0,ch2);
	    
	    //Convert from two 4 bit Hexadecimal #'s to one 8 bit Decimal #
	    if(48 <= ch1 and ch1 <= 57)
	    {
		res = To_Integer(ch1) - 48;
	    }
	    else if (65 <= ch1 and ch1 <= 70)
	    {
		res = To_Integer(ch1) - 55;
	    }
	    else if(97 <= ch1 and ch1 <= 102)
	    {
		res = To_Integer(ch1) - 87;
	    }
	    res *= 16;
	    if(48 <= ch2 and ch2 <= 57)
	    {
		res += To_Integer(ch2) - 48;
	    }
	    else if (65 <= ch2 and ch2 <= 70)
	    {
		res += To_Integer(ch2) - 55;
	    }
	    else if(97 <= ch2 and ch2 <= 102)
	    {
		res += To_Integer(ch2) - 87;
	    }
	    processOut << res;

	    //Output trace file
	    Integer stmp = inst.getS();
	    Integer xtmp = inst.getX();
	    Text conttmp = memory[s];
	    if(xtmp == 0)
	    {
		Text regtmp = r0.toHex();

		processTrace <<"IO 3," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';
	    }
	    else if(xtmp == 1)
	    {
		Text regtmp = r0.toHex();

		processTrace <<"IO 3," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';
	    }
	    else if(xtmp == 2)
	    {
		Text regtmp = r0.toHex();

		processTrace <<"IO 3," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';
	    }
	    else if(xtmp == 3)
	    {
		Text regtmp = r0.toHex();

		processTrace <<"IO 3," << stmp <<"(" << xtmp <<")\n";
		processTrace <<"Memory[" <<s <<"]: " << conttmp <<'\n';
		processTrace <<"Register 0: " << regtmp <<'\n';
	    }    
	}    
}

//----------------------------------------------------------------------

void W10_560_Machine::opC(Instruction &inst) //BR R,S(X)
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 0)
    {
	s += r0.toDecimal();
    }
    else if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if (inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();
    Character_OStream std;
    std.Open_External("");
    Integer r = inst.getR();
    std <<"r: " <<r <<'\n';
    //Check what Register R is
    if (inst.getR() == 0)
    {
	Integer x = inst.getX();
	std <<"Finished\nx: " <<x <<'\n';
	finished = true;
	if (inst.getX() == 0)
	{
	    return;
	}
	else if (inst.getX() == 1)
	{
	    Integer count = 0;
	    processOut << '\n';
	    while (count < 256)
	    {
		Word tmpWord (memory[count]);
		Integer tmp = tmpWord.toDecimal();
		processOut << "M[" << count << "] = " << tmp << '\n';
		count ++;
	    }
	}
	else if (inst.getX() == 2)
	{
	    Integer tmp_r0 = r0.toDecimal();
	    Integer tmp_r1 = r1.toDecimal();
	    Integer tmp_r2 = r2.toDecimal();
	    Integer tmp_r3 = r3.toDecimal();
	    Integer tmp_pc = pc.toDecimal();
	    processOut << "Register 0: " << tmp_r0 << '\n';
	    processOut << "Register 1: " << tmp_r1 << '\n';
	    processOut << "Register 2: " << tmp_r2 << '\n';
	    processOut << "Register 3: " << tmp_r3 << '\n';
	    processOut << "PC: " << tmp_pc << '\n';
	}
	else if (inst.getX() == 3)
	{
	    Integer count = 0;
	    processOut << '\n';
	    while (count < 256)
	    {
		Word tmpWord (memory[count]);
		Integer tmp = tmpWord.toDecimal();
		processOut << "M[" << count << "] = " << tmp << '\n';
		count ++;
	    }
	    Integer tmp_r0 = r0.toDecimal();
	    Integer tmp_r1 = r1.toDecimal();
	    Integer tmp_r2 = r2.toDecimal();
	    Integer tmp_r3 = r3.toDecimal();
	    Integer tmp_pc = pc.toDecimal();
	    processOut << "Register 0: " << tmp_r0 << '\n';
	    processOut << "Register 1: " << tmp_r1 << '\n';
	    processOut << "Register 2: " << tmp_r2 << '\n';
	    processOut << "Register 3: " << tmp_r3 << '\n';
	    processOut << "PC: " << tmp_pc << '\n';
	}
    }
    else if (inst.getR() == 1)
    {
	Integer count = 0;
	processOut << '\n';
	while (count < 256)
	{
	    Word tmpWord (memory[count]);
	    Integer tmp = tmpWord.toDecimal();
	    processOut << "M[" << count << "] = " << tmp << '\n';
	    count ++;
	}
	Integer tmp_r0 = r0.toDecimal();
	Integer tmp_r1 = r1.toDecimal();
	Integer tmp_r2 = r2.toDecimal();
	Integer tmp_r3 = r3.toDecimal();
	Integer tmp_pc = pc.toDecimal();
	processOut << "Register 0: " << tmp_r0 << '\n';
	processOut << "Register 1: " << tmp_r1 << '\n';
	processOut << "Register 2: " << tmp_r2 << '\n';
	processOut << "Register 3: " << tmp_r3 << '\n';
	processOut << "PC: " << tmp_pc << '\n';

	pc.setData(val);
    }
    else if (inst.getR() == 2)
    {
	pc.setData(pc.toDecimal() + val);
	Integer tmp_r = r2.toDecimal();
	Integer tmp_pc = pc.toDecimal();
	Integer tmp_s = inst.getS();
	Integer tmp_x = inst.getX();
	processTrace <<"BR 2," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << val <<'\n';
	processTrace <<"Register 2: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 3)
    {
	pc.setData(val);
	pc.setData(pc.toDecimal() + val);
	Integer tmp_r = r2.toDecimal();
	Integer tmp_pc = pc.toDecimal();
	Integer tmp_s = inst.getS();
	Integer tmp_x = inst.getX();
	processTrace <<"BR 2," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" << s <<"]: " << val <<'\n';
	processTrace <<"Register 2: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::opD(Instruction &inst) //BRZ R,S(X)
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 0)
    {
	s += r0.toDecimal();
    }
    else if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if (inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();

    //get contents of Register
    Integer contents;
    if (inst.getR() == 0)
    {
	contents = r0.toDecimal();
    }
    else if (inst.getR() == 1)
    {
	contents = r1.toDecimal();
    }
    else if (inst.getR() == 2)
    {
	contents = r2.toDecimal();
    }
    else if (inst.getR() == 3)
    {
	contents = r3.toDecimal();
    }

    //Check if contents of register is zero
    if (contents == 0)
    {
	pc.setData(contents);
    }

    //Output values
    Integer tmp_s = inst.getS();
    Integer tmp_x = inst.getX();
    Integer tmp_r = inst.getR();
    Integer tmp_pc = pc.toDecimal();
    if (inst.getR() == 0)
    {
	processTrace <<"BRZ 0," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 0: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 1)
    {
	processTrace <<"BRZ 1," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 1: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 2)
    {
	processTrace <<"BRZ 2," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 2: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 3)
    {
	processTrace <<"BRZ 3," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 3: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::opE(Instruction &inst) //BRN R,S(X)
{
    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 0)
    {
	s += r0.toDecimal();
    }
    else if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	s += r2.toDecimal();
    }
    else if (inst.getX() == 3)
    {
	s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();

    //get contents of Register
    Integer contents;
    if (inst.getR() == 0)
    {
	contents = r0.toDecimal();
    }
    else if (inst.getR() == 1)
    {
	contents = r1.toDecimal();
    }
    else if (inst.getR() == 2)
    {
	contents = r2.toDecimal();
    }
    else if (inst.getR() == 3)
    {
	contents = r3.toDecimal();
    }

    //Check if contents of register is negative (aka > 127 in decimal)
    if (contents > 127)
    {
	if (contents > 255)
	{
	    processOut << "Address range error.\n";
	    return;
	}
	else
	{
	    pc.setData(contents);
	}
    }

    //Output values
    Integer tmp_s = inst.getS();
    Integer tmp_x = inst.getX();
    Integer tmp_r = inst.getR();
    Integer tmp_pc = pc.toDecimal();
    if (inst.getR() == 0)
    {
	processTrace <<"BRN 0," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 0: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 1)
    {
	processTrace <<"BRN 1," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 1: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 2)
    {
	processTrace <<"BRN 2," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 2: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 3)
    {
	processTrace <<"BRN 3," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 3: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
}

//----------------------------------------------------------------------

void W10_560_Machine::opF(Instruction &inst) //BRS R,S(X)
{
    //R <- PC
    if (inst.getR() == 0)
    {
	r0.setData(pc.toDecimal());
    }
    else if (inst.getR() == 1)
    {
	r1.setData(pc.toDecimal());
    }
    else if (inst.getR() == 2)
    {
	r2.setData(pc.toDecimal());
    }
    else if (inst.getR() == 3)
    {
	r3.setData(pc.toDecimal());
    }

    //get contents at S(X)
    Integer s = inst.getS();
    if(inst.getX() == 1)
    {
	s += r1.toDecimal();
    }
    else if(inst.getX() == 2)
    {
	    s += r2.toDecimal();
    }
    else if(inst.getX() == 3)
    {
	    s += r3.toDecimal();
    }
    if (s < 0 or 255 < s)
    {
	processOut << "Address range error.\n";
	return;
    }

    //Get contents at memory
    Word tmpWord (memory[s]);
    Integer val = tmpWord.toDecimal();

    //pc <- contents at memory
    pc.setData(val);

    //Output values
    Integer tmp_s = inst.getS();
    Integer tmp_x = inst.getX();
    Integer tmp_r = inst.getR();
    Integer tmp_pc = pc.toDecimal();
    if (inst.getR() == 0)
    {
	processTrace <<"BRS 0," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 0: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 1)
    {
	processTrace <<"BRS 1," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 1: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 2)
    {
	processTrace <<"BRS 2," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 2: " << tmp_r <<'\n';
	processTrace <<"PC: " << tmp_pc <<'\n';
    }
    else if (inst.getR() == 3)
    {
	processTrace <<"BRS 3," << tmp_s <<"(" << tmp_x <<")\n";
	processTrace <<"Memory[" <<s <<"]: " << val <<'\n';
	processTrace <<"Register 3: " << tmp_r <<'\n';
	processTrace <<"PC: " <<tmp_pc <<'\n';
    }
}

///---------------------------------------------------------------------
/// Public Members -----------------------------------------------------
///---------------------------------------------------------------------

W10_560_Machine::W10_560_Machine(Text &execIn, Text &processIn, Text &processOut, Text &traceOut)
{
    execInName=execIn;
    processInName=processIn;
    processOutName=processOut;
    traceOutName=traceOut;
    initialize();
}

//----------------------------------------------------------------------

W10_560_Machine::~W10_560_Machine()
{
    finalize();
}

//----------------------------------------------------------------------

void W10_560_Machine::start()
{
    Character_OStream stdOut2;
    stdOut2.Open_External("");
    
    if(!started)
    {
	if(!loadExec())
        {
	    processOut << "Execute Load Error\n";
	    return;
        }
	started = true;
	Dump_Memory();
	Dump_Registers();
	run();
        Dump_Memory();
	Dump_Registers();
    }
}

//----------------------------------------------------------------------

#endif
