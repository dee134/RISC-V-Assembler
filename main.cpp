#include<bits/stdc++.h>
#include<iostream>
#include<sstream>

using namespace std;

unordered_map<int,string> registers;
unordered_map<string,int> register_alias;
unordered_map<string, vector<string>> instruction; // to store funct values

// register int to 5 bit binary   
bitset<5> int_to_bin5(int n) {
    bitset<5> bin(n);
    return bin;
}
bitset<7> hex_to_bin7(string hex) {
    unsigned int  decimal = stoi(hex,nullptr,16);
    bitset<7> bin(decimal);
    return bin;
}
bitset<3> hex_to_bin3(string hex) {
    unsigned int  decimal = stoi(hex,nullptr,16);
    bitset<3> bin(decimal);
    return bin;
}
bitset<6> hex_to_bin6(string hex) {
    unsigned int  decimal = stoi(hex,nullptr,16);
    bitset<6> bin(decimal);
    return bin;
}

// binary to hex
string binaryToHex(const string& binary) {
    stringstream ss;
    ss << hex << setfill('0') << setw(8) << bitset<32>(binary).to_ulong();
    return ss.str();
}

//populate register
void populate_registers() {
    for(int i = 0;i<32;i++) {
        registers[i] = int_to_bin5(i).to_string();
    }
    {
      
    register_alias.emplace("zero",0);
    register_alias.emplace("ra",1);
    register_alias.emplace("sp",2);
    register_alias.emplace("gp",3);
    register_alias.emplace("tp",4);
    register_alias.emplace("t0",5);
    register_alias.emplace("t1",6);
    register_alias.emplace("t2",7);
    register_alias.emplace("s0",8);
    register_alias.emplace("fp",8);  
    register_alias.emplace("s1",9);
    register_alias.emplace("a0",10);
    register_alias.emplace("a1",11);
    register_alias.emplace("a2",12);
    register_alias.emplace("a3",13);
    register_alias.emplace("a4",14);
    register_alias.emplace("a5",15);
    register_alias.emplace("a6",16);
    register_alias.emplace("a7",17);
    register_alias.emplace("s2",18);
    register_alias.emplace("s3",19);
    register_alias.emplace("s4",20);
    register_alias.emplace("s5",21);
    register_alias.emplace("s6",22);
    register_alias.emplace("s7",23);
    register_alias.emplace("s8",24);
    register_alias.emplace("s9",25);
    register_alias.emplace("s10",26);
    register_alias.emplace("s11",27);
    register_alias.emplace("t3",28);
    register_alias.emplace("t4",29);
    register_alias.emplace("t5",30);
    register_alias.emplace("t6",31);
    
    }
}

void populate_instruction() {
    //r format
    instruction.emplace("add",vector<string>{"0x0","0x00"});
    instruction.emplace("sub",vector<string>{"0x0","0x20"});
    instruction.emplace("xor",vector<string>{"0x4","0x00"});
    instruction.emplace("or",vector<string>{"0x6","0x00"});
    instruction.emplace("and",vector<string>{"0x7","0x00"});
    instruction.emplace("sll",vector<string>{"0x1","0x00"});
    instruction.emplace("srl",vector<string>{"0x5","0x00"});
    instruction.emplace("sra",vector<string>{"0x5","0x20"});
    instruction.emplace("slt",vector<string>{"0x2","0x00"});
    instruction.emplace("sltu",vector<string>{"0x3","0x00"});

    // i format
    instruction.emplace("addi",vector<string>{"0x0"});
    instruction.emplace("xori",vector<string>{"0x4"});
    instruction.emplace("ori",vector<string>{"0x6"});
    instruction.emplace("andi",vector<string>{"0x7"});
    instruction.emplace("slli",vector<string>{"0x1","0x00"});
    instruction.emplace("srli",vector<string>{"0x5","0x00"});
    instruction.emplace("srai",vector<string>{"0x5","0x10"});
    instruction.emplace("slti",vector<string>{"0x2"});
    instruction.emplace("sltiu",vector<string>{"0x3"});
    
    instruction.emplace("lb",vector<string>{"0x0"});
    instruction.emplace("lh",vector<string>{"0x1"});
    instruction.emplace("lw",vector<string>{"0x2"});
    instruction.emplace("ld",vector<string>{"0x3"});
    instruction.emplace("lbu",vector<string>{"0x4"});
    instruction.emplace("lhu",vector<string>{"0x5"});
    instruction.emplace("lwu",vector<string>{"0x6"});

    instruction.emplace("jalr",vector<string>{"0x0"});

    // s format
    instruction.emplace("sb",vector<string>{"0x0"});
    instruction.emplace("sh",vector<string>{"0x1"});
    instruction.emplace("sw",vector<string>{"0x2"});
    instruction.emplace("sd",vector<string>{"0x3"});

    // b format
    instruction.emplace("beq",vector<string>{"0x0"});
    instruction.emplace("bne",vector<string>{"0x1"});
    instruction.emplace("blt",vector<string>{"0x4"});
    instruction.emplace("bge",vector<string>{"0x5"});
    instruction.emplace("bltu",vector<string>{"0x6"});
    instruction.emplace("bgeu",vector<string>{"0x7"});


}

// r-format 
string rformat(vector<string>& inputTokens,stringstream& ss, const int lineCount) {
    // reading input
    string token;
    while(ss >> token) {
        //Error
        if(all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
        //Error
        if(token.front() == ',') return "Error: Line " + to_string(lineCount) + " invalid format";
        
        if(token.back() == ',') {
            token.pop_back();
        }
        if(token.front() == 'x') {
            token = token.substr(1);
        }
        if(token.size() !=0) inputTokens.push_back(token);
    }
    //Error
    if(inputTokens.size() != 4) return  "Error: Line " + to_string(lineCount) + " invalid format";
    
    string opcode = "0110011";

    // output strings
    string out;
    string funct3 = hex_to_bin3(instruction[inputTokens[0]][0]).to_string();
    string funct7 = hex_to_bin7(instruction[inputTokens[0]][1]).to_string();

    // handling register aliases
    for(int i = 1;i<=3;i++) {
        if(!all_of(inputTokens[i].begin(), inputTokens[i].end(), ::isdigit)) {
            
            //Error
            if(register_alias.find(inputTokens[i]) == register_alias.end()) return "Error: Line " + to_string(lineCount) + " invalid format";
            
            inputTokens[i] = to_string(register_alias[inputTokens[i]]);
        }
        
        //Error
        else if(stoi(inputTokens[i]) > 31 ||stoi(inputTokens[i]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    }
    out =funct7 + registers[stoi(inputTokens[3])]+ registers[stoi(inputTokens[2])] + funct3 + registers[stoi(inputTokens[1])]+ opcode;

    return out;
}

// i format // 
string iformat(vector<string>& inputTokens,stringstream& ss, const int lineCount) {
    string out;
    string opcode = "0010011"; 
    
    // input for load instructions
    if(inputTokens[0][0] == 'l' || inputTokens[0] == "jalr") {
        opcode = "0000011";
        // for jalr
        if(inputTokens[0] == "jalr") opcode = "1100111";
        
        string token;
        while(ss >> token) {
            //Error incorrect register names
            if(inputTokens.size() ==1 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
            
            if(inputTokens.size()==2) {
                // cout << token;
                int idx1,idx2;
                for(string::size_type i = 0; i<token.size();i++){
                    if(token[i]=='(') {
                        idx1 = i;
                        idx2 = idx1;
                    }
                    if(token[i]=='x') {
                        idx2 = i;
                    }
                }
                token.pop_back();
                string token1 = token.substr(0,idx1);
                string token2 = token.substr(idx2+1);
                //Error incorrect offset
                if(token1.size() == 0 || (token1[0] == '-' ? 
                !all_of(token1.begin() + 1, token1.end(), ::isdigit) :
                !all_of(token1.begin(), token1.end(), ::isdigit)) || stoll(token1) < -2048 || stoll(token1) > 2047) return "Error: Line " + to_string(lineCount) + " invalid offset";
                
                //Error incorrect reg name
                if(idx1 == idx2 && all_of(token2.begin(), token2.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
                if(token2.size() !=0) inputTokens.push_back(token2);
                if(token1.size() !=0) inputTokens.push_back(token1);
                continue;
            }
            if(token.back() == ',') {
                token.pop_back();
            }
            if(token.front() == 'x') {
                token = token.substr(1);
            }
            if(token.size() !=0) inputTokens.push_back(token);
        
        }
    }

    // input for instructions like addi
    else {     
        string token;
        while(ss >> token) {
            
            //Error
            if(inputTokens.size()!= 3 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
            
            if(token.back() == ',') {
                token.pop_back();
            }
            //Error
            if(inputTokens.size()==3 && ((token[0] == '-' ? 
                !all_of(token.begin() + 1, token.end(), ::isdigit) :
                !all_of(token.begin(), token.end(), ::isdigit)) || stoll(token) < -2048 || stoll(token)>2047)) return "Error: Line " + to_string(lineCount) + " invalid imm";
            
            if(token.front() == 'x') {
                token = token.substr(1);
            }
            if(token.size() !=0) inputTokens.push_back(token);
        }
    }
    
    //Error
    if(inputTokens.size() != 4) return "Error: Line " + to_string(lineCount) + " invalid format";
    string funct3 = hex_to_bin3(instruction[inputTokens[0]][0]).to_string();
    
    //handling register aliases
    for(int i = 1;i<=2;i++) {
        if(!all_of(inputTokens[i].begin(), inputTokens[i].end(), ::isdigit)) {
            if(register_alias.find(inputTokens[i]) == register_alias.end()) return "Error: Line " + to_string(lineCount) + " invalid format";
            inputTokens[i] = to_string(register_alias[inputTokens[i]]);
        }
        else if(stoi(inputTokens[i]) > 31 ||stoi(inputTokens[i]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    }
    string imm_str = inputTokens.back();

    // integer imm
    signed int imm_dec;
  
    imm_dec = stoi(imm_str);
    

    if(inputTokens[0] == "slli" || inputTokens[0] == "srli" || inputTokens[0] == "srai") {
        bitset<6> imm(imm_dec);
        string funct6 = hex_to_bin6(instruction[inputTokens[0]][1]).to_string();
        out = funct6 + imm.to_string() + registers[stoi(inputTokens[2])]+funct3+registers[stoi(inputTokens[1])]+opcode;
    }
    
    else { 
        bitset<12> imm(imm_dec);
        out = imm.to_string() + registers[stoi(inputTokens[2])]+funct3+registers[stoi(inputTokens[1])]+opcode;
    }
    
    return out;
}

// s format
string sformat(vector<string>& inputTokens, stringstream& ss, const int lineCount) { 
    string output;
    string opcode = "0100011";

    //input
    string token;
    while(ss >> token) {
        if(inputTokens.size()==2) {
            int idx1,idx2;
            for(string::size_type i =0; i<token.size();i++){
                if(token[i]=='(') {
                    idx1 = i;
                    idx2 = idx1;
                }
                if(token[i]=='x') {
                    idx2 = i;
                }
            }
            
            token.pop_back();
            string token1 = token.substr(0,idx1);
            string token2 = token.substr(idx2+1);
            //Error incorrect offset
            if(token1.size() == 0 || (token1[0] == '-' ? 
                !all_of(token1.begin() + 1, token1.end(), ::isdigit) :
                !all_of(token1.begin(), token1.end(), ::isdigit))|| stoll(token1) < -2048 || stoll(token1)>2047) return "Error: Line " + to_string(lineCount) + " invalid offset";
            //Error incorrect reg name
            if(idx1 == idx2 && all_of(token2.begin(), token2.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
            
            if(token1.size() !=0) inputTokens.push_back(token1);
            if(token2.size() !=0) inputTokens.push_back(token2);
            continue;
        }
        if(token.back() == ',') {
            token.pop_back();
        }

        //Error
        if(inputTokens.size()== 1 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
        
        if(token.front() == 'x') {
            token = token.substr(1);
        }
        if(token.size() !=0) inputTokens.push_back(token);
    }
    
    //Error
    if(inputTokens.size() != 4) return "Error: Line " + to_string(lineCount) + " invalid format";

    //handling register aliases
    for(int i = 1;i<=3;i++) {
        if(i!=2 && !all_of(inputTokens[i].begin(), inputTokens[i].end(), ::isdigit)) {
            
            //Error
            if(register_alias.find(inputTokens[i]) == register_alias.end()) return "Error: Line " + to_string(lineCount) + " invalid format";
            
            inputTokens[i] = to_string(register_alias[inputTokens[i]]);
        }
        //Error
        else if(i!=2 && stoi(inputTokens[i]) > 31 ||stoi(inputTokens[i]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    }

    // imm
    bitset<12> imm_dec(stoi(inputTokens[2]));
    string imm_string = imm_dec.to_string();
    string imm1 = imm_string.substr(0,7);
    string imm2 = imm_string.substr(7,5);
    
    //funct3
    string funct3 = hex_to_bin3(instruction[inputTokens[0]][0]).to_string();
    
    //output
    output = imm1+registers[stoi(inputTokens[1])] + registers[stoi(inputTokens[3])] + funct3 +imm2 + opcode;

    return output;
    
    
}

// b format
string bformat(vector<string>&inputTokens, stringstream& ss, unordered_map<string,int>& labels, const int lineCount) {
    string out;
    string opcode  = "1100011";
    
    //input
    string token;
    while(ss >> token) {
        if(token.back() == ',') {
            token.pop_back();
        }
        //Error
        if(inputTokens.size()!=3 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
        if(token.front() == 'x') {
            token = token.substr(1);
        }
        if(token.size() !=0) inputTokens.push_back(token);
    }

   //Error
    if(inputTokens.size() != 4) return "Error: Line " + to_string(lineCount) + " invalid format";

    // handling register aliases
    for(int i = 1;i<=2;i++) {
        
        if(!all_of(inputTokens[i].begin(), inputTokens[i].end(), ::isdigit)) {
            if(register_alias.find(inputTokens[i]) == register_alias.end()) {
                out = "Error: Line " + to_string(lineCount) + " invalid format";
                return out;
                
            }
            else inputTokens[i] = to_string(register_alias[inputTokens[i]]);
        }
        //Error
        else if(stoi(inputTokens[i]) > 31 ||stoi(inputTokens[i]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    }

    // label
    signed long long jump;
    if(inputTokens[3][0] == '-' ? !all_of(inputTokens[3].begin() + 1, inputTokens[3].end(), ::isdigit) :!all_of(inputTokens[3].begin(), inputTokens[3].end(), ::isdigit)) {
        //Error: label name does not exist
        if(labels.find(inputTokens[3]) == labels.end()|| labels[inputTokens[3]] == -1) return "Error: Line " + to_string(lineCount) + " invalid label";

        int labelLine = labels[inputTokens[3]];
        jump = (labelLine-lineCount) * 4;
    }
    else jump = stoll(inputTokens[3]);
    
    //Error
    if(jump < -4096 || jump > 4095 || jump%4 !=0) return "Error: Line " + to_string(lineCount) + " invalid jump";
    
    bitset<13> jumpBinary(jump);
    string imm = jumpBinary.to_string();
    
    //funct3
    string funct3 = hex_to_bin3(instruction[inputTokens[0]][0]).to_string();

    out = imm[0] + imm.substr(2,6) + registers[stoi(inputTokens[2])] + registers[stoi(inputTokens[1])] + funct3 + imm.substr(8,4) + imm[1] + opcode;

    return out;
}

// j format
string jformat(vector<string>&inputTokens, stringstream& ss, unordered_map<string,int>& labels, const int lineCount) {
    string output;
    string opcode = "1101111";
    //input
    string token;
    while(ss >> token) {
        if(token.back() == ',') {
            token.pop_back();
        }
        //Error
        if(inputTokens.size() == 1 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
        if(token.front() == 'x') {
            token = token.substr(1);
        }
        if(token.size() !=0) inputTokens.push_back(token);
    }
    
    //Error
    if(inputTokens.size() != 3) return "Error: Line " + to_string(lineCount) + " invalid format";

    // handling register aliases
    if(!all_of(inputTokens[1].begin(), inputTokens[1].end(), ::isdigit)) {
        //Error
        if(register_alias.find(inputTokens[1]) == register_alias.end()) return "Error: Line " + to_string(lineCount) + " invalid format";
        
        inputTokens[1] = to_string(register_alias[inputTokens[1]]);
    }
    //Error
    else if(stoi(inputTokens[1]) > 31 ||stoi(inputTokens[1]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    
    // label
    signed int jump;
    if(inputTokens[2][0] == '-' ? !all_of(inputTokens[2].begin() + 1, inputTokens[2].end(), ::isdigit) :!all_of(inputTokens[2].begin(), inputTokens[2].end(), ::isdigit)) {
        //Error label name does not exist
        if(labels.find(inputTokens[2]) == labels.end() || labels[inputTokens[2]] == -1) return "Error: Line " + to_string(lineCount) + " invalid label";
        int labelLine = labels[inputTokens[2]];
        jump = (labelLine-lineCount) * 4;
    }
    else jump = stoll(inputTokens[2]);
    long long range = pow(2,20);
    //Error
    if(jump < -range || jump > range-2 || jump%4 !=0) return "Error: Line " + to_string(lineCount) + "invalid jump";
    
    bitset<21> jumpBinary(jump);
    string imm = jumpBinary.to_string();
    
    output = imm[0]+imm.substr(10,10) + imm[9] +imm.substr(1,8) + registers[stoi(inputTokens[1])] +opcode; 
    return output;
}

// u format
string uformat(vector<string>&inputTokens, stringstream& ss, const int lineCount) {
    string output;
    string opcode  = "0110111";
    if(inputTokens[0] == "auipc") opcode = "0010111";    
    //input 
    string token;
    long range = pow(2,19);
    while(ss >> token) {
        if(token.back() == ',') {
            token.pop_back();
        }
        //Error
        if(inputTokens.size()== 1 && all_of(token.begin(), token.end(), ::isdigit)) return "Error: Line " + to_string(lineCount) + " invalid format";
        
        //Error
        if(inputTokens.size()== 2 && ((token[0] == '-' ? 
                !all_of(token.begin() + 1, token.end(), ::isdigit) :
                !all_of(token.begin(), token.end(), ::isdigit)) ||stoll(token) < -range || stoll(token) > range-1  )) return "Error: Line " + to_string(lineCount) + " invalid imm";
        
        if(token.front() == 'x') {
            token = token.substr(1);
        }
        if(token.size() !=0) inputTokens.push_back(token);
    }
    
    //Error
    if(inputTokens.size() != 3) return "Error: Line " + to_string(lineCount) + " invalid format";

    //handling register aliases
    if(!all_of(inputTokens[1].begin(), inputTokens[1].end(), ::isdigit)) {
        //Error
        if(register_alias.find(inputTokens[1]) == register_alias.end()) return "Error: Line " + to_string(lineCount) + " invalid format";
        
        inputTokens[1] = to_string(register_alias[inputTokens[1]]);
    }
    //Error
    else if(stoi(inputTokens[1]) > 31 ||stoi(inputTokens[1]) < 0 ) return "Error: Line " + to_string(lineCount) + " invalid format";
    
    string imm_str = inputTokens[2];
    signed long int imm_dec;
    
    imm_dec = stol(imm_str);
  
    bitset<20> bin(imm_dec);
    string imm = bin.to_string();

    output  = imm.substr(0,20) + registers[stoi(inputTokens[1])] + opcode ;
    return output;
}

void populate_format(unordered_map<string,char>& format) {
    
    // r-format
    format["add"] = 'r';
    format["sub"] = 'r';
    format["and"] = 'r';
    format["or"] = 'r';
    format["xor"] = 'r';
    format["sll"] = 'r';
    format["srl"] = 'r';
    format["sra"] = 'r';
    format["slt"] = 'r';
    format["sltu"] = 'r';

    
    //i- format
    format["addi"] = 'i';
    format["xori"] = 'i';
    format["ori"] = 'i';
    format["andi"] = 'i';
    format["slli"] = 'i';
    format["srli"] = 'i';
    format["srai"] = 'i';
    format["slti"] = 'i';
    format["sltiu"] = 'i';
    
    format["lb"] = 'i';
    format["lh"] = 'i';
    format["lw"] = 'i';
    format["ld"] = 'i';
    format["lbu"] = 'i';
    format["lhu"] = 'i';
    format["lwu"] = 'i';
    format["jalr"] = 'i';

    // s format
    format["sb"] = 's';
    format["sh"] = 's';
    format["sw"] = 's';
    format["sd"] = 's';

    // b format
    format["beq"] = 'b';
    format["bne"] = 'b';
    format["blt"] = 'b';
    format["bge"] = 'b';
    format["bltu"] = 'b';
    format["bgeu"] = 'b';

    // j format
    format["jal"] ='j';

    // u format
    format["lui"] = 'u';
    format["auipc"] = 'u';

}

void populate_lables(unordered_map<string,int>& labels, vector<int>& invalidLines) {
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error!" << endl;
    }

    int lineCount = 1;
    string input;
    while (getline(inputFile, input)) {
        stringstream ss(input);
        string token;
        ss >> token;
        if(token.back() == ':') token.pop_back();
        else {
            lineCount++;
            continue;
        }
        if(labels.find(token) == labels.end() || labels[token] == -1) labels[token] = lineCount;
        else if(input.size() != 1) {
            invalidLines.push_back(lineCount);
            invalidLines.push_back(labels[token]);
            labels[token] =-1;
        } 
        lineCount++;
    }
    inputFile.close();
}


int main() {
    unordered_map<string,char> format;
    unordered_map<string,int> labels;
    populate_format(format);
    populate_registers();
    populate_instruction();
    vector<int> invalidLines;
    populate_lables(labels, invalidLines);

    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error!" << endl;
        return 1;
    }

     // infile, outfile
    string input;
    int lineCount =1;
    while(getline(inputFile, input) && input.size() != 0) {
        //Error
        if(invalidLines.size() != 0 && find(invalidLines.begin(), invalidLines.end(),lineCount) != invalidLines.end()) {
            outputFile << "Error: Line "<< lineCount<< " invalid label"<<endl;
            lineCount++;
            continue;
        } 
        vector<string> inputTokens;
        stringstream ss(input);
        string token;
        ss >> token; 
        if(token.back() == ':') {
            ss >> token;

        }
        // comments
        if(token.front() == ';') {
            outputFile << "comment" << endl;
            lineCount++;
            continue;
        }
        if(token.back() == ';') {
           outputFile << "Error: Line "<< lineCount<< " invalid format"<<endl;
           lineCount++;
           continue;
        }
        
        inputTokens.push_back(token);
        
        //wt's the use.
        if(inputTokens.size() == 0) continue;
        string out;
        if (format[inputTokens[0]] == 'r') {
             out = rformat(inputTokens, ss, lineCount);
            }
        else if(format[inputTokens[0]] == 'i') {
             out = iformat(inputTokens,ss, lineCount);
        }
        else if(format[inputTokens[0]] == 's') {
             out = sformat(inputTokens,ss, lineCount);
        }
        else if(format[inputTokens[0]] == 'b') {
             out = bformat(inputTokens,ss,labels,lineCount);
        }
        else if(format[inputTokens[0]] == 'j') {
             out = jformat(inputTokens,ss,labels,lineCount);
        }
        else if(format[inputTokens[0]] == 'u') {
             out = uformat(inputTokens,ss,lineCount);
        }
        else {
            outputFile << "Error: Line " <<lineCount<< " invalid format" <<endl;
            lineCount++;
            continue;
        }
        // write to output file
        if(!all_of(out.begin(), out.end(), ::isdigit)) outputFile<<out<<endl;
        else outputFile << binaryToHex(out)<< endl;
        lineCount++;
    }

    inputFile.close();
    outputFile.close();
    return 0;

}