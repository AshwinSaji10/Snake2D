#include<fstream.h>
#include<conio.h>
#include<graphics.h>
#include<process.h>
#include<stdio.h>
#include<stdlib.h>
#include<dos.h>
#include<math.h>
#include<string.h>
#include<time.h>
//Build version 1.44
//*->With circular queue
//*->12 Levels
//Bugs>>
//*->saved players are temporarily unavailable after you load the game
//several times from the mainmenu
int privatelvl,privatesize,lvl(int,int);char legend[20];
void start(),load(),searchplayer(),updateplayer(),pausemenu(),arrange();
void ascend(),descend(),deleteplayer(),credits(),finmsg(int);
void comp(),mainmenu(),levelhud(float,float,int),menuhud(),intro();
void enterloop(),escapeloop();int instruction(int,int);
class queue
{
	int front,rear;
	int size;
	public:
	int qi[150],qj[150];
	queue()
	{
		front=rear=-1;
		size=150;
	}
	void ins(int,int),del();
	void display();
	int check(int,int);
};
void queue::ins(int i,int j)
{
	if((front==0&&rear==size-1)||(rear==(front-1)%(size-1)))
	{
		cleardevice();
		outtextxy(0,0,"///not enough space");
		delay(1500);
		exit(0);
	}
	if(front==-1)
	{
		front=rear=0;//0 for front indicates that you
		//have inserted a value
		qi[rear]=i;
		qj[rear]=j;
	}
	else if(rear==size-1&&front!=0)
	{
		rear=0;
		qi[rear]=i;
		qj[rear]=j;
	}
	else
	{
		rear++;
		qi[rear]=i;
		qj[rear]=j;
	}
}
void queue::del()
{
	if(front==-1)
	{
		cleardevice();
		outtextxy(0,0,"empty queue");
		delay(1500);
		exit(0);
	}
	if(front==rear)//means you have deleted all the elements in the array
	//also means you have only 1 element left in the array
	{
		front=rear=-1;
	}
	else if(front==size-1)
	{
		front=0;
	}
	else
	{
		front++;
	}
}
void queue::display()
{
	if(front==-1)
	{
		cleardevice();
		outtextxy(0,0,"empty");
		delay(1500);
		exit(0);
	}
	if(rear>=front)
	{
		for(int i=rear;i>=front;i--)
		{
			circle(qi[i],qj[i],1);
		}
	}
	else
	{
		for(int i=rear;i>=0;i--)
		{
			circle(qi[i],qj[i],1);
		}
		for(i=size-1;i>=front;i--)
		{
			circle(qi[i],qj[i],1);
		}
	}
}
int queue::check(int i,int j)
{
	float a,b,d;
	if(rear>=front)
	{
		for(int w=front;w<=rear;w++)
		{
			a=qi[w]-i;b=qj[w]-j;
			a=pow(a,2);b=pow(b,2);
			d=sqrt(a+b);
			if(d<2)
			{
				return 1;
			}
		}
	}
	else
	{
		for(int w=0;w<=rear;w++)
		{
			a=qi[w]-i;b=qj[w]-j;
			a=pow(a,2);b=pow(b,2);
			d=sqrt(a+b);
			if(d<2)
			{
				return 1;
			}
		}
		for(w=front;w<size;w++)
		{
			a=qi[w]-i;b=qj[w]-j;
			a=pow(a,2);b=pow(b,2);
			d=sqrt(a+b);
			if(d<2)
			{
				return 1;
			}
		}
	}
	return 0;
}
class player
{
	int lvl,size;
	public:
	char name[20];
	player()
	{
		lvl=1;
		size=20;
	}
	void display(int),changelvl(),changesize(int),getname(),dispname();
	int retlvl(),retsize();
};
void player::display(int v)
{
	outtextxy(5,200+v,name);
	char string[10];//to use decimalplace(base 10)value system:itoa
	itoa(lvl,string,10);
	outtextxy(75,200+v,string);
}
void player::changelvl()
{
	lvl=lvl+1;
}
void player::changesize(int snk)
{
	size=snk;
}
void player::getname()
{
	gets(name);
}
int player::retlvl()
{
	return lvl;
}
int player::retsize()
{
	return size;
}
void player::dispname()
{
	outtextxy(5,240,name);
}
//end of class
void main()
{
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	intro();
	mainmenu();
	getch();
	closegraph();
}
void intro()
{
	outtextxy(200,210,"S   N   A   K   E        2   D");
	setcolor(CYAN);
	rectangle(450,240,190,190);
	rectangle(450+3,240+3,190-3,190-3);
	setcolor(WHITE);
	enterloop();
}
void enterloop()
{
	int c;
	outtextxy(210,460," press ENTER");
	while(1)
	{
		if(kbhit())
		{
			c=getch();
			if(c==13)
			{
				return;
			}
		}
	}
}
void escapeloop()
{
	int c;
	outtextxy(560,460,"press ESC");
	while(1)
	{
		if(kbhit())
		{
			c=getch();
			if(c==27)
			{
				return;
			}
		}
	}
}
/*LEVEL INSTRUCTIONS*/
int instruction(int l,int s)
{
	char var[10];
	char var2[10];
	strcpy(var2,"");
	itoa(l,var,10);
	cleardevice();
	setcolor(YELLOW);
	rectangle(410,180,210,150);
	setcolor(WHITE);
	outtextxy(220,160,"L  E  V  E  L");
	outtextxy(390,160,var);
	outtextxy(220,250,"FOOD NEEDED");
	outtextxy(220,460,"press ENTER");
	setfillstyle(SOLID_FILL,WHITE);
	for(int i=0;i<s;i++)
	{
		circle(350+(i*16),253,4);
		floodfill(350+(i*16),253,WHITE);
		if((i+1)>=10)
		{
		itoa((i+1)/10,var,10);//for displaying numbers below circle
		itoa((i+1)%10,var2,10);
		}
		else
		itoa(i+1,var,10);
		outtextxy(348+(i*16),259,var);
		outtextxy(348+(i*16),269,var2);
	}
	int c;
	while(1)
	{
		if(kbhit())
		{
			c=getch();
			if(c==13)
			{
				break;
			}
		}
	}
	return 0;
}
void menuhud()
{
	setfillstyle(LTSLASH_FILL,DARKGRAY);
	setcolor(CYAN);
	rectangle(130,320,0,170);
	rectangle(130,170,0,155);
	line(0,455,650,455);
	floodfill(1,180,CYAN);
	setcolor(WHITE);
}
void mainmenu()
{
	int c;
	while(1)
	{	cleardevice();
		menuhud();
		outtextxy(5,160,"MAINMENU");
		outtextxy(5,200,"StartGame[S]");
		outtextxy(5,220,"LoadGame[L]");
		outtextxy(5,240,"RemoveSaves[R]");
		outtextxy(5,260,"Credits[C]");
		outtextxy(5,280,"Quit[Q]");
		outtextxy(496,460,"Build version 1.44");
		setcolor(WHITE);
		if(kbhit())
		{
			c=getch();
			if(c=='s'||c=='S')
			{
				start();
			}
			else if(c=='l'||c=='L')
			{
				load();
			}
			else if(c=='r'||c=='R')
			{
				remove("save.dat");
				cleardevice();
				outtextxy(5,160,"REMOVED");
				delay(2500);
			}
			else if(c=='c'||c=='C')
			{
				credits();
			}
			else if(c=='q'||c=='Q')
			{
				cleardevice();
				exit(0);
			}
		}
		delay(100);
	}
}
void credits()
{
	cleardevice();
	int head=460,ch;
	while(1)
	{
		if(kbhit())
		{
			ch=getch();
			if(ch==27)
			{
				mainmenu();
			}
		}
		setcolor(CYAN);
		outtextxy(220,head,"S N A K E  2 D");
		setcolor(WHITE);
		if(head<=420)
		{
			outtextxy(220,head+40,"GAME DESIGNER");
			line(220,head+50,420,head+50);
		}
		if((head+50)<=450)
		{
			outtextxy(350,head+60,"Ashwin Saji");
		}
		outtextxy(560,460,"press ESC");
		delay(50);
		head--;
		cleardevice();
	}
}
void start()
{
	cleardevice();
	char word[20];
	strcpy(word,"");
//	^^^^^^^^^^^^^^^^^^so that '&' will not appear
	int count=0;char input,ch[2]="\0";
	while(1)
	{
		cleardevice();
		menuhud();
		outtextxy(5,160,"STARTGAME");
		outtextxy(5,200,"EnterPlayerName");
		outtextxy(5,460,"press ENTER");
		outtextxy(560,460,"press ESC");
		if(kbhit())
		{
			input=getch();
			if(input==27)
			{
				strcpy(word,"");
				return;
			}
			else if(input==8)
			{
				if(count>0)
				{
					word[count-1]='\0';
					count--;
				}
			}
			else if(count>5)
			{
				cleardevice();
				outtextxy(5,160,"WORDLIMIT EXCEEDED");
				delay(3000);
				return;
			}
			else if(input==13)
			{
				if(count==0)
				{
					cleardevice();
					outtextxy(5,160,"INVALID");
					delay(3000);
					return;
				}
				else
				{
					while(word[count-1]==' ')
					{
						word[count-1]='\0';
						count--;
					}
					ifstream f("save.dat",ios::binary);
					player temp;
					while(f.read((char*)&temp,sizeof(temp)))
					{
						if(strcmp(temp.name,word)==0)
						{
							cleardevice();
							outtextxy(5,160,"NAME ALREADY TAKEN");
							delay(3000);
							return;
						}
					}
					f.close();
					player a;
					strcpy(a.name,word);
					strcpy(legend,a.name);
					privatelvl=a.retlvl();
					privatesize=a.retsize();
					ofstream out("save.dat",ios::binary|ios::app);
					out.write((char*)&a,sizeof(a));
					out.close();
					strcpy(word,"");
					comp();
				}
			}
			else
			{
				ch[0]=input;
				strcat(word,ch);
				count++;
			}


		}
		outtextxy(5,240,word);
		delay(100);
	}
}
void load()
{
	cleardevice();
	char str[10];int c;
	ifstream f("save.dat",ios::binary);
	if(!f)
	{
		outtextxy(5,160,"SAVE FILE ABSENT");
		delay(3500);
		return;
	}
	while(1)
	{
		cleardevice();
		menuhud();
		outtextxy(5,160,"LOADGAME");
		outtextxy(5,200,"Display[D]");
		outtextxy(5,220,"SearchPlayer[S]");
		outtextxy(5,240,"UpdatePlayer[U]");
		outtextxy(5,260,"RemovePlayer[X]");
		outtextxy(5,280,"ArrangeFile[A]");
		outtextxy(560,460,"press ESC");
		if(kbhit())
		{
			c=getch();
			if(c==27)
			{
				return;
			}
			if(c=='s'||c=='S')
			{
				searchplayer();
			}
			else if(c=='u'||c=='U')
			{
				updateplayer();
			}
			else if(c=='d'||c=='D')
			{
				cleardevice();
				ifstream f("save.dat",ios::binary);
				player temp;
				int v=0;
				outtextxy(5,160,"DISPLAY");
				outtextxy(5,180,"NAME");
				outtextxy(60,180,"LEVEL");
				while(f.read((char*)&temp,sizeof(temp)))
				{
					temp.display(v);
					v=v+10;
				}
				f.close();
				setcolor(CYAN);
				rectangle(130,170,0,155);
				rectangle(130,240+v,0,170);
				setcolor(WHITE);
				escapeloop();
			}
			else if(c=='x'||c=='X')
			{
				deleteplayer();
			}
			else if(c=='a'||c=='A')
			{
				arrange();
			}
		}
		delay(100);
	}
}
void arrange()
{
	cleardevice();
	int input;
	menuhud();
	outtextxy(5,160,"ARRANGEFILE");
	outtextxy(5,200,"Ascending[A]");
	outtextxy(5,220,"Descending[D]");
	outtextxy(560,460,"press ESC");
	while(1)
	{
	if(kbhit())
	{
		input=getch();
		if(input=='a'||input=='A')
		{
			ascend();
			return;
		}
		else if(input=='d'||input=='D')
		{
			descend();
			return;
		}
		else if(input==27)
		{
			return;
		}
	}
	}
}
void ascend()
{
	player a[20],temp;
	int i=0,n,min;
	int filepointer=0;
	ifstream f("save.dat",ios::binary);
	ofstream j("temp.dat",ios::binary|ios::app);
	while(f.read((char*)&temp,sizeof(temp)))
	{
		filepointer++;
		a[i]=temp;
		i++;
	}
	n=filepointer;
	for(i=0;i<n-1;i++)
	{
		min=i;
		for(int j=i+1;j<n;j++)
		{
			if((strcmp(a[j].name,a[min].name))<0)
			{
				min=j;
			}
		}
		temp=a[min];
		a[min]=a[i];
		a[i]=temp;
	}
	for(i=0;i<n;i++)
	{
		temp=a[i];
		j.write((char*)&temp,sizeof(temp));
	}
	f.close();
	j.close();
	remove("save.dat");
	rename("temp.dat","save.dat");
}
void descend()
{
	player a[20],temp;
	int i=0,n,min;
	int filepointer=0;
	ifstream f("save.dat",ios::binary);
	ofstream j("temp.dat",ios::binary|ios::app);
	while(f.read((char*)&temp,sizeof(temp)))
	{
		filepointer++;
		a[i]=temp;
		i++;
	}
	n=filepointer;
	for(i=0;i<n-1;i++)
	{
		min=i;
		for(int j=i+1;j<n;j++)
		{
			if((strcmp(a[j].name,a[min].name))>0)
			{
				min=j;
			}
		}
		temp=a[min];
		a[min]=a[i];
		a[i]=temp;
	}
	for(i=0;i<n;i++)
	{
		temp=a[i];
		j.write((char*)&temp,sizeof(temp));
	}
	f.close();
	j.close();
	remove("save.dat");
	rename("temp.dat","save.dat");
}
void searchplayer()
{
	cleardevice();
	char ch[2]="\0",word[10],entering;
	int yes=0,count=0;
	strcpy(word,"");
	while(1)
	{
		cleardevice();
		menuhud();
		outtextxy(5,160,"SEARCHPLAYER");
		outtextxy(5,220,"EnterPlayerName");
		outtextxy(5,460,"press ENTER");
		outtextxy(560,460,"press ESC");
		if(kbhit())
		{
			entering=getch();
			if(entering==27)
			{
				return;
			}
			else if(entering==8)
			{
				if(count>0)
				{
					word[count-1]='\0';
					count--;
				}
			}
			else if(count>5)
			{
				cleardevice();
				outtextxy(5,160,"WORDLIMIT EXCEEDED");
				delay(3000);
				return;
			}
			else if(entering==13)
			{
				if(count==0)
				{
					cleardevice();
					outtextxy(5,160,"INVALID");
					delay(3000);
					return;
				}
				else
				{
					ifstream f("save.dat",ios::binary);
					player temp;
					while(f.read((char*)&temp,sizeof(temp)))
					{
						if(strcmp(temp.name,word)==0)
						{
						strcpy(legend,temp.name);
						privatelvl=temp.retlvl();
						privatesize=temp.retsize();
						comp();yes=1;
						mainmenu();
						}
					}
					f.close();
					if(yes==0)
					{
					cleardevice();
					outtextxy(5,160,"NOT FOUND");
					delay(2500);
					return;
					}
				}
			}
			else
			{
				ch[0]=entering;
				strcat(word,ch);
				count++;
			}
		}
		outtextxy(5,240,word);
		delay(100);
	}
}
void updateplayer()
{
	cleardevice();
	char ch[2]="\0",word[20],entering;
	strcpy(word,"");
	int count=0,filepointer=0,yes=0;
	while(1)
	{
		cleardevice();
		menuhud();
		outtextxy(5,160,"UPDATEPLAYER");
		outtextxy(5,220,"EnterPlayerName");
		outtextxy(5,460,"press ENTER");
		outtextxy(560,460,"press ESC");
		if(kbhit())
		{
		entering=getch();
		if(entering==27)
		{
			return;
		}
		else if(entering==8)
		{
			if(count>0)
			{
			word[count-1]='\0';
			count--;
			}
		}
		else if(count>5)
		{
			cleardevice();
			outtextxy(5,160,"WORDLIMIT EXCEEDED");
			delay(2500);
			return;
		}
		else if(entering==13)
		{
			ifstream f("save.dat",ios::binary);
			player temp;
			while(f.read((char*)&temp,sizeof(temp)))
			{
			filepointer++;
			if(strcmp(temp.name,word)==0)
			{
				count=0;
				cleardevice();
				char mon[10];
				strcpy(mon,"");
				char res[2]="\0";
				while(1){
				cleardevice();
				menuhud();
				outtextxy(5,160,"UPDATEPLAYER");
				outtextxy(5,220,"EnterNewName");
				outtextxy(5,460,"press ENTER");
				outtextxy(560,460,"press ESC");
				if(kbhit())
				{
					entering=getch();
					if(entering==27)
					{
						return;
					}
					else if(entering==8)
					{
						if(count>0)
						{
						mon[count-1]='\0';
						count--;
						}
					}
					else if(count>5)
					{
						cleardevice();
						outtextxy(5,160,"WORDLIMIT EXCEEDED");
						delay(2500);
						return;
					}
					else if(entering==13)
					{
						if(count==0)
						{
						cleardevice();
						outtextxy(5,160,"INVALID");
						delay(3000);
						return;
						}
						else
						{
						while(mon[count-1]==' ')
						{
						mon[count-1]='\0';
						count--;
						}
						ifstream f("save.dat",ios::binary);
						player temp;
						while(f.read((char*)&temp,sizeof(temp)))
						{
						if(strcmp(temp.name,mon)==0)
						{
							cleardevice();
							outtextxy(5,160,"NAME ALREADY TAKEN");
							delay(3000);
							return;
						}
						}
						f.close();
						ofstream out("save.dat",ios::binary|ios::ate);
						out.seekp((filepointer-1)*sizeof(temp),ios::beg);
						strcpy(temp.name,mon);
						out.write((char*)&temp,sizeof(temp));
						out.close();
						return;
						}
						}
						else
						{
						res[0]=entering;
						strcat(mon,res);
						count++;
						}
						}
						outtextxy(5,240,mon);
						delay(100);
					}
					}
					}
					f.close();
					if(yes==0)
					{
						cleardevice();
						outtextxy(5,160,"NOT FOUND");
						delay(2500);
						return;
					}
			}
			else
			{
				ch[0]=entering;
				strcat(word,ch);
				count++;
			}
		}
		outtextxy(5,240,word);
		delay(100);
	}

}
void deleteplayer()
{
	cleardevice();
	char ch[2]="\0",word[10],entering;
	int count=0;
	strcpy(word,"");
	while(1)
	{
		cleardevice();
		menuhud();
		outtextxy(5,160,"REMOVEPLAYER");
		outtextxy(5,220,"EnterPlayerName");
		outtextxy(5,460,"press ENTER");
		outtextxy(560,460,"press ESC");
		if(kbhit())
		{
			entering=getch();
			if(entering==27)
			{
				return;
			}
			else if(entering==8)
			{
				if(count>0)
				{
					word[count-1]='\0';
					count--;
				}
			}
			else if(count>5)
			{
				cleardevice();
				outtextxy(5,160,"WORDLIMIT EXCEEDED");
				delay(2500);
				return;
			}
			else if(entering==13)
			{
				if(count==0)
				{
					cleardevice();
					outtextxy(5,160,"INVALID");
					delay(3000);
					return;
				}
				else
				{
					ifstream f("save.dat",ios::binary);
					player temp;int count=0;
					ofstream out("temp.dat",ios::binary);
					while(f.read((char*)&temp,sizeof(temp)))
					{
					if(strcmp(temp.name,word)!=0)
					{
					out.write((char*)&temp,sizeof(temp));
					}
					else
					count=1;
					}
					f.close();
					out.close();
					remove("save.dat");
					rename("temp.dat","save.dat");
					cleardevice();
					if(count==1)
					outtextxy(5,160,"REMOVED");
					else
					outtextxy(5,160,"NOT FOUND");
					delay(2500);
					return;
				}
			}
			else
			{
				ch[0]=entering;
				strcat(word,ch);
				count++;
			}
		}
		outtextxy(5,240,word);
		delay(100);
	}
}
void comp()
{
	int h;
	switch(privatelvl)
	{
		case 1:
		h=lvl(1,3);
		break;
		case 2:
		h=lvl(2,4);
		break;
		case 3:
		h=lvl(3,5);
		break;
		case 4:
		h=lvl(4,6);
		break;
		case 5:
		h=lvl(5,7);
		break;
		case 6:
		h=lvl(6,8);
		break;
		case 7:
		h=lvl(7,9);
		break;
		case 8:
		h=lvl(8,10);
		break;
		case 9:
		h=lvl(9,11);
		break;
		case 10:
		h=lvl(10,12);
		break;
		case 11:
		h=lvl(11,13);
		break;
		case 12:
		h=lvl(12,14);
		if(h==1)
		{
			credits();
		}
		break;
		default:mainmenu();//since it cannot return
	}
	if(h==1)//means level has passed
	{
		comp();
	}
	else
	{
		mainmenu();
	}
}
void levelhud(float score,float time,int lev)
{
	char scorechar[10],timec[10],lchar[10];//itoa decimal system
	outtextxy(490,460,"press ESC to pause");
	itoa(score,scorechar,10);
	itoa(60-(time/1000),timec,10);
	itoa(lev,lchar,10);
	outtextxy(2,30,"Score:");
	outtextxy(52,30,scorechar);
	setcolor(WHITE);
	outtextxy(500,5,"L E V E L");
	outtextxy(590,5,lchar);
	outtextxy(2,2,"TIME");
	setfillstyle(LTBKSLASH_FILL,LIGHTGRAY);
	rectangle(95-(time/1000),10,35,0);
	rectangle(95,10,0,0);
	floodfill(36,5,WHITE);
	setcolor(LIGHTGRAY);
	/*bx0*/rectangle(380,280,70,100);
	outtextxy(70,90,"NormalBOX");
	/*bx1*/rectangle(600,410,420,100);
	outtextxy(420,90,"InvertedBOX");
	setcolor(LIGHTCYAN);
	/*bx3*/rectangle(380,410,235,320);
	rectangle(383,413,232,317);
	outtextxy(232,307,"SlowBOX");
}
void pausemenu(int e,int f,int g,int h)
{
	outtextxy(5,0,"press Q to quit");
	outtextxy(5,460,"press M to goto mainmenu");
	outtextxy(480,460,"press ESC to resume");
	outtextxy(270,30,"P A U S E D");
	setfillstyle(SLASH_FILL,DARKGRAY);
	/*bx0*/rectangle(380,280,70,100);
	/*bx1*/rectangle(600,410,420,100);
	/*bx2*/rectangle(e,f,g,h);
	rectangle(e+3,f+3,g-3,h-3);
	/*bx3*/rectangle(380,410,235,320);
	rectangle(383,413,232,317);
	floodfill(71,101,WHITE);
	floodfill(421,101,WHITE);
	floodfill(g+1,h+1,WHITE);
	floodfill(236,321,WHITE);
}
int lvl(int lev,int s)
{
	instruction(lev,s);cleardevice();delay(200);
	int c=0,x=2,y=0,counting=0,change=0,filepointer=0;
	int box=0,nbox,fbox=0;
	float a=0,b=0,d=0,score=0,p,q,time=0,stime=0,delayc=40;
	int l,m,n,o,i=280,j=180,no,r=0,px=130,py=250,dir=2,ts=0,spcount=0;
	int snk=privatesize,prevsnk=privatesize;
	int e=215,f=410,g=70,h=320;
	randomize();
	p=rand()%380;
	q=rand()%280;
	a=130-p;b=250-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
	while(p<=72||q<=102||p>=378||q>=278&&d<=20)
	{
		p=rand() % 380;
		q=rand() % 280;
		a=130-p;b=250-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
	}
	circle(p,q,3);
	queue cq;
	while(1)
	{
		if((fbox!=box)&&spcount<s-3&&lev>2)
		{
			outtextxy(5,460,"Teleport[1]");
		}
		if(score>=s)
		{
			setcolor(LIGHTGREEN);
			outtextxy(2,50,"Required Score Reached");
		}
		setcolor(WHITE);
		levelhud(score,time,lev);
		setcolor(YELLOW);
		//setfillstyle(CLOSE_DOT_FILL,LIGHTGRAY);
		/*bx2*/rectangle(e,f,g,h);
		rectangle(e+3,f+3,g-3,h-3);
		outtextxy(g-3,h-3-10,"ShrinkingBOX");
		//floodfill(g-2,h-2,YELLOW);
		setcolor(WHITE);
		circle(i,j,1);
		if(counting<snk&&counting>0)
		{
			cq.display();
		}
		else if(counting>=snk&&counting>0)
		{
			if(snk>prevsnk)//snake increases size during runtime
			prevsnk++;
			else
			cq.del();
			cq.display();
		}
		if(cq.check(i,j)==1&&counting>0)
		{
			cq.display();
			outtextxy(250,20,"YOU ATE YOURSELF!");
			delay(3500);
			return 0;
		}
		cq.ins(i,j);
		//do not insert new elements before displaying the body
		//as it would check the same body part
		setcolor(WHITE);
		if((time/1000)-stime>=ts)
		{
			setcolor(LIGHTGRAY);
			circle(px,py,10);//display portal
			setcolor(WHITE);
			circle(px,py,r);
			setfillstyle(WIDE_DOT_FILL,WHITE);
			floodfill(px,py,WHITE);
			if(r>10)r=1;r++;
			a=px-i;b=py-j;d=sqrt(pow(a,2)+pow(b,2));
			if(d<10)//start checking portal
			{
				if(box==2||box==3)
				{
					stime=time/1000;
					if(fbox==0)
					{
						i=130;j=250;
						px=130;py=250;
						box=0;
						dir=2;
						ts=2;
						delayc=40;
					}
					else
					{
						i=550;j=320;
						px=550;py=320;
						box=1;
						dir=-2;
						ts=2;
						delayc=40;
					}
				}
				else
				{
					randomize();
					nbox=rand () % 4;
					while(nbox==box)
					{
						nbox=rand () % 8;
					}
					stime=time/1000;
					if(nbox==0)
					{
						i=130;j=250;
						px=130;py=250;
						box=0;
						dir=2;
						ts=2;
						delayc=40;
					}
					else if(nbox==1)
					{
						i=550;j=320;
						px=550;py=320;
						box=1;
						dir=-2;
						ts=2;
						delayc=40;
					}
					else if(nbox==2||nbox==4||nbox==6||nbox==7)
					{
						i=142;j=365;
						px=142;py=365;
						box=2;
						dir=2;
						ts=4;
						delayc=40;
					}
					else
					{
						i=308;j=365;
						px=308;py=365;
						box=3;
						dir=2;
						ts=4;
						delayc=160;
					}
				}
			}
		}
		if(box==0||box==1||box==3)
		{
			e=215;f=410;g=70;h=320;
			if(box==0)
			{
				l=380;m=280;n=70;o=100;
			}
			else if(box==1)
			{
				l=600;m=410;n=420;o=100;
			}
			else
			{
				l=380;m=410;n=235;o=320;
			}
			if(i>=l)
			{
				i=n+2;
			}
			else if(i<=n)
			{
				i=l-2;
			}
			else if(j>=m)
			{
				j=o+2;
			}
			else if(j<=o)
			{
				j=m-2;
			}
		}
		else if(box==2)
		{
			if(counting%10==0)
			{
				e--;f--;g++;h++;
			}
			l=e;m=f;n=g;o=h;
			if(i>=l||i<=n||j>=m||j<=o)
			{
				circle(i,j,1);
				cq.display();
				outtextxy(250,20,"YOU ARE DEAD!");
				delay(3000);
				cleardevice();
				return 0;
			}
		}
		if(kbhit())
		{
			c=getch();
			if(c==72)//up
			{       if(y)
				{
				}
				else
				{
				x=0;
				y=-dir;
				}
			}
			else if(c==75)//left
			{       if(x)
				{
				}
				else
				{
				x=-dir;
				y=0;
				}
			}
			else if(c==77)//right
			{       if(x){
				}
				else{
				x=dir;
				y=0;
				}
			}
			else if(c==80)//down
			{       if(y){
				}
				else{
				x=0;
				y=dir;
				}
			}
			else if(c=='1'&&spcount<s-3&&lev>2)
			{
				if(!(fbox==box))
				{
					stime=time/1000;
					if(fbox==0)
					{

						i=130;j=250;
						px=130;py=250;
						box=0;
						dir=2;
						ts=2;
						delayc=40;
					}
					else
					{
						i=550;j=320;
						px=550;py=320;
						box=1;
						dir=-2;
						ts=2;
						delayc=40;
					}
					spcount++;
				}
			}
			else if(c==27)
			{
				char ch;
				cleardevice();
				circle(i,j,1);
				setfillstyle(SOLID_FILL,WHITE);
				circle(p,q,3);
				floodfill(p,q,WHITE);
				if((time/1000)-stime>=ts)
				{
					circle(px,py,r);
				}
				pausemenu(e,f,g,h);
				cq.display();
				while(1)
				{
					if(kbhit())
					{
						c=getch();
						if(c=='q'||c=='Q')
						{
							cleardevice();
							exit(0);
						}
						else if(c=='m'||c=='M')
						{
							return 0;
						}
						else if(c==27)
						{
							break;
						}
					}
				}
			}
		}
		a=p-i;b=q-j;
		a=pow(a,2);b=pow(b,2);
		d=sqrt(a+b);
		if(d<=4)
		{
			score++;
			snk++;
			randomize();
			if(fbox==0)
			{
				p=rand() % 600;q=rand()% 410;
				fbox=1;
				a=550-p;b=320-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
				while(p<=422||q<=102||p>=598||q>=408||d<=13)//don’t use &&
				{
					p=rand() % 600;
					q=rand() % 410;
					a=550-p;b=320-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
				}

			}
			else if(fbox==1)
			{
				p=rand() % 380;q=rand()% 280;
				fbox=0;
				a=130-p;b=250-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
				while(p<=72||q<=102||p>=378||q>=278||d<=13)
				{
					p=rand() % 380;
					q=rand() % 280;
					a=130-p;b=250-q;a=pow(a,2);b=pow(b,2);d=sqrt(a+b);
				}
			}
		}
		else
		{
			setcolor(WHITE);
			setfillstyle(SOLID_FILL,WHITE);
			circle(p,q,3);
			floodfill(p,q,WHITE);
			setcolor(WHITE);
		}
		i=i+x;j=j+y;
		delay(delayc);time=time+delayc;
		if((time/1000)>=59)
		{
			if(score>=s)
			{
				if(lev==12)
				{
					cleardevice();
					setcolor(LIGHTGREEN);
					outtextxy(220,160,"L E V E L   P A S S E D");
					finmsg(score);
					delay(200);
					return 1;
				}
				ifstream inf("save.dat",ios::binary);
				player temp;
				while(inf.read((char*)&temp,sizeof(temp))&&change==0)
				{
					filepointer++;
					if(strcmpi(legend,temp.name)==0)
					{
						temp.changelvl();
						privatelvl=temp.retlvl();
						temp.changesize(snk);
						privatesize=temp.retsize();
						ofstream out("save.dat",ios::binary|ios::ate);
						out.seekp((filepointer-1)*sizeof(temp),ios::beg);
						out.write((char*)&temp,sizeof(temp));
						out.close();
						change=1;
					}
				}
				inf.close();
				cleardevice();
				setcolor(LIGHTGREEN);
				outtextxy(220,160,"L E V E L   P A S S E D");
				finmsg(score);
				cleardevice();
				delay(200);
				return 1;
			}
			else
			{
				cleardevice();
				setcolor(LIGHTRED);
				outtextxy(220,160,"L E V E L   F A I L E D");
				finmsg(score);
				cleardevice();
				delay(200);
				return 0;
			}
		}
		cleardevice();
		counting++;
	}
}
void finmsg(int score)
{
	char scorechar[10];
	itoa(score,scorechar,10);
	rectangle(410,180,210,150);
	setcolor(WHITE);
	outtextxy(220,250,"YOUR SCORE");
	outtextxy(350,250,scorechar);
	enterloop();
}
