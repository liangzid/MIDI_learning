#切割音乐

from pydub import AudioSegment as ause
from pydub.silence import split_on_silence as sos
from pydub.playback import play
import time
import pygame
import librosa

def CutMusic(from_path, to_dir, filename, cut_num=20):
    song = ause.from_mp3(from_path)
    length_of_song = len(song)
    per_long = int(length_of_song / cut_num)

    # 截取片段的部分
    song_PianDuan_s = [[]]
    # print(length_of_song)
    for i in range(cut_num):
        if i != cut_num - 1:
            # print(i, '\n', i * per_long, '\n', (i + 1) * per_long)
            song_PianDuan_s[i] = song[i * per_long: (i + 1) * per_long+5]
            song_PianDuan_s.append([])
        else:
            song_PianDuan_s[i] = song[i * per_long:]
            print("裁剪完毕，已经实现了对{0}的裁剪，已经裁剪得到了{1}个片段！".format(from_path,cut_num))

    m = 1
    # path = 'D://desktop//'
    # filename = 'kaluli.mp3'

    split_files = []
    # 存储
    for x in song_PianDuan_s:
        split_files.append(to_dir + str(m) + '_' + filename)

        x.export(to_dir + str(m) + '_' + filename+'.mp3', format='mp3')
        print("成功储存{0}到{1}中!".format(filename+str(m), to_dir + str(m) + '_' + filename))
        m += 1
    print("音乐文件处理完毕！\n")

    '''
    split_files = []
    path = 'D://desktop//'
    filename = 'kaluli.mp3'
    for i in range(cut_num):
        split_files.append(path + str(i) + '_' + filename)
    '''