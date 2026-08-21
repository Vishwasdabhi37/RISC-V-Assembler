#include "assembler.hpp"

string get_inst_type_util(string ins)
{
    string op = "";
    int i = 0;
    while (ins[i] != ' ')
    {
        op += ins[i];
        i++;
    }
    map<string, string> opname_to_type = {
        {"add", "R_type"},
        {"slt", "R_type"},
        {"sltu", "R_type"},
        {"and", "R_type"},
        {"or", "R_type"},
        {"xor", "R_type"},
        {"sll", "R_type"},
        {"srl", "R_type"},
        {"sub", "R_type"},
        {"sra", "R_type"},
        {"mul", "R_type"},
        {"rem", "R_type"},
        {"addi", "I_type_arithmetic"},
        {"andi", "I_type_arithmetic"},
        {"ori", "I_type_arithmetic"},
        {"xori", "I_type_arithmetic"},
        {"slli", "I_type_shift"},
        {"srli", "I_type_shift"},
        {"srai", "I_type_shift"},
        {"lw", "L_type"},
        {"lb", "L_type"},
        {"lh", "L_type"},
        {"ld", "L_type"},
        {"lwu", "L_type"},
        {"lsu", "L_type"},
        {"lbu", "L_type"},
        {"beq", "B_type"},
        {"bne", "B_type"},
        {"blt", "B_type"},
        {"bge", "B_type"},
        {"bltu", "B_type"},
        {"bgeu", "B_type"},
        {"jal", "J_type"},
        {"j", "J_type"},
        {"jalr", "JR_type"},
        {"jr", "JR_type"},
        {"sd", "S_type"},
        {"sw", "S_type"},
        {"sh", "S_type"},
        {"sb", "S_type"},
    };
    return opname_to_type[op];
}
Register ::Register() {}
Register::Register(string reg_name)
{
    reg_to_binary = {
        {"x0", "00000"},
        {"x1", "00001"},
        {"x2", "00010"},
        {"x3", "00011"},
        {"x4", "00100"},
        {"x5", "00101"},
        {"x6", "00110"},
        {"x7", "00111"},
        {"x8", "01000"},
        {"x9", "01001"},
        {"x10", "01010"},
        {"x11", "01011"},
        {"x12", "01100"},
        {"x13", "01101"},
        {"x14", "01110"},
        {"x15", "01111"},
        {"x16", "10000"},
        {"x17", "10001"},
        {"x18", "10010"},
        {"x19", "10011"},
        {"x20", "10100"},
        {"x21", "10101"},
        {"x22", "10110"},
        {"x23", "10111"},
        {"x24", "11000"},
        {"x25", "11001"},
        {"x26", "11010"},
        {"x27", "11011"},
        {"x28", "11100"},
        {"x29", "11101"},
        {"x30", "11110"},
        {"x31", "11111"},
        {"zero", "00000"},
        {"ra", "00001"},
        {"sp", "00010"},
        {"gp", "00011"},
        {"tp", "00100"},
        {"t0", "00101"},
        {"t1", "00110"},
        {"t2", "00111"},
        {"s0", "01000"},
        {"fp", "01000"},
        {"s1", "01001"},
        {"a0", "01010"},
        {"a1", "01011"},
        {"a2", "01100"},
        {"a3", "01101"},
        {"a4", "01110"},
        {"a5", "01111"},
        {"a6", "10000"},
        {"a7", "10001"},
        {"s2", "10010"},
        {"s3", "10011"},
        {"s4", "10100"},
        {"s5", "10101"},
        {"s6", "10110"},
        {"s7", "10111"},
        {"s8", "11000"},
        {"s9", "11001"},
        {"s10", "11010"},
        {"s11", "11011"},
        {"t3", "11100"},
        {"t4", "11101"},
        {"t5", "11110"},
        {"t6", "11111"},
    };
    this->reg_name = reg_name;
    this->reg_binary = reg_to_binary[reg_name];
}
string Register ::get_reg_name()
{
    return this->reg_name;
}
string Register ::get_reg_binary()
{
    return this->reg_binary;
}
Instructions ::Instructions() {}

Instructions ::Instructions(string ins)
{
    this->inst = ins;
    string op = "";
    int i = 0;
    while (ins[i] != ' ')
    {
        op += ins[i];
        i++;
    }
    this->opname = op;
    this->inst_type = get_inst_type_util(ins);
}
string Instructions ::get_inst_type()
{
    return this->inst_type;
}
R_type ::R_type() {}
R_type ::R_type(string inst) : Instructions(inst)
{
    this->opcode = "0110011";
    this->func3();
    this->func7();
    this->initialize();
}
void R_type ::func3()
{
    map<string, string> opname_to_binary{
        {"add", "000"},
        {"sub", "000"},
        {"sll", "001"},
        {"xor", "100"},
        {"srl", "101"},
        {"sra", "101"},
        {"or", "110"},
        {"and", "111"},
        {"mul", "000"},
        {"rem", "110"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void R_type ::func7()
{
    map<string, string> opname_to_binary{
        {"add", "0000000"},
        {"sub", "0100000"},
        {"sll", "0000000"},
        {"xor", "0000000"},
        {"srl", "0000000"},
        {"sra", "0100000"},
        {"or", "0000000"},
        {"and", "0000000"},
        {"mul", "0000001"},
        {"rem", "0000001"},
    };
    this->func7_bin = opname_to_binary[this->opname];
}
void R_type ::initialize()
{
    string curr = this->inst;
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    i++;
    while (curr[i] == ' ')
        i++;
    string src2_reg = "";
    while (curr[i] != ',')
    {
        src2_reg += curr[i];
        i++;
    }
    i++;
    while (curr[i] == ' ')
        i++;
    string src1_reg = "";
    while (i < curr.size() && curr[i] != ' ')
    {
        src1_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
    this->src1 = new Register(src1_reg);
    this->src2 = new Register(src2_reg);
}
string R_type ::get_binary()
{
    string ans = this->func7_bin + " " + this->src2->get_reg_binary() + " " + this->src1->get_reg_binary() + " " + this->func3_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}
I_type_arithmetic ::I_type_arithmetic() {}
I_type_arithmetic ::I_type_arithmetic(string inst) : Instructions(inst)
{
    this->opcode = "0010011";
    this->func3();
    this->immediate12();
    this->initialize();
}
void I_type_arithmetic ::func3()
{
    map<string, string> opname_to_binary{
        {"addi", "000"},
        {"andi", "111"},
        {"ori", "110"},
        {"xori", "100"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void I_type_arithmetic ::immediate12()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());
    int num = stoi(number);
    if (num > (1 << 12))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 11; i >= 0; i--)
    {
        if ((num >> i) & 1)
            num_to_binary += '1';
        else
            num_to_binary += '0';
    }
    this->immediate12_bin = num_to_binary;
}

void I_type_arithmetic ::initialize()
{
    string curr = this->inst;
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    i++;
    while (curr[i] == ' ')
        i++;
    string src_reg = "";
    while (curr[i] != ',')
    {
        src_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
    this->src = new Register(src_reg);
}
string I_type_arithmetic ::get_binary()
{
    string ans = this->immediate12_bin + " " + this->src->get_reg_binary() + " " + this->func3_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}
I_type_shift ::I_type_shift() {}
I_type_shift ::I_type_shift(string inst) : Instructions(inst)
{
    this->opcode = "0010011";
    this->func3();
    this->immediate5();
    this->immediate7();
    this->initialize();
}
void I_type_shift ::func3()
{
    map<string, string> opname_to_binary{
        {"slli", "001"},
        {"srli", "101"},
        {"srai", "101"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void I_type_shift ::immediate5()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());
    int num = stoi(number);
    if (num > (1 << 5))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 4; i >= 0; i--)
    {
        if ((num >> i) & 1)
        {
            num_to_binary += '1';
        }
        else
            num_to_binary += '0';
    }
    this->immediate5_bin = num_to_binary;
}
void I_type_shift ::immediate7()
{
    map<string, string> opname_to_binary{
        {"slli", "0000000"},
        {"srli", "0000000"},
        {"srai", "0100000"},
    };
    this->immediate7_bin = opname_to_binary[this->opname];
}
void I_type_shift ::initialize()
{
    string curr = this->inst;
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    i++;
    while (curr[i] == ' ')
        i++;
    string src_reg = "";
    while (curr[i] != ',')
    {
        src_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
    this->src = new Register(src_reg);
}
string I_type_shift ::get_binary()
{
    string ans = this->immediate7_bin + " " + this->immediate5_bin + " " + this->src->get_reg_binary() + " " + this->func3_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}

B_type ::B_type() {}
B_type ::B_type(string ins) : Instructions(ins)
{
    this->opcode = "1100011";
    this->func3();
    this->immediates();
    this->initialize();
}
void B_type ::func3()
{
    map<string, string> opname_to_binary{
        {"beq", "000"},
        {"bne", "001"},
        {"blt", "100"},
        {"bge", "101"},
        {"bltu", "110"},
        {"bgeu", "111"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void B_type ::immediates()
{
    string curr = this->inst;
    int n = curr.size();
    string number = "";
    int i = n - 1;
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());
    int num = stoi(number);
    if (num > (1 << 12))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 11; i >= 0; i--)
    {
        if ((num >> i) & 1)
            num_to_binary += '1';
        else
            num_to_binary += '0';
    }
    string last5 = "";
    for (int i = 0; i < 5; i++)
    {
        last5 += num_to_binary.back();
        num_to_binary.pop_back();
    }
    reverse(last5.begin(), last5.end());
    this->immediate7_bin = num_to_binary;
    this->immediate5_bin = last5;
}
void B_type ::initialize()
{
    string curr = this->inst;
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string src_reg1 = "";
    while (curr[i] != ',')
    {
        src_reg1 += curr[i];
        i++;
    }
    i++;
    while (curr[i] == ' ')
        i++;
    string src_reg2 = "";
    while (curr[i] != ',')
    {
        src_reg2 += curr[i];
        i++;
    }
    this->src1 = new Register(src_reg1);
    this->src2 = new Register(src_reg2);
}
string B_type ::get_binary()
{
    string ans = this->immediate7_bin + " " + this->src2->get_reg_binary() + " " + this->src1->get_reg_binary() + " " + this->func3_bin + " " + this->immediate5_bin + " " + this->opcode;
    return ans;
}
S_type ::S_type() {}
S_type ::S_type(string ins) : Instructions(ins)
{
    this->opcode = "0100011";
    this->func3();
    this->immediates();
    this->initialize();
}
void S_type ::func3()
{
    map<string, string> opname_to_binary{
        {"sd", "011"},
        {"sw", "010"},
        {"sh", "001"},
        {"sb", "000"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void S_type ::immediates()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    while (i >= 0 && curr[i] != '(')
        i--;
    i--;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());

    int num = stoi(number);
    if (num > (1 << 12))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 11; i >= 0; i--)
    {
        if ((num >> i) & 1)
        {
            num_to_binary += '1';
        }
        else
            num_to_binary += '0';
    }
    string last5 = "";
    for (int i = 0; i < 5; i++)
    {
        last5 += num_to_binary.back();
        num_to_binary.pop_back();
    }
    reverse(last5.begin(), last5.end());
    this->immediate7_bin = num_to_binary;
    this->immediate5_bin = last5;
}
void S_type ::initialize()
{
    string curr = this->inst;
    int n = curr.size();
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string src_reg2 = "";
    while (curr[i] != ',')
    {
        src_reg2 += curr[i];
        i++;
    }
    while (curr[i] != '(')
        i++;
    i++;
    string src_reg1 = "";
    while (curr[i] != ')')
    {
        src_reg1 += curr[i];
        i++;
    }
    this->data = new Register(src_reg2);
    this->src = new Register(src_reg1);
}
string S_type ::get_binary()
{
    string ans = this->immediate7_bin + " " + this->data->get_reg_binary() + " " + this->src->get_reg_binary() + " " + this->func3_bin + " " + this->immediate5_bin + " " + this->opcode;
    return ans;
}

J_type ::J_type() {}
J_type ::J_type(string ins) : Instructions(ins)
{
    this->opcode = "1101111";
    this->immediate20();
    this->initialize();
}
void J_type ::immediate20()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());
    int num = stoi(number);
    if (num > (1 << 20))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 19; i >= 0; i--)
    {
        if ((num >> i) & 1)
            num_to_binary += '1';
        else
            num_to_binary += '0';
    }
    string ans = "";
    ans += num_to_binary[19];
    for (int i = 9; i >= 0; i--)
        ans += num_to_binary[i];
    ans += num_to_binary[10];
    for (int i = 18; i >= 11; i--)
        ans += num_to_binary[i];
    this->immediate20_bin = ans;
}

void J_type ::initialize()
{
    string curr = this->inst;
    int n = curr.size();
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
}
string J_type ::get_binary()
{
    string ans = this->immediate20_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}
JR_type ::JR_type() {}
JR_type ::JR_type(string inst) : Instructions(inst)
{
    this->opcode = "1100111";
    this->func3();
    this->immediate12();
    this->initialize();
}
void JR_type ::func3()
{
    this->func3_bin = "0";
}
void JR_type ::immediate12()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    while (i >= 0 && curr[i] != '(')
        i--;
    i--;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());

    int num = stoi(number);
    if (num > (1 << 12))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 11; i >= 0; i--)
    {
        if ((num >> i) & 1)
            num_to_binary += '1';
        else
            num_to_binary += '0';
    }
    this->immediate12_bin = num_to_binary;
}

void JR_type ::initialize()
{
    string curr = this->inst;
    int n = curr.size();
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    while (curr[i] != '(')
        i++;
    i++;
    string src_reg = "";
    while (curr[i] != ')')
    {
        src_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
    this->base = new Register(src_reg);
}
string JR_type ::get_binary()
{
    string ans = this->immediate12_bin + " " + this->base->get_reg_binary() + " " + this->func3_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}
L_type ::L_type() {}
L_type ::L_type(string ins) : Instructions(ins)
{
    this->opcode = "0000011";
    this->func3();
    this->immediate12();
    this->initialize();
}
void L_type ::func3()
{
    map<string, string> opname_to_binary{
        {"lb", "000"},
        {"lh", "001"},
        {"lw", "010"},
        {"ld", "011"},
        {"lbu", "100"},
        {"lhu", "101"},
        {"lwu", "110"},
    };
    this->func3_bin = opname_to_binary[this->opname];
}
void L_type ::immediate12()
{
    string curr = this->inst;
    int n = curr.size();
    int i = n - 1;
    while (i >= 0 && curr[i] != '(')
        i--;
    i--;
    string number = "";
    while (i >= 0)
    {
        if (curr[i] == ' ' || curr[i] == ',')
            break;
        number += curr[i];
        i--;
    }
    reverse(number.begin(), number.end());

    int num = stoi(number);
    if (num > (1 << 12))
    {
        cerr << "immediate is too large" << endl;
        return;
    }
    string num_to_binary = "";
    for (int i = 11; i >= 0; i--)
    {
        if ((num >> i) & 1)
            num_to_binary += '1';
        else
            num_to_binary += '0';
    }
    this->immediate12_bin = num_to_binary;
}
void L_type ::initialize()
{
    string curr = this->inst;
    int n = curr.size();
    int i = 0;
    while (curr[i] != ' ')
        i++;
    i++;
    string dest_reg = "";
    while (curr[i] != ',')
    {
        dest_reg += curr[i];
        i++;
    }
    while (curr[i] != '(')
        i++;
    i++;
    string src_reg = "";
    while (curr[i] != ')')
    {
        src_reg += curr[i];
        i++;
    }
    this->dest = new Register(dest_reg);
    this->src = new Register(src_reg);
}
string L_type ::get_binary()
{
    string ans = this->immediate12_bin + " " + this->src->get_reg_binary() + " " + this->func3_bin + " " + this->dest->get_reg_binary() + " " + this->opcode;
    return ans;
}
string tolower(string s)
{
    string ans = "";
    for (auto &&it : s)
    {
        if (it >= 'A' && it <= 'Z')
            ans += (it + ('a' - 'A'));
        else
            ans += it;
    }
    return ans;
}
int main()
{
    freopen("Input_Max_of_N_numbers.txt", "r", stdin);
    R_type *R_obj;
    I_type_arithmetic *I_arithmetic_obj;
    I_type_shift *I_shift_obj;
    L_type *L_obj;
    B_type *B_obj;
    J_type *J_obj;
    JR_type *JR_obj;
    S_type *S_obj;
    string line;
    vector<string> instructions_vec;
    unordered_map<string, int> label_indices;
    while (getline(cin, line))
    {
        line = tolower(line);
        if (line[0] == '.')
        {
            while (line.back() != ':')
                line.pop_back();
            line.pop_back();
            label_indices[line] = instructions_vec.size();
        }
        instructions_vec.push_back(line);
    }
    vector<string> ans;
    int n = instructions_vec.size();
    for (int i = 0; i < n; i++)
    {
        string curr = instructions_vec[i];
        if (curr[0] == '.')
            continue;
        string curr_type = get_inst_type_util(curr);
        if (curr_type == "R_type")
        {
            R_obj = new R_type(curr);
            string binary = R_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "I_type_arithmetic")
        {
            I_arithmetic_obj = new I_type_arithmetic(curr);
            string binary = I_arithmetic_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "I_type_shift")
        {
            I_shift_obj = new I_type_shift(curr);
            string binary = I_shift_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "L_type")
        {
            L_obj = new L_type(curr);
            string binary = L_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "B_type")
        {
            string label = "";
            int n = curr.size();
            int i = n - 1;
            while (curr[i] == ' ')
                i--;
            while (curr[i] != '.')
            {
                label += curr[i];
                curr.pop_back();
                i--;
            }
            label += curr[i];
            curr.pop_back();
            reverse(label.begin(), label.end());
            int offset = label_indices[label] - i;
            string offset_str = to_string(offset);
            curr += offset_str;
            B_obj = new B_type(curr);
            string binary = B_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "J_type")
        {
            string label = "";
            if (curr[0] == 'j')
            {
                int j = 0;
                while (curr[j] != '.')
                    curr[j]++;
                label = "";
                while (j < curr.size())
                {
                    label += curr[j];
                    j++;
                }
            }
            string new_inst = "jal x0, ";
            int offset = label_indices[label] - i;
            string offset_str = to_string(offset);
            new_inst += offset_str;
            J_obj = new J_type(new_inst);
            string binary = J_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "JR_type")
        {
            string new_inst = "jalr x0, 0(ra)";
            JR_obj = new JR_type(new_inst);
            string binary = JR_obj->get_binary();
            ans.push_back(binary);
        }
        else if (curr_type == "S_type")
        {
            S_obj = new S_type(curr);
            string binary = S_obj->get_binary();
            ans.push_back(binary);
        }
    }
    freopen("Output_Max_of_N_numbers.txt", "w", stdout);
    for (auto &&it : ans)
        cout << it << endl;
    return 0;
}
