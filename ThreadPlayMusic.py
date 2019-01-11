import PlayMusic
import threading
import CutMusic
import time

class ThreadPlayMusic(threading.Thread):

    def __init__(self,thread_ID,name,filename,sleeptime):
        threading.Thread.__init__(self)
        print("线程{0}已被创立。线程名称为 {1} 。睡眠时间为{2}".format(thread_ID,name,sleeptime))
        self.thread_ID=thread_ID
        self.name=name
        self.filename=filename
        self.sleeptime=sleeptime

    def run(self):
        time.sleep(self.sleeptime)
        print("线程"+self.name+"已打开")
        PlayMusic.PlayMusicIndex(self.filename)
        print("线程"+self.name+"已退出")