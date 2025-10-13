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
    string ans = s;
    reverse(ans.begin(),ans.end());
    char ch = ans.back();
    while((int)ans.size()!=32){
        ans += ch;
    }
    reverse(ans.begin(),ans.end());
    int ans_num = 0;
    int sign = (ans[0] == '1') ? -1 : 1;
    for(int i = 1;i < 32;i++){
        ans_num = ans_num * 2 + (ans[i] - '0');
    }
    return ans_num * sign;
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
    string opcode = ins.substr(26);
    if(opcode == "" || opcode ==  "" || opcode == ""){
        return signedExtend(ins.substr(0,12));
    }
    if(opcode == ""){
        string str = "";
        str += ins[0];
        str += ins.substr(12,8);
        str += ins[11];
        str += ins.substr(1,10);
        return signedExtend(str);
    }
    if(opcode == "" || opcode == ""){
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
    freopen("Output_file.txt","r",stdin);
    for(int i = 0;i < 32;i++){
        GPR[i] = i;
    }
    for(auto && it : GPR){
            cout << it << ' ';
        }
        cout << endl;
    vector<string> ins_v;
    string str;
    while(getline(cin,str)){
        ins_v.push_back(str);
    }
    int sz = ins_v.size();
    int PC = 0;
    int cnt = 0;
    while(PC < sz){
        string ins = ins_v[PC];
        int NPC = PC + 1;
        int JPC = NPC + (signedExtend(ins.substr(0,20)) / 4);
        string rsl1 = ins.substr(12,5);
        string rsl2 = ins.substr(7,5);
        string rdl = ins.substr(20,5);
        string opcode = ins.substr(26);
        string func3 = ins.substr(17,3);
        string func7 = ins.substr(0,7);
        int imm = getimm(ins);
        Controller *CW = new Controller(opcode);
        int rs1 = 0;
        if(CW->regRead) rs1 = get_GPR(rsl1);
        int rs2 = 0;
        if(CW->regRead) rs2 = get_GPR(rsl2);
        int alusrc2 = 0;
        if(CW->ALUSrc) alusrc2 = imm;
        else alusrc2 = rs2;
        int alusrc1 = rs1;
        CW->ALUSelect = ALUControl(CW->ALUOp,func7,func3);
        int ALUResult = ALU(CW->ALUSelect, alusrc1,alusrc2);
        int ALUZeroFlag = (ALUResult == 0);
        int BPC = ((imm / 4)<< 1) + PC;
        int TPC = 0;
        if(CW->branch && ALUZeroFlag) TPC = BPC;
        else TPC = NPC;
        if(CW->Jump) TPC = JPC;
        int LDResult  = 0;
        if(CW->memRead) LDResult = DM[ALUResult];
        if(CW->memWrite) DM[ALUResult] = rs2;
        if(CW->regWrite) setGPR(rdl, outputselect(CW->mem2reg,CW->Jump, ALUResult, LDResult, NPC));
        PC = TPC;
        cout << "INS : " << cnt++ << ' ';
        for(auto && it : GPR){
            cout << it << ' ';
        }
        cout << endl;
    }
    return 0;
}
