# -*- coding:utf-8 -*-
def get_temperature_stats(filename):
    import csv
    data={}
    with open(filename, "r",encoding='utf-8') as f:
        csv_reader=csv.reader(f)
        for line in list(csv_reader):
            data[line[2]]=[int(line[0]),int(line[1])]
    return data

if __name__=='__main__':
    tfile=input("Please input your file name:")
    result=get_temperature_stats(tfile)
    print(result)