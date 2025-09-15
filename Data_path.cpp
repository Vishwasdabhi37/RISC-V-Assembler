#include <bits/stdc++.h>
using namespace std;

class Controller{
    public:
    bool regRead;
    bool regWrite;
    bool ALUSrc;
    bool memRead;
    bool memWrite;
    bool mem2reg;
    bool branch;
    bool Jump;
    string ALUOp;
    string ALUSelect;
    Controller();
    Controller(string opcode);
};
Controller :: Controller(){}
Controller :: Controller(string opcode){
    ALUSelect = "0000";
    if(opcode == "0110011"){
        regRead = 1;
        regWrite = 1;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "10";
    }else if(opcode == "0010011"){
        regRead = 1;
        regWrite = 1;
        ALUSrc = 1;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "10";
    }else if(opcode == "0000011"){
        regRead = 1;
        regWrite = 1;
        ALUSrc = 1;
        memRead = 1;
        memWrite = 0;
        mem2reg = 1;
        branch = 0;
        Jump = 0;
        ALUOp = "00";
    }else if(opcode == "0100011"){
        regRead = 1;
        regWrite = 0;
        ALUSrc = 1;
        memRead = 0;
        memWrite = 1;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "00";
    }else if(opcode == "1100011"){
        regRead = 1;
        regWrite = 0;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 1;
        Jump = 0;
        ALUOp = "01";
    }else if(opcode == "1101111"){
        regRead = 0;
        regWrite = 1;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 1;
        ALUOp = "11";
    }
}

//change the alu operation from string to pointer to the controller
string ALUControl(string ALUOp, string func7, string func3){
    if(ALUOp == "00"){
        return "0010";
    }
    if(ALUOp == "01"){
        return "0110";
    }
    if(ALUOp == "10"){
        if(func7[6] == '1'){
            if(func3 == "000"){
                return "0110";
            }
        }
        else{
            if(func3 == "000"){
                return "0010";
            }
            if(func3 == "111"){
                return "0000";
            }
            if(func3 == "110"){
                return "0001";
            }
        }
    }
    return "0000";
}

string signedExtend(string s){
    string ans = s;
    reverse(ans.begin(),ans.end());
    char ch = ans.back();
    while((int)ans.size()!=32){
        ans += ch;
    }
    reverse(ans.begin(),ans.end());
    return ans;
}


int main(){
    freopen("output.txt","r",stdin);
                                                                                                                                                                                                            
    return 0;
}
