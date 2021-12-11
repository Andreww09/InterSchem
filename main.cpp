#include <iostream>
#include <stdlib.h>
#include <winbgim.h>
#include <string>
#include <stack>

#define MARGINE_SUPERIOARA 0.20
#define DISTANTA_INTRE_COMPONENTE 0.05
#define INALTIME_ZONA_MENIU 0.60
#define LUNGIME_ZONA_MENIU 0.2
#define MARGINE_STANGA 0.40
#define FONT_STYLE DEFAULT_FONT
#define FONT_SIZE 0
#define distanta_intre_blocuri 10
#define distanta_max_leg 25
#define NMAX 201
#define max_zoom 8
#define option_width 100
#define option_height 45
#define distanta_mutare_ecran 50
#define min_x_ecran 0
#define max_x_ecran 12000
#define min_y_ecran 0
#define max_y_ecran 6000
/// zona unde pot fi puse blocurile
#define colt_x 0
#define colt_y 50
#define lungime 1200
#define inaltime 600
///coordonatele butoanelor de zoom
#define minus_x 1000
#define minus_y 670
#define plus_x 1100
#define plus_y 670
#define latura_zoom 20
///zona de informatii
#define info_x 1200
#define info_y 50
#define info_width 400
#define info_height 100
#define max_text_height 75
/// culorile folosite
#define color COLOR(102,255,178)
#define nod_color YELLOW
#define fundal WHITE
#define option_color COLOR(128,128,255)
#define text_color RED
#define line_color RED

using namespace std;

int ecrane[11]; // Numarul de ecrane al programului
int nrOptiuni;  //Cate optiuni are meniul ( poate fi modificat prin apelarea de un numar dorit de ori a functiei creeazaOptiune )
int ecranCurent; //Ecranul deschis in acest moment
float variabile[27];

int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=7,dist_leg=distanta_max_leg;
bool event=1,options=0,ecran_schimbat=1,bloc_start=0;
int nod_dest=-1,nod_st=-1,nod_dr=-1;
int culori_butoane[5]={RED,GREEN,RED,LIGHTBLUE,GREEN},nr_butoane=5;
int zoom_ratio=5,indice_zoom=4; /// zoom_ratio=5 va insemna 1/5 adica 20%
int ecran_x,ecran_y;
char mesaje_butoane[5][NMAX]={"Inapoi","Salvare","Executa","Undo","Redo"};

void zoom_bloc(int,int);

struct punct
{
    int x,y;
};

struct linie
{
    punct p1,p2;
};

struct dreptunghi
{
    punct p1,p2;
    int midx,midy;
} textbox;




//Structura unei optiuni din meniu
struct optiune
{
    dreptunghi drept;
    int heightText, widthText;
    int culoare;

} optiuni[11];


struct blocuri
{ int tip,nr;
  double x,y;
  double x1,y1;
  int width=100 ,height=50;
  int w1=100,h1=50;
  char text[201];
  int st=-1,dr=-1,ant=-1;
  blocuri(int a=0,int b=0,int c=0,int d=0,char s[NMAX]="")
  {tip=a;nr=b;x=x1=c;y=y1=d;
   width=w1+((indice_zoom-4)*w1)/zoom_ratio;
   height=h1+((indice_zoom-4)*h1)/zoom_ratio;
   if(indice_zoom!=4)
   { x1=(x*zoom_ratio)/(indice_zoom-4+zoom_ratio);
     y1=(y*zoom_ratio)/(indice_zoom-4+zoom_ratio);
   }
     if(tip==0) strcpy(text,"START");
     if(tip==1) strcpy(text,"STOP");
     if(tip>1) strcpy(text,s);
  }
};
blocuri a[NMAX];

void deseneaza_start(int x, int y, char s[NMAX]="START", int width=100, int height=50)
{ int lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
}


void deseneaza_stop(int x, int y, char s[NMAX]="STOP", int width=100, int height=50)
{ int lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_intrare(int x, int y, char s[NMAX]="INTRARE", int width=100, int height=50)
{ int dist=width/5,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-dist)
  { int n=(width-dist)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
  line(x,y,x+width,y);
  line(x+width,y,x+width-dist,y+height);
  line(x+width-dist,y+height,x+dist,y+height);
  line(x+dist,y+height,x,y);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_iesire(int x, int y, char s[NMAX]="IESIRE", int width=100, int height=50)
{ int dist=width/5,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-dist)
  { int n=(width-dist)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
  line(x+dist,y,x+width-dist,y);
  line(x+width-dist,y,x+width,y+height);
  line(x+width,y+height,x,y+height);
  line(x,y+height,x+dist,y);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_decizie(int x, int y, char s[NMAX]="DECIZIE", int width=100, int height=50)
{ int lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-45)
  { int n=(width-45)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/4,s1);
  line(x,y+height,x+width/2,y);
  line(x+width/2,y,x+width,y+height);
  line(x+width,y+height,x,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y,raza,raza);
  fillellipse(x,y+height,raza,raza);
  fillellipse(x+width,y+height,raza,raza);
}

void deseneaza_calcul(int x, int y, char s[NMAX]="CALCUL", int width=100, int height=50)
{ int lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
//  setusercharsize()
   /// settextstyle(6,HORIZ_DIR,1);///0 6 9
  if(lung_text>width-10)
  { int n=(width-10)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+width/2,y+height-1,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
  fillellipse(x+width/2,y,raza,raza);
}

void desenare_bloc(int i)
{ int x=a[i].x,y=a[i].y;
  if(i>=6) x-=ecran_x,y-=ecran_y-colt_y;
  switch (a[i].tip)

    { case 0: {deseneaza_start(x,y,a[i].text,a[i].width,a[i].height);break;}
      case 1: {deseneaza_stop(x,y,a[i].text,a[i].width,a[i].height);break;}
      case 2: {deseneaza_intrare(x,y,a[i].text,a[i].width,a[i].height);break;}
      case 3: {deseneaza_iesire(x,y,a[i].text,a[i].width,a[i].height);break;}
      case 4: {deseneaza_decizie(x,y,a[i].text,a[i].width,a[i].height);break;}
      case 5: {deseneaza_calcul(x,y,a[i].text,a[i].width,a[i].height);break;}
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
  int arie1,arie2,arie3,arie4,dist=width/5;
  arie1=arie(a,b,x,y,x+dist,y+height);
  arie2=arie(a,b,x+dist,y+height,x,y+height);
  arie3=arie(a,b,x,y+height,x,y);
  arie4=arie(x,y,x,y+height,x+dist,y+height);
  if(arie1+arie2+arie3==arie4) return false;
  a=x+x+width-a;
  arie1=arie(a,b,x,y,x+dist,y+height);
  arie2=arie(a,b,x+dist,y+height,x,y+height);
  arie3=arie(a,b,x,y+height,x,y);
  arie4=arie(x,y,x,y+height,x+dist,y+height);
  if(arie1+arie2+arie3==arie4) return false;
        return true;
}

bool apartine_iesire(int a, int b, int x, int y, int width=100, int height=50)
{ if(a<x || a>x+width || b<y || b>y+height) return false;
  int arie1,arie2,arie3,arie4,dist=width/5;
  arie1=arie(a,b,x,y,x+dist,y);
  arie2=arie(a,b,x,y,x,y+height);
  arie3=arie(a,b,x,y+height,x+dist,y);
  arie4=arie(x,y,x,y+height,x+dist,y);
  if(arie1+arie2+arie3==arie4) return false;
  a=x+x+width-a;
  arie1=arie(a,b,x,y,x+dist,y);
  arie2=arie(a,b,x,y,x,y+height);
  arie3=arie(a,b,x,y+height,x+dist,y);
  arie4=arie(x,y,x,y+height,x+dist,y);
  if(arie1+arie2+arie3==arie4) return false;
        return true;

}

bool apartine_decizie(int a, int b, int x, int y, int width=100, int height=50)
{ if(a+b>x+y+height && x+(x+width-a)+b>x+y+height && b>y && b<y+height) return true;
        return false;
}

bool verifica_bloc(int x, int y, int i)
{if(i>=6) x+=ecran_x,y+=ecran_y-colt_y;
 switch (a[i].tip)
    { case 2: {return apartine_intrare(x,y,a[i].x,a[i].y,a[i].width,a[i].height);}
      case 3: {return apartine_iesire(x,y,a[i].x,a[i].y,a[i].width,a[i].height);}
      case 4: {return apartine_decizie(x,y,a[i].x,a[i].y,a[i].width,a[i].height);}
      default : return apartine_dreptungi(x,y,a[i].x,a[i].y,a[i].width,a[i].height);
    }
}

bool apartine_zona(int x, int y, int width, int height)
{ x-=ecran_x;y-=ecran_y-colt_y;
  int d=dist_leg;
  if(x>colt_x+d && x+width<colt_x+lungime-d && y>colt_y && y+height<colt_y+inaltime-d) return true;
     return false;
}

void verifica_butoane(int x, int y)
{ for(int i=0;i<=5;i++)
      if(verifica_bloc(x,y,i)) bloc_nou=a[i].tip;


}

void sterge(int i)
{ int j=a[i].ant;
  if(a[i].tip==0) bloc_start=0;
  if(a[i].st!=-1) a[a[i].st].ant=-1;
  if(a[i].tip==4 && a[i].dr!=-1) a[a[i].dr].ant=-1;
  if(a[j].st==i) a[j].st=-1;
  if(a[j].tip==4 && a[j].dr==i) a[j].dr=-1;
  for(int j=i;j<nr_blocuri-1;j++)
    a[j]=a[j+1];
  nr_blocuri--;
  for(int j=6;j<nr_blocuri;j++)
  { if(a[j].st!=-1 && a[j].st>i) a[j].st--;
  if(a[j].tip==4 && a[j].dr!=-1 && a[j].dr>i) a[j].dr--;
  }

}

void verifica_optiuni(int i, int a, int b, int x, int y)
{ x-=ecran_x;y-=ecran_y-colt_y;
  if(x+option_width>colt_x+lungime) x-=option_width+100;
  if(y+option_height>colt_y+inaltime) y-=y+option_height-colt_y-inaltime;
  if(a>x+option_width || a<x || b>y+option_height || b<y) return ;
  if(b>y+option_height/2) {sterge(i); return ;}
  bloc_nou=i;
}

void afiseaza_optiuni(int x, int y)
{ int lung_text;
  x-=ecran_x;y-=ecran_y-colt_y;
  if(x+option_width>colt_x+lungime) x-=option_width+100;
  if(y+option_height>colt_y+inaltime) y-=y+option_height-colt_y-inaltime;
  setcolor(BLACK);
  rectangle(x,y,x+option_width,y+option_height);
  lung_text=textwidth("MUTA");
  outtextxy(x+option_width/2-lung_text/2,y+10,"MUTA");
  lung_text=textwidth("STERGE");
  outtextxy(x+option_width/2-lung_text/2,y+2*option_height/4,"STERGE");
  setfillstyle(SOLID_FILL,option_color);
  floodfill(x+10,y+10,BLACK);
  line(x,y+option_height/2,x+option_width,y+option_height/2);
}

void deseneaza_info()
{ int x1=info_x+10,y1=info_y+10;
  setcolor(BLUE);
  rectangle(info_x,info_y,info_x+info_width,info_y+info_height);
  outtextxy(x1,y1,"Informatii");
  line(info_x,info_y+30,info_x+info_width,info_y+30);
  //rectangle(info_x+20,info_y+50,info_x+100,)
}

void desenare_buton(int left, int right, int culoare, char text[])
{ int width=right-left,height=50,lung_text=textwidth(text);
  setcolor(BLUE);
  outtextxy(left+width/2-lung_text/2,height/2,text);
  rectangle(left,0,right,48);
  setfillstyle(SOLID_FILL,culoare);
  floodfill(left+1,1,BLUE);
}

void zoom_bloc(int nr, int i)
{a[i].x=a[i].x1+(nr*a[i].x1)/zoom_ratio;
 a[i].y=a[i].y1+(nr*a[i].y1)/zoom_ratio;
 a[i].width=a[i].w1+(nr*a[i].w1)/zoom_ratio;
 a[i].height=a[i].h1+(nr*a[i].h1)/zoom_ratio;
}

void zoom(int semn, int nr )
{for(int i=6;i<nr_blocuri;i++)
         {//cout<<a[i].x<<' '<<a[i].y<<' '<<a[i].width<<' '<<a[i].height<<' '<<' ';
         zoom_bloc(nr,i);
          //cout<<a[i].x<<' '<<a[i].y<<' '<<a[i].width<<' '<<a[i].height<<'\n';
         }

}

void verifica_zoom(int x, int y)
{ if(apartine_dreptungi(x,y,minus_x,minus_y,latura_zoom,latura_zoom) && indice_zoom>1)
        {indice_zoom--;
         raza--;
         zoom(-1,indice_zoom-4);
         dist_leg=distanta_max_leg-distanta_max_leg/zoom_ratio;
        }
  if(apartine_dreptungi(x,y,plus_x,plus_y,latura_zoom,latura_zoom) && indice_zoom<6)
        {indice_zoom++;
         raza++;
         zoom(1,indice_zoom-4);
         dist_leg=distanta_max_leg-distanta_max_leg/zoom_ratio;
        }
}

void deseneaza_butoane_zoom()
{ setcolor(BLACK);
  rectangle(minus_x,minus_y,minus_x+latura_zoom,minus_y+latura_zoom);
  line(minus_x+latura_zoom/10,minus_y+latura_zoom/2,minus_x+latura_zoom-latura_zoom/10,minus_y+latura_zoom/2);
  rectangle(plus_x,plus_y,plus_x+latura_zoom,plus_y+latura_zoom);
  line(plus_x+latura_zoom/2,plus_y+latura_zoom/10,plus_x+latura_zoom/2,plus_y+latura_zoom-latura_zoom/10);
  line(plus_x+latura_zoom/10,plus_y+latura_zoom/2,plus_x+latura_zoom-latura_zoom/10,plus_y+latura_zoom/2);
}

void verifica_mutare_ecran()
{ char c;
if(getch()==0) c=getch();
  if(c==72 || c==75 || c==77 || c==80)
  { c=(c-70)/3;cout<<(int)c<<'\n'; /// 1-stanga 0-sus 2-dreapta 3-jos
    int di[4]={0,-1,1,0},dj[4]={-1,0,0,1},d=distanta_mutare_ecran;
    if(ecran_x+di[c]*d>=min_x_ecran && ecran_x+di[c]*d<=max_x_ecran && ecran_y+dj[c]*d>=min_y_ecran && ecran_y+dj[c]*d<=max_y_ecran )
        ecran_x+=di[c]*d,ecran_y+=dj[c]*d,event=1;
  }


}

void init()
{ int y=710,x=50;
  nr_blocuri=6;
  bloc_start=0;
  ecran_x=min_x_ecran;ecran_y=min_y_ecran;
  setbkcolor(fundal);clearviewport();
  deseneaza_info();
  a[0]={0,0,50,710,"START"};
  a[1]={1,0,200,710,"STOP"};
  a[2]={2,0,350,710,"INTRARE"};
  a[3]={3,0,500,710,"IESIRE"};
  a[4]={4,0,650,710,"DECIZIE"};
  a[5]={5,0,800,710,"CALCUL"};
  for(int i=0;i<=5;i++)
      desenare_bloc(i);
  int width=getmaxwidth()/nr_butoane;
  for(int i=0;i<nr_butoane;i++)
      desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);
  deseneaza_butoane_zoom();
}

void sterge_info()
{ setfillstyle(SOLID_FILL,fundal);
  floodfill(textbox.p1.x,textbox.p1.y,BLUE);
}

bool verifica_textbox(char text[])
{ int lung=strlen(text);
  //if(lung+40>2*(textbox.right-textbox.left))  return false;
  if(lung>40) return false;
     return true;
}

/*int numar_caractere(char text[], int width)
{ char aux[NMAX];cout<<width<<' ';
  int nr=0,n=strlen(text);
  do
    {aux[nr]=text[nr];nr++;aux[nr]='\0';
    cout<<textwidth(text)<<' '<<aux<<' '<<text<<'\n';
  }   while(nr<n && textwidth(aux)<width);
  cout<<'\n';
    return nr;
}*/

void deseneaza_text(char text[])
{ setcolor(BLACK);
  textbox.p1.x=info_x+10;textbox.p2.x=info_x+300;
  textbox.p1.y=info_y+50;
  int width=textwidth("m"),height=textheight("W"),lung=textwidth(text);
  if(lung+10>textbox.p2.x-textbox.p1.x)
    {sterge_info();
     char s[NMAX];
     int n=20;//numar_caractere(text,textbox.p2.x-textbox.p1.x-10);
     strncpy(s,text,n);s[n]='\0';
     outtextxy(textbox.p1.x+10,textbox.p1.y,s);
     outtextxy(textbox.p1.x+10,textbox.p1.y+height,text+n);
     height*=2;
    }
    else   outtextxy(textbox.p1.x+10,textbox.p1.y,text);
  textbox.p2.y=textbox.p1.y+height;
  rectangle(textbox.p1.x,textbox.p1.y,textbox.p2.x,textbox.p2.y);

}

bool apartine_text(int x,int y)
{ if(x>textbox.p1.x && x<textbox.p2.x && y>textbox.p1.y && y<textbox.p2.y) return true;
    return false;
}

void citeste_text(int i)
{ char c,s[NMAX];
  int n;
  strcpy(s,a[i].text);
  n=strlen(s);//cout<<n;
  while(c!=13)
  {c=getch();
   if(c==8 && n>0) n--,s[n]='\0',sterge_info();
   if(!verifica_textbox(s)) {continue;}
   if(c!=8 && c!=13)  s[n++]=c,s[n]='\0';;
   deseneaza_text(s);
  }
  //if(evalExpresie(s))
    strcpy(a[i].text,s);

}

bool apartine_nod(int i, int a, int b, int x, int y, int &nod)
{ if(abs(b-y)+abs(a-x)<=raza)
     {nod=i; return true;}
  return false;
}

bool verifica_toate_nodurile(int x, int y)
{x+=ecran_x;y+=ecran_y-colt_y;
 nod_st=nod_dr=-1;
 for(int i=6;i<nr_blocuri;i++)
     {bool ok=0;
      switch (a[i].tip)
        { case 0: {ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y+a[i].height,nod_st); break ;}
          case 1: {ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);  break ;}
          case 4:
              {ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);
               ok+=apartine_nod(i,x,y,a[i].x,a[i].y+a[i].height,nod_st);
               ok+=apartine_nod(i,x,y,a[i].x+a[i].width,a[i].y+a[i].height,nod_dr);
               break ;
              }
          default :
            {ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y+a[i].height,nod_st);
             ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);
             break ;
            }
        }
      if(ok) return true ;
     }
  return false;
}

int intersectie(linie l,int nod)
{

    if(l.p1.x == l.p2.x)
    {
        if(a[nod].x <= l.p1.x && l.p1.x <= a[nod].x+a[nod].width && ( (l.p1.y < a[nod].y && a[nod].y < l.p2.y) || (l.p2.y < a[nod].y+a[nod].height && a[nod].y+a[nod].height < l.p1.y)))
            return 1;
        else
            return 0;
    }
    else
    {
        if(a[nod].y <= l.p1.y && l.p1.y <= a[nod].y+a[nod].height && ((l.p1.x < a[nod].x && a[nod].x < l.p2.x) || (l.p2.x < a[nod].x+a[nod].width && a[nod].x+a[nod].width < l.p1.x)))
            return 2;
        else
            return 0;
    }
}

void sorteaza(int v[],int n,int tip)
{
    int schimb = 1;
    do
    {
        schimb = 0;
        for(int i = 1;i<=n-1;i++)
        {
            if(tip == 1 && a[v[i]].y >= a[v[i+1]].y || tip == 2 && a[v[i]].x >= a[v[i+1]].x)
            {
                int aux = v[i];
                v[i] = v[i+1];
                v[i+1] = aux;
                schimb = 1;
            }
        }
    }while(schimb);
}

void inverseaza(int v[],int n)
{
    cout<<"INTRAT";
    for(int i=1;i<=n/2;i++)
    {
        int aux = v[i];
        v[i] = v[n-i+1];
        v[n-i+1] = aux;
    }
}

void trasareVertical(linie& l)
{
    int v[10];
    int len = 0;

    for(int i=6;i<nr_blocuri;i++)
    {
        int tipIntersectie = intersectie(l,i);
        if(tipIntersectie == 1)
        {
            v[++len] = i;
        }
    }

    sorteaza(v,len,1);
    int px = 20;

    if(l.p1.y >= l.p2.y)
        inverseaza(v,len);
    else
        px *= -1;

    linie l1;
    l1.p1.x = l.p1.x;
    l1.p1.y = l.p1.y;

    for(int i=1;i<=len;i++)
    {
        l1.p2.x = l1.p1.x;
        if(px > 0)
            l1.p2.y = (a[v[i]].y + a[v[i]].height)+px;
        else
            l1.p2.y = a[v[i]].y + px;

        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;
        if(px > 0)
        {

            if(l.p1.x <= a[v[i]].x + a[v[i]].width / 2 )
            {
                l1.p2.x = a[v[i]].x - px;

            }else
                l1.p2.x = a[v[i]].x + a[v[i]].width + px;
        }
        else
        {
            if(l.p1.x <= a[v[i]].x + a[v[i]].width / 2)
                l1.p2.x = a[v[i]].x + px;
            else
                l1.p2.x = a[v[i]].x + a[v[i]].width - px;
        }

        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;
        if(px > 0)
            l1.p2.y = a[v[i]].y - px;
        else
            l1.p2.y = a[v[i]].y + a[v[i]].height - px;
        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);


        l1.p1 = l1.p2;
        l1.p2.y = l1.p1.y;
        l1.p2.x = l.p2.x;
        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;
    }

    if(len == 0)
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);
    else
    {
        line(l1.p2.x,l1.p2.y,l.p2.x,l.p2.y);

    }
    l.p1 = l.p2;
}

void trasareOrizontal(linie& l)
{
    int v[10];
    int len = 0;

    for(int i=6;i<nr_blocuri;i++)
    {
        int tipIntersectie = intersectie(l,i);
        if(tipIntersectie == 2)
        {
            v[++len] = i;
        }
    }

    sorteaza(v,len,2);

    int px = 20;
    if(l.p1.x > l.p2.x)
    {
        px *= -1;
        inverseaza(v,len);
    }
    for(int i=1;i<=len;i++)
        cout<<v[i]<<" ";
    cout<<endl;

    linie l1;
    l1.p1.x = l.p1.x;
    l1.p1.y = l.p1.y;

    for(int i=1;i<=len;i++)
    {
        l1.p2.y = l1.p1.y;
        if(px > 0)
            l1.p2.x = a[v[i]].x - px;
        else
            l1.p2.x = a[v[i]].x + a[v[i]].width - px;

        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;
        if(px > 0)
        {
            if(l.p1.y <= a[v[i]].y + a[v[i]].height / 2 )
                l1.p2.y = a[v[i]].y - px;
            else
                l1.p2.y = a[v[i]].y + a[v[i]].height + px;
        }
        else
        {
            if(l.p1.y <= a[v[i]].y + a[v[i]].height / 2)
                l1.p2.y = a[v[i]].y + px;
            else
                l1.p2.y = a[v[i]].y + a[v[i]].height - px;
        }
        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;
        l1.p2.y = l1.p1.y;
        if(px > 0)
        {
            l1.p2.x = a[v[i]].x + a[v[i]].width + px;
        }else
        {
            l1.p2.x = a[v[i]].x + px;
        }
        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);


        l1.p1 = l1.p2;
        l1.p2.x = l1.p1.x;
        l1.p2.y = l.p1.y;
        line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y);

        l1.p1 = l1.p2;

    }

    if(len == 0)
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);
    else
    {
        line(l1.p1.x,l1.p1.y,l.p2.x,l.p2.y);
    }
    l.p1 = l.p2;
}

void trasare_legatura(int tip,int nod1,int nod2,int dr)
{
    /** x, y - pozitia nodului,  tip poate de ajuta la ceva
        (a[i].x+a[i].width/2, a[i].y)  sunt coordonatele nodului destinatie
      */
    a[nod1].x-=ecran_x;a[nod1].y-=ecran_y-colt_y;
    if(nod1!=nod2) a[nod2].x-=ecran_x,a[nod2].y-=ecran_y-colt_y;
    punct pctStart,pctStop;  // punctele de inceput si final
    linie l;
    setcolor(line_color);

    if(tip == 4) // daca este un bloc de decizie (are doua noduri de iesire aflate in colturi)
    {
        if(!dr) // daca am selectat blocul din stanga
            pctStart.x = a[nod1].x;
        else
            pctStart.x = a[nod1].x + a[nod1].width; //daca am selectat blocul din dreapta

        pctStart.y = a[nod1].y + a[nod1].height; // coordonata y ramane aceeasi
    }
    else // orice alt tip de bloc porneste cu un nod aflat in mijloc
    {
        pctStart.x = a[nod1].x + a[nod1].width / 2;
        pctStart.y = a[nod1].y + a[nod1].height;
    }

    // coordonatele destinatie ale legaturii (mereu se afla in mijlocul blocului)
    pctStop.x = a[nod2].x + a[nod2].width / 2;
    pctStop.y = a[nod2].y;

    int cx1 = min(a[nod1].x,a[nod1].x + a[nod1].width), cx2 = max(a[nod1].x,a[nod1].x + a[nod1].width);
    int cy1 = min(a[nod2].x,a[nod2].x + a[nod2].width), cy2 = max(a[nod2].x,a[nod2].x + a[nod2].width);

    if(pctStart.y <= pctStop.y) // daca blocul destinatie se afla sub blocul sursa ( legatura merge in jos)
    {
        l.p1 = pctStart;
        l.p2.x = pctStart.x;
        l.p2.y = (pctStart.y + pctStop.y) / 2;

        trasareVertical(l);

        l.p2.x = pctStop.x;
        l.p2.y = l.p1.y;

        trasareOrizontal(l);

        l.p2 = pctStop;

        trasareVertical(l);
    }
    else if(pctStart.y > pctStop.y && cx2 >= cy1 && cy2 >= cx1)
    {

        if(pctStop.x <= pctStart.x)
        {
            l.p1 = pctStart;
            l.p2.x = pctStart.x;
            l.p2.y = pctStart.y +a[nod2].height/2;

            trasareVertical(l);

            l.p2.x = min(a[nod1].x,a[nod2].x) - a[nod2].width/4;
            l.p2.y = l.p1.y;

            trasareOrizontal(l);

            l.p2.x = min(a[nod1].x,a[nod2].x) - a[nod2].width/4;
            l.p2.y = a[nod2].y - a[nod2].height/2;

            trasareVertical(l);

            l.p2.x = pctStop.x;
            l.p2.y = l.p1.y;

            trasareOrizontal(l);

            l.p2 = pctStop;

            trasareVertical(l);
        }
        else
        {
            l.p1 = pctStart;
            l.p2.x = pctStart.x;
            l.p2.y = pctStart.y + a[nod2].height/2;
            trasareVertical(l);

            l.p2.x = max(a[nod1].x + a[nod1].width,a[nod2].x + a[nod2].width) + a[nod2].width/4 ;
            l.p2.y = l.p1.y;

            trasareOrizontal(l);

            l.p2.x = l.p1.x;
            l.p2.y = a[nod2].y - a[nod2].height/2;

            trasareVertical(l);

            l.p2.x = pctStop.x;
            l.p2.y = l.p1.y;

            trasareOrizontal(l);

            l.p2 = pctStop;

            trasareVertical(l);
        }

    }
    else
    {
        int dist = abs((a[nod2].x + a[nod1].x + a[nod1].width) / 2 );

        l.p1 = pctStart;
        l.p2.x = pctStart.x;
        l.p2.y = pctStart.y + a[nod2].height/2;

        trasareVertical(l);

        l.p2.x = dist;
        l.p2.y = l.p1.y;

        trasareOrizontal(l);

        l.p2.x = l.p1.x;
        l.p2.y = pctStop.y - a[nod2].width/4;
        trasareVertical(l);

        l.p2.x = pctStop.x;
        l.p2.y = l.p1.y;

        trasareOrizontal(l);

        l.p2 = pctStop;

        trasareOrizontal(l);
    }
    a[nod1].x+=ecran_x;a[nod1].y+=ecran_y-colt_y;
    if(nod1!=nod2) a[nod2].x+=ecran_x,a[nod2].y+=ecran_y-colt_y;
}

void deseneaza_legaturi()
{
  for(int i=6; i<nr_blocuri; i++)
    if(apartine_zona(a[i].x,a[i].y,a[i].width,a[i].height))
    {
        if(a[i].st!=-1 && apartine_zona(a[a[i].st].x,a[a[i].st].y,a[a[i].st].width,a[a[i].st].height))
            trasare_legatura(a[i].tip,i,a[i].st,0);
        if(a[i].dr!=-1 && apartine_zona(a[a[i].dr].x,a[a[i].dr].y,a[a[i].dr].width,a[a[i].dr].height))
            trasare_legatura(a[i].tip,i,a[i].dr,1);
    }
}

void deseneaza_schema()
{ setcolor(BLUE);
  rectangle(0,colt_y,lungime,colt_y+inaltime);
  setfillstyle(SOLID_FILL,WHITE);
  floodfill(500,100,BLUE);
  deseneaza_legaturi();
  for(int i=6;i<nr_blocuri;i++)
     if(apartine_zona(a[i].x,a[i].y,a[i].width,a[i].height))
          desenare_bloc(i);
}

int verifica_toate_blocurile(int x, int y)
{ for(int i=6;i<nr_blocuri;i++)
      if(verifica_bloc(x,y,i)) return i;
  return -1;
}

void click_optiuni() /// click pe optiunile muta/sterge
{int x=mousex(),y=mousey();
 clearmouseclick(WM_LBUTTONDOWN);
 verifica_optiuni(selectat,x,y,a[selectat].x+a[selectat].width,a[selectat].y);
 event=1;
 selectat=-1;
 options=0;
}

void click_dreapta_pe_bloc()
{int x=mousex(),y=mousey();
 afiseaza_optiuni(a[selectat].x+a[selectat].width,a[selectat].y);
 clearmouseclick(WM_RBUTTONDOWN);
 event=0;
 options=1;
}

void anuleaza() /// click dreapta cand se muta un bloc pentru a anula actiunea
{clearmouseclick(WM_RBUTTONDOWN);
 bloc_nou=-1;
 event=1;
}

void mutare(int i)
{int x=mousex(),y=mousey();
 x+=ecran_x;y+=ecran_y-colt_y;
 a[nr_blocuri+1]=a[i];
 x-=a[bloc_nou].width/2;
 a[nr_blocuri+1].x=x;a[nr_blocuri+1].y=y;
 if(apartine_zona(x,y,a[nr_blocuri+1].width,a[nr_blocuri+1].height))
    {desenare_bloc(nr_blocuri+1);
     event=1;
    }
}

void inapoi()
{cleardevice();
 setbkcolor(BLACK);
 clearviewport();
 ecrane[0] = 1;
 ecrane[1] = 0;
 ecranCurent = 0;
 ecran_schimbat=1;
}

void salvare()
{

}

void executa()
{

}

void undo()
{

}

void redo()
{

}

void buton(int i)
{ switch (i)
    { case 0: inapoi();
      case 1: salvare();
      case 2: executa();
      case 3: undo();
      case 4: redo();
    }

}

void bara_meniu(int x, int y)
{ int width=getmaxwidth()/nr_butoane;
  for(int i=0;i<nr_butoane;i++)
      if(y>=0 && y<50 && x>=i*width && x<=(i+1)*width) buton(i);
}

bool intersectare_dreptunghiuri(int x1, int y1, int x2, int y2, int cx1, int cy1, int cx2, int cy2)
{ int d=distanta_intre_blocuri;
  if(cx1+d<x2 || x1>cx2+d || cy1+d<y2 || y1>cy2+d) return false;
     return true;
}

bool intersecteaza_alte_blocuri(int x, int y)
{ for(int i=6;i<nr_blocuri;i++)
      if(i!=bloc_nou && intersectare_dreptunghiuri(x,y,a[i].x,a[i].y,x+a[0].width,y+a[0].height,a[i].x+a[i].width,a[i].y+a[i].height)) return true;
  return false;
}

void click_stanga() /// click stanga pentru a plasa blocuri si pentru a adauga blocuri noi
{int x=mousex(),y=mousey();
 clearmouseclick(WM_LBUTTONDOWN);
 event=1;
 if(selectat>5 && a[selectat].tip>1 && apartine_text(x,y) ) {citeste_text(selectat); }
 if(bloc_nou!=-1)
    { x-=a[bloc_nou].width/2;
      bool inter=intersecteaza_alte_blocuri(x+ecran_x,y+ecran_y);
      ///if(inter)  se va afisa o eroare
      if(apartine_zona(x+ecran_x,y+ecran_y-colt_y,a[bloc_nou].width,a[bloc_nou].height) && !inter)
            {if(bloc_nou==0)
                {if(bloc_start==0 && bloc_nou==0) bloc_start=1,a[nr_blocuri++]={bloc_nou,1,x+ecran_x,y+ecran_y-colt_y};
                }
             else if(bloc_nou<=5) a[nr_blocuri++]={bloc_nou,1,x+ecran_x,y+ecran_y-colt_y};
             else a[bloc_nou].x=a[bloc_nou].x1=x+ecran_x,a[bloc_nou].y=a[bloc_nou].y1=y+ecran_y-colt_y;
            }
      bloc_nou=-1;
    }
 verifica_butoane(x,y);
 if(verifica_toate_nodurile(x,y))
    { if(nod_dest!=-1 && a[nod_dest].ant==-1)
        { /// if(nod_dest==nod_st || nod_dest==nod_dr) se va afisa o eroare
          if(nod_st!=-1)
            {a[nod_st].st=nod_dest;
             a[nod_dest].ant=nod_st;
             nod_st=nod_dr=nod_dest=-1;
            }
          if(nod_dr!=-1)
            {a[nod_dr].dr=nod_dest;
             a[nod_dest].ant=nod_dr;
             nod_dr=nod_st=nod_dest=-1;
            }
        }
    }
    else nod_st=nod_dr=nod_dest=-1,selectat=verifica_toate_blocurile(x,y);
 bara_meniu(x,y);
 verifica_zoom(x,y);
}


//Determina coordonatele unei optiuni in functie de marimile ecranului si proportiile alese (constantele de la inceputul programului)
dreptunghi coordonateOptiune(const int& width,const int& height, const int& componenta,const int& nrComponente)
{
    //Setam layoutul ecranului
    int heightSuperior        = height * MARGINE_SUPERIOARA;
    int heightIntreComponente = height * DISTANTA_INTRE_COMPONENTE;
    int heightComponenta      = (height * INALTIME_ZONA_MENIU - nrComponente * heightIntreComponente) / nrComponente;
    int widthStanga           = width * MARGINE_STANGA;
    int widthComponenta       = width * LUNGIME_ZONA_MENIU;

    //Calculam coordonatele dreptunghiului
    dreptunghi rect;
    int y = heightSuperior + (componenta-1) * heightIntreComponente + (componenta-1) * heightComponenta + heightComponenta/2;
    int x = widthStanga + widthComponenta / 2;
    rect.p1.x = x - widthComponenta/2;
    rect.p1.y = y - heightComponenta/2;
    rect.p2.x = x + widthComponenta/2;
    rect.p2.y = y + heightComponenta/2;
    rect.midx = x;
    rect.midy = y;

    return rect;

}


void creeazaOptiune(const int& width,const int& height,const int& componenta,const int& nrComponente,char mesaj[])
{
    if(nrOptiuni < nrComponente)
    {
        //Salvam fiecare optiune
        optiune opt;

        //Marimea fontului, optiunea se va adapta in lungime
        int widthText = textwidth(mesaj);
        int heightText = textheight(mesaj);
        opt.heightText = heightText;
        opt.widthText = widthText;

        //Obtinem coordonatele dreptungiului, care determina pozitia optiunii pe ecran
        dreptunghi drept = coordonateOptiune(width,height,componenta,nrComponente);
        opt.drept = drept;

        //Setam culoarea dreptunghiului
        opt.culoare = 0;

        //O memoram pentru a putea implementa click handlerul si pentru a calcula marimea celorlalte optiuni
        nrOptiuni = componenta;
        optiuni[nrOptiuni] = opt;
    }

      setcolor(BLACK);
    //Afisam optiunea cu mesajul curent
    rectangle(optiuni[componenta].drept.p1.x,optiuni[componenta].drept.p1.y,optiuni[componenta].drept.p2.x,optiuni[componenta].drept.p2.y);
      setcolor(text_color);
    //Afisam mesajul curent in mod centrat in dreptunghiul corespunzator
    outtextxy(optiuni[componenta].drept.midx - optiuni[componenta].widthText / 2, optiuni[componenta].drept.midy - optiuni[componenta].heightText / 2, mesaj);

}

//Detecteaza daca o anumita coordonata se afla intr-un dreptunghi cu anumite coordonate
bool clickInDrepunghi(dreptunghi drept,int x,int y)
{
    int left = drept.p1.x;
    int top = drept.p1.y;
    int right = drept.p2.x;
    int bottom = drept.p2.y;

    return (left <= x && x <= right && y >= top && y <= bottom);
}

//Ecranele programului

//Meniul
void ecran0()
{
    //Seteaza stilul si marimea fontului
    //settextstyle(FONT_STYLE, HORIZ_DIR, FONT_SIZE);

    //Ia marimea ecranului pentru a-l face responsive
    int widthEcran = getmaxwidth(), heightEcran = getmaxheight()-25;
    setbkcolor(fundal);clearviewport();
    //Campurile meniului, se va specifica lungimea si inaltimea ecranului, a cata optiune este, numarul de optiuni si mesajul dorit
    creeazaOptiune(widthEcran,heightEcran,1,4,"CREEAZA SCHEMA");
    creeazaOptiune(widthEcran,heightEcran,2,4, "SALVATE");
    creeazaOptiune(widthEcran,heightEcran,3,4, "SCRIE COD");
    creeazaOptiune(widthEcran,heightEcran,4,4, "IESIRE");
    while(!ecran_schimbat)  /// cand se apasa pe o optiune de meniu, ecran_schimbat devine 1, se opreste loop-ul asta si se revine in loop-ul din main
    {
    if(ismouseclick(WM_LBUTTONDOWN))
    { int x=mousex(),y=mousey();
      clearmouseclick(WM_LBUTTONDOWN);
      for(int i=1; i<=nrOptiuni; i++)
      {
         if(clickInDrepunghi(optiuni[i].drept,x,y) && ecrane[0])
         {
            ecrane[0] = 0;
            ecrane[i] = 1;
            ecranCurent = i;
            //ok = 1;
            ecran_schimbat=1;
         }
      }
    }
    }
}


void ecran1()
{  init();
   while(!ecran_schimbat)
   {delay(100);
   if(kbhit()) verifica_mutare_ecran();
   if(event) deseneaza_schema(),event=0;

   if(selectat<=5) sterge_info();

   if(options && ismouseclick(WM_LBUTTONDOWN)) {click_optiuni(); continue ;}

   if(selectat>5)
        {   if(a[selectat].tip>1) deseneaza_text(a[selectat].text);
            if(ismouseclick(WM_RBUTTONDOWN)) {click_dreapta_pe_bloc(); continue ;}
        }

   if(bloc_nou!=-1 && ismouseclick(WM_RBUTTONDOWN))  {anuleaza(); }

   if(bloc_nou!=-1) mutare(bloc_nou);

   if(ismouseclick(WM_LBUTTONDOWN)) click_stanga();
   }
}

void ecran2()
{
    outtextxy(0,0, "Ecran2");
}

void ecran3()
{
    outtextxy(0,0, "Ecran3");
}

int main()
{
    ecrane[0] = 1; //Meniul este primul ecran apelat
    ecranCurent = 0;

   // int page = 1;  // Initializarea animatiei

    //initwindow(1500,768);
    initwindow(getmaxwidth(),getmaxheight()-25);///1530 810

   // registermousehandler(WM_LBUTTONUP,click_handler); //Click Handlerul

    do
    { if(ecran_schimbat)
      {ecran_schimbat=0;
        delay(100);
        if(ecrane[0])
            ecran0();
        else if(ecrane[1])
            ecran1();
        else if(ecrane[2]) ;
            //ecran2();
        else if(ecrane[3]) ;
            //ecran3();
        else if(ecrane[4])
            exit(1);
      }

        //page = 1-page;

    }
    while(true);

    getch();
    closegraph();

    return 0;
}
