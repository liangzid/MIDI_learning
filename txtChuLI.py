import re

path='D://desktop//rgb.txt'
file=open(path,'r')
def FindCharFromALine(charr,line):
    for i in line:
        if i==charr:
            print("在这一行里面有符号："+charr)
            return True
    return False



ArrayList=[[]]
for line in file.readlines():
    if FindCharFromALine('{',line):
        ArrayList.append([])



















