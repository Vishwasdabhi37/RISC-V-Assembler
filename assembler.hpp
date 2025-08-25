#include <bits/stdc++.h>
using namespace std;

/*

Registers ---------done
Instructions
R_type
I_type_arithmetic
I_type_shift
B_type
S_type
J_type
JR_type
*/



Class Registers{
    map<string,string> reg_to_binary;
    string reg_name,reg_binary;
    public:
    Registers(string reg_name); // done
    string get_reg_name(); // done
    string get_reg_binary(); // done
};
Class Instructions{
    string inst;
    map<string,string> opname_to_type;
    public:
    Instructions();
    virtual string get_binary() = 0; // this is the final ans (instruction into binary)
    virtual string initialize() = 0; // this will process the string inst, and assign the corrosponding values to attributes
};
Class R_type : public Instructions{
    string opcode;
    Register dest,src1,src2;
    string func3_bin,func7_bin;
    string func3(string ins);
    string func7(string ins);
    public:
    R_type(string inst); // I will set opcode here
    string get_binary() override;
    string initialize() override;
};
Class I_type_arithmetic : public Instructions{
    string opcode;
    Register dest,src;
    string func3_bin, immediate20_bin;
    string func3(string ins);
    string immediate20(string ins);
    public:
    I_type(string inst);
    string get_binary() override;
    string initialize() override;
};
Class I_type_shift : public Instructions{
    string opcode;
    Register dest,src;
    string func3_bin, immediate5_bin,immediate7_bin;
    string func3(string ins);
    string immediate5(string ins);
    string immediate7(string ins);
    public:
    I_type(string inst);
    string get_binary() override;
    string initialize() override;
};
class B_type : public Instructions{
    string opcode;
    Register src1,src2;
    string func3_bin,immediate5_bin,immediate7_bin; // in BNE we will take offset as an int instead of string
    string func3(string ins);
    string immediate5(string ins);
    string immediate7(string ins);
    public:
    string get_binary() override;
    string initialize() override;
};
class S_type : public Instructions{
    string opcode;
    Register src,data;
    string func3_bin,immediate5_bin,immediate7_bin;
    string func3(string ins);
    string immediate5_bin(string ins);
    string immediate7_bin(string ins);
    public:
    string get_binary() override;
    string initialize() override;
};
Class J_type : public Instructions{
    string opcode;
    Register dest;
    string immediate20_bin;
    string immediate20(string ins);
    public:
    string get_binary() override;
    string initialize() override;
};
Class JR_type : public Instructions{
    string opcode;
    Register base,dest;
    string func3_bin, immediate12_bin;
    string func3(string ins);
    string immediate12(string ins);
    public:
    string get_binary() override;
    string initialize() override;
};
