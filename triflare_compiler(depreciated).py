# This is an older version, would not recommend using.
import subprocess,os,sys
path=os.path.dirname(os.path.abspath(__file__))
programname=" ".join(sys.argv[1:])
file=open(os.path.join(path,f"{programname}.cpp"),"w")
programfile=open(os.path.join(path,f"{programname}.tri"),"r",encoding="utf-8")
program=programfile.read()
def getchonk(cha1,cha2):
    global size
    size=2
    if cha1==1 and cha2==0:return "while(tape[pointer]!=0){"
    elif cha1==0 and cha2==1:return "}"
    elif cha1==cha2 and cha1==2:return "std::cout<<tape[pointer];"
    else:
        size=1
        if cha1==0:return "pointer--;"
        elif cha1==1:return "one()"
        elif cha1==2:return "tape[pointer]=!tape[pointer];"
file.write('#include <iostream>\n#include <vector>\nstd::vector<bool> tape(8,false);int pointer=0;void one(){pointer++;if(tape.size()<=pointer){tape.resize(pointer+8,false);}}int main(){')
i,size=0,1
while True:
    i+=size
    try:file.write(getchonk(int(program[i-1]),int(program[i])) or '')
    except IndexError:break
file.write("std::cout<<'\\n';}")
file.close()
programfile.close()
subprocess.run(["g++","-Wall","-O2",os.path.join(path,f"{programname}.cpp"),"-o",os.path.join(path,programname)])
os.remove(os.path.join(path,f"{programname}.cpp"))