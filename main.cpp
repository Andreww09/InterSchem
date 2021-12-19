#include <iostream>
#include <stdlib.h>
#include <winbgim.h>
#include <string>
#include <stack>
#include <list>
#include <set>
#include <queue>
#include <vector>
#include "evaluareExp.h"

#define MARGINE_SUPERIOARA 0.20
#define DISTANTA_INTRE_COMPONENTE 0.05
#define INALTIME_ZONA_MENIU 0.60
#define LUNGIME_ZONA_MENIU 0.2
#define MARGINE_STANGA 0.40
#define FONT_STYLE DEFAULT_FONT
#define FONT_SIZE 0
#define distanta_max_leg 25
#define NMAX 201
#define NR_ERORI 20
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
///coordonatele butoanelor left-right de eroare
#define left_x 1450
#define left_y 90
#define latura_left 20
#define dist_left 40
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

int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=7,dist_leg=distanta_max_leg,distanta_intre_blocuri=20;
bool event=1,options=0,ecran_schimbat=1,rezult;
int nr_blocuri_start=0,start_main=0;
int nod_dest=-1,nod_st=-1,nod_dr=-1;
int culori_butoane[5]={RED,GREEN,RED,LIGHTBLUE,GREEN},nr_butoane=5;
int zoom_ratio=5,indice_zoom=4; /// zoom_ratio=5 va insemna 1/5 adica 20%
int ecran_x,ecran_y;
char mesaje_butoane[5][NMAX]={"Inapoi","Salvare","Executa","Undo","Redo"};
bool error[NR_ERORI];
int indice_info,indice_undo;
char mesaje_eroare[NR_ERORI][100]={"Blocul % nu este legat\n","Blocul % nu are o expresie corecta\n","Programul nu este legat la nici un bloc de tip stop\n",
"Programul nu are niciun bloc de tip start\n"};
char mesaje_rezultat[3][100]={"Variabila % are valoarea % ","Rezultatul blocului % este % "};

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

struct erori
{
    int bloc,nr;
    erori(int a, int b)
    {
        bloc=a;
        nr=b;
    }
};
vector<erori> S;

struct afisari
{
    int bloc,nr,val;
    char var;
    afisari(int a, int b, int c, char d='0' )
    {
        bloc=a;
        nr=b;
        val=c;
        var=d;
    }
};
vector<afisari> Rezultat;



struct blocuri
{
    int tip,nr;
    double x,y;
    double x1,y1;
    int width=100,height=50;
    int w1=100,h1=50;
    char text[201];
    bool ok=0;
    int st=-1,dr=-1;
    bool bucla;
    list<int> ant;
    blocuri(int a=0,int b=0,int c=0,int d=0,char s[NMAX]="")
    {
        tip=a;
        nr=b;
        x=x1=c;
        y=y1=d;
        width=w1+((indice_zoom-4)*w1)/zoom_ratio;
        height=h1+((indice_zoom-4)*h1)/zoom_ratio;
        if(tip==0) strcpy(text,"START");
        if(tip==1) strcpy(text,"STOP");
        if(tip>1) strcpy(text,s);
    }
};
blocuri a[NMAX];

struct schimbari
{ int tip;
  int bloc1,bloc2,x1,y1,x2,y2;
  bool dreapta;
  bool leg=0;
  char text1[NMAX],text2[NMAX];
  blocuri x;
  vector<bool> viz;
  schimbari(int a=0, int b=0, int c=0, int d=0, int e=0, int f=0, int g=0, bool h=0, char s1[]="", char s2[]="")
  {tip=a;bloc1=b;bloc2=c;
   x1=d;y1=e;
   x2=f;y2=g;
   dreapta=h;
   strcpy(text1,s1);
   strcpy(text2,s2);
  }
};
vector<schimbari> undo;

void deseneaza_start(int x, int y, char s[NMAX]="START", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    setcolor(BLACK);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s);
    rectangle(x,y,x+width,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,BLACK);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y+height,raza,raza);
}


void deseneaza_stop(int x, int y, char s[NMAX]="STOP", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    setcolor(BLACK);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s);
    rectangle(x,y,x+width,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,BLACK);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_intrare(int x, int y, char s[NMAX]="INTRARE", int width=100, int height=50)
{
    int dist=width/5,lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setcolor(BLACK);
    if(lung_text>width-dist)
    {
        int n=(width-dist)/textwidth("M");
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
{
    int dist=width/5,lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setcolor(BLACK);
    if(lung_text>width-dist)
    {
        int n=(width-dist)/textwidth("M");
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
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setcolor(BLACK);
    if(lung_text>width-45)
    {
        int n=(width-45)/textwidth("M");
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
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setcolor(BLACK);
//  setusercharsize()
    /// settextstyle(6,HORIZ_DIR,1);///0 6 9
    if(lung_text>width-10)
    {
        int n=(width-10)/textwidth("M");
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
{
    int x=a[i].x,y=a[i].y;
    //if(i>=6) x-=ecran_x,y-=ecran_y-colt_y;
    switch (a[i].tip)

    {
    case 0:
    {
        deseneaza_start(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    case 1:
    {
        deseneaza_stop(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    case 2:
    {
        deseneaza_intrare(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    case 3:
    {
        deseneaza_iesire(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    case 4:
    {
        deseneaza_decizie(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    case 5:
    {
        deseneaza_calcul(x,y,a[i].text,a[i].width,a[i].height);
        break;
    }
    }
}

bool apartine_dreptungi(int a, int b, int x, int y, int width=100, int height=50)
{
    if(a>x && a<x+width && b>y && b<y+height) return true;
    return false;
}

int arie(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/2;
}

bool apartine_intrare(int a, int b, int x, int y, int width=100, int height=50)
{
    if(a<x || a>x+width || b<y || b>y+height) return false;
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
{
    if(a<x || a>x+width || b<y || b>y+height) return false;
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
{
    if(a+b>x+y+height && x+(x+width-a)+b>x+y+height && b>y && b<y+height) return true;
    return false;
}

bool verifica_bloc(int x, int y, int i)
{
    //if(i>=6) x+=ecran_x,y+=ecran_y-colt_y;
    switch (a[i].tip)
    {
    case 2:
    {
        return apartine_intrare(x,y,a[i].x,a[i].y,a[i].width,a[i].height);
    }
    case 3:
    {
        return apartine_iesire(x,y,a[i].x,a[i].y,a[i].width,a[i].height);
    }
    case 4:
    {
        return apartine_decizie(x,y,a[i].x,a[i].y,a[i].width,a[i].height);
    }
    default :
        return apartine_dreptungi(x,y,a[i].x,a[i].y,a[i].width,a[i].height);
    }
}

bool apartine_zona(int x, int y, int width, int height)
{
    x-=ecran_x;
    y-=ecran_y-colt_y;
    int d=dist_leg;
    if(x>=colt_x && x+width<colt_x+lungime-d && y>=colt_y && y+height<colt_y+inaltime-d) return true;
    return false;
}

void afiseaza_toate_legaturile()
{
    for(int i=6; i<nr_blocuri; i++)
    {
        cout<<'\n'<<i<<" : "<<a[i].st<<' '<<a[i].dr<<"  ";
        for(list<int>::iterator it=a[i].ant.begin(); it!=a[i].ant.end(); it++)
            cout<<(*it)<<' ';
        cout<<'\n';
    }

}

void verifica_butoane(int x, int y)
{
    for(int i=0; i<=5; i++)
        if(verifica_bloc(x,y,i)) bloc_nou=a[i].tip;


}

void adauga_undo()
{
  while(undo.size()>indice_undo)
            undo.pop_back();
  indice_undo++;
}

void adauga(int i, int k, blocuri y)
{
    list<int>::iterator it;
    if(y.tip==0) nr_blocuri_start++;
    if(start_main<6) start_main=i;
    for(int j=6; j<nr_blocuri; j++)
    {
        if(a[j].st!=-1 && a[j].st>=i)
            a[j].st++;
        if(a[j].tip==4 && a[j].dr!=-1 && a[j].dr>=i)
            a[j].dr++;
        for(it=a[j].ant.begin(); it!=a[j].ant.end(); it++)
            if((*it)>=i) (*it)++;
    }
    nr_blocuri++;
    for(int j=nr_blocuri;j>i;j--)
        a[j]=a[j-1];
    a[i]=y;
    if(a[i].st!=-1)  a[a[i].st].ant.push_back(i);
    if(a[i].tip==4 && a[i].dr!=-1)  a[a[i].dr].ant.push_back(i);
    for(it=a[i].ant.begin(); it!=a[i].ant.end(); it++)
    {
        if(a[*it].tip==4 && undo[k].viz[*it])
            a[*it].dr=i;
           else a[*it].st=i;
    }
}

void sterge(int i, int ind=-1)
{
    if(ind==-1)   // stergere din optiuni
        {adauga_undo();
         undo.push_back({6,i});
         ind=undo.size()-1;
        }
    undo[ind].x=a[i];
    int k=undo[ind].bloc1;
    list<int>::iterator it1;
    undo[ind].viz.assign(nr_blocuri,0);
    for(it1=a[k].ant.begin(); it1!=a[k].ant.end(); it1++) // vizitam toate blocurile decizie care au legatura dreapta cu blocul de sters
        if(a[*it1].tip==4 && a[*it1].dr==k) undo[ind].viz[*it1]=1;

    list<int>::iterator it;
    if(a[i].tip==0) nr_blocuri_start--;
    if(i==start_main) start_main=0;
    for(it=a[i].ant.begin(); it!=a[i].ant.end(); it++)
    {
        if(a[*it].st==i) a[*it].st=-1;
        if(a[*it].tip==4 && a[*it].dr==i) a[*it].dr=-1;
    }
    for(int j=6; j<nr_blocuri; j++)
        a[j].ant.remove(i);
    for(int j=i; j<nr_blocuri-1; j++)
        a[j]=a[j+1];
    nr_blocuri--;
    for(int j=6; j<nr_blocuri; j++)
    {
        if(a[j].st!=-1 && a[j].st>i)
            a[j].st--;
        if(a[j].tip==4 && a[j].dr!=-1 && a[j].dr>i)
            a[j].dr--;
        for(it=a[j].ant.begin(); it!=a[j].ant.end(); it++)
            if((*it)>i) (*it)--;
    }

}

void sterge_legatura(int i, int j, bool dr)
{
    if(dr) a[i].dr=-1;
        else a[i].st=-1;
    a[j].ant.remove(i);
}

void adauga_legatura(int i, int j, bool dr)
{
    if(dr) a[i].dr=j;
        else a[i].st=j;
    a[j].ant.push_back(i);
}

void schimba_text(int i, char *s)
{
    strcpy(a[i].text,s);
}

void mutare_bloc(int i, int x, int y)
{
    int x1,y1;
    x1=((x+ecran_x)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
    y1=((y+ecran_y-colt_y)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
    a[i].x=x;
    a[i].y=y;
    a[i].x1=x1;
    a[i].y1=y1;
}

void verifica_optiuni(int i, int a1, int b, int x, int y)
{
    //x-=ecran_x;y-=ecran_y-colt_y;
    if(x+option_width>colt_x+lungime) x-=option_width+100;
    if(y+option_height>colt_y+inaltime) y-=y+option_height-colt_y-inaltime;
    if(a1>x+option_width || a1<x || b>y+option_height || b<y) return ;
    if(b>y+option_height/2)
    {
        sterge(i);
        return ;
    }
    bloc_nou=i;
}

void afiseaza_optiuni(int x, int y)
{
    int lung_text;
// x-=ecran_x;y-=ecran_y-colt_y;
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
{
    int x1=info_x+10,y1=info_y+10;
    setcolor(BLUE);
    rectangle(info_x,info_y,info_x+info_width,info_y+info_height);
    outtextxy(x1,y1,"Informatii");
    line(info_x,info_y+30,info_x+info_width,info_y+30);
    //rectangle(info_x+20,info_y+50,info_x+100,)
}

void desenare_buton(int left, int right, int culoare, char text[])
{
    int width=right-left,height=50,lung_text=textwidth(text);
    setcolor(BLUE);
    outtextxy(left+width/2-lung_text/2,height/2,text);
    rectangle(left,0,right,48);
    setfillstyle(SOLID_FILL,culoare);
    floodfill(left+1,1,BLUE);
}

void schimba_coord()
{
    for(int i=6; i<nr_blocuri; i++)
    {
        a[i].ok=0;
        if(apartine_zona(a[i].x,a[i].y,a[i].width,a[i].height))
            a[i].ok=1;
        a[i].x-=ecran_x;
        a[i].y-=ecran_y-colt_y;
    }
}

void refacere_coord()
{
    for(int i=6; i<nr_blocuri; i++)
    {
        a[i].ok=0;
        a[i].x+=ecran_x;
        a[i].y+=ecran_y-colt_y;
    }
}

void zoom_bloc(int nr, int i)
{
    a[i].x=a[i].x1+(nr*a[i].x1)/zoom_ratio;
    a[i].y=a[i].y1+(nr*a[i].y1)/zoom_ratio;
    a[i].width=a[i].w1+(nr*a[i].w1)/zoom_ratio;
    a[i].height=a[i].h1+(nr*a[i].h1)/zoom_ratio;
}

void zoom(int semn, int nr )
{
    for(int i=6; i<nr_blocuri; i++)
    {
        //cout<<a[i].x<<' '<<a[i].y<<' '<<a[i].width<<' '<<a[i].height<<' '<<' ';
        zoom_bloc(nr,i);
        //cout<<a[i].x<<' '<<a[i].y<<' '<<a[i].width<<' '<<a[i].height<<'\n';
    }

}

void verifica_zoom(int x, int y)
{
    refacere_coord();
    if(apartine_dreptungi(x,y,minus_x,minus_y,latura_zoom,latura_zoom) && indice_zoom>1)
    {
        indice_zoom--;
        raza--;
        distanta_intre_blocuri-=4;
        zoom(-1,indice_zoom-4);
        dist_leg=distanta_max_leg-distanta_max_leg/zoom_ratio;
    }
    if(apartine_dreptungi(x,y,plus_x,plus_y,latura_zoom,latura_zoom) && indice_zoom<6)
    {
        indice_zoom++;
        raza++;
        distanta_intre_blocuri+=4;
        zoom(1,indice_zoom-4);
        dist_leg=distanta_max_leg-distanta_max_leg/zoom_ratio;
    }
    schimba_coord();
}

void deseneaza_butoane_zoom()
{
    setcolor(BLACK);
    rectangle(minus_x,minus_y,minus_x+latura_zoom,minus_y+latura_zoom);
    line(minus_x+latura_zoom/10,minus_y+latura_zoom/2,minus_x+latura_zoom-latura_zoom/10,minus_y+latura_zoom/2);
    rectangle(plus_x,plus_y,plus_x+latura_zoom,plus_y+latura_zoom);
    line(plus_x+latura_zoom/2,plus_y+latura_zoom/10,plus_x+latura_zoom/2,plus_y+latura_zoom-latura_zoom/10);
    line(plus_x+latura_zoom/10,plus_y+latura_zoom/2,plus_x+latura_zoom-latura_zoom/10,plus_y+latura_zoom/2);
}

void verifica_mutare_ecran()
{
    char c;
    if(getch()==0) c=getch();
    if(c==72 || c==75 || c==77 || c==80)
    {
        c=(c-70)/3;/// 1-stanga 0-sus 2-dreapta 3-jos
        int di[4]= {0,-1,1,0},dj[4]= {-1,0,0,1},d=distanta_mutare_ecran;
        if(ecran_x+di[c]*d>=min_x_ecran && ecran_x+di[c]*d<=max_x_ecran && ecran_y+dj[c]*d>=min_y_ecran && ecran_y+dj[c]*d<=max_y_ecran )
        {
            refacere_coord();
            ecran_x+=di[c]*d,ecran_y+=dj[c]*d;
            event=1;
            schimba_coord();
        }
    }


}

void desenare_margine()
{ for(int i=0;i<=5;i++)
      desenare_bloc(i);
  int width=getmaxwidth()/nr_butoane;
  for(int i=0;i<nr_butoane;i++)
      desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);
  deseneaza_butoane_zoom();

}

void init()
{ int y=710,x=50;
  nr_blocuri=6;
  nr_blocuri_start=0;
  indice_zoom=4;
  raza=7;
  ecran_x=min_x_ecran;ecran_y=min_y_ecran;
  setbkcolor(fundal);clearviewport();
  deseneaza_info();
  a[0]={0,0,50,710,"START"};
  a[1]={1,0,200,710,"STOP"};
  a[2]={2,0,350,710,"INTRARE"};
  a[3]={3,0,500,710,"IESIRE"};
  a[4]={4,0,650,710,"DECIZIE"};
  a[5]={5,0,800,710,"CALCUL"};
  desenare_margine();
}

void sterge_info()
{
    setfillstyle(SOLID_FILL,fundal);
    floodfill(textbox.p1.x,textbox.p1.y,BLUE);
}

bool verifica_textbox(char text[])
{
    int lung=strlen(text);
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

bool apartine_left_right(int x, int y)
{ int d=dist_left,nr;
  if(apartine_dreptungi(x,y,left_x,left_y,latura_left,latura_left))
        { indice_info=max(indice_info-1,0);
          //cout<<indice_info<<' ';
            return true;
        }
  if(apartine_dreptungi(x,y,left_x+d,left_y,latura_left,latura_left))
        {if(eroare) nr=S.size()-1;
            //else nr=Rezultat.size()-1;
         // indice_info++;
         indice_info=min(indice_info+1,nr);
         //cout<<nr<<' '<<indice_info<<' ';
            return true;
        }
    return false;
}

void deseneaza_butoane_left_right(int culoare, int nr)
{ int d=dist_left;
  setcolor(BLACK);
  if(indice_info>0)  rectangle(left_x,left_y,left_x+latura_left,left_y+latura_left);
  if(indice_info<nr-1) rectangle(left_x+d,left_y,left_x+d+latura_left,left_y+latura_left);
  setcolor(culoare);
  setlinestyle(SOLID_LINE,1,2);
  if(indice_info>0)
    {line(left_x,left_y+latura_left/2,left_x+latura_left,left_y);
     line(left_x,left_y+latura_left/2,left_x+latura_left,left_y+latura_left);
    }
  if(indice_info<nr-1)
    {line(left_x+latura_left+d,left_y+latura_left/2,left_x+d,left_y);
     line(left_x+latura_left+d,left_y+latura_left/2,left_x+d,left_y+latura_left);
    }
  setlinestyle(SOLID_LINE,1,0);
}

void deseneaza_text(char text[], int caz=0, char mesaj[]="")
{ setcolor(BLACK);
  if(caz==0)
        {textbox.p1.x=info_x+10;textbox.p2.x=info_x+300;
         textbox.p1.y=info_y+50;
        }
    else
    {textbox.p1.x=info_x+10;textbox.p2.x=info_x+300;
     textbox.p1.y=info_y+60;textbox.p2.y=info_y+info_height;
    }
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
  if(caz==0)
    {textbox.p2.y=textbox.p1.y+height;
     rectangle(textbox.p1.x,textbox.p1.y,textbox.p2.x,textbox.p2.y);
    }
    else {int culoare=GREEN,nr=Rezultat.size();
          if(eroare) culoare=RED,nr=S.size();
            setcolor(culoare);
            outtextxy(info_x+10,info_y+35,mesaj);
            deseneaza_butoane_left_right(culoare,nr);
         }

}

bool apartine_text(int x,int y)
{
    if(x>textbox.p1.x && x<textbox.p2.x && y>textbox.p1.y && y<textbox.p2.y) return true;
    return false;
}

void citeste_text(int i)
{
    char c,s[NMAX];
    int n;
    strcpy(s,a[i].text);
    n=strlen(s);//cout<<n;
    while(c!=13)
    {
        c=getch();
        if(c==8 && n>0) n--,s[n]='\0',sterge_info();
        if(!verifica_textbox(s))
        {
            continue;
        }
        if(c!=8 && c!=13)  s[n++]=c,s[n]='\0';
        deseneaza_text(s);
    }
    //if(evalExpresie(s))

    strcpy(a[i].text,s);
    /*cout<<evalueazaExpresie(a[i].text)<<'\n';
       for(int j=0;j<26;j++) cout<<(char)('a'+j)<<' '<<variabile[j]<<'\n';*/
}

bool apartine_nod(int i, int a, int b, int x, int y, int &nod)
{
    if(abs(b-y)+abs(a-x)<=raza)
    {
        nod=i;
        return true;
    }
    return false;
}

bool verifica_toate_nodurile(int x, int y)
{
    //x+=ecran_x;y+=ecran_y-colt_y;
    int nod1=nod_st,nod2=nod_dr;
    bool ok=0;
    for(int i=6; i<nr_blocuri; i++)
        if(a[i].ok)
        {
            switch (a[i].tip)
            {
            case 0:
            {
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y+a[i].height,nod_st);
                break ;
            }
            case 1:
            {
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);
                break ;
            }
            case 4:
            {
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);
                ok+=apartine_nod(i,x,y,a[i].x,a[i].y+a[i].height,nod_st);
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width,a[i].y+a[i].height,nod_dr);
                break ;
            }
            default :
            {
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y+a[i].height,nod_st);
                ok+=apartine_nod(i,x,y,a[i].x+a[i].width/2,a[i].y,nod_dest);
                break ;
            }
            }
        }
    if(ok)
    {
        if(nod_st!=-1 && nod_dr!=-1)
        {
            if(nod1!=-1) nod_st=-1;
            if(nod2!=-1) nod_dr=-1;
        }
        return true ;
    }
    else nod_dest=nod_st=nod_dr=-1;

    return false;
}


int intersectie(linie l,int nod) // returneaza 1 daca ii o intersectie verticala, 2 daca ii o intersectie orizontala, 0 altfel
{
    int left = a[nod].x, right = a[nod].x + a[nod].width, top = a[nod].y, bottom = a[nod].y + a[nod].height; // coordonatele blocului nod

    if(l.p1.x == l.p2.x) // daca linia l verticala si intersecteaza blocul nod, returneaza 1
        // expresia cu sau trateaza cazurile in care linia merge de sus in jos, dar si de jos in sus
        if(left <= l.p1.x && l.p1.x <= right && ((l.p1.y < top && top < l.p2.y) || (l.p2.y < bottom && bottom < l.p1.y)))
            return 1;
        else
            return 0;

    else // daca linia l orizontala si intersecteaza blocul nod, returneaza 2
        //expresia cu sau trateaza cazurile in care linia merge de la stanga la dreapta, dar si de la dreapta spre stanga
        if(top <= l.p1.y && l.p1.y <= bottom && ((l.p1.x < left && left < l.p2.x) || (l.p2.x < right && right < l.p1.x)))
            return 2;
        else
            return 0;
}

void sorteaza(int v[],int n,int tip)
{
    // sortam crescator dupa y, daca tip = 1, altfel daca tip = 2, atunci sortam dupa x crescator
    bool schimb = true;
    do
    {
        schimb = false;
        for(int i = 1; i<=n-1; i++)
            if( (tip == 1 && a[v[i]].y >= a[v[i+1]].y) || (tip == 2 && a[v[i]].x >= a[v[i+1]].x) )
            {
                int aux = v[i];
                v[i] = v[i+1];
                v[i+1] = aux;
                schimb = true;
            }
    }
    while(schimb);
}

void inverseaza(int v[],int n)
{
    for(int i=1; i<=n/2; i++)
    {
        int aux = v[i];
        v[i] = v[n-i+1];
        v[n-i+1] = aux;
    }
}

void ocolireVertical(linie& l,linie& aux,int px,int i)
{
    aux.p2.x = aux.p1.x; // ramanem pe acelasi x

    if(px > 0) // daca linia vine din jos in sus
        aux.p2.y = (a[i].y + a[i].height) + px;
    else // daca linia vine din sus in jos
        aux.p2.y = a[i].y + px;

    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y); // trasam prima parte, linia verticala dinaintea blocului

    aux.p1 = aux.p2;

    if(px > 0) // daca linia vine din jos in sus
        if(l.p1.x <= a[i].x + a[i].width / 2) // daca i mai convenabil prin stanga
            aux.p2.x = a[i].x - px;
        else                                  // daca i mai convenabil prin dreapta
            aux.p2.x = a[i].x + a[i].width + px;
    else       // daca linia vine din sus in jos
        if(l.p1.x <= a[i].x + a[i].width / 2) // daca i mai convenabil prin stanga
            aux.p2.x = a[i].x + px;
        else                                  //daca i mai convenabil prin dreapta
            aux.p2.x = a[i].x + a[i].width - px;

    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y); // trasam partea a doua, linia orizontala care ocoleste blocul

    aux.p1 = aux.p2;

    if(px > 0) // daca linia vine din jos in sus
        aux.p2.y = a[i].y - px;
    else
        aux.p2.y = a[i].y + a[i].height - px;

    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y); // trasam partea a treia, linia verticala care ocoleste blocul

    aux.p1 = aux.p2;

    aux.p2.y = aux.p1.y; // ramanem pe aceasi orizontala
    aux.p2.x = l.p2.x;  // schimbam x-ul

    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y); // trasam ultima parte, care leaga linia de ocolire, cu punctul prin care ar fi trebuit sa treaca linia, daca nu exista blocul i

    aux.p1 = aux.p2;
}

bool punctInterior(linie& l,int i)
{
    return (a[i].x <= l.p2.x && l.p2.x <= a[i].x+a[i].width && a[i].y <= l.p2.y && l.p2.y <= a[i].y+a[i].height);
}

void ocolireVerticalaDesteapta(linie& l1,int i,linie* next,int px,int y)
{

    l1.p2.x = l1.p1.x; // ramanem pe aceasi verticala

    if(px > 0) // daca mergem din jos in sus
        l1.p2.y = a[i].y + a[i].height + px;
    else // daca mergem din sus in jos
        l1.p2.y = a[i].y + px;

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam prima parte, pana la inceputul blocului

    l1.p1 = l1.p2;

    if(next->p1.x >= next->p2.x) // daca linia urmatoare o ia spre stanga
        if(px > 0) // daca mergem din jos in sus
            l1.p2.x = a[i].x - px; //
        else // daca mergem din sus in jos
            l1.p2.x = a[i].x + px;
    else // daca linia urmatoare o ia spre dreapta
        if(px > 0) // daca mergem din jos in sus
            l1.p2.x = a[i].x + a[i].width + px; //
        else // daca mergem din sus in jos
            l1.p2.x = a[i].x + a[i].width - px;

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam a doua parte, pana ce ocolim pe orizontala

    l1.p1 = l1.p2;

    l1.p2.x = l1.p1.x; // ramanem pe aceasi verticala
    l1.p2.y = y; // mentinem y-ul punctului in care ar fi trebuit sa ajungem, care s-ar fi aflat in interiorul ultimului bloc

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam a treia parte, ca sa ajungem in punctul de coordonate y corespunzator, dar in afara blocului

}


void trasareVertical(linie& l,linie* next) //traseaza o linie pe verticala, verificand si evitand eventuale blocuri aflate pe aceasta directie
{
    int v[10]; // vector in care se vor stoca indicele blocurilor care s-ar afla peste linia trasata
    int len = 0; // initial nu exista blocuri care se suprapun cu linia
    int px = a[5].width / 10; //
    linie aux;

    for(int i=6; i<nr_blocuri; i++) // parcurgem toate blocurile care nu se afla in meniul de jos
    {
        int tipIntersectie = intersectie(l,i); // functia intersectie ne va returna tipul intersectiei dintre o linie si un dreptunghi de indice i (0 - niciuna,1 - vertical, 2 - orizontal)
        if(tipIntersectie == 1) // daca avem o intersectie pe verticala a blocului i, atunci il adaugam in vector
            v[++len] = i;
    }

    sorteaza(v,len,1); // sortam blocurile in functie de coordonata y(tratam cazul in care avem mai multe blocuri pe directia liniei)

    if(l.p1.y >= l.p2.y) // daca linia merge de jos in sus atunci inversam vectorul, deaoarece initial vectorul este ordonat crescator
        inverseaza(v,len);
    else
        px *= -1; // daca linia merge de sus in jos, atunci px va fi negativ

    aux.p1.x = l.p1.x;
    aux.p1.y = l.p1.y;

    for(int i=1; i<len; i++) // parcurge blocurile cu care se intersecteaza, fara ultimul bloc (sfarsitul liniei s-ar putea sa se afle in interiorul acestui bloc)
        ocolireVertical(l,aux,px,v[i]); // ocoleste blocul v[i]

    if(len == 0) // daca nu exista bloc cu care linia se intersecteaza
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);
    else if(next != NULL && punctInterior(l,v[len])) // verificam daca i o linie care are punctul destinatie intr-un bloc
    {
        ocolireVerticalaDesteapta(aux,v[len],next,px,l.p2.y); // ocoleste destept acest obstacol (punctul se va afla pe acelasi y cu punctul in care ar fi trebuit sa fie, dar x ul este diferit in functie de linia urmatoare)
        l = aux; // actualizam inceputul liniei urmatoare
    }
    else // daca nu are punct interior atunci il ocolim ca celelalte blocuri
    {
        ocolireVertical(l,aux,px,v[len]);
        line(aux.p2.x,aux.p2.y,l.p2.x,l.p2.y); // il legam cu punctul in care ar trebui sa ajunga linia
    }
    l.p1 = l.p2; // copiem p2 in p1, din acest punct incepem urmatoarea linie
}

void ocolireOrizontal(linie& l,linie& aux,int px,int i)
{
    aux.p2.y = aux.p1.y;
    if(px > 0)
        aux.p2.x = a[i].x - px;
    else
        aux.p2.x = a[i].x + a[i].width - px;

    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y);

    aux.p1 = aux.p2;
    if(px > 0)
    {
        if(l.p1.y <= a[i].y + a[i].height / 2 )
            aux.p2.y = a[i].y - px;
        else
            aux.p2.y = a[i].y + a[i].height + px;
    }
    else
    {
        if(l.p1.y <= a[i].y + a[i].height / 2)
            aux.p2.y = a[i].y + px;
        else
            aux.p2.y = a[i].y + a[i].height - px;
    }
    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y);

    aux.p1 = aux.p2;
    aux.p2.y = aux.p1.y;
    if(px > 0)
    {
        aux.p2.x = a[i].x + a[i].width + px;
    }
    else
    {
        aux.p2.x = a[i].x + px;
    }
    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y);


    aux.p1 = aux.p2;
    aux.p2.x = aux.p1.x;
    aux.p2.y = l.p1.y;
    line(aux.p1.x,aux.p1.y,aux.p2.x,aux.p2.y);

    aux.p1 = aux.p2;
}

void ocolireOrizontalaDesteapta(linie& l1,int i,linie* next,int px,int x)
{
    l1.p2.y = l1.p1.y; // ramanem pe aceasi verticala

    if(px < 0) // daca mergem din jos in sus
        l1.p2.x = a[i].x + a[i].width - px;
    else // daca mergem din sus in jos
        l1.p2.x = a[i].x - px;

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam prima parte, pana la inceputul blocului

    l1.p1 = l1.p2;

    if(next->p1.y >= next->p2.y) // daca linia urmatoare o ia in sus
        if(px < 0) // daca mergem din jos in sus
            l1.p2.y = a[i].y + px; //
        else // daca mergem din sus in jos
            l1.p2.y = a[i].y - px;
    else // daca linia urmatoare o ia spre dreapta
        if(px < 0) // daca mergem din jos in sus
            l1.p2.y = a[i].y + a[i].height - px; //
        else // daca mergem din sus in jos
            l1.p2.y = a[i].y + a[i].height + px;

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam a doua parte, pana ce ocolim pe orizontala

    l1.p1 = l1.p2;

    l1.p2.y = l1.p1.y; // ramanem pe aceasi verticala
    l1.p2.x = x; // mentinem y-ul punctului in care ar fi trebuit sa ajungem, care s-ar fi aflat in interiorul ultimului bloc

    line(l1.p1.x,l1.p1.y,l1.p2.x,l1.p2.y); // trasam a treia parte, ca sa ajungem in punctul de coordonate y corespunzator, dar in afara blocului
}

void trasareOrizontal(linie& l,linie* next)
{
    int v[10]; // vector in care se vor stoca indicele blocurilor care s-ar afla peste linia trasata
    int len = 0; // initial nu exista blocuri care se suprapun cu linia
    int px = a[5].width / 10; //
    linie aux;

    for(int i=6; i<nr_blocuri; i++)
    {
        int tipIntersectie = intersectie(l,i);
        if(tipIntersectie == 2)
            v[++len] = i;
    }

    sorteaza(v,len,2); // sorteaza dupa x in ordine crescatoare

    if(l.p1.x >= l.p2.x) // daca linia merge de la dreapta spre stanga atunci inversam vectorul, deaoarece initial vectorul este ordonat crescator
    {
        px *= -1;
        inverseaza(v,len);
    }

    aux.p1.x = l.p1.x;
    aux.p1.y = l.p1.y;

    for(int i=1; i<len; i++)
        ocolireOrizontal(l,aux,px,v[i]);

    if(len == 0) // daca nu exista bloc cu care linia se intersecteaza
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);
    else if(next != NULL && punctInterior(l,v[len])) // verificam daca i o linie care are punctul destinatie intr-un bloc
    {
        ocolireOrizontalaDesteapta(aux,v[len],next,px,l.p2.x); // ocoleste destept acest obstacol (punctul se va afla pe acelasi y cu punctul in care ar fi trebuit sa fie, dar x ul este diferit in functie de linia urmatoare)
        l = aux; // actualizam inceputul liniei urmatoare
    }
    else // daca nu are punct interior atunci il ocolim ca celelalte blocuri
    {
        ocolireOrizontal(l,aux,px,v[len]);
        line(aux.p2.x,aux.p2.y,l.p2.x,l.p2.y); // il legam cu punctul in care ar trebui sa ajunga linia
    }
    l.p1 = l.p2;
}

void trasare_legatura(int nod1,int nod2,int tip,int dr)
{
    punct pStart,pStop;  // punctele de inceput si final
    setcolor(line_color);
    linie l[5],aux;      // legatura este impartita in mai multe linii orizontale/verticale
    int px = a[nod1].width / 10;

    if(tip == 4) // daca este un bloc de decizie (are doua noduri de iesire aflate in colturi)
    {
        if(!dr) // daca am selectat blocul din stanga
            pStart.x = a[nod1].x;
        else    //daca am selectat blocul din dreapta
            pStart.x = a[nod1].x + a[nod1].width;
        pStart.y = a[nod1].y + a[nod1].height; // coordonata y ramane aceeasi
    }
    else // orice alt tip de bloc porneste cu un nod aflat in mijloc
    {
        pStart.x = a[nod1].x + a[nod1].width / 2;
        pStart.y = a[nod1].y + a[nod1].height;
    }

    // coordonatele destinatie ale legaturii (mereu se afla in mijlocul blocului destinatie)
    pStop.x = a[nod2].x + a[nod2].width / 2;
    pStop.y = a[nod2].y;

    // coordonate folosite pentru a detecta cazul in care blocul destinatie se afla strict deasupra blocului sursa
    // cx1,cx2 reprezinta segmentul aflat la baza nodului sursa, iar cy1,cy2 reprezinta segmentul aflat la baza nodului destinatie
    int cx1 = a[nod1].x, cx2 = a[nod1].x + a[nod1].width;
    int cy1 = a[nod2].x, cy2 = a[nod2].x + a[nod2].width;

    if(pStart.y <= pStop.y) // daca blocul destinatie se afla sub blocul sursa (legatura merge in jos)
    {
        // prima linie merge pana la mijlocul distantei verticale dintre cele doua blocuri
        l[0].p1 = pStart;
        l[0].p2.x = pStart.x;
        l[0].p2.y = (pStart.y + pStop.y) / 2;

        // a doua linie va merge pe orizontala pana ajunge deasupra nodului destinatie
        l[1].p1 = l[0].p2;
        l[1].p2.x = pStop.x;
        l[1].p2.y = l[0].p1.y;

        // a treia linie se conecteaza cu nodul destinatie
        l[2].p1 = l[1].p2;
        l[2].p2 = pStop;

        // incepem trasarea linilor, care pot suferi modificari din cauza unor suprapuneri cu alte blocuri (de asta se memoreaza linile in l)
        aux = l[0];

        trasareVertical(aux,&l[1]);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p1.y;

        trasareOrizontal(aux,&l[2]);

        aux.p2 = l[2].p2;
        trasareVertical(aux,NULL);
    }

    else if(pStart.y > pStop.y && cx2 >= cy1 && cy2 >= cx1)
    {

        l[0].p1 = pStart;
        l[0].p2.x = pStart.x;
        l[0].p2.y = pStart.y + px;

        l[1].p1 = l[0].p2;
        if(pStop.x <= pStart.x)
            l[1].p2.x = min(a[nod1].x,a[nod2].x) - px;
        else
            l[1].p2.x = max(a[nod1].x + a[nod1].width,a[nod2].x + a[nod2].width) + px;
        l[1].p2.y = l[0].p2.y;

        l[2].p1 = l[1].p2;
        l[2].p2.x = l[1].p2.x;
        l[2].p2.y = a[nod2].y - px;

        l[3].p1 = l[2].p2;
        l[3].p2.x = pStop.x;
        l[3].p2.y = l[2].p2.y;

        l[4].p1 = l[3].p2;
        l[4].p2 = pStop;

        //trasareVertical(l);

        aux = l[0];

        trasareVertical(aux,&l[1]);

        if(pStop.x <= pStart.x)
            aux.p2.x = min(a[nod1].x,a[nod2].x) - px;
        else
            aux.p2.x = max(a[nod1].x + a[nod1].width,a[nod2].x + a[nod2].width) + px;
        aux.p2.y =  l[0].p2.y;;

        trasareOrizontal(aux,&l[2]);

        aux.p2.x = aux.p1.x;
        aux.p2.y = a[nod2].y - px;

        trasareVertical(aux,&l[3]);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p2.y;

        trasareOrizontal(aux,&l[4]);

        aux.p2 = pStop;

        trasareVertical(aux,NULL);

    }
    else
    {
        int dist = abs((a[nod2].x + a[nod1].x + a[nod1].width) / 2 );

        l[0].p1 = pStart;
        l[0].p2.x = pStart.x;
        l[0].p2.y = pStart.y + px;

        //trasareVertical(l);
        l[1].p1 = l[0].p2;
        l[1].p2.x = dist;
        l[1].p2.y = l[0].p1.y;

        //trasareOrizontal(l);

        l[2].p1 = l[1].p2;
        l[2].p2.x = l[1].p1.x;
        l[2].p2.y = pStop.y - px;
        //trasareVertical(l);

        l[3].p1 = l[2].p2;
        l[3].p2.x = pStop.x;
        l[3].p2.y = l[2].p2.y;

        //trasareOrizontal(l);

        l[4].p1 = l[3].p2;
        l[4].p2 = pStop;

        //trasareOrizontal(l);
        aux = l[0];
        trasareVertical(aux,&l[1]);

        aux.p2.x = dist;
        aux.p2.y = aux.p1.y;

        trasareOrizontal(aux,&l[2]);

        aux.p2.x = aux.p1.x;
        aux.p2.y = pStop.y - px;

        trasareVertical(aux,&l[3]);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p2.y;

        trasareOrizontal(aux,&l[4]);

        aux.p2 = pStop;

        trasareVertical(aux,NULL);
    }
}

void deseneaza_legaturi()
{
    for(int i=6; i<nr_blocuri; i++) // parcurgem toate blocurile care nu se afla in meniul de jos
       // if(a[i].ok)
        {
            if(a[i].st !=-1)// && a[a[i].st].ok)
                trasare_legatura(i,a[i].st,a[i].tip,0); // legatura stanga

            if(a[i].dr!=-1)// && a[a[i].dr].ok)
                trasare_legatura(i,a[i].dr,a[i].tip,1); // legatura dreapta
        }
}


void deseneaza_schema()
{ setcolor(BLUE);
  rectangle(0,colt_y,lungime,colt_y+inaltime);
  setfillstyle(SOLID_FILL,fundal);
  floodfill(500,100,BLUE);
  deseneaza_legaturi();
  /// nu se mai redeseneaza marginea daca liniile astea sunt comentate
  setcolor(BLUE);
  rectangle(0,colt_y,lungime,colt_y+inaltime);
  setfillstyle(SOLID_FILL,fundal);
  floodfill(801,711,BLUE);
  desenare_margine();
  //
  for(int i=6;i<nr_blocuri;i++)
     if(a[i].ok)
          desenare_bloc(i);
}

int verifica_toate_blocurile(int x, int y)
{
    for(int i=6; i<nr_blocuri; i++)
        if(a[i].ok && verifica_bloc(x,y,i)) return i;
    return -1;
}

void click_optiuni() /// click pe optiunile muta/sterge
{
    int x=mousex(),y=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
    verifica_optiuni(selectat,x,y,a[selectat].x+a[selectat].width,a[selectat].y);
    event=1;
    selectat=-1;
    options=0;
}

void click_dreapta_pe_bloc()
{
    int x=mousex(),y=mousey();
    afiseaza_optiuni(a[selectat].x+a[selectat].width,a[selectat].y);
    clearmouseclick(WM_RBUTTONDOWN);
    event=0;
    options=1;
}

void anuleaza() /// click dreapta cand se muta un bloc pentru a anula actiunea
{
    clearmouseclick(WM_RBUTTONDOWN);
    bloc_nou=-1;
    event=1;
}

void mutare(int i)
{
    int x=mousex(),y=mousey();
    a[nr_blocuri+1]= {a[i].tip,1,x,y,a[i].text};
    x-=a[nr_blocuri+1].width/2;
    a[nr_blocuri+1].x=x;
    if(apartine_zona(x+ecran_x,y+ecran_y-colt_y,a[nr_blocuri+1].width,a[nr_blocuri+1].height))
    {
        desenare_bloc(nr_blocuri+1);
        event=1;
    }
}

void inapoi()
{
    cleardevice();
    setbkcolor(BLACK);
    clearviewport();
    ecrane[0] = 1;
    ecrane[1] = 0;
    ecranCurent = 0;
    ecran_schimbat=1;
}

void number_to_text(int n, char* s)
{ int nr=0;
  do
  { s[nr++]='0'+(n%10);
    n/=10;
  }while(n);
  s[nr]='\0';
  for(int i=0;i<nr/2;i++)
  {int aux=s[i];
   s[i]=s[nr-i-1];
   s[nr-i-1]=aux;
  }
}

void replace_var(int n, char text[])
{
 char *p,s[NMAX],aux[NMAX];
 number_to_text(n,aux);
 strcpy(s,text);
 p=strchr(s,'%');
 p[0]='\0';
 strcpy(text,s);
 strcat(text,aux);
 strcat(text,p+1);

}

void afiseaza_erori()
{ //S.push({21,0});S.push({6110,1});
 sterge_info();
 int j=indice_info;
 char aux[NMAX],text[NMAX];
 strcpy(text,"EROARE   ");
 number_to_text(j+1,aux);
 strcat(text,aux);
 strcat(text,"/");
 number_to_text(S.size(),aux);
 strcat(text,aux);
    if(S[j].bloc==0)
        { deseneaza_text(mesaje_eroare[S[j].nr],1,text);
        }
    else
        {int bloc=S[j].bloc,nr=S[j].nr;
         if(nr<=1)
            {char text1[NMAX];
             strcpy(text1,mesaje_eroare[S[j].nr]);
             replace_var(bloc,text1);
             deseneaza_text(text1,1,text);
            }
        }
}

int variabile_iesire(char text[])
{
  for(int i=0;i<strlen(text);i++)
        if(text[i]>='a' && text[i]<='z')// && (i+1==strlen(text) || text[i+1]==',' || text[i+1]=='=') )
            return i;
  return -1;
}

void afiseaza_rezultat()
{ /*for(int j=0;j<Rezultat.size();j++)
    { ////cout<<mesaje_rezultat[Rezultat[j].caz]<<Rezultat[j].val<<'\n';
    }*/
 event=0;
 sterge_info();
 int j=indice_info;
 char aux[NMAX],text[NMAX];
 strcpy(text,"REZULTAT   ");
 number_to_text(j+1,aux);
 strcat(text,aux);
 strcat(text,"/");
 number_to_text(Rezultat.size(),aux);
 strcat(text,aux);

 if(Rezultat.size())
    {int bloc=Rezultat[j].bloc,nr=Rezultat[j].nr,rez=Rezultat[j].val;
     char *p,text1[NMAX];
     strcpy(text1,mesaje_rezultat[nr]);
     if(nr==0)
     { p=strchr(text1,'%');
       p[0]=Rezultat[j].var;
     }
     if(nr==1) replace_var(bloc,text1);
     replace_var(rez,text1);
     deseneaza_text(text1,1,text);
    }

  outtextxy(info_x+10,info_y+info_height+10,"Variabile");
  setcolor(BLACK);
  for(int j=0;j<26;j++)
  { char text[NMAX];
    text[0]='a'+j;
    text[1]=':';
    text[2]='\0';
    number_to_text(variabile[j],text+2);
    outtextxy(info_x+10+j/13*(info_width/2),info_y+10+info_height+(j%13+1)*20,text);
  }

}

void verifica_erori_desen()
{ if(start_main<6) S.push_back({0,3});
  bool stop=0;
  for(int i=6;i<nr_blocuri;i++)
     if(a[i].tip!=0)
     {   int nr_leg=0;
         int nr_total_leg=2;
         if(a[i].tip==1)
            { if(!a[i].ant.empty()) stop=1;
              continue ;
            }
         if(a[i].tip>1)
             {if(a[i].st!=-1) nr_leg++;
              if(!a[i].ant.empty()) nr_leg++;
             }
         if(a[i].tip==4)
         {nr_total_leg=3;
          if(a[i].dr!=-1) nr_leg++;
         }
         if(nr_total_leg!=nr_leg && nr_leg!=0) S.push_back({i,0});
     }
  if(!stop) S.push_back({0,2});
  if(S.size()) eroare=1;
}

void salvare()
{
    afiseaza_toate_legaturile();
}



void cautaVariabile(int variabileSetate[],int& nrVar,set<int>& vizitate,int nod)
{
    if(vizitate.find(nod) == vizitate.end())
    {
        vizitate.insert(nod);
        if(a[nod].tip > 1)
        {
            int len = strlen(a[nod].text);
            for(int i=0; i<len; i++)
            {
                char ch = a[nod].text[i];
                if(isalpha(ch))
                {
                    if(!variabileSetate[ch-'a'])
                    {
                        variabileSetate[ch-'a'] = 1;
                        nrVar++;
                    }
                }
            }
        }

        if(a[nod].st != -1)
        {
            cautaVariabile(variabileSetate,nrVar,vizitate,a[nod].st);
        }
        if(a[nod].dr != -1)
        {
            cautaVariabile(variabileSetate,nrVar,vizitate,a[nod].dr);
        }
    }
}

void adauga_variabile(string& cod,int nod)
{
    int variabileSetate[26] = {0};
    set<int> vizitate;
    int nrVar = 0;

    cautaVariabile(variabileSetate,nrVar,vizitate,nod);
    if(nrVar)
    {
        cod += "float ";
    }
    for(int i=0; i<26; i++)
    {
        if(variabileSetate[i])
        {
            cod += 'a' + i;
            nrVar--;
            if(nrVar)
                cod += ",";
            else
                cod += ";";
        }
    }
}

int depth = 0;

void adaugaTabs(string& cod)
{
    for(int i=1; i<=2*depth; i++)
        cod += "  ";
}


bool varificaBucla(int nod,int start,set<int>& vizitate)
{
    if(vizitate.find(nod) == vizitate.end())
    {
        vizitate.insert(nod);

        if(a[nod].bucla)
            return false;

        if(a[nod].tip == 1)
        {
            return false;
        }
        else
        {
            bool left = false, right = false;
            if(a[nod].st != -1)
            {
                left = varificaBucla(a[nod].st,start,vizitate);
            }
            if(a[nod].dr != -1)
            {
                right = varificaBucla(a[nod].dr,start,vizitate);
            }
            return left || right;
        }
    }
    else if(nod == start)
    {
        a[nod].bucla++;
        return true;

    }
    else
    {
        return false;
    }
}

void adaugaDoWhile(string& cod,int nod)
{
    set<int> vizitate;
    if(varificaBucla(nod,nod,vizitate))
    {


        for(int i=1; i<=a[nod].bucla; i++)
        {
            adaugaTabs(cod);
            depth++;
            cod += "do {\n\n";
        }
    }
}
void adaugaCin(string& cod,int nod)
{
    adaugaDoWhile(cod,nod);

    int len = strlen(a[nod].text);
    int variabile[26] = {0};
    queue<char> q;

    for(int i=0; i<len; i++)
    {
        char ch = a[nod].text[i];
        if(isalpha(ch))
        {
            if(variabile[ch-'a'])
            {
                eroare = 1;
            }
            else
            {
                variabile[ch-'a'] = 1;
                q.push(ch);
            }
            if(i > 1 && isalpha(a[nod].text[i-1]))
            {
                eroare = 1; // nu pot aparea variabile de lungime mai mare decat 2;
            }
        }
        else if(ch == ',' && (i == 0 || a[nod].text[i-1] == ',' || i == len - 1)) // doua virgule una dupa alta, sau virgula la inceput sau sfarsit
        {
            eroare = 1;
        }
        else
        {
            eroare = 1;
        }
    }

    if(!q.empty())
    {
        adaugaTabs(cod);
        cod += "cin>>";
        while(!q.empty())
        {
            cod += q.front();
            q.pop();
            if(!q.empty())
                cod += ">>";
            else
                cod += ";";
        }
        cod += "\n\n";
    }
}

void adaugaCout(string& cod,int nod)
{
    adaugaDoWhile(cod,nod);

    int len = strlen(a[nod].text);
    int paranteze = 0;

    if(len)
    {
        adaugaTabs(cod);
        for(int i=0; i<len; i++)
        {
            char ch = a[nod].text[i];
            if((ch == '>' || ch == '<') && !paranteze )
            {
                eroare = 1;

            }
            else if(ch == '(')
            {
                paranteze++;
            }
            else if(ch == ')')
            {
                paranteze--;
            }
        }

        cod += "cout<<";
        cod += a[nod].text;
        cod += ";\n\n";
    }
}

void adaugaBloc(string& cod,int nod)
{
    set<int> vizitate;
    if(varificaBucla(nod,nod,vizitate))
    {
        adaugaTabs(cod);
        depth++;
        cod += "while(";
        cod += a[nod].text;
        cod += ") {\n\n";
    }
    else
    {
        adaugaTabs(cod);
        depth++;
        cod += "if(";
        cod += a[nod].text;
        cod += ") {\n\n";
    }
}

void adaugaExpresie(string& cod,int nod)
{
    adaugaDoWhile(cod,nod);

    if( strlen(a[nod].text) > 0)
    {
        adaugaTabs(cod);
        cod += a[nod].text;
        cod += ";";
        cod += "\n\n";
    }

}

void genereaza_mesaj(string& cod,set<int>& vizitate,int nod)
{
    if(vizitate.find(nod) != vizitate.end())
    {
        depth--;
        adaugaTabs(cod);
        cod += "}\n\n";
    }
    else
    {
        vizitate.insert(nod);
        if(a[nod].tip == 0)
        {
            cod = "#include<iostream>\n\nusing namespace std;\n\n";
            adauga_variabile(cod,nod);
            cod += "\n\nint main() {\n\n";
            depth++;
            if(a[nod].st != -1)
                genereaza_mesaj(cod,vizitate,a[nod].st);
        }
        else if(a[nod].tip == 1)
        {

            if(depth == 1)
            {
                cod += "return 0;\n\n";
            }

            depth--;
            adaugaTabs(cod);
            cod += "}\n\n";

        }
        else if(a[nod].tip == 2)
        {
            adaugaCin(cod,nod);
            if(a[nod].st != -1)
            {
                genereaza_mesaj(cod,vizitate,a[nod].st);
            }
        }
        else if(a[nod].tip == 3)
        {
            adaugaCout(cod,nod);
            if(a[nod].st != -1)
            {
                genereaza_mesaj(cod,vizitate,a[nod].st);
            }
        }
        else if(a[nod].tip == 4)
        {
            if(a[nod].st != -1 && a[a[nod].st].bucla && a[a[nod].st].tip != 4)
            {

                depth--;
                adaugaTabs(cod);
                cod += "}while(";
                cod += a[nod].text;
                cod += ");\n\n";
            }
            else
            {
                adaugaBloc(cod,nod);
                if(a[nod].st != -1)
                {
                    genereaza_mesaj(cod,vizitate,a[nod].st);
                }
            }


            if(a[nod].dr != -1)
            {
                genereaza_mesaj(cod,vizitate,a[nod].dr);
            }
        }
        else
        {
            adaugaExpresie(cod,nod);
            if(a[nod].st != -1)
            {
                genereaza_mesaj(cod,vizitate,a[nod].st);
            }
        }
    }
}

void executa()
{ eroare=rezult=0;//cout<<evalueazaExpresie("a=2+3");
  verifica_erori_desen();
  int i=start_main,rez;
  while(a[i].tip!=1 && !eroare)
  { if(a[i].tip>1)
        rez=evalueazaExpresie(a[i].text);
     if(!eroare)
            { if(a[i].tip==3)
                {int poz=variabile_iesire(a[i].text);
                 if(poz==-1) Rezultat.push_back({i,1,rez});
                 while(poz!=-1)
                          {Rezultat.push_back({i,0,rez,a[i].text[poz]});
                           poz=variabile_iesire(a[i].text+poz+1);
                          }
                }
              if(a[i].st!=-1)  i=a[i].st;
                  else eroare=1;
              if(a[i].tip==4)
                 {if(a[i].dr!=-1)
                      i=a[i].dr;
                  else eroare=1;

                 }
            }
  }
  if(!eroare) rezult=1,event=0;
    string cod;
    set<int> vizitate;

    for(int i=6; i<nr_blocuri; i++)
        a[i].bucla = false;

    genereaza_mesaj(cod,vizitate,start_main);
    cout<<'\n'<<'\n'<<cod;

}



void Undo(int Redo=0)
{
  int j=indice_undo-1;//cout<<j<<' ';
  if(j+1)
    { int tip=undo[j].tip;
      if(Redo) tip=7-tip;//cout<<tip<<'\n';
      switch (tip)
      { case 0:
          { mutare_bloc(undo[j].bloc1,undo[j].x1,undo[j].y1);
              break;
          }
        case 1:
          {
            sterge(undo[j].bloc1,j);
              break;
          }
        case 2:
          { sterge_legatura(undo[j].bloc1,undo[j].bloc2,undo[j].dreapta);
            if(undo[j].leg)  adauga_legatura(undo[j-1].bloc1,undo[j-1].bloc2,undo[j-1].dreapta);
              break;
          }
        case 3:
          { schimba_text(undo[j].bloc1,undo[j].text1);
              break;
          }
        case 4:
          { schimba_text(undo[j].bloc1,undo[j].text2);
              break;
          }
        case 5:
          { if(undo[j].leg) sterge_legatura(undo[j-1].bloc1,undo[j-1].bloc2,undo[j-1].dreapta);
            adauga_legatura(undo[j].bloc1,undo[j].bloc2,undo[j].dreapta);
              break;
          }
        case 6:
          { adauga(undo[j].bloc1,j,undo[j].x);
              break;
          }
        case 7:
          { mutare_bloc(undo[j].bloc1,undo[j].x2,undo[j].y2);
              break;
          }
      }
    }
  if(!Redo && j+1) indice_undo--;
}

void redo()
{
    if(indice_undo>=undo.size()) return ;
    indice_undo++;
    Undo(1);
}


void buton(int i)
{
    switch (i)
    {
    case 0:
        {inapoi();
         break ;
        }
    case 1:
        {salvare();
         break ;
        }
    case 2:
        {executa();
         break ;
        }
    case 3:
        {Undo();
         break ;
        }
    case 4:
        {redo();
         break ;
        }
    }

}

void bara_meniu(int x, int y)
{
    int width=getmaxwidth()/nr_butoane;
    for(int i=0; i<nr_butoane; i++)
        if(y>=0 && y<50 && x>=i*width && x<=(i+1)*width)
            {buton(i);
             break ;
            }
}

bool intersectare_dreptunghiuri(int x1, int y1, int x2, int y2, int cx1, int cy1, int cx2, int cy2)
{
    int d=distanta_intre_blocuri;
    if(cx1+d<x2 || x1>cx2+d || cy1+d<y2 || y1>cy2+d) return false;
    return true;
}

bool intersecteaza_alte_blocuri(int x, int y)
{
    for(int i=6; i<nr_blocuri; i++)
        if(i!=bloc_nou && intersectare_dreptunghiuri(x,y,a[i].x,a[i].y,x+a[0].width,y+a[0].height,a[i].x+a[i].width,a[i].y+a[i].height)) return true;
    return false;
}


void click_stanga() /// click stanga pentru a plasa blocuri si pentru a adauga blocuri noi
{
    int x=mousex(),y=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
    event=1;
    if(eroare || rezult)
        if(!apartine_left_right(x,y)) eroare=rezult=0,S.clear(),Rezultat.clear(),indice_info=0;
    bara_meniu(x,y);
    if(eroare) afiseaza_erori();
    if(rezult) afiseaza_rezultat();
    if(selectat>5 && a[selectat].tip>1 && apartine_text(x,y) )
    {   adauga_undo();
        undo.push_back({3,selectat,0,0,0,0,0,a[selectat].text});
        citeste_text(selectat);
        strcpy(undo.back().text2,a[selectat].text);
    }
    if(bloc_nou!=-1)
    {
        if(bloc_nou>5) x-=a[bloc_nou].width/2;
        else
        {
            int w1=a[bloc_nou].width;
            x-=(w1+((indice_zoom-4)*w1)/zoom_ratio)/2;
        }
        bool inter=intersecteaza_alte_blocuri(x,y);
        ///if(inter)  se va afisa o eroare
        if(apartine_zona(x+ecran_x,y+ecran_y-colt_y,a[bloc_nou].width,a[bloc_nou].height) && !inter) /// plasare blocuri
        {
            int x1=x+ecran_x,y1=y+ecran_y-colt_y;
            if(indice_zoom!=4) ///  calculam coordonatele initiale fara zoom
            {
                x1=((x+ecran_x)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
                y1=((y+ecran_y-colt_y)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
            }
            if(bloc_nou==0 && start_main==0)
            {
                adauga_undo();
                undo.push_back({1,nr_blocuri});
                start_main=nr_blocuri;
                a[nr_blocuri]= {bloc_nou,1,x,y};
                a[nr_blocuri].x1=x1;
                a[nr_blocuri++].y1=y1;
            }
            else if(bloc_nou<=5)
            {
                adauga_undo();
                undo.push_back({1,nr_blocuri});
                a[nr_blocuri]= {bloc_nou,1,x,y};
                a[nr_blocuri].x1=x1;
                a[nr_blocuri++].y1=y1;
                if(bloc_nou==0) nr_blocuri_start++;
            }
            else
            {
                adauga_undo();
                undo.push_back({0,bloc_nou,0,a[bloc_nou].x,a[bloc_nou].y,x,y});
                a[bloc_nou].x=x;
                a[bloc_nou].y=y;
                a[bloc_nou].x1=x1;
                a[bloc_nou].y1=y1;
            }
        }
        bloc_nou=-1;
    }
    verifica_butoane(x,y);
    if(verifica_toate_nodurile(x,y))
    {
        //cout<<nod_dest<<' '<<nod_st<<' '<<nod_dr<<'\n';
        if(nod_dest!=-1 && (nod_st!=-1 || nod_dr!=-1))
        {
            /// if(nod_dest==nod_st || nod_dest==nod_dr) se va afisa o eroare
            if(nod_st!=-1)
            {
                adauga_undo();
                undo.push_back({2,nod_st,nod_dest});
                if(a[nod_st].st!=-1) a[a[nod_st].st].ant.remove(nod_st),undo.back().leg=1;
                a[nod_st].st=nod_dest;
                a[nod_dest].ant.push_back(nod_st);
                nod_st=nod_dr=nod_dest=-1;
            }
            if(nod_dr!=-1)
            {
                adauga_undo();
                undo.push_back({2,nod_dr,nod_dest,0,0,0,0,1});
                if(a[nod_dr].dr!=-1) a[a[nod_dr].dr].ant.remove(nod_dr),undo.back().leg=1;
                a[nod_dr].dr=nod_dest;
                a[nod_dest].ant.push_back(nod_dr);
                a[nod_dest].ant.unique();
                nod_dr=nod_st=nod_dest=-1;
            }
        }
    }
    else selectat=verifica_toate_blocurile(x,y);
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
    setbkcolor(fundal);
    clearviewport();
    //Campurile meniului, se va specifica lungimea si inaltimea ecranului, a cata optiune este, numarul de optiuni si mesajul dorit
    creeazaOptiune(widthEcran,heightEcran,1,4,"CREEAZA SCHEMA");
    creeazaOptiune(widthEcran,heightEcran,2,4, "SALVATE");
    creeazaOptiune(widthEcran,heightEcran,3,4, "SCRIE COD");
    creeazaOptiune(widthEcran,heightEcran,4,4, "IESIRE");
    while(!ecran_schimbat)  /// cand se apasa pe o optiune de meniu, ecran_schimbat devine 1, se opreste loop-ul asta si se revine in loop-ul din main
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int x=mousex(),y=mousey();
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
{
    init();
    while(!ecran_schimbat)
    {
        delay(100);
        if(kbhit()) verifica_mutare_ecran();
        if(event) deseneaza_schema(),event=0;

        if(selectat<=5 && !eroare && !rezult) sterge_info();

        if(options && ismouseclick(WM_LBUTTONDOWN))
        {
            click_optiuni();
            continue ;
        }

        if(selectat>5)
        {
            if(a[selectat].tip>1) deseneaza_text(a[selectat].text);
            if(ismouseclick(WM_RBUTTONDOWN))
            {
                click_dreapta_pe_bloc();
                continue ;
            }
        }

        if(bloc_nou!=-1 && ismouseclick(WM_RBUTTONDOWN))
        {
            anuleaza();
        }

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
    {
        if(ecran_schimbat)
        {
            ecran_schimbat=0;
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

