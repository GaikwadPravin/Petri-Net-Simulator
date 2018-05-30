//“I have neither given nor received any unauthorized aid on this assignment”

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXCHAR 1000

//<instruction, destination, source1, source2/immediateVal>	
//<ADD,R5,R3,R1>
struct instructionTuples	
{
    char instruction[5] ;
    char destination[3] ;
	int immediateVal ;
    char source1[4] ;
    char source2[3] ;
};

//<instruction, destination, @source1, @source2/@immediateVal>	
//<ADD,R5,13,35>
struct commomBuffer			
{
    char instruction[5] ;
    char destination[3] ;
	int immediateVal ;
    int source1 ;
    int source2 ;
};

//<register, @register>
//<R0,10>
struct registerFile			
{
    char registerName[3] ;
    int  registerVal ;
};

//<address, @address>
//<1,47>
struct damFile
{
    int address;
    int addressVal;
};

struct instructionTuples instructions[16] = {0};
struct registerFile registers[16] = {0};
struct damFile datamemory[16] = {0};
struct commomBuffer INB;
struct commomBuffer AIB;
struct commomBuffer SIB;
struct commomBuffer PRB;
struct registerFile REB[2]={0};		// stores result from PRB and AIB
struct registerFile ADB;			//effective address for store instruction by adding the offset with the source operand

int no_of_instructions = 0, regCount = 0, registerUsed[16]={20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
int step = 0,memCount = 0, memUsed[16]={20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
int INBused=0,AIBused=0,SIBused=0,PRBused=0,ADBused=0,REBused=0;

FILE *outputFile;

//sort data memory 
void sortMem()
{
	int i, j;
	struct damFile temp;

	for (i = 0; i < memCount - 1; i++)
    {
        for (j = 0; j < (memCount - 1-i); j++)
        {
			if ( datamemory[j].address > datamemory[j + 1].address )
            {
                temp = datamemory[j];
                datamemory[j] = datamemory[j + 1];
                datamemory[j + 1] = temp;
            }
        }
    }
}

//sort register file
void sortRegfile()
{
	int i, j, regNum1 = 0, regNum2 = 0;
	char *token;
	struct registerFile temp;

	for (i = 0; i < regCount - 1; i++)
    {
        for (j = 0; j < (regCount - 1-i); j++)
        {
			token  = strtok(registers[j].registerName, "R");
			regNum1 = atoi(token);
			token  = strtok(registers[j+1].registerName, "R");
			regNum2 = atoi(token);
			if ( regNum1 > regNum2 )
            {
                temp = registers[j];
                registers[j] = registers[j + 1];
                registers[j + 1] = temp;
            }
        }
    }

}


void printSimulation()
{
    int i, counting = 0;
//--------------------------print INM buffer-----------------------------------------------
	//printf("INM:");
	fprintf(outputFile,"INM:");
	
    if(no_of_instructions-step > 16)
	{
		while(counting <15)
		{
		    if(strcmp(instructions[counting+step].instruction,"ST") == 0)
			{
				//printf("<%s,%s,%s,%d>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].immediateVal);
				fprintf(outputFile,"<%s,%s,%s,%d>,",instructions[counting+step].instruction,instructions[counting+step].destination,instructions[counting+step].source1,instructions[counting+step].immediateVal);
			}
			else
			{
				//printf("<%s,%s,%s,%s>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].source2);
				fprintf(outputFile,"<%s,%s,%s,%s>,",instructions[counting+step].instruction,instructions[counting+step].destination,instructions[counting+step].source1,instructions[counting+step].source2);
			}
            counting++;
		}
        if(strcmp(instructions[counting + step].instruction,"ST") == 0)
		{
			//printf("<%s,%s,%s,%d>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].immediateVal);
			fprintf(outputFile,"<%s,%s,%s,%d>",instructions[counting + step].instruction,instructions[counting + step].destination,instructions[counting + step].source1,instructions[counting + step].immediateVal);
		}
		else
		{
			//printf("<%s,%s,%s,%s>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].source2);
			fprintf(outputFile,"<%s,%s,%s,%s>",instructions[counting + step].instruction,instructions[counting + step].destination,instructions[counting + step].source1,instructions[counting + step].source2);
		}
        counting=0;
	}
	else
	{
        for(i = step; i < no_of_instructions - 1; i ++)
        {   
            if(strcmp(instructions[i].instruction,"ST") == 0)
            {
                //printf("<%s,%s,%s,%d>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].immediateVal);
                fprintf(outputFile,"<%s,%s,%s,%d>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].immediateVal);
            }
            else
            {
                    //printf("<%s,%s,%s,%s>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].source2);
                    fprintf(outputFile,"<%s,%s,%s,%s>,",instructions[i].instruction,instructions[i].destination,instructions[i].source1,instructions[i].source2);
            }
        }
        if(step!=no_of_instructions)
        {
            if(strcmp(instructions[no_of_instructions - 1].instruction,"ST") == 0)
            {
                //printf("<%s,%s,%s,%d>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].immediateVal);
                fprintf(outputFile,"<%s,%s,%s,%d>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].immediateVal);
            }
            else
            {
                //printf("<%s,%s,%s,%s>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].source2);
                fprintf(outputFile,"<%s,%s,%s,%s>",instructions[no_of_instructions - 1].instruction,instructions[no_of_instructions - 1].destination,instructions[no_of_instructions - 1].source1,instructions[no_of_instructions - 1].source2);
            }
        }
	}
//--------------------------print INB buffer-----------------------------------------------
	//printf("\nINB:");
	fprintf(outputFile,"\nINB:");
	
	if(strlen(INB.instruction) != 0 && strcmp(INB.instruction,"NNN") != 0 && strcmp(INB.instruction,"ST") == 0)
	{
		//printf("<%s,%s,%d,%d>",INB.instruction,INB.destination,INB.source1,INB.immediateVal);
		fprintf(outputFile,"<%s,%s,%d,%d>",INB.instruction,INB.destination,INB.source1,INB.immediateVal);
	}
	else if( strcmp(INB.instruction,"SUB") == 0 || strcmp(INB.instruction,"ADD") == 0 || strcmp(INB.instruction,"MUL") == 0)
	{
		//printf("<%s,%s,%d,%d>",INB.instruction,INB.destination,INB.source1,INB.source2);
		fprintf(outputFile,"<%s,%s,%d,%d>",INB.instruction,INB.destination,INB.source1,INB.source2);
	}
//--------------------------print AIB buffer-----------------------------------------------
	//printf("\nAIB:");
	fprintf(outputFile,"\nAIB:");
	
	if(strlen(AIB.instruction) != 0 && strcmp(AIB.instruction,"NNN") != 0)
	{
		//printf("<%s,%s,%d,%d>",AIB.instruction,AIB.destination,AIB.source1,AIB.source2);
		fprintf(outputFile,"<%s,%s,%d,%d>",AIB.instruction,AIB.destination,AIB.source1,AIB.source2);
	}

//--------------------------print SIB buffer-----------------------------------------------
	//printf("\nSIB:");
	fprintf(outputFile,"\nSIB:");
	
	if(strlen(SIB.instruction) != 0 && strcmp(SIB.instruction,"NNN") != 0)
	{
		//printf("<%s,%s,%d,%d>",SIB.instruction,SIB.destination,SIB.source1,SIB.immediateVal);
		fprintf(outputFile,"<%s,%s,%d,%d>",SIB.instruction,SIB.destination,SIB.source1,SIB.immediateVal);	
	}
//--------------------------print PRB buffer-----------------------------------------------
	//printf("\nPRB:");
	fprintf(outputFile,"\nPRB:");
	
	if(strlen(PRB.instruction) != 0 && strcmp(PRB.instruction,"NNN") != 0)
	{
		//printf("<%s,%s,%d,%d>",PRB.instruction,PRB.destination,PRB.source1,PRB.source2);
		fprintf(outputFile,"<%s,%s,%d,%d>",PRB.instruction,PRB.destination,PRB.source1,PRB.source2);	
	}
//--------------------------print ADB buffer-----------------------------------------------
	//printf("\nADB:");
	fprintf(outputFile,"\nADB:");
	
	if(strlen(ADB.registerName) != 0 && strcmp(ADB.registerName,"NNN") != 0)
	{
		//printf("<%s,%d>",ADB.registerName,ADB.registerVal);
		fprintf(outputFile,"<%s,%d>",ADB.registerName,ADB.registerVal);
	}
//--------------------------print REB buffer-----------------------------------------------
	//printf("\nREB:");
	fprintf(outputFile,"\nREB:");
	
	//print contents of REB if registerName in not NNN and length!=0
	if(strlen(REB[0].registerName) != 0 && strcmp(REB[0].registerName,"NNN") != 0)
	{
		//printf("<%s,%d>",REB[0].registerName,REB[0].registerVal);
		fprintf(outputFile,"<%s,%d>",REB[0].registerName,REB[0].registerVal);
	}
	if(strlen(REB[1].registerName) != 0 && strcmp(REB[1].registerName,"NNN") != 0)
	{
		//printf(",<%s,%d>",REB[1].registerName,REB[1].registerVal);
		fprintf(outputFile,",<%s,%d>",REB[1].registerName,REB[1].registerVal);
	}
	
//-------------register file-------------------------------------------------------
	sortRegfile();
	//printf("\nRGF:");
	fprintf(outputFile,"\nRGF:");
	
	for(i = 0; i < regCount - 1 ; i ++)
	{
		//printf("<%s,%d>,",registers[i].registerName,registers[i].registerVal);
		fprintf(outputFile,"<%s,%d>,",registers[i].registerName,registers[i].registerVal);
	}
	//printf("<%s,%d>",registers[regCount - 1].registerName,registers[regCount - 1].registerVal);
	fprintf(outputFile,"<%s,%d>",registers[regCount - 1].registerName,registers[regCount - 1].registerVal);

//-------------data memory----------------------------------------------------------
	sortMem();
	//printf("\nDAM:");
	fprintf(outputFile,"\nDAM:");
	
	for(i = 0; i < memCount-1 ; i ++)
	{
		//printf("<%d,%d>,",datamemory[i].address,datamemory[i].addressVal);
		fprintf(outputFile,"<%d,%d>,",datamemory[i].address,datamemory[i].addressVal);
	}
	//printf("<%d,%d>\n",datamemory[memCount-1].address,datamemory[memCount-1].addressVal);
	fprintf(outputFile,"<%d,%d>\n",datamemory[memCount-1].address,datamemory[memCount-1].addressVal);
}

void execute_instruction() // execute data in ADB and REB
{
	int regNum = 20, regVal = 0, extraMflag=1,i=0,j=0, extraRflag=1;
	char *token;

	if(strlen(ADB.registerName) != 0 && strcmp(ADB.registerName,"NNN") != 0)
	{
		token  = strtok(ADB.registerName, "R");
		regNum = atoi(token);
		regVal = ADB.registerVal;
		//printf("\n");
		for(i = 0; i < memCount; i ++)
		{
			if(datamemory[i].address == regVal)
			{
				for(j=0;j<16;j++)
				{
					if(strcmp(registers[j].registerName,ADB.registerName) == 0)
					{
						datamemory[i].addressVal = registers[j].registerVal;
						break;
					}
				}
				extraMflag = 0;
				break;
			}
		}
		if(extraMflag == 1)
		{
			datamemory[i].address=regVal;
			for(j=0;j<16;j++)
			{
				if(strcmp(registers[j].registerName,ADB.registerName) == 0)
				{
					datamemory[i].addressVal = registers[j].registerVal;
					break;
				}
			}
			memUsed[i] = registers[regNum].registerVal;
			memCount++;
		}
		extraMflag = 1;
		strcpy(ADB.registerName,"NNN");
		ADB.registerVal = 0;
		ADBused = 0;
	}

	if(strlen(REB[0].registerName) != 0 && strcmp(REB[0].registerName,"NNN") != 0)
	{
		token  = strtok(REB[0].registerName, "R");
		regNum = atoi(token);
		regVal = REB[0].registerVal;
		
		for(i = 0; i < 16 ; i ++)
		{
			if(registerUsed[i] == regNum)
			{
                registerUsed[i]=regVal;
				for(i = 0; i < 16 ; i ++)
                {
                    if(strcmp(REB[0].registerName,registers[i].registerName) == 0)
                    {
                        registers[i].registerVal = regVal;
                        break;
                    }
                }
				extraRflag = 0;
				break;
			}
		}
		if(extraRflag == 1)
		{
			registerUsed[regCount]=regNum;
			strcpy(registers[regCount].registerName,REB[0].registerName);
			registers[regCount].registerVal = regVal;
			regCount++;
		}
		extraRflag = 1;
		if(strlen(REB[1].registerName) != 0 && strcmp(REB[1].registerName,"NNN") != 0)
		{
			strcpy(REB[0].registerName,REB[1].registerName);
			REB[0].registerVal = REB[1].registerVal;
			REBused = 1;
		}
		else
		{
			REBused = 0;
			strcpy(REB[0].registerName,"NNN");
			REB[0].registerVal = 0;
		}
		strcpy(REB[1].registerName,"NNN");
		REB[1].registerVal = 0;
	}
}

void sib_to_adb()
{
	if(strlen(SIB.instruction) != 0 && strcmp(SIB.instruction,"NNN") != 0)
	{
		strcpy(ADB.registerName,SIB.destination);
		ADB.registerVal = SIB.source1 + SIB.immediateVal;
		strcpy(SIB.instruction,"NNN");
		SIB.source1 = 0;
		SIB.immediateVal = 0;
		ADBused = 1;
		SIBused = 0;
	}
}
void inb_to_aib_or_sib()
{
	if(strlen(INB.instruction) != 0 && strcmp(INB.instruction,"NNN") != 0)
	{
		if(strcmp(INB.instruction,"ST") == 0)
		{
			strcpy(SIB.instruction,INB.instruction);
			strcpy(SIB.destination,INB.destination);
			SIB.source1 = INB.source1;
			SIB.immediateVal = INB.immediateVal;
			SIBused = 1;
		}
		else
		{
			strcpy(AIB.instruction,INB.instruction);
			strcpy(AIB.destination,INB.destination);
			AIB.source1 = INB.source1;
			AIB.source2 = INB.source2;
			AIBused = 1;
		}
		strcpy(INB.instruction,"NNN");
		INB.source1 = 0;
		INB.source2 = 0;
		INB.immediateVal = 0;
		INBused=0;
	}
}

void aib_to_prb_or_reb()
{
	int replacedFlag = 0;
	if(strlen(AIB.instruction) != 0 && strcmp(AIB.instruction,"NNN") != 0)
	{
		if(strcmp(AIB.instruction,"MUL") == 0)
		{
			strcpy(PRB.instruction,AIB.instruction);
			strcpy(PRB.destination,AIB.destination);
			PRB.source1 = AIB.source1;
			PRB.source2 = AIB.source2;
			strcpy(AIB.instruction,"NNN");
			PRBused = 1;
		}
		else
		{
			if(strcmp(AIB.instruction,"ADD") == 0)
			{
				if(strlen(REB[0].registerName) == 0 || strcmp(REB[0].registerName,"NNN") == 0)
				{
					strcpy(REB[0].registerName,AIB.destination);
					REB[0].registerVal = AIB.source1 + AIB.source2;
					replacedFlag = 1;
				}
				if((strlen(REB[1].registerName) == 0 || strcmp(REB[1].registerName,"NNN") == 0) && replacedFlag != 1)
				{
					strcpy(REB[1].registerName,AIB.destination);
					REB[1].registerVal = AIB.source1 + AIB.source2;
				}
				strcpy(AIB.instruction,"NNN");
				REBused = 1;
			}
			if(strcmp(AIB.instruction,"SUB") == 0)
			{
				if(strlen(REB[0].registerName) == 0 || strcmp(REB[0].registerName,"NNN") == 0)
				{
					strcpy(REB[0].registerName,AIB.destination);
					REB[0].registerVal = AIB.source1 - AIB.source2;
					replacedFlag = 1;
				}
				if((strlen(REB[1].registerName) == 0 || strcmp(REB[1].registerName,"NNN") == 0) && replacedFlag != 1)
				{
					strcpy(REB[1].registerName,AIB.destination);
					REB[1].registerVal = AIB.source1 - AIB.source2;
				}
				strcpy(AIB.instruction,"NNN");
				REBused = 1;
			}
		}
		AIB.source1 = 0;
		AIB.source2 = 0;
		AIBused = 0;
	}
}

void prb_to_reb()
{
	int replacedFlag = 0;
	if(strlen(PRB.instruction) != 0 && strcmp(PRB.instruction,"NNN") != 0)
	{
		if(strlen(REB[0].registerName) == 0 || strcmp(REB[0].registerName,"NNN") == 0)
		{
			strcpy(REB[0].registerName,PRB.destination);
			REB[0].registerVal = PRB.source1 * PRB.source2;
			replacedFlag = 1;
		}
		if((strlen(REB[1].registerName) == 0 || strcmp(REB[1].registerName,"NNN") == 0) && replacedFlag != 1)
		{
			strcpy(REB[1].registerName,PRB.destination);
			REB[1].registerVal = PRB.source1 * PRB.source2;
		}
		strcpy(PRB.instruction,"NNN");
		PRB.source1 = 0;
		PRB.source2 = 0;
		REBused = 1;
		PRBused = 0;
	}
}

int main()
{
    char *token, *instr, *dest, *src1, *src2, *reg, *regVal, *addr, *addrVal;
    int i;
    char str[MAXCHAR];
    int instr_over = 0, instr_counter = 0;
    int m = 0;
    char regNumber[3];
	
	FILE *instrFile, *regFile, *damFile;

	instrFile = fopen("instructions.txt","r");
	while(fgets(str, 40, instrFile)!=NULL )
    {
		token=strtok(str, "<>");
        instr = strtok(token,",");
        strcpy(instructions[no_of_instructions].instruction,instr);

        dest = strtok(NULL,",");
        strcpy(instructions[no_of_instructions].destination,dest);

		src1 = strtok(NULL,",");
        strcpy(instructions[no_of_instructions].source1,src1);

		src2 = strtok(NULL,",");
        if(strcmp(instr,"ST") == 0)
        {
            instructions[no_of_instructions].immediateVal = atoi(src2);
        }
        else
        {
            strcpy(instructions[no_of_instructions].source2,src2);
        }
        no_of_instructions = no_of_instructions + 1;
	}
	fclose(instrFile);
//---------------------------------------------------------------------------------------------------------------------------------------
	regFile   = fopen("registers.txt","r");
	while(fgets(str, 40, regFile)!=NULL )
	{
        token=strtok(str, "<>");
        reg = strtok(token,",");

        for(m = 1; m<3; m++)
            regNumber[m-1] = reg[m];

        registerUsed[regCount] = atoi(regNumber);
        strcpy(registers[regCount].registerName,reg);

        regVal = strtok(NULL,",");
        registers[regCount].registerVal = atoi(regVal);
		regCount++;

	}
	fclose(regFile);

//---------------------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<16;i++)
		datamemory[i].address=20;

	damFile   = fopen("datamemory.txt","r");
	while(fgets(str, 40, damFile)!=NULL )
	{
        token=strtok(str, "<>");
        addr = strtok(token,",");
        addrVal = strtok(NULL,",");

        datamemory[memCount].address = atoi(addr);
        datamemory[memCount].addressVal = atoi(addrVal);

        memUsed[memCount] = atoi(addr);
        memCount++;

    }
	fclose(damFile);
//---------------------------------------------------------------------------------------------------------------------------------------
	outputFile = fopen("simulation.txt","w");
	//printf("STEP %d:\n",step);
	fprintf(outputFile,"STEP %d:\n",step);
	
	printSimulation();
	// iterate till instructions are there in instructions[]
	while(step!=no_of_instructions && instr_over != 1) 
	{
		execute_instruction();
		sib_to_adb();
		prb_to_reb();
		aib_to_prb_or_reb();
		inb_to_aib_or_sib();
		strcpy(INB.instruction,instructions[step].instruction);
		strcpy(INB.destination,instructions[step].destination);
		for(i=0;i<16;i++)
		{
			if(strcmp(instructions[step].source1,registers[i].registerName)==0)
			{
				INB.source1 = registers[i].registerVal;
				break;
			}
		}

		if(strcmp(INB.instruction,"ST") == 0)
		{
			INB.immediateVal = instructions[step].immediateVal;
		}
		else if( strcmp(INB.instruction,"SUB") == 0 || strcmp(INB.instruction,"ADD") == 0 || strcmp(INB.instruction,"MUL") == 0)
		{
			for(i=0;i<16;i++)
			{
				if(strcmp(instructions[step].source2,registers[i].registerName)==0)
				{
					INB.source2 = registers[i].registerVal;
					break;
				}
			}
		}
		INBused = 1;
		step++;
		//printf("\nSTEP %d:\n",step);
		fprintf(outputFile,"\nSTEP %d:\n",step);
		printSimulation();
	}
	instr_over = 1;
	instr_counter = step + 1;
	// iterate till instructions are there anu buffer
	while(INBused==1 || AIBused==1 || SIBused==1 || PRBused==1 || ADBused==1 || REBused==1)
	{
		execute_instruction();
		sib_to_adb();
		prb_to_reb();
		aib_to_prb_or_reb();
		inb_to_aib_or_sib();
		//printf("\nSTEP %d:\n",instr_counter);
		fprintf(outputFile,"\nSTEP %d:\n",instr_counter);
		printSimulation();
		instr_counter++;
	}
	return 0;
}

