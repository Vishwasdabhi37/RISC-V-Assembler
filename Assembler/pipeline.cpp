#include <bits/stdc++.h>
using namespace std;

int GPR[32];
int DM[1024 * 1024];
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
    void print();
};
void Controller :: print(){
    cout << regRead << ' ' << regWrite << ' ' << ALUSrc << ' ' << memRead << ' ' << memWrite << ' ' << mem2reg << ' ' << branch 
        << ' ' << Jump << ' ' << ALUOp << ' ' << ALUSelect << endl;
}
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
        regWrite = 0;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 1;
        ALUOp = "11";
    }else if(opcode == "1100111"){
        regRead = 1;
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

class IFID{
    public:
    int DPC;
    int NPC;
    int IR;
    
    IFID(){
        DPC = NPC = IR = 0;
    }
    IFID(int dpc,int npc,int ir) : DPC(dpc), NPC(npc), IR(ir) {}
};

class IDEX{
    int DPC;
    int JPC;
    int imm;
    Controller cw;
    int RS1;
    int RS2;
    string func3;
    string func7;
    int Rdl;
    
    IDEX(){
        DPC = JPC = imm = RS1 = RS2 = Rdl = 0;
        func3 = "000";
        func7 = "0000000";
        cw = Controller();
    }
    IDEX(int dpc,int jpc,int immm,Controller cww,int rs1,int rs2, string func3s, string func7s, int rdl) : DPC(dpc), JPC(jpc), imm(immm), 
    RS1(rs1), RS2(rs2), func3(func3s), func7(func7s), Rdl(rdl){
        cw = cww;
    }
};

class EXMO{
    int DPC;
    int BPC;
    int JPC;
    Controller cw;
    int ALUOut;
    int RS2;
    int Rdl;
    
    EXMO(){
        DPC = BPC = JPC = RS2 = ALUOut = Rdl = 0;
        cw = Controller();
    }
    EXMO(int dpc,int bpc,int jpc,Controller cww,int aluout, int rs2, int rdl) : DPC(dpc), BPC(bpc), JPC(jpc), 
    ALUOut(aluout), RS2(rs2), Rdl(rdl){
        cw = cww;
    }
};

class MOWB{
    int DPC;
    int BPC;
    int JPC;
    Controller cw;
    int LDOut;
    int ALUOut;
    int Rdl;
    
    MOWB(){
        DPC = BPC = JPC = LDOut = ALUOut = Rdl = 0;
        cw = Controller();
    }
    MOWB(int dpc,int bpc,int jpc,Controller cww,int ldout,int aluout,int rdl) : DPC(dpc), BPC(bpc), JPC(jpc),
    LDOut(ldout), ALUOut(aluout), Rdl(rdl){
        cw = cww;
    }
};



//change the alu operation from string to pointer to the controller
string ALUControl(string ALUOp, string func7, string func3){
    if(ALUOp == "00"){
        return "0010";
    }
    if(ALUOp == "01"){
        if(func3 == "000")
            return "0110";
        if(func3 == "100")
            return "0011";
        if(func3 == "101")
            return "0111";
    }
    if(ALUOp == "10"){
        if(func7[1] == '1'){
            if(func3 == "000"){
                return "0110";
            }
        }
        else if(func7[6] == '1'){
            if(func3 == "000"){
                return "0100";
            }
            if(func3 == "110"){
                return "0101";
            }
        }
        else{
            if(func3 == "001"){
                return "1000";
            }
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

int signedExtend(string s){
    if(s.empty()) return 0;
    int n = s.size();
    int val = 0;
    for(int i = 0;i < n;i++){
        val = (val << 1) | (s[i] - '0');
    }
    if(s[0] == '1' && n < 32){
        val |= (~0u << n);
    }
    return val;
}

int getDecimalU(string s){
    int num = 0;
    reverse(s.begin(),s.end());
    for(int i = 0;i < s.size();i++){
        if(s[i] == '1'){
            num += (1 << i);
        }
    }
    return num;
}
int getimm(string ins){
    string opcode = ins.substr(25);
    if(opcode == "0010011" || opcode ==  "1100111" || opcode == "0000011"){
        return signedExtend(ins.substr(0,12));
    }
    if(opcode == "1101111"){
        string str = "";
        str += ins[0];
        str += ins.substr(12,8);
        str += ins[11];
        str += ins.substr(1,10);
        return signedExtend(str);
    }
    if(opcode == "1100011"){
        string str = "";
        str += ins[0];
        str += ins[20];
        str += ins.substr(1,6);
        str += ins.substr(17,4);
        return signedExtend(str);
    }
    if(opcode == "0100011"){
        return signedExtend(ins.substr(0,7) + ins.substr(20,5));
    }
    
    return 0;
}
int get_GPR(string s){
    int num = 0;
    reverse(s.begin(),s.end());
    for(int i = 0;i  < s.size();i++){
        if(s[i] == '1'){
            num += (1 << i);
        }
    }
    return GPR[num];
}
void setGPR(string s,int val){
    int num = 0;
    reverse(s.begin(),s.end());
    for(int i = 0;i < s.size();i++){
        if(s[i] == '1'){
            num += (1 << i);
        }
    }
    cout << "reg :" << num << " val : " << val << endl;
    GPR[num] = val;
}

int ALU(string ALUSelect,int alusrc1,int alusrc2){
    if(ALUSelect == "0000"){
        return (alusrc1 & alusrc2);
    }
    if(ALUSelect == "0001"){
        return (alusrc1 | alusrc2);
    }
    if(ALUSelect == "0010"){
        return (alusrc1 + alusrc2);
    }
    if(ALUSelect == "0011"){
        return (alusrc1 < alusrc2) ? 0 : 1;
    }
    if(ALUSelect == "0100"){
        return alusrc1 * alusrc2;
    }
    if(ALUSelect == "0101"){
        return alusrc1 % alusrc2;
    }
    if(ALUSelect == "0110"){
        return alusrc1 - alusrc2;
    }
    if(ALUSelect == "0111"){
        return (alusrc1 >= alusrc2) ? 0 : 1;
    }
    if(ALUSelect == "1000"){
        if(alusrc2 > 30)return 0;
        return (alusrc1 << alusrc2);
    }
    return 0;
}

int outputselect(int mem2reg, int jump,int ALUResult,int LDResult, int NPC){
    if(mem2reg)return LDResult;
    if(jump)return NPC;
    return ALUResult;
}
int main(){
    cout << "DONE" << endl;   
    return 0;
}
