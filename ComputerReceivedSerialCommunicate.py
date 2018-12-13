import serial
import os

# 使用CH340转换芯片利用USB口模拟串口实现单片机和电脑的通讯。

ser=serial.Serial("com4",9600,timeout=1) # windows系统下使用COM3口连接串行口
print("the name of the danpianji is:"+ser.name)
print("the port is:"+str(ser.port))
print("the baudrate is:"+str(ser.baudrate))
if not ser.isOpen():
    ser.open()# 打开端口
'''
for i in range(100):
    print("==========times:{}===========".format(i+1))
    sth=b'aaaaaaaaaa    '
    ser.write(sth)
    s=ser.readline()
    print("===========\n"+str(s))
'''
print(str(ser.readline()))

ser.close()       # 关闭串口






