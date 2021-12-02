#include <iostream>
#include <stdlib.h>
#include <winbgim.h>
#include <string>

#define MARGINE_SUPERIOARA 0.20
#define DISTANTA_INTRE_COMPONENTE 0.05
#define INALTIME_ZONA_MENIU 0.60
#define LUNGIME_ZONA_MENIU 0.2
#define MARGINE_STANGA 0.40
#define FONT_STYLE DEFAULT_FONT
#define FONT_SIZE 0
/// zona unde pot fi puse blocurile
#define colt_x 0
#define colt_y 50
#define lungime 1200
#define inaltime 600
/// culorile folosite
#define color COLOR(102,255,178)
#define nod_color YELLOW
#define fundal WHITE
#define option_color COLOR(128,128,255)
#define text_color RED

using namespace std;

int ecrane[11]; // Numarul de ecrane al programului
int nrOptiuni;  //Cate optiuni are meniul ( poate fi modificat prin apelarea de un numar dorit de ori a functiei creeazaOptiune )
int ecranCurent; //Ecranul deschis in acest moment

int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=10;
bool event=1,options=0,ecran_schimbat=1;
int ok = 1;

struct dreptunghi
{
    int top,left,right,bottom;
    int midx,midy;
};

//Structura unei optiuni din meniu
struct optiune
{
    dreptunghi drept;
    int heightText, widthText;
    int culoare;

} optiuni[11];


struct blocuri
{ int tip,nr;
  int x,y;
  int width=100,height=50;
  char var1[5]="?",op[5]="+",var2[5]="?";
  char text[101];
  int st=-1,dr=-1;
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
  nr_blocuri=6;
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

bool apartine_nod()
{

}

bool verifica_toate_nodurile()
{ for(int i=6;i<nr_blocuri;i++)
     {

     }

}

void trasare_legatura(int x, int y, int tip, int i)
{ /** x, y - pozitia nodului,  tip poate de ajuta la ceva
      (a[i].x+a[i].width/2, a[i].y)  sunt coordonatele nodului destinatie
    */

    line(1,1,100,100);

}

void deseneaza_legaturi()
{for(int i=6;i<nr_blocuri;i++)
       {if(a[i].st!=-1)  trasare_legatura(a[i].x,a[i].y+a[i].height,a[i].tip,a[i].st);
        if(a[i].dr!=-1)  trasare_legatura(a[i].x+a[i].width,a[i].y+a[i].height,a[i].tip,a[i].dr);
       }
}

void deseneaza_schema()
{ setcolor(BLUE);
  rectangle(0,colt_y,lungime,colt_y+inaltime);
  setfillstyle(SOLID_FILL,WHITE);
  floodfill(500,100,BLUE);
  deseneaza_legaturi();
  for(int i=6;i<nr_blocuri;i++)
       desenare_bloc(a[i].x,a[i].y,a[i].tip,a[i].text);
}

int verifica_toate_blocurile(int x, int y)
{ for(int i=6;i<nr_blocuri;i++)
      if(verifica_bloc(a[i].tip,x,y,a[i].x,a[i].y)) return i;
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

void mutare()
{int x=mousex(),y=mousey();
 x-=a[bloc_nou].width/2;
 if(apartine_zona(x,y))
    {desenare_bloc(x,y,a[bloc_nou].tip,a[bloc_nou].text);
     event=1;
    }
}

void click_stanga() /// click stanga pentru a plasa blocuri si pentru a adauga blocuri noi
{int x=mousex(),y=mousey();
 clearmouseclick(WM_LBUTTONDOWN);
 event=1;
 if(bloc_nou!=-1)
    { x-=a[bloc_nou].width/2;
      if(apartine_zona(x,y))
            {if(bloc_nou<=5)
                {a[nr_blocuri++]={bloc_nou,1,x,y};
                 if(nr_blocuri>6) a[nr_blocuri-1].st=nr_blocuri-1;
                }
             else a[bloc_nou].x=x,a[bloc_nou].y=y;
            }
      bloc_nou=-1;
    }
 verifica_butoane(x,y);
 selectat=verifica_toate_blocurile(x,y);
 ///nod_selectat=verifica_toate_nodurile();
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
    rect.left = x - widthComponenta/2;
    rect.top = y - heightComponenta/2;
    rect.right = x + widthComponenta/2;
    rect.bottom = y + heightComponenta/2;
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
    rectangle(optiuni[componenta].drept.left,optiuni[componenta].drept.top,optiuni[componenta].drept.right,optiuni[componenta].drept.bottom);
      setcolor(text_color);
    //Afisam mesajul curent in mod centrat in dreptunghiul corespunzator
    outtextxy(optiuni[componenta].drept.midx - optiuni[componenta].widthText / 2, optiuni[componenta].drept.midy - optiuni[componenta].heightText / 2, mesaj);

}

//Detecteaza daca o anumita coordonata se afla intr-un dreptunghi cu anumite coordonate
bool clickInDrepunghi(dreptunghi drept,int x,int y)
{
    int left = drept.left;
    int top = drept.top;
    int right = drept.right;
    int bottom = drept.bottom;

    return (left <= x && x <= right && y >= top && y <= bottom);
}

void click_handler(int x,int y)
{
    for(int i=1; i<=nrOptiuni; i++)
    {
        if(clickInDrepunghi(optiuni[i].drept,x,y) && ecrane[0])
        {
            ecrane[0] = 0;
            ecrane[i] = 1;
            ecranCurent = i;
            ok = 1;
            ecran_schimbat=1;
        }
        if(ecrane[1] && x >= 0 && x <= 100 && y >= 0 && y <= 100)
        {
            cleardevice();
            setbkcolor(BLACK);
            clearviewport();

            ecrane[0] = 1;
            ecrane[1] = 0;
            ecranCurent = 0;
            ecran_schimbat=1;
        }
    }

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

}


void ecran1()
{
    if(ok)
    {
        init();
        ok = 0;
    }
    //delay(100);
   if(event) deseneaza_schema(),event=0;

   if(options && ismouseclick(WM_LBUTTONDOWN)) {click_optiuni(); return  ;}

   if(selectat>5 && ismouseclick(WM_RBUTTONDOWN)) {click_dreapta_pe_bloc(); return  ;}

   if(bloc_nou!=-1 && ismouseclick(WM_RBUTTONDOWN))  anuleaza();

   if(bloc_nou!=-1) mutare();

   if(ismouseclick(WM_LBUTTONDOWN)) click_stanga();
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
    initwindow(getmaxwidth(),getmaxheight()-25);

    registermousehandler(WM_LBUTTONUP,click_handler); //Click Handlerul

    do
    { //if(ecran_schimbat)
      //{ecran_schimbat=0;
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


        //page = 1-page;

    }
    while(true);

    getch();
    closegraph();

    return 0;
}
