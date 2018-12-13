from pydub import AudioSegment as ause
from pydub.silence import split_on_silence as sos


class MusicCuts:
    def __init__(self,path,using_cut_nums=True,cut_num=10,cut_time=None):
        song=ause.from_mp3(path)
        self.song=song
        if using_cut_nums:
            self.cut_num=cut_num
            print(song.get_frame())








