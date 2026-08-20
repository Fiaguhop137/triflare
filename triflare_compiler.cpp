#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
int size;
std::string getchonk(char a,char b) {
    size=2;
    if(a=='1'&&b=='0'){
        return("while(tape[pointer]!=0){");
    }
    if(a=='0'&&b=='1'){
        return("}");
    }
    if(a=='2'&&b=='2'){
        return("std::cout<<tape[pointer];");
    }
    size=1;
    if(a=='0'){
        return("pointer--;");
    }
    if(a=='1'){
        return("one();");
    }
    if(a=='2'){
        return("tape[pointer]=!tape[pointer];");
    }
    return("");
}
int main(int argc,char *argv[]) {
    std::string programname;
    for(int i=1;i<argc;i++){
        if(i>1){
            programname+=" ";
        }
        programname+=argv[i];
    }
    std::ifstream programfile(programname + ".tri");
    std::string program((std::istreambuf_iterator<char>(programfile)),std::istreambuf_iterator<char>());
    std::ofstream file(programname+".cpp");
    file<<"#include <iostream>\n#include <vector>\nstd::vector<bool> tape(8,false);int pointer=0;void one(){pointer++;if(tape.size()<=pointer)tape.resize(pointer+8,false);}int main(){";
    int i=0;
    while(i<program.size()){
        file<<getchonk(program[i],program[i+1]);
        i+=size;
    }
    file<<"std::cout<<'\\n';}";
    file.close();
    system(("g++ -O2 "+programname+".cpp -o "+programname).c_str());
    remove((programname + ".cpp").c_str());
}