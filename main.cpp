#include <iostream>
#include <fstream>
#include <graphics.h>
#include <cstring>
#include <vector>
using namespace std;
int color=COLOR(102,255,178),nod_color=YELLOW,fundal=WHITE,option_color=COLOR(128,128,255),text_color=YELLOW;
int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=10;
int colt_x=0,colt_y=50,lungime=1200,inaltime=600; /// zona unde pot fi puse blocuri
struct blocuri
{ int tip,nr;
  int x,y;
  int width=100,height=50;
  char var1[5]="?",op[5]="+",var2[5]="?";
  char text[101];
  blocuri(int a=0,int b=0,int c=0,int d=0,char s1[51]="")
  {tip=a;nr=b;x=c;y=d;char s[51];
   strcpy(text,s1);
   if(nr==1)
   { if(tip==0) strcpy(text,"START");
     if(tip==1) strcpy(text,"STOP");
     if(tip==2 || tip==3) {strcpy(s,var1);s[strlen(s)]='\0';}
     if(tip>3) {strcpy(s,var1);strcat(s,"  ");strcat(s,op);strcat(s,"  ");strcat(s,var2);}
     if(tip>=2) strcpy(text,s);

   }
  }
};
blocuri a[1001];

void deseneaza_start(int x, int y, char s[51]="START")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+25,y+25,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
}


void deseneaza_stop(int x, int y, char s[51]="STOP")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+25,y+25,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y,raza,raza);
}


void deseneaza_intrare(int x, int y, char s[51]="INTRARE")
{ int width=100,height=50,dist=20,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  line(x+dist,y,x+width-dist,y);
  line(x+width-dist,y,x+width,y+height);
  line(x+width,y+height,x,y+height);
  line(x,y+height,x+dist,y);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height/2,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_iesire(int x, int y, char s[51]="IESIRE")
{ int width=100,height=50,dist=20,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  line(x,y,x+width,y);
  line(x+width,y,x+width-dist,y+height);
  line(x+width-dist,y+height,x+dist,y+height);
  line(x+dist,y+height,x,y);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height/2,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_decizie(int x, int y, char s[51]="DECIZIE")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  line(x,y+height,x+width/2,y);
  line(x+width/2,y,x+width,y+height);
  line(x+width,y+height,x,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height/2,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y,raza,raza);
  fillellipse(x,y+height,raza,raza);
  fillellipse(x+width,y+height,raza,raza);
}

void deseneaza_calcul(int x, int y, char s[51]="CALCUL")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+25,y+25,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void desenare_bloc(int x, int y, int tip, char text[]="")
{ switch (tip)
    { case 0: {deseneaza_start(x,y,text);break;}
      case 1: {deseneaza_stop(x,y,text);break;}
      case 2: {deseneaza_intrare(x,y,text);break;}
      case 3: {deseneaza_iesire(x,y,text);break;}
      case 4: {deseneaza_decizie(x,y,text);break;}
      case 5: {deseneaza_calcul(x,y,text);break;}
    }
}

bool apartine_dreptungi(int a, int b, int x, int y, int width=100, int height=50)
{ if(a>x && a<x+width && b>y && b<y+height) return true;
        return false;
}

int arie(int x1, int y1, int x2, int y2, int x3, int y3)
{ return abs(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/2;
}

bool apartine_intrare(int a, int b, int x, int y, int width=100, int height=50)
{ if(a<x || a>x+width || b<y || b>y+height) return false;
  int arie1,arie2,arie3,arie4;
  arie1=arie(a,b,x,y,x+20,y);
  arie2=arie(a,b,x,y,x,y+height);
  arie3=arie(a,b,x,y+height,x+20,y);
  arie4=arie(x,y,x,y+height,x+20,y);
  if(arie1+arie2+arie3==arie4) return false;
  a=x+x+width-a;
  arie1=arie(a,b,x,y,x+20,y);
  arie2=arie(a,b,x,y,x,y+height);
  arie3=arie(a,b,x,y+height,x+20,y);
  arie4=arie(x,y,x,y+height,x+20,y);
  if(arie1+arie2+arie3==arie4) return false;
        return true;
}

bool apartine_iesire(int a, int b, int x, int y, int width=100, int height=50)
{ if(a<x || a>x+width || b<y || b>y+height) return false;
  int arie1,arie2,arie3,arie4;
  arie1=arie(a,b,x,y,x+20,y+height);
  arie2=arie(a,b,x+20,y+height,x,y+height);
  arie3=arie(a,b,x,y+height,x,y);
  arie4=arie(x,y,x,y+height,x+20,y+height);
  if(arie1+arie2+arie3==arie4) return false;
  a=x+x+width-a;
  arie1=arie(a,b,x,y,x+20,y+height);
  arie2=arie(a,b,x+20,y+height,x,y+height);
  arie3=arie(a,b,x,y+height,x,y);
  arie4=arie(x,y,x,y+height,x+20,y+height);
  if(arie1+arie2+arie3==arie4) return false;
        return true;

}

bool apartine_decizie(int a, int b, int x, int y, int width=100, int height=50)
{ if(a+b>x+y+height && x+(x+width-a)+b>x+y+height && b>y && b<y+height) return true;
        return false;
}

bool verifica_bloc(int tip, int a, int b, int x, int y)
{ switch (tip)
    { case 2: {return apartine_intrare(a,b,x,y);}
      case 3: {return apartine_iesire(a,b,x,y);}
      case 4: {return apartine_decizie(a,b,x,y);}
      default : return apartine_dreptungi(a,b,x,y);
    }
}

bool apartine_zona(int x, int y)
{ if(x>colt_x+raza && x+100<colt_x+lungime-raza && y>colt_y+raza && y+50<colt_y+inaltime-raza) return true;
    return false;
}

void verifica_butoane(int x, int y)
{ for(int i=0;i<=5;i++)
      if(verifica_bloc(a[i].tip,x,y,a[i].x,a[i].y)) bloc_nou=a[i].tip;


}

void sterge(int i)
{ for(int j=i;j<nr_blocuri-1;j++)
    a[j]=a[j+1];
  nr_blocuri--;
  //for(int j=6;j<nr_blocuri;j++)
  //  a[j].remove(i);
}

void verifica_optiuni(int i, int a, int b, int x, int y)
{ int width=125,height=75;
  if(x+width>colt_x+lungime) x-=width+100;
  if(y+height>colt_y+inaltime) y-=y+height-colt_y-inaltime;
  if(a>x+width || a<x || b>y+height || b<y) return ;
  if(b>y+height/2) {sterge(i); return ;}
  bloc_nou=i;
}

void afiseaza_optiuni(int x, int y)
{ int width=125,height=75,lung_text;
  if(x+width>colt_x+lungime) x-=width+100;
  if(y+height>colt_y+inaltime) y-=y+height-colt_y-inaltime;
  setcolor(BLACK);
  rectangle(x,y,x+width,y+height);
  lung_text=textwidth("MUTA");
  outtextxy(x+width/2-lung_text/2,y+10,"MUTA");
  lung_text=textwidth("STERGE");
  outtextxy(x+width/2-lung_text/2,y+2*height/4,"STERGE");
  setfillstyle(SOLID_FILL,option_color);
  floodfill(x+10,y+10,BLACK);
  line(x,y+height/2,x+width,y+height/2);
}

void init()
{ int y=710,x=50,width=100,nr=1;
  setbkcolor(fundal);clearviewport();
  a[0]={0,0,50,710,"START"};
  a[1]={1,0,200,710,"STOP"};
  a[2]={2,0,350,710,"INTRARE"};
  a[3]={3,0,500,710,"IESIRE"};
  a[4]={4,0,650,710,"DECIZIE"};
  a[5]={5,0,800,710,"CALCUL"};
  for(int i=0;i<=5;i++)
      desenare_bloc(a[i].x,a[i].y,a[i].tip,a[i].text);
}

void deseneaza_schema()
{ setcolor(BLUE);
  rectangle(0,colt_y,lungime,colt_y+inaltime);
  setfillstyle(SOLID_FILL,WHITE);
  floodfill(500,100,BLUE);
  for(int i=6;i<nr_blocuri;i++)
       desenare_bloc(a[i].x,a[i].y,a[i].tip,a[i].text);
}

int verifica_toate_blocurile(int x, int y)
{ for(int i=6;i<nr_blocuri;i++)
      if(verifica_bloc(a[i].tip,x,y,a[i].x,a[i].y)) return i;
  return -1;
}

int main()
{ initwindow(getmaxwidth(),getmaxheight()-25); /// 1530 810
  init();
  bool event=1,optiuni=0;
  while(1)
  {delay(100);
   if(event) deseneaza_schema(),event=0;
   if(optiuni && ismouseclick(WM_LBUTTONDOWN)) /// click pe optiunile muta/sterge
        {int x=mousex(),y=mousey();
         clearmouseclick(WM_LBUTTONDOWN);
         verifica_optiuni(selectat,x,y,a[selectat].x+a[selectat].width,a[selectat].y);
         event=1;
         selectat=-1;
         optiuni=0;
         continue ;
        }
   if(selectat>5 && ismouseclick(WM_RBUTTONDOWN)) /// click dreapta pe un bloc
        {int x=mousex(),y=mousey();
         afiseaza_optiuni(a[selectat].x+a[selectat].width,a[selectat].y);
         clearmouseclick(WM_RBUTTONDOWN);
         event=0;
         optiuni=1;
         continue ;
        }
   if(bloc_nou!=-1 && ismouseclick(WM_RBUTTONDOWN)) /// click dreapta cand se muta un bloc pentru a anula actiunea
        {clearmouseclick(WM_RBUTTONDOWN);
         bloc_nou=-1;
         event=1;
        }
   if(bloc_nou!=-1)  /// mutarea unui bloc
        {int x=mousex(),y=mousey();
         x-=a[bloc_nou].width/2;
         if(apartine_zona(x,y))
            {desenare_bloc(x,y,a[bloc_nou].tip,a[bloc_nou].text);
             event=1;
            }
        }
   if(ismouseclick(WM_LBUTTONDOWN)) /// click stanga pentru a plasa blocuri si pentru a adauga blocuri noi
        {int x=mousex(),y=mousey();
         clearmouseclick(WM_LBUTTONDOWN);
         event=1;
         if(bloc_nou!=-1)
            { x-=a[bloc_nou].width/2;
              if(apartine_zona(x,y))
                   {if(bloc_nou<=5)
                        a[nr_blocuri++]={bloc_nou,1,x,y};
                   else a[bloc_nou].x=x,a[bloc_nou].y=y;
                   }
              bloc_nou=-1;
            }
         verifica_butoane(x,y);
         selectat=verifica_toate_blocurile(x,y);
         }

  }
getch();
    return 0;
}
