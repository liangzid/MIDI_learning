from pydub import AudioSegment as ause
from pydub.silence import split_on_silence as sos


class MusicCuts:
    def __init__(self,path,using_cut_nums=True,cut_num=10,cut_time=None):
        '''
        :param path:需要剪裁的音乐文件的路径
        :param using_cut_nums: 是否使用剪裁的片段的数量来进行剪裁
        :param cut_num: 剪裁成片段的数量
        :param cut_time: 剪裁成每个片段的时间
        '''
        song=ause.from_mp3(path)
        self._song=song
        if using_cut_nums:
            self._cut_num=cut_num
            print(song.get_frame())
        self._cut_time=cut_time

    def get_song(self):
        return self.song

    def __getattr__(self, item):
        return self.item








        return lists








