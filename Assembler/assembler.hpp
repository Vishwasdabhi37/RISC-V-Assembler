#include <bits/stdc++.h>
using namespace std;

/*

Registers
Instructions
R_type
I_type_arithmetic
I_type_shift
B_type
S_type
J_type
JR_type

*/

class Register
{
    map<string, string> reg_to_binary;
    string reg_name, reg_binary;

public:
    Register();
    Register(string reg_name); // done
    string get_reg_name();     // done
    string get_reg_binary();   // done
};
class Instructions
{
protected:
    string inst;
    string inst_type;
    string opname;

public:
    Instructions();
    Instructions(string inst);
    string get_inst_type();
    virtual string get_binary() = 0;
    virtual void initialize() = 0;
};
class R_type : public Instructions
{
    string opcode;
    Register *dest, *src1, *src2;
    string func3_bin, func7_bin;

public:
    void func3(); 
    void func7(); 
    R_type();
    R_type(string inst);         
    string get_binary() override;
    void initialize() override;  
};
class I_type_arithmetic : public Instructions
{
    string opcode;
    Register *dest, *src;
    string func3_bin, immediate12_bin;
    void func3();       
    void immediate12(); 
public:
    I_type_arithmetic();            
    I_type_arithmetic(string inst); 
    string get_binary() override;   
    void initialize() override;     
};
class I_type_shift : public Instructions
{
    string opcode;
    Register *dest, *src;
    string func3_bin, immediate5_bin, immediate7_bin;
    void func3();      
    void immediate5(); 
    void immediate7(); 
public:
    I_type_shift();              
    I_type_shift(string inst);   
    string get_binary() override;
    void initialize() override;  
};
class B_type : public Instructions
{
    string opcode;
    Register *src1, *src2;
    string func3_bin, immediate5_bin, immediate7_bin;
    void func3();
    void immediates();

public:
    B_type();
    B_type(string inst);
    string get_binary() override;
    void initialize() override;
};
class S_type : public Instructions
{
    string opcode;
    Register *src, *data;
    string func3_bin, immediate5_bin, immediate7_bin;
    void func3();
    void immediates();

public:
    S_type();
    S_type(string inst);
    string get_binary() override;
    void initialize() override;
};
class J_type : public Instructions
{
    string opcode;
    Register *dest;
    string immediate20_bin;
    void immediate20();

public:
    J_type();
    J_type(string inst);
    string get_binary() override;
    void initialize() override;
};
class JR_type : public Instructions
{
    string opcode;
    Register *base, *dest;
    string func3_bin, immediate12_bin;
    void func3();
    void immediate12();

public:
    JR_type();
    JR_type(string inst);
    string get_binary() override;
    void initialize() override;
};
class L_type : public Instructions
{
    string opcode;
    Register *src, *dest;
    string func3_bin, immediate12_bin;
    void func3();
    void immediate12();

public:
    L_type();
    L_type(string inst);
    string get_binary() override;
    void initialize() override;
};