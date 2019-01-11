import CutMusic
import PlayMusic
import serial
import os

import threading

from_path = "D://desktop//lgy.mp3"
cut_num = 100
to_dir = "D://desktop//MIDIlgy//"
filename2 = "tiankongzhicheng"
filename = "lgy"
# 切割音乐文件
CutMusic.CutMusic(from_path, to_dir, filename, cut_num )

# 播放（这里仅仅是顺序播放）
# PlayMusic.PlayMusicDemo(PlayMusic.getSplitFiles(to_dir, filename, cut_num))

# 尝试使用多线程
PlayMusic.PlayMusicMultiThreadings(PlayMusic.getSplitFiles(to_dir,filename,cut_num))




# 打开串口
ser = serial.Serial("com8", 9600, timeout=1)
print("the name of the danpianji is:" + ser.name)
print("the port is:" + str(ser.port))
print("the baudrate is:" + str(ser.baudrate))
if not ser.isOpen():
    ser.open()  # 打开端口
print("串口已经打开！正在获取键值信息！")

key_value_past = None
CiShu_Key = []
for i in range(64):
    CiShu_Key.append(0)


files = PlayMusic.getSplitFiles(to_dir, filename, cut_num)

while 1:
    key_values = ser.read()
    key_value=key_values[0:2]
    if key_value == key_value_past:
        continue
    if key_value == b'\xff':
        continue
    key_value_past = key_value
    print(key_value)
    if key_value == b'\x00':             #0
        PlayMusic.PlayMusicIndex(files[2])
        continue
    if key_value == b'\x01':             #1
        PlayMusic.PlayMusicIndex(files[4])
        continue
    if key_value == b'\x02':             #2
        PlayMusic.PlayMusicIndex(files[10])
        continue
    if key_value == b'\x03':             #3
        PlayMusic.PlayMusicIndex(files[14])
        continue
    if key_value == b'\x04':             #4
        PlayMusic.PlayMusicIndex(files[12])
        continue
    if key_value == b'\x05':             #5
        PlayMusic.PlayMusicIndex(files[8])
        continue
    if key_value == b'\x06':             #6
        PlayMusic.PlayMusicIndex(files[4])
        continue
    if key_value == b'\x07':             #7
        PlayMusic.PlayMusicIndex(files[0])
        continue
    if key_value == b'\x08':             #8
        PlayMusic.PlayMusicIndex(files[18])
        continue
    if key_value == b'\t':               #9
        PlayMusic.PlayMusicIndex(files[22])
        continue
    if key_value == b'\n':               #10
        PlayMusic.PlayMusicIndex(files[26])
        continue
    if key_value == b'\x0b':             #11
        PlayMusic.PlayMusicIndex(files[30])
        continue
    if key_value == b'\x0c':             #12
        PlayMusic.PlayMusicIndex(files[34])
        continue
    if key_value == b'\r':               #13
        PlayMusic.PlayMusicIndex(files[24])
        continue
    if key_value == b'\x0e':             #14
        PlayMusic.PlayMusicIndex(files[20])
        continue
    if key_value == b'\x0f':             #15
        PlayMusic.PlayMusicIndex(files[16])
        continue
    if key_value == b'\x10':             #16
        PlayMusic.PlayMusicIndex(files[34])
        continue
    if key_value == b'\x11':             #17
        PlayMusic.PlayMusicIndex(files[38])
        continue
    if key_value == b'\x12':             #18
        PlayMusic.PlayMusicIndex(files[42])
        continue
    if key_value == b'\x13':             #19
        PlayMusic.PlayMusicIndex(files[46])
        continue
    if key_value == b'\x14':             #20
        PlayMusic.PlayMusicIndex(files[44])
        continue
    if key_value == b'\x15':             #21
        PlayMusic.PlayMusicIndex(files[40])
        continue
    if key_value == b'\x16':             #22
        PlayMusic.PlayMusicIndex(files[36])
        continue
    if key_value == b'\x17':             #23
        PlayMusic.PlayMusicIndex(files[32])
        continue
    if key_value == b'\x18':             #24
        PlayMusic.PlayMusicIndex(files[50])
        continue
    if key_value == b'\x19':             #25
        PlayMusic.PlayMusicIndex(files[54])
        continue
    if key_value == b'\x1a':             #26
        PlayMusic.PlayMusicIndex(files[58])
        continue
    if key_value == b'\x1b':             #27
        #PlayMusic.PlayMusicIndex(files[62])
        continue
    if key_value == b'\x1c':             #28
        #PlayMusic.PlayMusicIndex(files[60])
        continue
    if key_value == b'\x1d':             #29
        PlayMusic.PlayMusicIndex(files[56])
        continue
    if key_value == b'\x1e':             #30
        PlayMusic.PlayMusicIndex(files[52])
        continue
    if key_value == b'\x1f':             #31
        PlayMusic.PlayMusicIndex(files[48])
        continue
    if key_value == b' ':                #32
        PlayMusic.PlayMusicIndex(files[51])
        continue
    if key_value == b'!':                #33
        PlayMusic.PlayMusicIndex(files[55])
        continue
    if key_value == b'"':                #34
        PlayMusic.PlayMusicIndex(files[59])
        continue
    if key_value == b'#':                #35
        #PlayMusic.PlayMusicIndex(files[63])
        continue
    if key_value == b'$':                #36
        #PlayMusic.PlayMusicIndex(files[61])
        continue
    if key_value == b'%':                #37
        PlayMusic.PlayMusicIndex(files[57])
        continue
    if key_value == b'&':                #38
        PlayMusic.PlayMusicIndex(files[53])
        continue
    if key_value == b"'":                #39
        PlayMusic.PlayMusicIndex(files[49])
        continue
    if key_value == b'(':                #40
        PlayMusic.PlayMusicIndex(files[35])
        continue
    if key_value == b')':                #41
        PlayMusic.PlayMusicIndex(files[39])
        continue
    if key_value == b'*':                #42
        PlayMusic.PlayMusicIndex(files[43])
        continue
    if key_value == b'+':                #43
        PlayMusic.PlayMusicIndex(files[47])
        continue
    if key_value == b',':                #44
        PlayMusic.PlayMusicIndex(files[45])
        continue
    if key_value == b'-':                #45
        PlayMusic.PlayMusicIndex(files[41])
        continue
    if key_value == b'.':                #46
        PlayMusic.PlayMusicIndex(files[37])
        continue
    if key_value == b'/':                #47
        PlayMusic.PlayMusicIndex(files[33])
        continue
    if key_value == b'0':                #48
        PlayMusic.PlayMusicIndex(files[19])
        continue
    if key_value == b'1':                #49
        PlayMusic.PlayMusicIndex(files[23])
        continue
    if key_value == b'2':                #50
        PlayMusic.PlayMusicIndex(files[27])
        continue
    if key_value == b'3':                #51
        PlayMusic.PlayMusicIndex(files[31])
        continue
    if key_value == b'4':                #52
        PlayMusic.PlayMusicIndex(files[29])
        continue
    if key_value == b'5':                #53
        PlayMusic.PlayMusicIndex(files[25])
        continue
    if key_value == b'6':                #54
        PlayMusic.PlayMusicIndex(files[21])
        continue
    if key_value == b'7':                #55
        PlayMusic.PlayMusicIndex(files[17])
        continue
    if key_value == b'8':                #56
        PlayMusic.PlayMusicIndex(files[3])
        continue
    if key_value == b'9':                #57
        PlayMusic.PlayMusicIndex(files[7])
        continue
    if key_value == b':':                #58
        PlayMusic.PlayMusicIndex(files[11])
        continue
    if key_value == b';':                #59
        PlayMusic.PlayMusicIndex(files[15])
        continue
    if key_value == b'<':                #60
        PlayMusic.PlayMusicIndex(files[19])
        continue
    if key_value == b'=':                #61
        PlayMusic.PlayMusicIndex(files[9])
        continue
    if key_value == b'>':                #62
        PlayMusic.PlayMusicIndex(files[5])
        continue
    if key_value == b'?':                 #63
        PlayMusic.PlayMusicIndex(files[1])
        continue









'''
   if key_value == b'\x00':
        CiShu_Key[0] += 1
        print("第{}次接受到键值a0".format(CiShu_Key[0]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[0] - 1) * 63 + 0])

    if key_value == b'\x01':
        CiShu_Key[1] += 1
        print("第{}次接受到键值a0".format(CiShu_Key[1]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[0] - 1) * 63 + 1])

    if key_value == b'\x02':
        CiShu_Key[2] += 1
        print("第{}次接受到键值a1".format(CiShu_Key[2]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[2] - 1) * 63 + 2])

    if key_value == b'\x03':
        CiShu_Key[3] += 1
        print("第{}次接受到键值a24".format(CiShu_Key[3]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[3] - 1) * 63 + 3])

    if key_value == b'\x04':
        CiShu_Key[4] += 1
        print("第{}次接受到键值a14".format(CiShu_Key[4]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[4] - 1) * 63 + 4])

    if key_value == b'\x05':
        CiShu_Key[5] += 1
        print("第{}次接受到键值a43".format(CiShu_Key[5]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[5] - 1) * 63 + 5])

    if key_value == b'\x06':
        CiShu_Key[6] += 1
        print("第{}次接受到键值a33".format(CiShu_Key[6]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[6] - 1) * 63 + 6])

    if key_value == b'\x07':
        CiShu_Key[7] += 1
        print("第{}次接受到键值a23".format(CiShu_Key[7]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[7] - 1) * 63 + 7])

    if key_value == b'\x08':
        CiShu_Key[8] += 1
        print("第{}次接受到键值a13".format(CiShu_Key[8]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[8] - 1) * 63 + 8])

    if key_value == b'\t':
        CiShu_Key[9] += 1
        print("第{}次接受到键值a42".format(CiShu_Key[9]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[9] - 1) * 63 + 9])

    if key_value == b'\n':
        CiShu_Key[10] += 1
        print("第{}次接受到键值a32".format(CiShu_Key[10]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[10] - 1) * 63 + 10])

    if key_value == b'\x0b':
        CiShu_Key[11] += 1
        print("第{}次接受到键值a22".format(CiShu_Key[11]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[12] - 1) * 63 + 12])

    if key_value == b'\x0c':
        CiShu_Key[13] += 1
        print("第{}次接受到键值a12".format(CiShu_Key[13]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[13] - 1) * 63 + 13])

    if key_value == b'\r':
        CiShu_Key[14] += 1
        print("第{}次接受到键值a41".format(CiShu_Key[14]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[14] - 1) * 63 + 14])

    if key_value == b'\x0e':
        CiShu_Key[15] += 1
        print("第{}次接受到键值a31".format(CiShu_Key[15]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[15] - 1) * 63 + 15])

    if key_value == b'\x0f':
        CiShu_Key[16] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[16]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[16] - 1) * 63 + 16])

    if key_value == b'0':
        CiShu_Key[17] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[17]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[17] - 1) * 63 + 17])

    if key_value == b'1':
        CiShu_Key[18] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[18]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[18] - 1) * 63 + 18])

    if key_value == b'2':
        CiShu_Key[19] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[19]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[19] - 1) * 63 + 19])

    if key_value == b'3':
        CiShu_Key[20] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[20]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[20] - 1) * 63 + 20])

    if key_value == b'4':
        CiShu_Key[21] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[21]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[21] - 1) * 63 + 21])

    if key_value == b'\x05':
        CiShu_Key[22] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[22]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[22] - 1) * 63 + 22])

    if key_value == b'6':
        CiShu_Key[23] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[23]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[23] - 1) * 63 + 23])

    if key_value == b'7':
        CiShu_Key[24] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[24]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[24] - 1) * 63 + 24])

    if key_value == b'8':
        CiShu_Key[25] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[25]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[25] - 1) * 63 + 25])

    if key_value == b'9':
        CiShu_Key[26] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[26]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[26] - 1) * 63 + 26])

    if key_value == b':':
        CiShu_Key[27] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[27]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[27] - 1) * 63 + 27])

    if key_value == b';':
        CiShu_Key[28] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[28]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[28] - 1) * 63 + 28])

    if key_value == b'<':
        CiShu_Key[29] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[2]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[29] - 1) * 63 + 29])

    if key_value == b'=':
        CiShu_Key[30] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[30]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[30] - 1) * 63 + 30])

    if key_value == b'>':
        CiShu_Key[31] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[31]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[31] - 1) * 63 + 31])

    if key_value == b'?':
        CiShu_Key[32] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[32]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[32] - 1) * 63 + 32])

    if key_value == b'@':
        CiShu_Key[33] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[33]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[33] - 1) * 63 + 33])

    if key_value == b'A':
        CiShu_Key[34] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[34]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[34] - 1) * 63 + 34])

    if key_value == b'B':
        CiShu_Key[35] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[35]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[35] - 1) * 63 + 35])

    if key_value == b'C':
        CiShu_Key[36] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[36]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[36] - 1) * 63 + 36])

    if key_value == b'D':
        CiShu_Key[37] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[37]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[37] - 1) * 63 + 37])

    if key_value == b'E':
        CiShu_Key[38] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[38]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[38] - 1) * 63 + 38])

    if key_value == b'F':
        CiShu_Key[39] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[39]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[39] - 1) * 63 + 39])

    if key_value == b'G':
        CiShu_Key[40] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[40]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[40] - 1) * 63 + 40])

    if key_value == b'H':
        CiShu_Key[41] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[41]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[41] - 1) * 63 + 41])

    if key_value == b'I':
        CiShu_Key[42] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[42]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[42] - 1) * 63 + 42])

    if key_value == b'J':
        CiShu_Key[43] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[43]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[43] - 1) * 63 + 43])

    if key_value == b'K':
        CiShu_Key[44] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[44]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[44] - 1) * 63 + 44])

    if key_value == b'L':
        CiShu_Key[45] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[45]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[45] - 1) * 63 + 45])

    if key_value == b'M':
        CiShu_Key[46] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[46]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[46] - 1) * 63 + 46])

    if key_value == b'N':
        CiShu_Key[47] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[47]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[47] - 1) * 63 + 47])

    if key_value == b'O':
        CiShu_Key[48] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[48]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[48] - 1) * 63 + 48])

    if key_value == b'p':
        CiShu_Key[49] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[49]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[49] - 1) * 63 + 49])

        if key_value == b'q':
            CiShu_Key[50] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[50]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[50] - 1) * 63 + 50])

        if key_value == b'r':
            CiShu_Key[51] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[51]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[51] - 1) * 63 + 51])

        if key_value == b's':
            CiShu_Key[52] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[52]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[52] - 1) * 63 + 52])

        if key_value == b't':
            CiShu_Key[53] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[53]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[53] - 1) * 63 + 53])

        if key_value == b'u':
            CiShu_Key[54] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[54]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[54] - 1) * 63 + 54])

        if key_value == b'v':
            CiShu_Key[55] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[55]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[55] - 1) * 63 + 55])

        if key_value == b'w':
            CiShu_Key[56] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[56]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[56] - 1) * 63 + 56])

        if key_value == b'x':
            CiShu_Key[57] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[57]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[57] - 1) * 63 + 57])

        if key_value == b'y':
            CiShu_Key[58] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[58]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[58] - 1) * 63 + 58])

        if key_value == b'z':
            CiShu_Key[59] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[59]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[59] - 1) * 63 + 59])

        if key_value == b'{':
            CiShu_Key[60] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[60]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[60] - 1) * 63 + 60])

        if key_value == b'|':
            CiShu_Key[61] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[61]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[61] - 1) * 63 + 61])

        if key_value == b'}':
            CiShu_Key[62] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[62]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[62] - 1) * 63 + 62])

        if key_value == b'~':
            CiShu_Key[63] += 1
        print("第{}次接受到键值a21".format(CiShu_Key[63]))

        PlayMusic.PlayMusicIndex(files[(CiShu_Key[63] - 1) * 63 + 63])

        if key_value == b'\x7f':
            break
            
            '''
ser.close()  # 关闭串口



