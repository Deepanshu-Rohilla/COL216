#include<iostream>
#include<fstream>
#include<map>
#include<math.h>
#include<unordered_map>
#include<vector> 
#include<set>
//#include "code_2.cpp"
#include <queue>

using namespace std;


int registers[32] = {0};
int freqOfCommand[10] = {0};
int occurenceInQueue[32] = {0};
int x = pow(2,20);
int DRAM[1024][1024];
int Row_Buffer[1024];
int ROW_ACCESS_DELAY;
int COL_ACCESS_DELAY;
string Register_map[32];
deque<vector<int>> queueArray[1024];
int cnt1;
int row=-1;          //this is used in lw and sw as row while accesssing DRAM
int col=-1;          ////this is used in lw and sw as row while accesssing DRAM
int number_of_row_buffer_update=0;
int numberOfClockCycles=0;
int counter=0;
int Array_cmd[1024]={0};


bool check_memory_address(int memory_address,int cnt1,int x){              // vert that memory _address used in lw and sw are valid or not
                if(memory_address%4!=0 && memory_address < 4*cnt1 ){
                    cout<<"Error:\n";
                    cout<<"Memory of ""DATA INSTRUCTION"" cannot be used for store operation and  Memory_Address must be multiple of 4 as per DRAM_MEMORY Architecture"<<"\n";
                    return true;
                }else if(memory_address%4!=0 &&  memory_address >=x){
                    cout<<"Error:\n";
                    cout<<"Memory  ""OUT OF BOUND"" cannot be used for store operation and  Memory_Address must be multiple of 4 as per DRAM_MEMORY Architecture"<<"\n";
                    return true;
                }else if(memory_address%4!=0){
                    cout<<"Error:\n";
                    cout<<"Memory_Address must be multiple of 4 as per DRAM_MEMORY Architecture"<<"\n";
                    return true;

                }else if(memory_address < 4*cnt1){
                    cout<<"Error:\n";
                    cout<<"Memory of ""DATA INSTRUCTION"" cannot be used for store operation "<<"\n";
                    return true;

                }else if( memory_address >=x){
                    cout<<"Error:\n";
                    cout<<"Memory  ""OUT OF BOUND"" cannot be used for store operation "<<"\n";
                    return true;
                }
        return false;
}

void load_data(int Row_Buffer [],int DRAM [] [1024] ,int row){    //Copy  DRAM to from Row_buffer

    for(int i=0;i<1024;i++){
        Row_Buffer[i]=DRAM[row][i];
    }
}

void load_data_back(int Row_Buffer [],int DRAM [] [1024] ,int row){    //for Writing back from Row_buffer to DRAM
    for(int i=0;i<1024;i++){
        DRAM[row][i]=Row_Buffer[i];
    }
}

string decToHexa(int n){  // converting decimal to hexadecimal
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
                //hexaDeciNum[i] = temp + 55; 
                hexaDeciNum[i] = temp + 87;
                i++;
            }
            n = n / 16;
            }
    }

   string Hex_Val="";
   for (int j = i - 1; j >= 0; j--) Hex_Val+=hexaDeciNum[j];
   
   return Hex_Val;
}

void printRegisters(int numberOfClockCycles){   // to print all register contents
    // cout<<"Fointowing are the values in registers after "<<numberOfClockCycles<<" cycles\n";
    cout<<"Cycle number: "<<numberOfClockCycles<<"\n";
    for(int i=0;i<32;i++){
        // cout<<" Value stored in $r"<<i<<" is "<<decToHexa(registers[i])<<"\n";
        //cout<<decToHexa(registers[i])<<" ";
         cout<<(registers[i])<<" ";
    }
    cout<<"\n";
}

void printChange_in_Each_Cycle(int ROW_ACCESS_DELAY,int COL_ACCESS_DELAY,int type,int instruction_Num,int numberOfClockCycles,string inst_name,string Register_mod,string Memory ,string DARM_Activity){
    // to print all possible changes in each cycle

   
    if(instruction_Num!=8 && instruction_Num!=9){
        cout<<"Cycle number "<<numberOfClockCycles<<": ";
       cout<<"Executed Instruction : "+ inst_name+" \n" +"Modified Register : "+Register_mod+" \n"+"Modified Memory Location : " + Memory + "\n"+"Activity on DRAM : " +DARM_Activity+ " .";
    }

    if(instruction_Num ==9 || instruction_Num==8){
        if(type==0){

           cout<<"Cycle number "<<numberOfClockCycles<<": ";
           cout<<"Executed Instruction : "+ inst_name+" \n" +"Modified Register : "+Register_mod+" \n"+"Modified Memory Location : " + Memory + "\n"+"Activity on DRAM : " +DARM_Activity+ " .";
        }else if(type==1){

           cout<<"Cycle number "<<(numberOfClockCycles-COL_ACCESS_DELAY+1)<<" - "<<numberOfClockCycles<<": ";
            cout<<"Executed Instruction : "+ inst_name+" \n" +"Modified Register : "+Register_mod+" \n"+"Modified Memory Location : " + Memory + "\n"+"Activity on DRAM : " +DARM_Activity+ " .";

        }else if(type==2){

           cout<<"Cycle number "<<(numberOfClockCycles-ROW_ACCESS_DELAY+1)<<" - "<<numberOfClockCycles<<": ";
           cout<<"Executed Instruction : "+ inst_name+" \n" +"Modified Register : "+Register_mod+" \n"+"Modified Memory Location : " + Memory + "\n"+"Activity on DRAM : " +DARM_Activity+ " .";

        }
    }

    cout<<"\n\n";
}

void printStats(int numberOfClockCycles,int number_of_row_buffer_update){   // After program complete it's execution of all it's instruction then Doing Stastics
    cout<<"Following are relevant Statistics :"<<"\n";
    if(numberOfClockCycles==0){
        cout<<"Input file is empty \n";
    }
    else{
        cout<<"Total execution time in  Clock Cycles is: "<<numberOfClockCycles<<"\n";
        cout<<"Number of row buffer updates: "<<number_of_row_buffer_update<<"\n";
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
       
    }
    
}

void print_lw(int rowNum,vector<int> v){

    int memory_address = v[2]+registers[v[3]];
    // if(check_memory_address(memory_address,cnt1,x)){
    //         return 1;  //terminate the program
    // }
    // else{
        int i = v[4];
        if(row==-1){

        // row =(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
            row = rowNum;
            load_data(Row_Buffer,DRAM,row);
            numberOfClockCycles+=ROW_ACCESS_DELAY;
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,8,numberOfClockCycles,"Nothing","Nothing","Nothing","Loading row " +to_string(row)+ " into row-buffer");
            col= (DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
            registers[DRAM[(i+1)/1024][(i+1)%1024]]=Row_Buffer[col];
            numberOfClockCycles+=COL_ACCESS_DELAY;
            number_of_row_buffer_update+=1;
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,8,numberOfClockCycles,"Nothing",Register_map[DRAM[(i+1)/1024][(i+1)%1024]] +" = "+to_string(Row_Buffer[col]) ,"Nothing","DARM column access and write value in register");

        }
        else if(row>=0 && row < 1024){
             //row = rowNum;
            if(row==(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024){
                col= (DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
                registers[DRAM[(i+1)/1024][(i+1)%1024]]=Row_Buffer[col];
                numberOfClockCycles+=COL_ACCESS_DELAY;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,8,numberOfClockCycles,"Nothing",Register_map[DRAM[(i+1)/1024][(i+1)%1024]] +" = "+to_string(Row_Buffer[col]),"Nothing" ,"DARM column access and write value in register");
            }
            else{
                load_data_back(Row_Buffer,DRAM,row);
                numberOfClockCycles+=ROW_ACCESS_DELAY;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,8,numberOfClockCycles,"Nothing","Nothing","Nothing","Write-back row: "+to_string(row));
                row=(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
                load_data(Row_Buffer,DRAM,row);
                numberOfClockCycles+=ROW_ACCESS_DELAY;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,8,numberOfClockCycles,"Nothing","Nothing","Nothing","Loading row " +to_string(row)+ " into row-buffer ");
                col=(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
                registers[DRAM[(i+1)/1024][(i+1)%1024]]=Row_Buffer[col];
                numberOfClockCycles+=COL_ACCESS_DELAY;
                number_of_row_buffer_update+=1;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,8,numberOfClockCycles,"Nothing",Register_map[DRAM[(i+1)/1024][(i+1)%1024]] +" = "+to_string(Row_Buffer[col]),"Nothing ","DARM column access and write value in register");
            }

        }
                //registers[DRAM[i+1]] = DRAM[DRAM[i+2]+registers[DRAM[i+3]]];
                // i = i+4;
    //}



}
void print_sw(int rowNum,vector<int> v){

        int memory_address = v[2]+registers[v[3]];
        // if(check_memory_address(memory_address,cnt1,x)){
        //             return 1;  //terminate the program
        // }else{

            int i = v[4];
            if(row==-1){    // When first time "sw" is called (means when row_buffer is empty)
                    
                row = rowNum;
                row =(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
                load_data(Row_Buffer,DRAM,row);
                numberOfClockCycles+=ROW_ACCESS_DELAY;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,9,numberOfClockCycles,"Nothing","Nothing","Nothing","Activate row: "+to_string(row));
                col= (DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
                Row_Buffer[col]=registers[DRAM[(i+1)/1024][(i+1)%1024]];
                numberOfClockCycles+=COL_ACCESS_DELAY;
                number_of_row_buffer_update+=2;
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,9,numberOfClockCycles,"Nothing","Nothing","Address "+to_string(memory_address)+" = "+to_string(Row_Buffer[col]),"DARM column access and update");
                // DRAM[row][col]=registers[DRAM[(i+1)/1024][(i+1)%1024]];
                

                }else if(row>=0 && row < 1024){

                       /// row = rowNum;
                        //cout<<row<<"\n"<<"::";
                        if(row==(DRAM[(i+2)/1024][(i+2)%1024]+ registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024){  // it same row come and only column_offset requirr to modify data   
                            col= (DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
                            Row_Buffer[col]=registers[DRAM[(i+1)/1024][(i+1)%1024]];
                            numberOfClockCycles+=COL_ACCESS_DELAY;
                            number_of_row_buffer_update+=1;
                            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,9,numberOfClockCycles,"Nothing","Nothing","Address "+to_string(memory_address)+" = "+to_string(Row_Buffer[col]),"DARM column access and update");

                        }else{   // When row's come different then writing back and copying take place along with column offset
                            load_data_back(Row_Buffer,DRAM,row);
                            numberOfClockCycles+=ROW_ACCESS_DELAY;
                            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,9,numberOfClockCycles,"Nothing","Nothing","Nothing","Write-back row: "+to_string(row));
                            row=(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
                            load_data(Row_Buffer,DRAM,row);
                            numberOfClockCycles+=ROW_ACCESS_DELAY;
                             printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,2,9,numberOfClockCycles,"Nothing","Nothing","Nothing","Activate row: "+to_string(row));
                            col= (DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024;
                            Row_Buffer[col]=registers[DRAM[(i+1)/1024][(i+1)%1024]];
                            //cout<<registers[DRAM[(i+1)/1024][(i+1)%1024]]<<"\n";
                            numberOfClockCycles+=COL_ACCESS_DELAY;
                            number_of_row_buffer_update+=2;
                            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,1,9,numberOfClockCycles,"Nothing","Nothing","Address "+to_string(memory_address)+" = "+to_string(Row_Buffer[col]),"DARM column access and update");
                        }

                }

                DRAM[(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024][(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])%1024] = registers[DRAM[(i+1)/1024][(i+1)%1024]];




}

void executeQueue(deque<vector<int>> &q, int rowNum){
    auto it = q.begin();
    while(it!=q.end()){
        if((*it)[0]==8){
            print_lw(rowNum,(*it));
        }
        else{
            print_sw(rowNum,(it));
        }
        it++;
    }
}

void execute(set<int>&s_row){
   
    auto it1 = s_row.begin();
    int j=0;
    deque<vector<int>> q;
    while(it1!=s_row.end() && row!=-1){
        j=(*it1);
        if(j==row){
            //cout<<j<<"\n";
            q=queueArray[j];
            executeQueue(q,j);
            queueArray[j].clear();
            s_row.erase(it1);
           break;

        }

        it1++;
    }
    auto it = s_row.begin();
    while(it!=s_row.end()){
        int i = (*it);
        q =queueArray[i];
        // deque<vector<int>> q =queueArray[i];
        executeQueue(q,i);
        queueArray[i].clear();

        it++;
    }
   
}

void isSafe(vector<int>&v_2){
    //cout<<"hello"<<"\n";
   set<int>s_row;
    if(v_2[0]==1 || v_2[0]==2 || v_2[0]==3 || v_2[0]==6){
        for(int i=0;i<1024;i++){
            if(!queueArray[i].empty()){
               // cout<<i<<"\n";
                auto it = queueArray[i].begin();
                while(it!=queueArray[i].end()){
                    if((*it)[0]==8){
                        if(((*it)[1]==v_2[1] || (*it)[1]==v_2[2] || (*it)[1]==v_2[3] &&(*it)[1]!=0)  || ( (*it)[3]==v_2[1] && (*it)[3]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                    else{
                        if(((*it)[1]==v_2[1] || (*it)[3]==v_2[1]) && (v_2[1]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                }
            }
        }
    }
    else if(v_2[0]==4 || v_2[0]==5){
        for(int i=0;i<1024;i++){
            if(!queueArray[i].empty()){
               // cout<<i<<"\n";
                auto it = queueArray[i].begin();
                while(it!=queueArray[i].end()){
                    if((*it)[0]==8){
                        if(((*it)[1]==v_2[1] || (*it)[1]==v_2[2]) && ((*it)[1]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                    else{
                        it++;
                    }
                }
            }
        }
    }
    else if(v_2[0]==10){
        for(int i=0;i<1024;i++){
            if(!queueArray[i].empty()){
               // cout<<i<<"\n";
                auto it = queueArray[i].begin();
                while(it!=queueArray[i].end()){
                    if((*it)[0]==8){
                        if(((*it)[1]==v_2[1] || (*it)[1]==v_2[2] && (*it)[1]!=0) || (*it)[3]==v_2[1] && v_2[1]!=0){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                    else{
                        if(((*it)[1]==v_2[1] || (*it)[3]==v_2[1]) && (v_2[1]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                }
            }
        }
    }
   else if(v_2[0]==8 ){
        for(int i=0;i<1024;i++){
            if(!queueArray[i].empty()){
               // cout<<i<<"\n";
                auto it = queueArray[i].begin();
                while(it!=queueArray[i].end()){
                    if((*it)[0]==8){
                        if(((*it)[1]==v_2[3] || (*it)[1]==v_2[1] && (*it)[1]!=0)  || ( (*it)[3]==v_2[1] && (*it)[3]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                    else{
                        if(((*it)[1]==v_2[1] || (*it)[3]==v_2[1]) && (v_2[1]!=0)){
                            s_row.insert(i);
                            break;
                        }
                        else{
                            it++;
                        }
                    }
                }
            }
        }
    }
   else if(v_2[0]==9 ){
        for(int i=0;i<1024;i++){
            if(!queueArray[i].empty()){
               // cout<<i<<"\n";
                auto it = queueArray[i].begin();
                while(it!=queueArray[i].end()){
                    if((*it)[0]==8){
                        if(((*it)[1]==v_2[3] || (*it)[1]==v_2[1] && (*it)[1]!=0) ){
                            s_row.insert(i);
                            break;
                        }
                        
                    }
                    it++;
                    // else{
                    //     if(((*it)[1]==v_2[1] || (*it)[3]==v_2[1]) && (v_2[1]!=0)){
                    //         s_row.insert(i);
                    //         break;
                    //     }
                    //     else{
                    //         it++;
                    //     }
                    // }
                }
            }
        }
    }
    if(s_row.size()!=0){
       execute(s_row);
    }
}











int main(int argc, char** argv){

    string filePath = argv[1];
    fstream newfile;
    ROW_ACCESS_DELAY=stoi(argv[2]);
    COL_ACCESS_DELAY=stoi(argv[3]);
    
    unordered_map<string,int> instructionNumber;
    unordered_map<string,int> Line_Number;
    unordered_map<int,string> map_line;
    vector<pair<int,bool>> Instruction_to_Run;

    instructionNumber["add"] = 1;        // mapping of instuction with number so that it can easily handle as number while using array 
    instructionNumber["sub"] = 2;
    instructionNumber["mul"] = 3;
    instructionNumber["beq"] = 4;
    instructionNumber["bne"] = 5;
    instructionNumber["slt"] = 6;
    instructionNumber["j"] = 7;
    instructionNumber["lw"] = 8;
    instructionNumber["sw"] = 9;
    instructionNumber["addi"] = 10;

    // int cnt1=0;                   // it count's total number of instruction 
    cnt1=0;
    for(int i=0;i<x;i++){
       
        DRAM[(i/1024)][(i%1024)]=0;         //initalizing DRAM with value "0"
    }

    try
    {
        newfile.open(filePath,ios::in);        // commmand for opening a file using it's address
        int i1=0;

       if(newfile.is_open()){ // opening the file

          string tp;
          int command;            // this is used to store command number for ex-> for add it is "1"
        //   int r1,r2;

          while(getline(newfile,tp)){  //reading file line by line as string  and accessing each information from an instruction

             vector<string>v; // declaring vector to store instruction command and parameters
             string s="";  // this is used to extract valid string from stream of character
             int cnt=0;  // this is used to handle charcter

             for(int i=0;i<tp.size();i++){
               
               if(((tp[i]-'a')>=0 && (tp[i]-'a')<=25)  || (tp[i]=='$') || (tp[i]==' ') || (tp[i]==',') || ((tp[i]-'0')>=0 && (tp[i]-'0')<=9) || (tp[i] ==':') || (tp[i]=='\t') ||(tp[i]==')') || (tp[i]=='(')){  // condition for acception valid character
             
                if(tp[i]!=' ' && tp[i]!= ',' && tp[i]!='\t' && (tp[i]!=')') && (tp[i]!='(') ){  // neglecting useless character

                      s+=tp[i];
            
                }
                else{
                        if(s!=""){
                           v.push_back(s);  // putting valid string into vector
                        }
                        s="";
                    }
               }
            else{
                cout<<"Invalid Syntax\n";    // Handling error for invalid syntax 
                return 1;
            }
          }
            if(s!=""){
              v.push_back(s);

            }
            int l = v.size();

            if(l==0){
                continue;
            }

            command = instructionNumber[v[0]];

            if(command ==0){         // checking whether the valid string is label or not

                if(l==1 && v[0][v[0].size()-1]==':'){
                    Line_Number[v[0]]=cnt1*4;

                }else{

                   cout<<"Invalid Command\n" <<"\n";
                return 1;
                }
            }

            int reg[3]={0};             // used to store register number from 0-31 for each type of register 

            //conversion of corresponding register
          
        for(int i=1;i<l;i++){                //extracting register and converting into number from 0-31

            if(v[i]=="$zero"){
         
                reg[i-1] =0;
                Register_map[0]=v[i]; 
              
            }else if(v[i]=="$at"){
                reg[i-1] =1;
                Register_map[1]=v[i];  

            }else if(v[i]=="$gp"){
                reg[i-1] =28;
                Register_map[28]=v[i];  

            }else if(v[i]=="$sp"){

                reg[i-1] =29;
                Register_map[29]=v[i];  
            }else if(v[i]=="$fp"){

               reg[i-1] =30;
               Register_map[30]=v[i];  
 
            }else if(v[i]=="$ra"){
                reg[i-1] =31; 
                Register_map[31]=v[i];  

            }else if(v[i][0]=='$' && v[i][1]=='v') {
                if(stoi(v[i].substr(2))>=0 && stoi(v[i].substr(2))<=1){
                  reg[i-1]=stoi(v[i].substr(2))+2;
                  Register_map[reg[i-1]]=v[i];
                }else{
                    cout<<v[i]<<" is invalid register."<<"\n";
                    return 1;
                }
                 
            }else if(v[i][0]=='$' && v[i][1]=='a') {
                if(stoi(v[i].substr(2))>=0 && stoi(v[i].substr(2))<=3){
                    reg[i-1]=stoi(v[i].substr(2))+4;
                    Register_map[reg[i-1]]=v[i];
                 
                }else{
                    cout<<v[i]<<" is invalid register."<<"\n";
                    return 1;
                }
              
            }else if(v[i][0]=='$' && v[i][1]=='t') {
                
                if(stoi(v[i].substr(2))<=7 && stoi(v[i].substr(2))>=0){
                  reg[i-1]=stoi(v[i].substr(2))+8;
                  Register_map[reg[i-1]]=v[i];
                }else if (stoi(v[i].substr(2))<=9 && stoi(v[i].substr(2))>=8){
                      reg[i-1]=stoi(v[i].substr(2))+16;
                      Register_map[reg[i-1]]=v[i];
                }else{
                    cout<<v[i]<<" is invalid register."<<"\n";
                    return 1;
                }
                 
            }else if(v[i][0]=='$' && v[i][1]=='s') {

                if(stoi(v[i].substr(2))>=0 && stoi(v[i].substr(2))<=7){
       
                  reg[i-1]=stoi(v[i].substr(2))+16;
                  Register_map[reg[i-1]]=v[i];
                }else{
                     cout<<v[i]<<" is invalid register."<<"\n";
                     return 1;
                }
                 
            }else if(v[i][0]=='$' && v[i][1]=='k') {
  
                if(stoi(v[i].substr(2))>=0 && stoi(v[i].substr(2))<=1){
       
                  reg[i-1]=stoi(v[i].substr(2))+26;
                  Register_map[reg[i-1]]=v[i];
                }else{
                     cout<<v[i]<<" is invalid register."<<"\n";
                     return 1;
                }               
            
                 
            }else if(v[i][0]=='$'){    // assuming that label cannot start with $ symbol
                cout<<"Invalid Register used:"<<"\n";
                return 1;
            }

        }

            // Storing values in DRAM corresponding to different operators.
            if(l==4){                 // this store all 9 operation except "unconditional jump"
                cnt1++;
                DRAM[i1/1024][i1%1024] = command;
            
                if(command<4 || command==6){
              
                      DRAM[(i1+1)/1024][(i1+1)%1024] = reg[0];
                      DRAM[(i1+2)/1024][(i1+2)%1024] = reg[1];
                      DRAM[(i1+3)/1024][(i1+3)%1024] = reg[2];

                }else if(command==8 || command==9){                   //handling "lw" and "sw"

                      DRAM[(i1+1)/1024][(i1+1)%1024]= reg[0];
                      DRAM[(i1+2)/1024][(i1+2)%1024] = stoi(v[2]);
                      DRAM[(i1+3)/1024][(i1+3)%1024]= reg[2];
                }
                else{
                     if(v[0]=="addi"){

                         DRAM[(i1+1)/1024][(i1+1)%1024] = reg[0];
                         DRAM[(i1+2)/1024][(i1+2)%1024] = reg[1];
                         DRAM[(i1+3)/1024][(i1+3)%1024]= stoi(v[3]);

                     }else{
                         
                         DRAM[(i1+1)/1024][(i1+1)%1024]  = reg[0];
                         DRAM[(i1+2)/1024][(i1+2)%1024] = reg[1];
                         DRAM[(i1+3)/1024][(i1+3)%1024]= -2;                       
                         map_line[cnt1]=v[3]+":";
                     }
                      
                }
                i1 = i1+4;
            }
            else if (l==2){ // j operator
                cnt1++;
                DRAM[i1/1024][i1%1024]=command;
                DRAM[(i1+1)/1024][(i1+1)%1024] = -2;
                DRAM[(i1+2)/1024][(i1+2)%1024] = 0;
                DRAM[(i1+3)/1024][(i1+3)%1024]= 0; 
                map_line[cnt1]=v[1]+":";
                i1 = i1+4;
            }
        }
    }
    else{
        cout<<"Cannot open file\n";
        return 0;
     }

    }
    catch(const std::exception& e)
    {
        
        cerr<<"Invalid syntax in file\n";            //Through error 
        return 1;
    }
    
    auto it=map_line.begin();   // for label type operator mapping label string with "line number in instruction memory where one suppose to jump"

    while(it!=map_line.end()){

        if(DRAM[((it->first)*4-1)/1024][((it->first)*4-1)%1024]==-2){
       
           DRAM[((it->first)*4-1)/1024][((it->first)*4-1)%1024]=Line_Number[it->second];

        }else{
           DRAM[((it->first)*4-3)/1024][((it->first)*4-3)%1024]=Line_Number[it->second];
        }
        it++;

    }

    // for ( int i=0;i<4*cnt1;i+=4){
    //         Instruction_to_Run.push_back({i,false});    // Intializing the execution of each instruction set "false " so that Non-Memory block can be Perform
    // }
    /*for(int i=0;i<4*cnt1;i++)
    {
         
        cout<<DRAM[i/1024][i%1024]<<" ";
        if(i%4==3){
            cout<<"\n";
        }

    }*/

    x = pow(2,20);
    int i=0;
    // int numberOfClockCycles=0; 
    int j=0;
    while(i<x ){     // In this loop we will execute the command of "Instruction Set" which is aleready stored in DRAM
        
        
        if(i==-1 || DRAM[i/1024][i%1024]==-1){ // Exiting condition for the program
            break;
        }
    
        int memory_address;                // this is only use for "lw" and "sw" operation so that memory validation can be justify
        int c = DRAM[i/1024][i%1024];
        if(c==8 || c==9){
            memory_address=DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]];  
        }
        vector<int>v_1;
        switch (c) // Do the computation corresponding to the operator
        {
        case 1:            //add

           
            v_1.push_back(1);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[0]++;
            
            registers[DRAM[(i+1)/1024][(i+1)%1024]] = registers[DRAM[(i+2)/1024][(i+2)%1024]] + registers[DRAM[(i+3)/1024][(i+3)%1024]];
            //printRegisters(numberOfClockCycles);
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,1,numberOfClockCycles,"add",Register_map[DRAM[(i+1)/1024][(i+1)%1024]]+" = "+ to_string(registers[DRAM[(i+1)/1024][(i+1)%1024]] ),"Nothing ","Nothing");
            i=i+4;
            break;
            
        case 2:           //sub
          
            v_1.push_back(2);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[1]++;
            
            registers[DRAM[(i+1)/1024][(i+1)%1024]] = registers[DRAM[(i+2)/1024][(i+2)%1024]] - registers[DRAM[(i+3)/1024][(i+3)%1024]];
     
            //printRegisters(numberOfClockCycles);
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,2,numberOfClockCycles,"sub",Register_map[DRAM[(i+1)/1024][(i+1)%1024]]+" = "+ to_string(registers[DRAM[(i+1)/1024][(i+1)%1024]] ),"Nothing ","Nothing");
            i=i+4;
            break;
           

        case 3:      //mul
        
            v_1.push_back(3);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++; 
            freqOfCommand[2]++;
            registers[DRAM[(i+1)/1024][(i+1)%1024]] = registers[DRAM[(i+2)/1024][(i+2)%1024]] * registers[DRAM[(i+3)/1024][(i+3)%1024]];
  
            //printRegisters(numberOfClockCycles);
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,3,numberOfClockCycles,"mul",Register_map[DRAM[(i+1)/1024][(i+1)%1024]]+" = "+ to_string(registers[DRAM[(i+1)/1024][(i+1)%1024]] ),"Nothing ","Nothing");
            i=i+4;
            break;
            

        case 4:   //beq
            
            v_1.push_back(4);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[3]++;
            
            if(registers[DRAM[(i+1)/1024][(i+1)%1024]] == registers[DRAM[(i+2)/1024][(i+2)%1024]]){
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,4,numberOfClockCycles,"beq","Nothing ","Nothing ","Nothing");
                i = DRAM[(i+3)/1024][(i+3)%1024];
                if(i%4!=0 || i<0 || i>=x){
                    cout<<"Invalid Memory Excess\n";
                    return 1;
                }
                // printRegisters(numberOfClockCycles);
                break;
            }
            else{
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,4,numberOfClockCycles,"beq","Nothing ","Nothing ","Nothing");
                i = i+4;
                // printRegisters(numberOfClockCycles);
                break;
            }
            

        case 5:   //bne

            //  if(row!=-1){
            //      if(!queueArray[row].empty()){
            //          if(counter==COL_ACCESS_DELAY){
                        
            //          }
            //      }else{
            //          for(int i=0;i<1024;i++){
            //              if(!queueArray[i].empty()){

                             
            //                  break;
            //              }
            //          }
            //      }
            //  }
            
            v_1.push_back(5);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[4]++;
            
            if(registers[DRAM[(i+1)/1024][(i+1)%1024]] != registers[DRAM[(i+2)/1024][(i+2)%1024]]){
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,5,numberOfClockCycles,"bne","Nothing ","Nothing ","Nothing");
                i = DRAM[(i+3)/1024][(i+3)%1024];
               if(i%4!=0 || i<0 || i>=x){
                    cout<<"Invalid Memory Excess\n";
                    return 1;
                }
                // printRegisters(numberOfClockCycles);
                break;
            }
            else{
                printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,5,numberOfClockCycles,"bne","Nothing ","Nothing ","Nothing");
                i = i+4;
                if(i%4!=0 || i<0 || i>=x){
                    cout<<"Invalid Memory Excess\n";
                    return 1;
                }
                // printRegisters(numberOfClockCycles);
                break;
             }
            
        case 6:  //slt
            
            v_1.push_back(6);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[5]++;
            
            if(registers[DRAM[(i+2)/1024][(i+2)%1024]] < registers[DRAM[(i+3)/1024][(i+3)%1024]]){
                registers[DRAM[(i+1)/1024][(i+1)%1024]] = 1;
            }
            else{
                registers[DRAM[(i+1)/1024][(i+1)%1024]] = 0;
            }
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,6,numberOfClockCycles,"slt",Register_map[DRAM[(i+1)/1024][(i+1)%1024]]+" = "+ to_string(registers[DRAM[(i+1)/1024][(i+1)%1024]] ),"Nothing ","Nothing");
            i=i+4;
            // printRegisters(numberOfClockCycles);
            break;
           

        case 7: // jump 
            numberOfClockCycles++;
            freqOfCommand[6]++;
            
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,7,numberOfClockCycles,"j","Nothing ","Nothing ","Nothing");

            i = DRAM[(i+1)/1024][(i+1)%1024];
                            if(i%4!=0 || i<0 || i>=x){
                    cout<<"Invalid Memory Excess\n";
                    return 1;
                }
            // printRegisters(numberOfClockCycles);
            break;

        case 8:  // lw
           
            numberOfClockCycles++;
            freqOfCommand[7]++;
        
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,8,numberOfClockCycles,"lw","Nothing","Nothing","DRAM request issued");

            if(check_memory_address(memory_address,cnt1,x)){
                    return 1;  //terminate the program
            }else{
                 
                
                 v_1.push_back(8);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
                 isSafe(v_1);
                 int row1 =(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
                 //Array_cmd[row1]=1;
                 queueArray[row1].push_back(v_1);
            }
            
                i = i+4;
                break;
            

        case 9:  // sw
           
            numberOfClockCycles++;
            freqOfCommand[8]++;
         
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,9,numberOfClockCycles,"sw","Nothing","Nothing","DRAM request issued");
           if(check_memory_address(memory_address,cnt1,x)){
                    return 1;  //terminate the program
            }else{
                
                 v_1.push_back(9);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
                 isSafe(v_1);
                 int row1 =(DRAM[(i+2)/1024][(i+2)%1024] + registers[DRAM[(i+3)/1024][(i+3)%1024]])/1024;
                 Array_cmd[row1]=1;
                 queueArray[row1].push_back(v_1);
            }
            
                i = i + 4;
                break;
            

        case 10:  //addi
            
            v_1.push_back(10);v_1.push_back(DRAM[(i+1)/1024][(i+1)%1024]);v_1.push_back(DRAM[(i+2)/1024][(i+2)%1024]);v_1.push_back(DRAM[(i+3)/1024][(i+3)%1024]);v_1.push_back(i);
            isSafe(v_1);
            numberOfClockCycles++;
            freqOfCommand[9]++;
          
            registers[DRAM[(i+1)/1024][(i+1)%1024]] = registers[DRAM[(i+2)/1024][(i+2)%1024]] + DRAM[(i+3)/1024][(i+3)%1024];
            
            // printRegisters(numberOfClockCycles);
            printChange_in_Each_Cycle(ROW_ACCESS_DELAY,COL_ACCESS_DELAY,0,10,numberOfClockCycles,"addi",Register_map[DRAM[(i+1)/1024][(i+1)%1024]]+" = "+ to_string(registers[DRAM[(i+1)/1024][(i+1)%1024]] ),"Nothing ","Nothing");
            i = i+4;
            break;
   
        default:
            i=-1;
        }
        v_1.clear();
    //   }
       
    //   j++;  
    }

    for(int i=0;i<1024;i++){
        if(!queueArray[i].empty()){
            if(row!=-1 && row==i){
                deque<vector<int>> q =queueArray[i];
                executeQueue(q,i);
               queueArray[i].clear();
               break;
            }
        }
    }

    for(int i=0;i<1024;i++){

        if(!queueArray[i].empty()){
          deque<vector<int>> q =queueArray[i];
          executeQueue(q,i);
          queueArray[i].clear();
        }
    }

     printStats(numberOfClockCycles,number_of_row_buffer_update);
    // printRegisters(numberOfClockCycles);
     
    return 0;
}