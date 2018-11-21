import os
import midi
import platform as pf
# 通过下面的代码可以生成一段新的MIDI文件
music=midi.Pattern()
track=midi.Track()
music.append(track)
on=midi.NoteOnEvent(tick=0,velocity=50,pitch=midi.G_3)
track.append(on)
off=midi.NoteOffEvent(tick=100,pitch=midi.G_3)
track.append(off)
eot=midi.EndOfTrackEvent(tick=1)
track.append(eot)
path="example.mid"
midi.write_midifile(path,music)
print(repr(music))

systerm=pf.system()
print(systerm)
if (systerm=="Windows"):
    os.system(path)
elif systerm=='Linux':
    os.system("./"+path)






