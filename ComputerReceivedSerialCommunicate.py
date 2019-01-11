import serial
import os

def TransferKeyvalueToString(key_value):
    key="NoKeyValue"
    if key_value == b'\x80':
        key = "a44"
    if key_value == b'\x81':
        key = "a34"
    if key_value == b'\x82':
        key = "a24"
    if key_value == b'\x83':
        key = "a14"
    if key_value == b'\x08':
        key = "a43"
    if key_value == b'\t':
        key = "a33"
    if key_value == b'\n':
        key = "a23"
    if key_value == b'\x0b':
        key = "a13"
    if key_value == b'\x10':
        key = "a42"
    if key_value == b'\x11':
        key = "a32"
    if key_value == b'\x12':
        key = "a22"
    if key_value == b'\x13':
        key = "a12"
    if key_value == b'\x18':
        key = "a41"
    if key_value == b'\x19':
        key = "a31"
    if key_value == b'\x1a':
        key = "a21"
    if key_value == b'\x1b':
        key = "a11"
    return key


# 使用CH340转换芯片利用USB口模拟串口实现单片机和电脑的通讯。


ser=serial.Serial("com4",9600,timeout=1) # windows系统下使用COM3口连接串行口
print("the name of the danpianji is:"+ser.name)
print("the port is:"+str(ser.port))
print("the baudrate is:"+str(ser.baudrate))
if not ser.isOpen():
    ser.open()# 打开端口
print("串口已经打开！正在获取键值信息！")
'''
for i in range(100):
    print("==========times:{}===========".format(i+1))
    sth=b'aaaaaaaaaa    '
    ser.write(sth)
    s=ser.readline()
    print("===========\n"+str(s))
'''


key_value_past=None
while 1:
    key_value = ser.read()
    if key_value == key_value_past:
        continue
    if key_value == b'\xff':
        continue
    key_value_past = key_value
    print(TransferKeyvalueToString(key_value))
    if key_value == b'\x1a':
        break

ser.close()       # 关闭串口






