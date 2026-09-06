#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
int size;
std::string getchonk(char a,char b){
    size=2;
    if(a=='1' && b=='0'){return "while(tape[pointer]!=0){";}
    if(a=='0' && b=='1'){return "}";}
    if(a=='2' && b=='2'){return "twentytwo();";}
    size=1;
    if(a=='0'){return "pointer--;";}
    if(a=='1'){return "one();";}
    if(a=='2'){return "tape[pointer]=!tape[pointer];";}
    return "";
}
int main(int argc,char* argv[]){
    std::string input;
    std::string output;
    for(int i=1;i<argc;++i){
        std::string arg=argv[i];
        if(arg=="-o"){
            if(i+1>=argc){
                std::cerr<<"Error: -o requires an output filename\n";
                return 1;
            }
            output=argv[++i];
        }
        else if(!arg.empty()&&arg[0]=='-'){
            std::cerr<<"Error: unknown option \""<<arg<<"\"\n";
            return 1;
        }
        else{
            if(!input.empty()){
                std::cerr<<"Error: multiple input files specified\n";
                return 1;
            }
            input=arg;
        }
    }
    if(input.empty()){
        std::cerr<<"Error: no input file specified\n"<<"Usage: tric [-o output] input.tri\n";
        return 1;
    }
    if(output.empty()){
        std::cerr<<"Error: no output file specified\n"<<"Usage: tric [-o output] input.tri\n";
        return 1;
    }
    std::ifstream programfile(input);
    if(!programfile){
        std::cerr<<"Error: could not open input file \""<<input<<"\"\n";
        return 1;
    }
    std::string program((std::istreambuf_iterator<char>(programfile)),std::istreambuf_iterator<char>());
    std::ofstream file("__tricache__");
    if(!file){
        std::cerr<<"Error: could not create compiler cache\n";
        return 1;
    }
    file<<"#include <iostream>\n#include <vector>\n#include <cstdint>\nstd::vector<bool> tape(8,false);std::uint32_t pointer=0;void twentytwo(){uint8_t value=0;for(int i=0;i<8;++i){value=(value<<1)|tape[pointer+i];}std::cout<<static_cast<char>(value);}void one(){pointer++;if(tape.size()<=pointer)tape.resize(pointer+8,false);}int main(){";
    int i=0;
    while(i<static_cast<int>(program.size())){
        char a=program[i];
        char b=(i+1<static_cast<int>(program.size()))?program[i+1]:'\0';
        file<<getchonk(a,b);
        i+=size;
    }
    file<<"std::cout<<'\\n';}";
    file.close();
    int result=system(("g++ -O3 -flto=auto \"__tricache__\" -o \""+output+"\"").c_str());
    remove("__tricache__");
    if(result!=0){std::cerr<<"Error: g++ returned "<<result<<'\n';}
    return result;
}
