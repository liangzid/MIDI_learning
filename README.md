# 这个什么东西？
这是一个和嵌入式平台进行交互的音乐系统，目前仅仅是刚刚起步，待到**2018年12月18日**应该可以结束。

目前利用**UART**来实现交互，Linux与windows均支持。但是**需要在电脑上配备python解释器**（因为我只准备拿他给自己玩 = =！）
# 使用它需要如何安装配置环境？
主要包括：
1）配置python3
2）配置串行通讯库pyserial
3）配置midi读写库python-midi

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
