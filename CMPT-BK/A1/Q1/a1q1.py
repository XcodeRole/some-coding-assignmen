from isfloat import isfloat

import readline

class THO:
    def __init__(self):
        self.R=float(0)
        self.var={}
    def Excute(self,instruction):
        instructions=instruction.split()
        if instructions[0]=="ASK":
            self.var[instructions[1]]=float(input(instructions[1]+"?"))
        elif instructions[0]=="ADD":
            if isfloat(instructions[1]):
                self.R=self.R+float(instructions[1]) 
            else:   
                self.R=self.R+self.var[instructions[1]]
        elif instructions[0]=="DIV":
            if isfloat(instructions[1]):
                self.R=self.R/float(instructions[1]) 
            else:   
                self.R=self.R+self.var[instructions[1]]
        elif instructions[0]=="SUB":
            if isfloat(instructions[1]):
                self.R=self.R-float(instructions[1]) 
            else:   
                self.R=self.R+self.var[instructions[1]]
        elif instructions[0]=="MUL":
            if isfloat(instructions[1]):
                self.R=self.R*float(instructions[1]) 
            else:   
                self.R=self.R+self.var[instructions[1]]
        elif instructions[0]=="TELL":
            print(self.R)

if __name__=='__main__':
    tho=THO()
    scriptname = input("File to load:")
    with open(scriptname,"r") as f:
        for instruction in f.readlines():
            if instruction.strip():
                tho.Excute(instruction)