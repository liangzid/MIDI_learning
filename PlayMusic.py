import time
import pygame
import librosa
import ThreadPlayMusic

def PlayMusicDemo(split_files):
    '''
    split_files:一个列表，里面有所有的需要播放的文件
    '''
    pygame.mixer.init()
    for i in split_files:

        track=pygame.mixer.music.load(i)
        pygame.mixer.music.play()
        timeee = librosa.get_duration(filename=i)
        time.sleep(timeee)
        # print(len(i))
        # print('第{}段播放完毕！'.format(i))
    pygame.mixer.music.stop()

def PlayMusicMultiThreadings(split_files):
    ii=0
    sleeptime=0
    for i in split_files:
        ii+=1
        ThreadPlayMusic.ThreadPlayMusic(thread_ID=ii,name="打开 {} 的进程".format(i),filename=i,
                                        sleeptime=sleeptime).start()
        sleeptime+=GetMusicTime(i)-0.205
    print("退出主线程")


def getSplitFiles(from_path,filename,cut_num):
    split_files = []
    for i in range(cut_num):
        split_files.append(from_path + str(i+1) + '_' + filename+'.mp3')
    return split_files

def PlayMusicIndex(i):
    pygame.mixer.init()
    track=pygame.mixer.music.load(i)
    pygame.mixer.music.play()
    timee=librosa.get_duration(filename=i)
    time.sleep(timee)
    pygame.mixer.stop()

def GetMusicTime(filename):
    return librosa.get_duration(filename=filename)

