import numpy as np

def Excute(matrix,instr):
    instr_list=instr.split()
    if instr_list[0]=="L":
        # print("L")
        tmp_m1=matrix[0:int(instr_list[1]),:]
        tmp_m2=np.roll(matrix[int(instr_list[1]),:],-1)
        tmp_m3=matrix[int(instr_list[1])+1:,:]
        ret_m=np.vstack((tmp_m1,tmp_m2))
        ret_m=np.vstack((ret_m,tmp_m3))
    elif instr_list[0]=="R":
        # print("R")
        tmp_m1=matrix[0:int(instr_list[1]),:]
        tmp_m2=np.roll(matrix[int(instr_list[1]),:],1)
        tmp_m3=matrix[int(instr_list[1])+1:,:]
        ret_m=np.vstack((tmp_m1,tmp_m2))
        ret_m=np.vstack((ret_m,tmp_m3))
    elif instr_list[0]=="U":
        # print("U")
        tmp_m1=matrix[:,0:int(instr_list[1])]
        tmp_m2=np.roll(matrix[:,int(instr_list[1])],-1).reshape(3,1)
        tmp_m3=matrix[:,int(instr_list[1])+1:]
        ret_m=np.hstack((tmp_m1,tmp_m2))
        ret_m=np.hstack((ret_m,tmp_m3))
    elif instr_list[0]=="D":
        # print("D")
        tmp_m1=matrix[:,0:int(instr_list[1])]
        tmp_m2=np.roll(matrix[:,int(instr_list[1])],1).reshape(3,1)
        tmp_m3=matrix[:,int(instr_list[1])+1:]
        ret_m=np.hstack((tmp_m1,tmp_m2))
        ret_m=np.hstack((ret_m,tmp_m3))
    return ret_m
if __name__=='__main__':
    filename=input("Please enter the file name:")
    with open(filename,"r") as f:
        lines=f.readlines()
    count=0;
    list=[]
    for line in lines:
        for elem in line.split():
            list.append(int(elem))
        count=count+1
        if count==3:
            break;
    matrix=np.array(list)
    matrix=matrix.reshape(3,3)
    count=0
    for line in lines:
        if count>3 and line.strip():
            print(line)
            matrix=Excute(matrix,line)
        count=count+1
    print(matrix)
