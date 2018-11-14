#include ��stdlib.h��
#include ��stdio.h��
#include ��io.h��
#include ��string.h��
#define C1 60 //C��1�ļ���ֵ
#define FOURPAINUM 64 //1/4��������
#define MIDICLOCK 24 //ÿ1/64������MIDICLOCK��
#define JumpNullChar(x) / //�������ַ�
   { /
	   while (*x == ' ' /
		   ||*x == '/t' /
		   ||*x == '/n' /
		   ||*x == '|') /
		   x++; /
   };
   enum ERRORCODE { //���������Ϣ
	   ChangeOK, //ת���ɹ�
	   TextFileNotOpen, //�ı��ļ����ܴ�
	   MidiFileCanNotCreate, //ָ����MIDI�ļ����ܽ���
	   TextFileToBig, //�ı��ļ�̫��
	   MallocError, //�ڴ�������
	   InvalideChar, //���ı��ļ��г����˷Ƿ��ַ�
	   NotFoundTrack, //û���ҵ�ָ���Ĵŵ���Ϣ   //�Ҳ�������
	   NotMIDITextFile, //�ı��ļ�����MIDI�ı��ļ�
   };
   void SWAP(char *x, char *y) //�����ݽ���
   {
	   char i;
	   i = *x;
	   *x = *y;
	   *y = i;
   }
   union LENGHT  //union��ͬ�壬����������б�����ͬռ��һ���ڴ�ռ䡣����������Ϊlength��Ϊ4λByte���������ִ���
   {
	   long length;
	   char b[4];
   };
   struct MH { //MIDI�ļ�ͷ
	   char MidiId[4]; //MIDI�ļ���־MThd
	   long length; //ͷ����Ϣ����
	   int format; //��ŵĸ�ʽ
	   int ntracks; //�ŵ���Ŀ
	   int PerPaiNum; //ÿ�ڼ�����ֵ  -----------------------------------------------------------------��������������       
   };
   struct TH //����ͷ
   {
	   char TrackId[4]; //�ŵ���־MTrk
	   long length; //��Ϣ����
   };
   class MIDI
   {
   public:
	   char ErrorMsg[100]; //������Ϣ
   private:
	   unsigned char *TextFileBuf,
		   *TextFileOldBuf;
	   unsigned char *MidiFileBuf,
		   *MidiFileOldBuf;
	   char OneVal; //ĳ��ʱ,1�ļ�ֵ
	   char PaiNum; //��һС�ڽ�������
	   char OnePaiToneNum; //�ü���������Ϊһ������
   public:
	   //����ȫMIDI�鶨��ʽ���ı��ļ�����MIDI�ļ�
	   int ChangeTextToMidi(char *TextFileName,
		   char *MidiFileName);
	   char *GetErrorMsg() //��ȡ������Ϣ
	   {
		   return(ErrorMsg);
	   }
   private:
	   char GetCurPaiSpeed(int n); //ȡ��ǰ�ĵİ���ǿ��
	   void WriteSoundSize(char ntrack, unsigned int);
	   void SetOnePaiToneNum(int n)
	   {
		   OnePaiToneNum = n;
	   };
	   void SetOneVal(char *m); //ȡm�����С��ʱ,1��ʵ�ʼ�ֵ
	   char GetToneNum(char c, //ȡ������Ӧ�ļ�ֵ
		   char flag);
	   void WriteMHToFile(long length, //����MIDI�ļ�ͷ
		   int format,
		   int ntracks,
		   int PerPaiNum,
		   FILE *fp);
	   void WriteTHToFile(long lenght,
		   FILE *fp); //����MIDI�ŵ�ͷ
	   void WriteTrackMsgToFile(FILE *fp);
	   //���ŵ�������Ϣ�����ļ���
	   void WriteSpeed(int speed);
	   void SetPaiNum(int n)
	   {
		   PaiNum = n;
	   }
	   long NewLong(long n); //�µ�longֵ
	   int NewInt(int n) //�µ�intֵ
	   {
		   return(n����8 | n����8);
	   }
	   //��n��Ϊ�ɱ䳤��,����buf��
	   void WriteLenghtToBuf(unsigned long n,
		   char *buf);
	   void ChangePrommgram(char channel, //������ɫ
		   char promgram);
	   void NoteOn(char n, //��������
		   char speed,
		   unsigned long delaytime);
	   void WriteNoteOn(char, char, char, unsigned long);
	   void WriteTextMsg(char *msg); //��һ���ı���Ϣ
	   void WriteTimeSignature(char n, //����ʱ����Ϣ
		   char d);
	   void WriteTrackEndMsg(); //���ôŵ�������Ϣ
   };


















   ����/**************************************************
	 ����/* ����:������MIDI�ı��ļ���text�ļ�ת����MIDI */
	   ����/* �ļ�. */
		 ����/* ��ڲ���:TextFileName �ı��ļ��� */
		   ����/* MidiFileName MIDI�ļ��� */
			 ����/* ���ڲ���:�� ERRORCODE ˵�� */
	   /*************************************************/
   int MIDI::ChangeTextToMidi(char *TextFileName,
	   char *MidiFileName)
   {
	   int tracks, ntrack, delaytime;
	   int speed, IsFirst, nn, dd;
	   unsigned char buf[80], *msgbuf, c;
	   FILE *TextFp, *MidiFp;
	   long FileSize;
	   char SpeedVal;
	   TextFp = fopen(TextFileName, "r");     //���ı��ļ�
	   if (TextFp == NULL)
	   {
		   sprintf(ErrorMsg,
			   "�ı��ļ�[%s]���ܴ򿪡�/n", TextFileName);
		   return(TextFileNotOpen);
	   }
	   fseek(TextFp, 0, SEEK_END); /*�����ļ���С*/
	   //����ftell����ȡ�ļ�����
	   FileSize = ftell(TextFp);
	   TextFileBuf = (char *)malloc(FileSize);/*Ϊ�ļ������ڴ�*/
	   //TextFileBuf��һ��ָ�룬ָ����һ����ô���ڴ�ռ��ָ��

	   //���ʵ���ϲ���Ҫ�ӣ����ǿ��Լ���
	   if (TextFileBuf == NULL)
	   {
		   sprintf(ErrorMsg,
			   "�ı��ļ�[%s]̫��û���㹻���ڴ洦��/n",
			   TextFileName);
		   fclose(TextFp);
		   return(TextFileToBig);
	   }

	   memset(TextFileBuf, 0, FileSize);    //memset��������ָ��TextFileBufָ���Filesize�����ֽ�ȫ��ȡֵλ0���൱�ڳ�ʼ��
	   MidiFileBuf = (char *)malloc(FileSize * 4);//   ��һ������û����
	   if (MidiFileBuf == NULL)
	   {
		   sprintf(ErrorMsg, "����ΪMIDI�ļ������ڴ档/n");
		   fclose(TextFp);
		   free(TextFileBuf);
		   return(MallocError);
	   }

	   MidiFp = fopen(MidiFileName, "wb");//��һ�����ǹؼ����������õ��ǿ���д��Ķ����ƴ򿪷�ʽ
	   
	   if (MidiFp == NULL)
	   {
		   sprintf(ErrorMsg,
			   "Midi�ļ�[%s]���ܽ�����/n", MidiFileName);
		   fclose(TextFp);
		   free(MidiFileBuf);
		   free(TextFileBuf);
		   return(MidiFileCanNotCreate);
	   }
	   
	   MidiFileOldBuf = MidiFileBuf;
	   TextFileOldBuf = TextFileBuf;
	   fseek(TextFp, 0, SEEK_SET);
	   fread(TextFileBuf, FileSize, 1, TextFp);//�� ��TextFp�еĳ���ΪFileSize�ֽڳ��ȵ����ݴ��͵�TextFileBuf��
	   fclose(TextFp);

	   //������һ�����ַ�
	   JumpNullChar(TextFileBuf);
	   //strnicmp()��������Ƚ�textfilebuf��ǰ�����ַ��롾MIDI���ģ�����ͬ����0�����ڷ���������С�ڷ��ظ���
	   c = strnicmp(TextFileBuf, "[MIDI]", 6);//�ж��Ƿ���MIDI�ļ���������ʵ�����Ͻ�
	   if (c)
	   {
		   sprintf(ErrorMsg,
			   "�ı��ļ�[%s]����MIDI�ı��ļ���/n", MidiFileName);
		   fcloseall();
		   free(TextFileOldBuf);
		   free(MidiFileOldBuf);
		   return(NotMIDITextFile);
	   }
	   TextFileBuf += 6;                         //��������������������������������������������������
	   JumpNullChar(TextFileBuf);
	   //sscanf�������Խ�ĳһ���ַ�����Ϊ����Ӷ��õ�һЩ������ֵ
	   sscanf(TextFileBuf, "%c,%d/%d,%d,%d", //ȡ���ŵ���Ϣ
		   c, nn, dd, speed, tracks);
	   buf[0] = c; buf[1] = 0; SetOneVal(buf); //���øõ�1�ļ�ֵ          ��������������������
	   if (nn��1 || nn�� 7) nn = 4;
	   if (dd��2 || dd��16) dd = 4;
	   while (*TextFileBuf != '/n') TextfileBuf++;
	   JumpNullChar(TextFileBuf);
	   if (speed��60 || speed ��200) speed = 120;
	   JumpNullChar(TextFileBuf);
	   if (tracks��1 || tracks��16) tracks = 1;
	   JumpNullChar(TextFileBuf);
	   ntrack = 1;
	   WriteMHToFile(6, 1, tracks, speed, MidiFp);
	   WriteTimeSignature(nn, dd); //����ʱ���¼��ʽ
	   SetPaiNum(nn);
	   WriteSpeed(speed); //���������ٶ�
	   while (ntrack�� = tracks * TextFileBuf != 0)
	   {
		   sprintf(buf, "[%d]", ntrack);
		   TextFileBuf = strstr(TextFileBuf, buf);//���Ҹôŵ���ʼλ��
		   if (TextFileBuf == NULL) //û���ҵ�
		   {
			   sprintf(ErrorMsg,
				   "���ļ�[%s]�У���%d�ŵ�������Ϣû�ҵ���/n.",
				   TextFileName, ntrack);
			   free(MidiFileOldBuf);
			   free(TextFileOldBuf);
			   fcloseall();
			   return(NotFoundTrack);
		   }
		   if (ntrack != 1) MidiFileBuf = MidiFileOldBuf;
		   SpeedVal = 0;
		   TextFileBuf += strlen(buf);
		   IsFirst = 1;
		   while (*TextFileBuf != 0 * TextFileBuf != '[')
		   {
			   JumpNullChar(TextFileBuf);
			   c = *(TextFileBuf++);
			   if ((c�� = '0' c�� = '7')
				   || (c�� = 'a' c�� = 'g')
				   || (c�� = 'A' c�� = 'G')
				   )
			   {
				   JumpNullChar(TextFileBuf);
				   if (*TextFileBuf == 'b' || *TextFileBuf == '#')
				   {
					   c = GetToneNum(c, *TextFileBuf);/*ȡ��ʵ�ʵ�����*/
					   TextFileBuf++;
					   JumpNullChar(TextFileBuf);
				   }
				   else c = GetToneNum(c, ' ');
				   switch (*(TextFileBuf++))
				   {
				   case '-': //�ӳ�һ��
					   delaytime = 2 * FOURPAINUM;
					   JumpNullChar(TextFileBuf);
					   while (*TextFileBuf == '-')
					   {
						   TextFileBuf++;
						   delaytime += FOURPAINUM;
						   JumpNullChar(TextFileBuf);
					   }
					   break;
				   case '_': //8������
					   delaytime = FOURPAINUM / 2;
					   JumpNullChar(TextFileBuf);
					   if (*TextFileBuf == '.')
					   {
						   TextFileBuf++;
						   delaytime = delaytime * 3 / 2;
					   }
					   break;
				   case '=': //16������
					   delaytime = FOURPAINUM / 4;
					   JumpNullChar(TextFileBuf);
					   if (*TextFileBuf == '.')
					   {
						   delaytime = delaytime * 3 / 2;
						   TextFileBuf++;
					   }
					   break;
				   case '.': //��������
					   delaytime = FOURPAINUM * 3 / 2;
					   break;
				   case ':': //32������
					   delaytime = FOURPAINUM / 16;
					   JumpNullChar(TextFileBuf);
					   if (*TextFileBuf == '.')
					   {
						   delaytime = delaytime * 3 / 2;
						   TextFileBuf++;
					   }
					   break;
				   case ';': //64������
					   delaytime = FOURPAINUM / 32;
					   if (*TextFileBuf == '.')
					   {
						   delaytime = delaytime * 3 / 2;
						   TextFileBuf++;
					   }
					   break;
				   default:
					   delaytime = FOURPAINUM;
					   TextFileBuf--;
					   break;
				   }

				   if (IsFirst)
				   {
					   WriteNoteOn(ntrack, c,
						   GetCurPaiSpeed(SpeedVal / (FOURPAINUM * 4 / dd) + 1),
						   delaytime);
					   IsFirst = 0;
				   }
				   else
					   NoteOn(c,
						   GetCurPaiSpeed(SpeedVal / (FOURPAINUM * 4 / dd) + 1),
						   delaytime);
				   SpeedVal = (SpeedVal + delaytime) //��һ���������Ľ���
					   % (PaiNum*FOURPAINUM * 4 / dd);
			   }
			   else
			   {
				   switch (c)
				   {
				   case 'S':
				   case 's':
				   case 'p':
				   case 'P': /*������ɫ*/
					   sscanf(TextFileBuf, "%d", IsFirst);
					   while (*TextFileBuf�� = '0' *TextFileBuf�� = '9')
						   TextFileBuf++;
					   if (c == 'P' || c == 'p') //��ΪP,��ʾ�ı���ɫ
						   ChangePrommgram(ntrack, (char)IsFirst);
					   else //����,��ʾ����������С
						   WriteSoundSize(ntrack, (unsigned int)IsFirst);
					   IsFirst = 1;
					   break;
				   case '{': /*д���*/
					   msgbuf = buf;
					   while (*TextFileBuf != '}'
						   *TextFileBuf != '/n'
						   *TextFileBuf != 0
						   * TextFileBuf != '[')
						   *(msgbuf++) = *(TextFileBuf++);
					   *msgbuf = 0;
					   IsFirst = 1;
					   WriteTextMsg(buf);
					   if (*TextFileBuf == '}') TextFileBuf++;
					   break;
				   case '//': //���˶�
					   OneVal -= 12;
					   break;
				   case '/': //���˶�
					   OneVal += 12;
					   break;
				   case '[':
				   case 0:
					   TextFileBuf--;
					   break;
				   default:
					   sprintf(ErrorMsg, "�ı��ļ�[%s]���ַǷ��ַ�(%c)��",
						   TextFileName, c);
					   free(MidiFileOldBuf);
					   free(TextFileOldBuf);
					   fcloseall();
					   return(InvalideChar);
				   }
			   }
		   }
		   WriteTrackEndMsg(); //���ôŵ�������Ϣ
		   WriteTrackMsgToFile(MidiFp); //���ŵ�������Ϣ�����ļ���
		   ntrack++;
	   }
	   free(MidiFileOldBuf);
	   free(TextFileOldBuf);
	   fclose(MidiFp);
	   sprintf(ErrorMsg, "MIDI�ļ�[%s]ת���ɹ���", MidiFileName);
	   return(ChangeOK);
   }

















   ��/*****************************************************/
	����/*���ã������������ݱ�ɿɱ䳤�ȣ�����buf�� */
	  ����/*��ڲ�����n ���� buf ��������� */
	/****************************************************/
   void MIDI::WriteLenghtToBuf(unsigned long n, char *buf)
   {
	   unsigned char b[4] = { 0 };
	   int i;
	   b[3] = (unsigned char)(n0x7f);
	   i = 2;
	   while (n����7)
	   {
		   n���� = 7;
		   b[i--] = (char)((n0x7f) | 0x80);
	   }
	   for (i = 0; i��4; i++)
		   if (b[i]) *(buf++) = b[i];
	   *buf = 0;
   }
   long MIDI::NewLong(long n) //�����������ݸĳɸ�λ��ǰ
   {
	   union LENGHT l = { 0 };
	   char i;
	   l.length = n;
	   SWAP(l.b[3], l.b[0]);
	   SWAP(l.b[2], l.b[1]);
	   return(l.length);
   }
   //��ʼ��������
   void MIDI::WriteNoteOn(char channel, //ͨ����
	   char note, //����ֵ
	   char speed, //�����ٶ�
	   unsigned long delaytime) //��ʱ��
   {
	   unsigned char buf[5];
	   int i;
	   channel--;
	   *(MidiFileBuf++) = 0;
	   *(MidiFileBuf++) = 0x90 | channel0x7f;//Write Channel
	   *(MidiFileBuf++) = note;
	   *(MidiFileBuf++) = speed;
	   WriteLenghtToBuf(delaytime*MIDICLOCK, buf);
	   i = 0;
	   while (buf[i]�� = 0x80) //Write Delay Time
		   *(MidiFileBuf++) = buf[i++];
	   *(MidiFileBuf++) = buf[i];
	   *(MidiFileBuf++) = note;
	   *(MidiFileBuf++) = 0;
   }
   void MIDI::NoteOn(char note,
	   char speed,
	   unsigned long delaytime) //����
   {
	   unsigned char buf[5];
	   int i;
	   *(MidiFileBuf++) = 0;
	   *(MidiFileBuf++) = note;
	   *(MidiFileBuf++) = speed;
	   WriteLenghtToBuf(delaytime*MIDICLOCK, buf);
	   i = 0;
	   while (buf[i]��0x80)
		   *(MidiFileBuf++) = buf[i++];
	   *(MidiFileBuf++) = buf[i];
	   *(MidiFileBuf++) = note;
	   *(MidiFileBuf++) = 0;
   }
   void MIDI::ChangePrommgram(char channel, char n) //�ı���ɫ
   {
	   *(MidiFileBuf++) = 0;
	   *(MidiFileBuf++) = 0xc0 | (channel - 1)0x7f;
	   *(MidiFileBuf++) = n;
   }
   void MIDI::WriteTextMsg(char *msg) //���ڴ�д��һ�ı���Ϣ
   {
	   char bufmsg[100] = { 0xff,5,0,0,0 };
	   int len;
	   *(MidiFileBuf++) = 0;
	   bufmsg[2] = (char)strlen(msg);
	   strcpy(bufmsg[3], msg);
	   strcpy(MidiFileBuf, bufmsg);
	   MidiFileBuf += strlen(bufmsg) + 3;
   }
   void MIDI::WriteTrackEndMsg() //�ŵ�������Ϣ
   {
	   *(MidiFileBuf++) = 0;
	   *(MidiFileBuf++) = 0xff;
	   *(MidiFileBuf++) = 0x2f;
	   *(MidiFileBuf++) = 0;
   }
   char MIDI::GetToneNum(char n, char flag)
	   ����/*��ڲ����� n ����
		 ���� flag �����Ǻ�
			��������ֵ�� ��������ʵ�ʱ��ֵ*/
   {
	   static char val[7] = { 9 ,11,0,2,4,5,7 };
	   static char one[7] = { 0,2,4,5,7,9,11 };
	   int i;
	   i = OneVal;
	   if (n�� = '7' n�� = '1') i = i + one[n - '1'];
	   else
		   if (n�� = 'a' n�� = 'g')
			   i = i + val[n - 'a'] - 12; //��������12������
		   else
			   if (n�� = 'A' n�� = 'G') //��������12������
				   i = i + val[n - 'A'] + 12;
			   else //����ʶΪ��ֹ��
				   i = 0;
	   if (flag == 'b') i--;
	   else if (flag == '#') i++;
	   return(i);
   }

   if (IsFirst)
   {
	   WriteNoteOn(ntrack, c,
		   GetCurPaiSpeed(SpeedVal / (FOURPAINUM * 4 / dd) + 1),
		   delaytime);
	   IsFirst = 0;
   }
   else
	   NoteOn(c,
		   GetCurPaiSpeed(SpeedVal / (FOURPAINUM * 4 / dd) + 1),
		   delaytime);
   SpeedVal = (SpeedVal + delaytime) //��һ���������Ľ���
	   % (PaiNum*FOURPAINUM * 4 / dd);
  }
  else
  {
	  switch (c)
	  {
	  case 'S':
	  case 's':
	  case 'p':
	  case 'P': /*������ɫ*/
		  sscanf(TextFileBuf, "%d", IsFirst);
		  while (*TextFileBuf�� = '0' *TextFileBuf�� = '9')
			  TextFileBuf++;
		  if (c == 'P' || c == 'p') //��ΪP,��ʾ�ı���ɫ
			  ChangePrommgram(ntrack, (char)IsFirst);
		  else //����,��ʾ����������С
			  WriteSoundSize(ntrack, (unsigned int)IsFirst);
		  IsFirst = 1;
		  break;
	  case '{': /*д���*/
		  msgbuf = buf;
		  while (*TextFileBuf != '}'
			  *TextFileBuf != '/n'
			  *TextFileBuf != 0
			  * TextFileBuf != '[')
			  *(msgbuf++) = *(TextFileBuf++);
		  *msgbuf = 0;
		  IsFirst = 1;
		  WriteTextMsg(buf);
		  if (*TextFileBuf == '}') TextFileBuf++;
		  break;
	  case '//': //���˶�
		  OneVal -= 12;
		  break;
	  case '/': //���˶�
		  OneVal += 12;
		  break;
	  case '[':
	  case 0:
		  TextFileBuf--;
		  break;
	  default:
		  sprintf(ErrorMsg, "�ı��ļ�[%s]���ַǷ��ַ�(%c)��",
			  TextFileName, c);
		  free(MidiFileOldBuf);
		  free(TextFileOldBuf);
		  fcloseall();
		  return(InvalideChar);
	  }
  }
  }
  WriteTrackEndMsg(); //���ôŵ�������Ϣ
  WriteTrackMsgToFile(MidiFp); //���ŵ�������Ϣ�����ļ���
  ntrack++;
  }
  free(MidiFileOldBuf);
  free(TextFileOldBuf);
  fclose(MidiFp);
  sprintf(ErrorMsg, "MIDI�ļ�[%s]ת���ɹ���", MidiFileName);
  return(ChangeOK);
  }
  ��/*****************************************************/
   ����/*���ã������������ݱ�ɿɱ䳤�ȣ�����buf�� */
	 ����/*��ڲ�����n ���� buf ��������� */
	/****************************************************/
  void MIDI::WriteLenghtToBuf(unsigned long n, char *buf)
  {
	  unsigned char b[4] = { 0 };
	  int i;
	  b[3] = (unsigned char)(n0x7f);
	  i = 2;
	  while (n����7)
	  {
		  n���� = 7;
		  b[i--] = (char)((n0x7f) | 0x80);
	  }
	  for (i = 0; i��4; i++)
		  if (b[i]) *(buf++) = b[i];
	  *buf = 0;
  }
  long MIDI::NewLong(long n) //�����������ݸĳɸ�λ��ǰ
  {
	  union LENGHT l = { 0 };
	  char i;
	  l.length = n;
	  SWAP(l.b[3], l.b[0]);
	  SWAP(l.b[2], l.b[1]);
	  return(l.length);
  }
  //��ʼ��������
  void MIDI::WriteNoteOn(char channel, //ͨ����
	  char note, //����ֵ
	  char speed, //�����ٶ�
	  unsigned long delaytime) //��ʱ��
  {
	  unsigned char buf[5];
	  int i;
	  channel--;
	  *(MidiFileBuf++) = 0;
	  *(MidiFileBuf++) = 0x90 | channel0x7f;//Write Channel
	  *(MidiFileBuf++) = note;
	  *(MidiFileBuf++) = speed;
	  WriteLenghtToBuf(delaytime*MIDICLOCK, buf);
	  i = 0;
	  while (buf[i]�� = 0x80) //Write Delay Time
		  *(MidiFileBuf++) = buf[i++];
	  *(MidiFileBuf++) = buf[i];
	  *(MidiFileBuf++) = note;
	  *(MidiFileBuf++) = 0;
  }
  void MIDI::NoteOn(char note,
	  char speed,
	  unsigned long delaytime) //����
  {
	  unsigned char buf[5];
	  int i;
	  *(MidiFileBuf++) = 0;
	  *(MidiFileBuf++) = note;
	  *(MidiFileBuf++) = speed;
	  WriteLenghtToBuf(delaytime*MIDICLOCK, buf);
	  i = 0;
	  while (buf[i]��0x80)
		  *(MidiFileBuf++) = buf[i++];
	  *(MidiFileBuf++) = buf[i];
	  *(MidiFileBuf++) = note;
	  *(MidiFileBuf++) = 0;
  }
  void MIDI::ChangePrommgram(char channel, char n) //�ı���ɫ
  {
	  *(MidiFileBuf++) = 0;
	  *(MidiFileBuf++) = 0xc0 | (channel - 1)0x7f;
	  *(MidiFileBuf++) = n;
  }
  void MIDI::WriteTextMsg(char *msg) //���ڴ�д��һ�ı���Ϣ
  {
	  char bufmsg[100] = { 0xff,5,0,0,0 };
	  int len;
	  *(MidiFileBuf++) = 0;
	  bufmsg[2] = (char)strlen(msg);
	  strcpy(bufmsg[3], msg);
	  strcpy(MidiFileBuf, bufmsg);
	  MidiFileBuf += strlen(bufmsg) + 3;
  }
  void MIDI::WriteTrackEndMsg() //�ŵ�������Ϣ
  {
	  *(MidiFileBuf++) = 0;
	  *(MidiFileBuf++) = 0xff;
	  *(MidiFileBuf++) = 0x2f;
	  *(MidiFileBuf++) = 0;
  }
  char MIDI::GetToneNum(char n, char flag)
	  ����/*��ڲ����� n ����
		���� flag �����Ǻ�
		   ��������ֵ�� ��������ʵ�ʱ��ֵ*/
  {
	  static char val[7] = { 9 ,11,0,2,4,5,7 };
	  static char one[7] = { 0,2,4,5,7,9,11 };
	  int i;
	  i = OneVal;
	  if (n�� = '7' n�� = '1') i = i + one[n - '1'];
	  else
		  if (n�� = 'a' n�� = 'g')
			  i = i + val[n - 'a'] - 12; //��������12������
		  else
			  if (n�� = 'A' n�� = 'G') //��������12������
				  i = i + val[n - 'A'] + 12;
			  else //����ʶΪ��ֹ��
				  i = 0;
	  if (flag == 'b') i--;
	  else if (flag == '#') i++;
	  return(i);
  }