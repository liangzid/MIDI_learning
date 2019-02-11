# 这个什么东西？
这是一个和嵌入式平台进行交互的音乐系统，已经完成。

目前利用**UART**来实现交互，Linux与windows均支持。但是**需要在电脑上配备python解释器**（因为我只准备拿他给自己玩 = =！）
# 使用它需要如何安装配置环境？
主要包括：
1）配置python3
2）配置串行通讯库pyserial
3）配置midi读写库python-midi
4）配置音乐文件剪裁库pydub
5）配置音乐文件播放库


## 配置python3
可以直接去官网下载，当然也可以使用anaconda。这个不需要说明

## pyserial
```shell
conda install pyserial
```

## python-midi
```shell
#首先下载代码，可以考虑： git clone https://github.com/vishnubob/python-midi.git
#cd他或者下载后走进下载文件夹。
conda install ipykernel
pip install git+https://github.com/vishnubob/python-midi@feature/python3
#最后一条指令可能要多次运行直到出现sucesefull install的提示为止
#如果是python2，可以直接考虑运用GitHub上的提示进行。
```
reference：[Issuses](https://github.com/vishnubob/python-midi/issues/139)
## pydub
使用这个函数库来实现对音乐的剪裁处理。安装前需要先安装python对mp3的支持库FFmpeg
### 安装FFmpeg
详细安装指导请参阅FFmpeg安装[官网](https://github.com/adaptlearning/adapt_authoring/wiki/Installing-FFmpeg)
上面提供了对所有的系统如何安装该依赖的指南。Ubuntu系统由于太简单就不说了。下面给出如何在Windows系统下如何安装
该程序的意见：
[参考](https://www.wikihow.com/Install-FFmpeg-on-Windows)
1. 下载[压缩包](https://ffmpeg.zeranoe.com/builds/win64/static/ffmpeg-20181211-876ed08-win64-static.zip)
网速有点慢请耐心等待
2.设置环境变量
### 安装pydub
```
pip install pudub
```
## 安装pygame
```
pip install pygame
```
### 安装librosa
```
pip install librosa
```
# 配置单片机端
硬件部分：单片机端包含两个硬件环节，一个是使用IAP15F2K61S2来控制按键和灯光的部分，另一个是使用STC12C来控制光立方和AD转换
的部分。这两个部分的设计原理图和PCB版图局部可见项目文件中。
# 声明
啊，想到了再写。




