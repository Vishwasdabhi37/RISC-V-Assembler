#include <bits/stdc++.h>
using namespace std;

int GPR[32];
int lock_count[32];
bool lock_flag[32];
int DM[1024 * 1024];
class Controller
{
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
    bool ins;
    Controller();
    Controller(string opcode);
    void print();
};
void Controller ::print()
{
    cout << regRead << ' ' << regWrite << ' ' << ALUSrc << ' ' << memRead << ' ' << memWrite << ' ' << mem2reg << ' ' << branch
         << ' ' << Jump << ' ' << ALUOp << ' ' << ALUSelect << endl;
}
Controller ::Controller() {}
Controller ::Controller(string opcode)
{
    ALUSelect = "0000";
    if (opcode == "0110011")
    {
        regRead = 1;
        regWrite = 1;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "10";
        ins = 0;
    }
    else if (opcode == "0010011")
    {
        regRead = 1;
        regWrite = 1;
        ALUSrc = 1;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "10";
        ins = 0;
    }
    else if (opcode == "0000011")
    {
        regRead = 1;
        regWrite = 1;
        ALUSrc = 1;
        memRead = 1;
        memWrite = 0;
        mem2reg = 1;
        branch = 0;
        Jump = 0;
        ALUOp = "00";
        ins = 0;
    }
    else if (opcode == "0100011")
    {
        regRead = 1;
        regWrite = 0;
        ALUSrc = 1;
        memRead = 0;
        memWrite = 1;
        mem2reg = 0;
        branch = 0;
        Jump = 0;
        ALUOp = "00";
        ins = 1;
    }
    else if (opcode == "1100011")
    {
        regRead = 1;
        regWrite = 0;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 1;
        Jump = 0;
        ALUOp = "01";
        ins = 1;
    }
    else if (opcode == "1101111")
    {
        regRead = 0;
        regWrite = 0;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 1;
        ALUOp = "11";
        ins = 1;
    }
    else if (opcode == "1100111")
    {
        regRead = 1;
        regWrite = 1;
        ALUSrc = 0;
        memRead = 0;
        memWrite = 0;
        mem2reg = 0;
        branch = 0;
        Jump = 1;
        ALUOp = "11";
        ins = 1;
    }
}

class IFID
{
public:
    int DPC;
    int NPC;
    string IR;
    bool valid, stall;
    IFID()
    {
        DPC = NPC = 0;
        IR = "";
        valid = false, stall = false;
    }
    IFID(int dpc, int npc, string ir, bool valid_flag = false, bool stall_flag = false) : DPC(dpc), NPC(npc), IR(ir), valid(valid_flag), stall(stall_flag) {}
};

class IDEX
{
public:
    int DPC;
    int JPC;
    int imm;
    Controller cw;
    int RS1;
    int RS2;
    string func3;
    string func7;
    string Rdl;
    bool stall, valid;
    IDEX()
    {
        DPC = JPC = imm = 0;
        func3 = "000";
        func7 = "0000000";
        RS1 = RS2 = 0;
        Rdl = "";
        cw = Controller();
        stall = false, valid = false;
    }
    IDEX(int dpc, int jpc, int immm, Controller cww, int rs1, int rs2, string func3s, string func7s, string rdl, bool valid_flag = false, bool stall_flag = false) : DPC(dpc), JPC(jpc), imm(immm),
                                                                                                                                                                     RS1(rs1), RS2(rs2), func3(func3s), func7(func7s), Rdl(rdl), valid(valid_flag), stall(stall_flag)
    {
        cw = cww;
    }
};

class EXMO
{
public:
    int DPC;
    int BPC;
    int JPC;
    Controller cw;
    int ALUOut;
    int RS2;
    string Rdl;
    bool stall, valid;
    EXMO()
    {
        DPC = BPC = JPC = ALUOut = RS2 = 0;
        Rdl = "";
        cw = Controller();
        stall = false, valid = false;
    }
    EXMO(int dpc, int bpc, int jpc, Controller cww, int aluout, int rs2, string rdl, bool valid_flag = false, bool stall_flag = false) : DPC(dpc), BPC(bpc), JPC(jpc),
                                                                                                                                         ALUOut(aluout), RS2(rs2), Rdl(rdl), valid(valid_flag), stall(stall_flag)
    {
        cw = cww;
    }
};

class MOWB
{
public:
    int DPC;
    int BPC;
    int JPC;
    Controller cw;
    int LDOut;
    int ALUOut;
    string Rdl;
    bool stall, valid;
    MOWB()
    {
        DPC = BPC = JPC = LDOut = ALUOut = 0;
        Rdl = "";
        cw = Controller();
        stall = false, valid = false;
    }
    MOWB(int dpc, int bpc, int jpc, Controller cww, int ldout, int aluout, string rdl, bool valid_flag = false, bool stall_flag = false) : DPC(dpc), BPC(bpc), JPC(jpc),
                                                                                                                                           LDOut(ldout), ALUOut(aluout), Rdl(rdl), valid(valid_flag), stall(stall_flag)
    {
        cw = cww;
    }
};

// change the alu operation from string to pointer to the controller
string ALUControl(string ALUOp, string func7, string func3)
{
    if (ALUOp == "00")
    {
        return "0010";
    }
    if (ALUOp == "01")
    {
        if (func3 == "000")
            return "0110";
        if (func3 == "100")
            return "0011";
        if (func3 == "101")
            return "0111";
    }
    if (ALUOp == "10")
    {
        if (func7[1] == '1')
        {
            if (func3 == "000")
                return "0110";
        }
        else if (func7[6] == '1')
        {
            if (func3 == "000")
                return "0100";
            if (func3 == "110")
                return "0101";
        }
        else
        {
            if (func3 == "001")
                return "1000";
            if (func3 == "000")
                return "0010";
            if (func3 == "111")
                return "0000";
            if (func3 == "110")
                return "0001";
        }
    }
    return "0000";
}

int signedExtend(string s)
{
    if (s.empty())
        return 0;
    int n = s.size();
    int val = 0;
    for (int i = 0; i < n; i++)
        val = (val << 1) | (s[i] - '0');
    if (s[0] == '1' && n < 32)
        val |= (~0u << n);
    return val;
}

int getDecimalU(string s)
{
    int num = 0;
    reverse(s.begin(), s.end());
    for (int i = 0; i < s.size(); i++)
        if (s[i] == '1')
            num += (1 << i);
    return num;
}
int getimm(string ins)
{
    string opcode = ins.substr(25);
    if (opcode == "0010011" || opcode == "1100111" || opcode == "0000011")
        return signedExtend(ins.substr(0, 12));
    if (opcode == "1101111")
    {
        string str = "";
        str += ins[0];
        str += ins.substr(12, 8);
        str += ins[11];
        str += ins.substr(1, 10);
        return signedExtend(str);
    }
    if (opcode == "1100011")
    {
        string str = "";
        str += ins[0];
        str += ins[20];
        str += ins.substr(1, 6);
        str += ins.substr(17, 4);
        return signedExtend(str);
    }
    if (opcode == "0100011")
        return signedExtend(ins.substr(0, 7) + ins.substr(20, 5));

    return 0;
}
int get_GPR(string s)
{
    int num = 0;
    reverse(s.begin(), s.end());
    for (int i = 0; i < s.size(); i++)
        if (s[i] == '1')
            num += (1 << i);
    return GPR[num];
}
void setGPR(string s, int val)
{
    int num = 0;
    reverse(s.begin(), s.end());
    for (int i = 0; i < s.size(); i++)
        if (s[i] == '1')
            num += (1 << i);
    cout << "reg :" << num << " val : " << val << endl;
    GPR[num] = val;
}

int ALU(string ALUSelect, int alusrc1, int alusrc2)
{
    if (ALUSelect == "0000")
        return (alusrc1 & alusrc2);
    if (ALUSelect == "0001")
        return (alusrc1 | alusrc2);
    if (ALUSelect == "0010")
        return (alusrc1 + alusrc2);
    if (ALUSelect == "0011")
        return (alusrc1 < alusrc2) ? 0 : 1;
    if (ALUSelect == "0100")
        return alusrc1 * alusrc2;
    if (ALUSelect == "0101")
        return alusrc1 % alusrc2;
    if (ALUSelect == "0110")
        return alusrc1 - alusrc2;
    if (ALUSelect == "0111")
        return (alusrc1 >= alusrc2) ? 0 : 1;
    if (ALUSelect == "1000")
    {
        if (alusrc2 > 30)
            return 0;
        return (alusrc1 << alusrc2);
    }
    return 0;
}

int outputselect(int mem2reg, int jump, int ALUResult, int LDResult, int NPC)
{
    if (mem2reg)
        return LDResult;
    if (jump)
        return NPC;
    return ALUResult;
}
IFID ifid = IFID();
IDEX idex = IDEX();
EXMO exmo = EXMO();
MOWB mowb = MOWB();
int PC = 0;
bool PC_valid = true;
vector<string> ins_v;
int NPC;
void instruction_fetch()
{
    if (ifid.stall || !PC_valid)
        return;
    if (PC >= ins_v.size())
    {
        ifid.valid = false;
        return;
    }
    ifid.IR = ins_v[PC];
    NPC = PC + 1;
    ifid.NPC = NPC;
    ifid.valid = true;
    PC_valid = false;
}
void instruction_decode()
{
    if (idex.stall || !ifid.valid)
        return;
    idex.JPC = ifid.NPC + signedExtend(ifid.IR.substr(0, 20));
    string rsl1 = ifid.IR.substr(12, 5);
    string rsl2 = ifid.IR.substr(7, 5);
    idex.Rdl = ifid.IR.substr(20, 5);
    string opcode = ifid.IR.substr(25);
    idex.func3 = ifid.IR.substr(17, 3);
    idex.func7 = ifid.IR.substr(0, 7);
    idex.imm = getimm(ifid.IR);
    idex.cw = Controller(opcode);
    idex.DPC = PC;
    if (idex.cw.regRead)
    {
        if (lock_flag[getDecimalU(rsl1)] == true)
            idex.RS1 = getDecimalU(rsl1);
        else
        {
            ifid.stall = true;
            return;
        }
    }
    if (idex.cw.regRead)
    {
        if (lock_flag[getDecimalU(rsl2)] == true)
            idex.RS2 = getDecimalU(rsl2);
        else
        {
            ifid.stall = true;
            return;
        }
    }
    if (idex.cw.ins == 0)
    {
        lock_count[getDecimalU(idex.Rdl)]++;
        lock_flag[getDecimalU(idex.Rdl)] = 0;
    }
    ifid.stall = false;
    idex.valid = true;
    ifid.valid = false;
}
void instruction_execution()
{
    if (exmo.stall || !idex.valid)
        return;
    int alusrc2 = 0;
    if (idex.cw.ALUSrc)
        alusrc2 = idex.imm;
    else
        alusrc2 = GPR[idex.RS2];
    int alusrc1 = GPR[idex.RS1];
    // cout << alusrc1 << ' ' << alusrc2 << endl;
    idex.cw.ALUSelect = ALUControl(idex.cw.ALUOp, idex.func7, idex.func3);
    // cout << CW->ALUSelect << endl;
    exmo.ALUOut = ALU(idex.cw.ALUSelect, alusrc1, alusrc2);
    int ALUZeroFlag = (exmo.ALUOut == 0);

    exmo.BPC = idex.imm + idex.DPC;
    exmo.cw = idex.cw;
    exmo.cw.branch = idex.cw.branch & ALUZeroFlag;
    exmo.Rdl = idex.Rdl;
    exmo.JPC = idex.JPC;
    exmo.DPC = idex.DPC;
    exmo.RS2 = idex.RS2;
    idex.stall = false;
    exmo.valid = true;
    idex.valid = false;
}

void memory_operation()
{
    if (mowb.stall || !exmo.valid)
        return;
    if (exmo.cw.memWrite)
    {
        cout << "memwrite " << exmo.ALUOut << ' ' << exmo.RS2 << endl;
        DM[exmo.ALUOut] = GPR[exmo.RS2];
    }
    if (exmo.cw.memRead)
    {
        cout << "memRead " << exmo.ALUOut << ' ' << DM[exmo.ALUOut] << endl;
        mowb.LDOut = DM[exmo.ALUOut];
    }
    mowb.cw = exmo.cw;
    mowb.ALUOut = exmo.ALUOut;
    mowb.Rdl = exmo.Rdl;
    mowb.DPC = exmo.DPC;
    mowb.BPC = exmo.BPC;
    mowb.JPC = exmo.JPC;
    exmo.stall = false;
    mowb.valid = true;
    exmo.valid = false;
}

void Writeback()
{
    if (!mowb.valid)
        return;
    if (mowb.cw.regWrite)
    {
        // GPR[getDecimalU(mowb.Rdl)] = outputselect(mowb.cw.mem2reg,mowb.cw.Jump,mowb.ALUOut,mowb.LDOut,mowb.DPC);
        int tmp = outputselect(mowb.cw.mem2reg, mowb.cw.Jump, mowb.ALUOut, mowb.LDOut, mowb.DPC);
        setGPR(mowb.Rdl, tmp);
        lock_count[getDecimalU(mowb.Rdl)]--;
        if (lock_count[getDecimalU(mowb.Rdl)] == 0)
            lock_flag[getDecimalU(mowb.Rdl)] = 1;
    }
    int TPC = 0;
    if (mowb.cw.branch)
        TPC = mowb.BPC;
    else if (mowb.cw.Jump)
        TPC = mowb.JPC;
    else
        TPC = NPC;
    PC = TPC;
    mowb.valid = false;
    PC_valid = true;
}
int main()
{
    // freopen("machine_code_prime_number.txt","r",stdin);
    freopen("in_tmp.txt", "r", stdin);
    for (int i = 0; i < 32; i++)
    {
        GPR[i] = i;
        lock_count[i] = 0;
        lock_flag[i] = 1;
    }
    // int base_add = 4096;
    // GPR[8] = base_add;
    // int N = 15;
    // DM[base_add] = N;
    // for(int i = 1;i <= N;i++){
    //     DM[base_add + i] = i;
    // }
    GPR[8] = 6;
    string str;
    ins_v.clear();
    while (getline(cin, str))
        ins_v.push_back(str);
    int sz = ins_v.size();
    int cnt = 0;
    while (PC < sz || ifid.valid || idex.valid || exmo.valid || mowb.valid)
    {
        cnt++;
        // cout << "PC" << PC << endl;
        // for (int i = 1; i < 6; i++)
        // {
        //     cout << GPR[i] << ' ';
        // }
        // cout << endl;
        Writeback();
        memory_operation();
        instruction_execution();
        instruction_decode();
        instruction_fetch();
    }
    cout << cnt << endl;
    cout << "COMPLETED" << endl;
    return 0;
}
