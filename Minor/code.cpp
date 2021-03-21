#include<iostream>
#include<fstream>
#include<map>
#include<math.h>
#include<unordered_map>
#include<vector> 
using namespace std;
int registers[32] = {0};
int freqOfCommand[10] = {0};

string decToHexa(int n){
    // char array to store hexadecimal number
    char hexaDeciNum[100];
 
    // counter for hexadecimal number array
    int i = 0;
    if(n==0){
        return "0";
    }
    else{
        while (n != 0){
            // temporary variable to store remainder
            int temp = 0;
    
            // storing remainder in temp variable.
            temp = n % 16;
    
            // check if temp < 10
            if (temp < 10) {
                hexaDeciNum[i] = temp + 48;
                i++;
            }
            else{
                hexaDeciNum[i] = temp + 55;
                i++;
            }
            n = n / 16;
            }
    }

   string Hex_Val="";
   for (int j = i - 1; j >= 0; j--) Hex_Val+=hexaDeciNum[j];
   
   return Hex_Val;
}

void printRegisters(int numberOfClockCycles){
    cout<<"Cycle number: "<<numberOfClockCycles<<"\n";
    for(int i=0;i<32;i++){
        cout<<decToHexa(registers[i])<<" ";
    }
    cout<<"\n";
}

void printStats(int numberOfClockCycles, int memory[]){
    if(numberOfClockCycles==1){
        cout<<"Input file is empty \n";
    }
    else{
        cout<<"Following is the frequency of each command used: \n";
        cout<<"add:  "<<freqOfCommand[0]<<"\n";
        cout<<"sub:  "<<freqOfCommand[1]<<"\n";
        cout<<"mul:  "<<freqOfCommand[2]<<"\n";
        cout<<"beq:  "<<freqOfCommand[3]<<"\n";
        cout<<"bne:  "<<freqOfCommand[4]<<"\n";
        cout<<"slt:  "<<freqOfCommand[5]<<"\n";
        cout<<"j:    "<<freqOfCommand[6]<<"\n";
        cout<<"lw:   "<<freqOfCommand[7]<<"\n";
        cout<<"sw:   "<<freqOfCommand[8]<<"\n";
        cout<<"addi: "<<freqOfCommand[9]<<"\n";
        cout<<"and the total clock cycles taken is: "<<numberOfClockCycles<<"\n";
    }
}

int main(int argc, char** argv){
    string filePath = argv[1];
    int partNumber = stoi(argv[2]); // Part-1 or Part-2 of the problem
    int ROW_ACCESS_DELAY = stoi(argv[3]); 
    int COL_ACCESS_DELAY = stoi(argv[4]);
    fstream newfile;


    // add  => 1 add $r1,$r2,$r3
    // sub  => 2 sub $r1,$r2,$r3
    // mul  => 3 mul $r2,$r2,$r3
    // addi => 10 addi $r3,$r1,100
    // slt  => 6 slt rd, rs, rt
    // beq  => 4 beq $t0, $t1, 100
    // bne  => 5 bne $t0, $0, 200
    // j    => 7 j target
    // lw   => 8 lw $2, 4($4)    # $2 <- mem($4+4)
    // sw   => 9  sw $5, 8($7)   # mem[$7+8] <- $5
    
    unordered_map<string,int> instructionNumber;
    unordered_map<string,int> registerNumber;
    // Integer values representing different instructions
    instructionNumber["add"] = 1;
    instructionNumber["sub"] = 2;
    instructionNumber["mul"] = 3;
    instructionNumber["beq"] = 4;
    instructionNumber["bne"] = 5;
    instructionNumber["slt"] = 6;
    instructionNumber["j"] = 7;
    instructionNumber["lw"] = 8;
    instructionNumber["sw"] = 9;
    instructionNumber["addi"] = 10;

    // Integer values representing different register names.
    // Note that I avoided loop here and mentioned each value separately so that this piece of code also
    // serves as a look-up table for values that I have given to different registers. 
    registerNumber["$zero"] = 0;
	registerNumber["$at"] = 1;
	registerNumber["$v0"] = 2;
	registerNumber["$v1"] = 3;
    registerNumber["$a0"] = 4;
    registerNumber["$a1"] = 5;
    registerNumber["$a2"] = 6;
    registerNumber["$a3"] = 7;
    registerNumber["$t0"] = 8;
    registerNumber["$t1"] = 9;
    registerNumber["$t2"] = 10;
    registerNumber["$t3"] = 11;
    registerNumber["$t4"] = 12;
    registerNumber["$t5"] = 13;
    registerNumber["$t6"] = 14;
    registerNumber["$t7"] = 15;
    registerNumber["$s0"] = 16;
    registerNumber["$s1"] = 17;
    registerNumber["$s2"] = 18;
    registerNumber["$s3"] = 19;
    registerNumber["$s4"] = 20;
    registerNumber["$s5"] = 21;
    registerNumber["$s6"] = 22;
    registerNumber["$s7"] = 23;
    registerNumber["$t8"] = 24;
    registerNumber["$t9"] = 25;
    registerNumber["$k0"] = 26;
    registerNumber["$k1"] = 27;
    registerNumber["$gp"] = 28;
    registerNumber["$sp"] = 29;
    registerNumber["$s8"] = 30;
    registerNumber["$ra"] = 31;


    int x = pow(2,20);
    int memory[x];
    for(int i=0;i<x;i++){
        memory[i] = -1;
    }

    try
    {
        newfile.open(filePath,ios::in);
        int i1=0;
       if(newfile.is_open()){ // opening the file
          string tp;
          int command;
          int r1,r2;
          while(getline(newfile,tp)){
             vector<string>v; // declaring vector to store instruction command and parameters
             string s=""; 
             string s1;
             int cnt=0;
             for(int i=0;i<tp.size();i++){
                if(tp[i]!=' ' && tp[i]!= ','){
                    cnt=0;
                   s+=tp[i];
                }
                else{
                    if(cnt==0){
                        v.push_back(s);
                        s="";
                        cnt++;
                    }
                }
          }


            v.push_back(s);
            int l = v.size();
            if(v[0]!=""){
                command = instructionNumber[v[0].substr(1)];
            }
            

            if(command ==0){
                if(v.size()==1){
                    continue;
                }
                else{
                    cout<<"Invalid Command\n" <<"\n";
                    return 1;
                }
            }


            if(l==0){
                continue;
            }
            // Storing values in memory corresponding to different operators.
            if(l==4){ 
                r1 = registerNumber[v[1]]; // adding register values to the variables
                r2 = registerNumber[v[2]]; // adding register values to the variables
                if(r1<0 || r1>31 || r2<0 || r2>31){
                    cout<<"Invalid register name\n";
                    return 1;
                }
                // Adding instruction in memory
                memory[i1] = command;
                memory[i1+1] = r1;
                memory[i1+2] = r2;
                if(command<4 || command==6){
                    memory[i1+3] = registerNumber[v[3]];
                }
                else{
                    memory[i1+3]= stoi(v[3]);
                }
                i1 = i1+4;
            }
            else if(l==3){
                r1 = registerNumber[v[1]]; // adding register values to the variables
                if(r1<0 || r1>31){
                    cout<<"Invalid register name\n";
                    return 1;
                }
                int it=0;
                for(it=0;it<v[2].length();it++){
                    if(v[2][it]=='('){
                        break;
                    }
                }
                int offset = stoi(v[2].substr(0,it));
                if(offset<0 || offset>pow(2,20) || offset%4!=0){
                    cout<<"Invalid memory location access\n";
                    return 1;
                }
                string sourceRegister = v[2].substr(it+1,v[2].length()-1);
                r2 = registerNumber[sourceRegister.substr(0,sourceRegister.length()-1)];
                if(r2<0 || r2>31){
                    cout<<"Invalid register name\n";
                    return 1;
                }
                // Adding instruction in memory
                memory[i1] = command;
                memory[i1+1] = r1;
                memory[i1+2] = r2;
                memory[i1+3] = offset;
                i1 = i1+4;
            }
            else if (l==2){ // j operator
                memory[i1] = command;
                if(stoi(v[1])<0 || stoi(v[1])>pow(2,20) || stoi(v[1])%4!=0){
                    cout<<"Invalid memory location access\n";
                    return 1;
                }
                memory[i1+1] = stoi(v[1]);
                memory[i1+2] = 0;
                memory[i1+3] = 0;
                i1 = i1+4;
            }
            cout<<"Memory i is "<<memory[i1-4]<<"\n";
            cout<<"Memory i+1 is "<<memory[i1-3]<<"\n";
            cout<<"Memory i+2 is "<<memory[i1-2]<<"\n";
            cout<<"Memory i+3 is "<<memory[i1-1]<<"\n";
            cout<<"\n";
        }
    }
    else{
        cout<<"Cannot open file\n";
        return 0;
    }
    }
    catch(const std::exception& e)
    {
        cerr<<"Invalid syntax in file\n";
        return 1;
    }
    
    
    int rowOfDRAM = -1;
    int engagedForDuration=0;
    int registerInDRAM = -1;
    x = pow(2,20);
    int i=0;
    int numberOfClockCycles=0;
    while(i<x){
        if(i==-1 || memory[i]==-1){ // Exiting condition for the program
            break;
        }
        int c = memory[i];
        switch (c) // Do the computation corresponding to the operator
        {
            // LOGIC PART BEGINS HERE
        case 1:
            if(registerInDRAM==memory[i+1] || registerInDRAM==memory[i+2] || registerInDRAM==memory[i+3]){
                if(engagedForDuration>0){
                    numberOfClockCycles+=engagedForDuration; 
                    engagedForDuration=0;
                }
            }
            if(engagedForDuration>0){
                engagedForDuration--;
            }
            numberOfClockCycles++;
            freqOfCommand[0]++;
            registers[memory[i+1]] = registers[memory[i+2]] + registers[memory[i+3]];
            i=i+4;
            break;
        case 2:
            numberOfClockCycles++;
            freqOfCommand[1]++;
            registers[memory[i+1]] = registers[memory[i+2]] - registers[memory[i+3]];
            i=i+4;
            break;
        case 3:  
            numberOfClockCycles++;
            freqOfCommand[2]++;
            registers[memory[i+1]] = registers[memory[i+2]] * registers[memory[i+3]];
            i=i+4;
            break;
        case 4:
            numberOfClockCycles++;
            freqOfCommand[3]++;
            if(registers[memory[i+1]] == registers[memory[i+2]]){
                i = memory[i+3];
                break;
            }
            else{
                i = i+4;
                break;
            }
        case 5:
            numberOfClockCycles++;
            freqOfCommand[4]++;
            if(registers[memory[i+1]] != registers[memory[i+2]]){
                i = memory[i+3];
                break;
            }
            else{
                i = i+4;
                break;
            }
        case 6:
            numberOfClockCycles++;
            freqOfCommand[5]++;
            if(registers[memory[i+2]] < registers[memory[i+3]]){
                registers[memory[i+1]] = 1;
            }
            else{
                registers[memory[i+1]] = 0;
            }
            i=i+4;
            break;
        case 7:
            numberOfClockCycles++;
            freqOfCommand[6]++;
            i = memory[i+1];
            break;
        case 8:
            if(partNumber==1){
                numberOfClockCycles++; //DRAM Request issued
                if(freqOfCommand[7]+freqOfCommand[8]==0){
                    // Fresh beginning to the row buffer
                    numberOfClockCycles+=ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    rowOfDRAM = (registers[memory[i+2]] + memory[i+3])/1024;
                }
                else{
                    cout<<"LOL\n";
                    int rowNum = (registers[memory[i+2]] + memory[i+3])/1024;
                    if(rowNum==rowOfDRAM){
                        numberOfClockCycles+=COL_ACCESS_DELAY;

                    }
                    else{
                        numberOfClockCycles+=ROW_ACCESS_DELAY*2 + COL_ACCESS_DELAY;
                        rowOfDRAM = rowNum;
                    }
                }
            }
            else{
                if(freqOfCommand[7]+freqOfCommand[8]==0){
                    // Fresh beginning to the row buffer
                    numberOfClockCycles++;
                    rowOfDRAM = (registers[memory[i+2]] + memory[i+3])/1024;
                    engagedForDuration = ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                }
                else{
                    int rowNum = (registers[memory[i+2]] + memory[i+3])/1024;
                    numberOfClockCycles+=engagedForDuration; // prev instruction completed
                    numberOfClockCycles++; // DRAM request called
                    if(rowNum==rowOfDRAM){
                        engagedForDuration = COL_ACCESS_DELAY; // Engaged for new instruction
                    }
                    else{ 
                        rowOfDRAM = rowNum;
                        // Engaged for new instruction
                        engagedForDuration = ROW_ACCESS_DELAY*2 + COL_ACCESS_DELAY; 
                    }
                }
            }
            registerInDRAM = memory[i+1];
            freqOfCommand[7]++;
            registers[memory[i+1]] = memory[registers[memory[i+2]] + memory[i+3]];
            i = i+4;
            break;
        case 9:
            if(partNumber==1){
                numberOfClockCycles++; //DRAM Request issued
                if(freqOfCommand[7]+freqOfCommand[8]==0){
                    // Fresh beginning to the row buffer
                    numberOfClockCycles+=ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                    rowOfDRAM = (registers[memory[i+2]] + memory[i+3])/1024;
                }
                else{
                    cout<<"LOL\n";
                    int rowNum = (registers[memory[i+2]] + memory[i+3])/1024;
                    if(rowNum==rowOfDRAM){
                        numberOfClockCycles+=COL_ACCESS_DELAY;

                    }
                    else{
                        numberOfClockCycles+=ROW_ACCESS_DELAY*2 + COL_ACCESS_DELAY;
                        rowOfDRAM = rowNum;
                    }
                }
            }
            else{
                if(freqOfCommand[7]+freqOfCommand[8]==0){
                    // Fresh beginning to the row buffer
                    numberOfClockCycles++;
                    rowOfDRAM = (registers[memory[i+2]] + memory[i+3])/1024;
                    engagedForDuration = ROW_ACCESS_DELAY + COL_ACCESS_DELAY;
                }
                else{
                    int rowNum = (registers[memory[i+2]] + memory[i+3])/1024;
                    numberOfClockCycles+=engagedForDuration; // prev instruction completed
                    numberOfClockCycles++; // DRAM request called
                    if(rowNum==rowOfDRAM){
                        engagedForDuration = COL_ACCESS_DELAY; // Engaged for new instruction
                    }
                    else{ 
                        rowOfDRAM = rowNum;
                        // Engaged for new instruction
                        engagedForDuration = ROW_ACCESS_DELAY*2 + COL_ACCESS_DELAY; 
                    }
                }
            }
            registerInDRAM = memory[i+1];
            freqOfCommand[8]++;
            memory[registers[memory[i+2]] + memory[i+3]] = registers[memory[i+1]];
            i = i + 4;
            break;
        case 10:
            if(engagedForDuration>0 && (registerInDRAM==memory[i+1] || registerInDRAM==memory[i+2])){
                numberOfClockCycles+=engagedForDuration; 
                engagedForDuration=0;
            }
            if(engagedForDuration>0){
                engagedForDuration--;
            }
            numberOfClockCycles++;
            freqOfCommand[9]++;
            registers[memory[i+1]] = registers[memory[i+2]] + memory[i+3];
            i = i+4;
            break;
        default:
            i=-1;   
        }
        printRegisters(numberOfClockCycles);
    }
    numberOfClockCycles+=engagedForDuration;
    printStats(numberOfClockCycles, memory);
    
    return 0;
}