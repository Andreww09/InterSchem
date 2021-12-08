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
#define NMAX 201
/// zona unde pot fi puse blocurile
#define colt_x 0
#define colt_y 50
#define lungime 1200
#define inaltime 600
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

int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=10;
bool event=1,options=0,ecran_schimbat=1,bloc_start=0,bloc_stop=0;
int nod_dest=-1,nod_st=-1,nod_dr=-1;
int culori_butoane[5]={RED,GREEN,RED,RED};
char mesaje_butoane[5][NMAX]={"Inapoi","Salvare"};

struct dreptunghi
{
    int top,left,right,bottom;
    int midx,midy;
}textbox;

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
  char text[201];
  int st=-1,dr=-1,ant=-1;
  blocuri(int a=0,int b=0,int c=0,int d=0,char s[NMAX]="")
  {tip=a;nr=b;x=c;y=d;
     if(tip==0) strcpy(text,"START");
     if(tip==1) strcpy(text,"STOP");
     if(tip>1) strcpy(text,s);
  }
};
blocuri a[1001];

void deseneaza_start(int x, int y, char s[NMAX]="START")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+25,y+25,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y+height,raza,raza);
}


void deseneaza_stop(int x, int y, char s[NMAX]="STOP")
{ int width=100,height=50,lung_text=textwidth(s);
  setcolor(BLACK);
  outtextxy(x+width/2-lung_text/2,y+height/2,s);
  rectangle(x,y,x+width,y+height);
  setfillstyle(SOLID_FILL,color);
  floodfill(x+25,y+25,BLACK);
  setfillstyle(SOLID_FILL,nod_color);
  fillellipse(x+width/2,y,raza,raza);
}

void deseneaza_intrare(int x, int y, char s[NMAX]="INTRARE")
{ int width=100,height=50,dist=20,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-20)
  { int n=(width-20)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2,s1);
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

void deseneaza_iesire(int x, int y, char s[NMAX]="IESIRE")
{ int width=100,height=50,dist=20,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-20)
  { int n=(width-20)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2,s1);
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

void deseneaza_decizie(int x, int y, char s[NMAX]="DECIZIE")
{ int width=100,height=50,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-40)
  { int n=(width-40)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2,s1);
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

void deseneaza_calcul(int x, int y, char s[NMAX]="CALCUL")
{ int width=100,height=50,lung_text=textwidth(s);
  char s1[NMAX];strcpy(s1,s);
  setcolor(BLACK);
  if(lung_text>width-10)
  { int n=(width-10)/textwidth("M");
    s1[n]='\0';
    strcat(s1,"...");
    lung_text=textwidth(s1);
  }
  outtextxy(x+width/2-lung_text/2,y+height/2,s1);
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

bool apartine_iesire(int a, int b, int x, int y, int width=100, int height=50)
{  if(a<x || a>x+width || b<y || b>y+height) return false;
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
{ int j=a[i].ant;
  if(a[j].st==i) a[j].st=-1;
  if(a[j].tip==4 && a[j].dr==i) a[j].dr=-1;
  for(int j=i;j<nr_blocuri-1;j++)
    a[j]=a[j+1];
  nr_blocuri--;

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
  //outtextxy(left+width/2-lung_text/2,height/2,text);
  rectangle(left,0,right,48);
  setfillstyle(SOLID_FILL,culoare);
  floodfill(right/2,25,BLUE);
}

void init()
{ int y=710,x=50;
  nr_blocuri=6;
  setbkcolor(fundal);clearviewport();
  deseneaza_info();
  a[0]={0,0,50,710,"START"};
  a[1]={1,0,200,710,"STOP"};
  a[2]={2,0,350,710,"INTRARE"};
  a[3]={3,0,500,710,"IESIRE"};
  a[4]={4,0,650,710,"DECIZIE"};
  a[5]={5,0,800,710,"CALCUL"};
  for(int i=0;i<=5;i++)
      desenare_bloc(a[i].x,a[i].y,a[i].tip,a[i].text);
  int width=getmaxwidth()/4;
  for(int i=0;i<1;i++)
      desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);


}

void sterge_info()
{ setfillstyle(SOLID_FILL,fundal);
  floodfill(textbox.left,textbox.top,BLUE);
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
  textbox.left=info_x+10;textbox.right=info_x+300;
  textbox.top=info_y+50;
  int width=textwidth("m"),height=textheight("W"),lung=textwidth(text);
  if(lung+10>textbox.right-textbox.left)
    {sterge_info();
     char s[NMAX];
     int n=20;//numar_caractere(text,textbox.right-textbox.left-10);
     strncpy(s,text,n);s[n]='\0';
     outtextxy(textbox.left+10,textbox.top,s);
     outtextxy(textbox.left+10,textbox.top+height,text+n);
     height*=2;
    }
    else   outtextxy(textbox.left+10,textbox.top,text);
  textbox.bottom=textbox.top+height;
  rectangle(textbox.left,textbox.top,textbox.right,textbox.bottom);

}

bool apartine_text(int x,int y)
{ if(x>textbox.left && x<textbox.right && y>textbox.top && y<textbox.bottom) return true;
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
   if(c!=8)  s[n++]=c;
   s[n]='\0';
   deseneaza_text(s);
  }
  //if(evalExpresie(s))
    strcpy(a[i].text,s);//cout<<a[i].text<<' ';
  selectat=-1;

}

bool apartine_nod(int i, int a, int b, int x, int y, int &nod)
{ if(abs(b-y)+abs(a-x)<=raza)
     {nod=i; return true;}
  return false;
}

bool verifica_toate_nodurile(int x, int y)
{ for(int i=6;i<nr_blocuri;i++)
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

void trasare_legatura(int x1, int y1, int x2, int y2, int cx1, int cy1, int cx2, int cy2)   //(int tip,int nod1,int nod2,int dr)
{
    /** x, y - pozitia nodului,  tip poate de ajuta la ceva
        (a[i].x+a[i].width/2, a[i].y)  sunt coordonatele nodului destinatie
      */

    setcolor(line_color);

    /*if(tip == 4)
    {
        if(!dr)
            x1 = a[nod1].x;
        else
            x1 = a[nod1].x + a[nod1].width;
        y1 = a[nod1].y + a[nod1].height;
    }
    else
    {
        x1 = a[nod1].x + a[nod1].width / 2;
        y1 = a[nod1].y + a[nod1].height;
    }
    x2 = a[nod2].x + a[nod2].width / 2;
    y2 = a[nod2].y;

    cout<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<a[nod1].y<<endl;

    int cx1 = min(a[nod1].x,a[nod1].x + a[nod1].width), cx2 = max(a[nod1].x,a[nod1].x + a[nod1].width);
    int cy1 = min(a[nod2].x,a[nod2].x + a[nod2].width), cy2 = max(a[nod2].x,a[nod2].x + a[nod2].width);*/

    if(y1 <= y2)
    {
        line(x1,y1,x1,(y1+y2)/2);
        line(x1,(y1+y2)/2,x2,(y1+y2)/2);
        line(x2,(y1+y2)/2,x2,y2);
    }
    else if(cx2 >= cy1 && cy2 >= cx1)
    {
        if(x2 <= x1)
        {
            line(x1,y1,x1,y1+25);
            line(x1,y1+25,min(x1,x2) - 25,y1 + 25);
            line(min(x1,x2) - 25,y1+25,min(x1,x2) - 25,y2 - 25);
            line(min(x1,x2) - 25,y2 - 25,x2,y2 - 25);
            line(x2,y2 - 25,x2,y2);
        }
        else
        {
            line(x1,y1,x1,y1+25);
            line(x1,y1+25,max(cx1,cx2) + 25,y1 + 25);
            line(max(cx1,cx2) + 25,y1+25,max(cx1,cx2) + 25,y2 - 25);
            line(max(cx1,cx2) + 25,y2 - 25,x2,y2 - 25);
            line(x2,y2 - 25,x2,y2);
        }
    }
    else
    {
        if(y1 <= y2 && y2 <= y1)
        {
            line(x1,y1,x1,y1+25);

            int dist = abs( (x2 + cx1) / 2 );
            int dist2 = (y1 + y2 ) / 2;
            line(x1,y1+25,dist,y1+25);
            line(dist,y1+25,dist,dist2);
            line(dist,dist2,x2,dist2);
            line(x2,dist2,x2,y2);
        }
        else
        {
            line(x1,y1,x1,y1+25);
            int dist = abs( (x2 + cx1) / 2 );
            line(x1,y1 + 25,dist,y1+25);
            line(dist,y1 + 25,dist,y2 - 25);
            line(dist,y2 - 25,x2,y2 - 25);
            line(x2,y2 - 25,x2,y2);

        }
    }

}


void deseneaza_legaturi()
{for(int i=6;i<nr_blocuri;i++)
       {if(a[i].st!=-1)
            { if(a[i].tip==4)
                trasare_legatura(a[i].x, a[i].y+a[i].height, a[a[i].st].x+a[a[i].st].width/2, a[a[i].st].y,
                                 a[i].x+a[i].width, a[i].y+a[i].height, a[i].x+a[a[i].st].width, a[a[i].st].y+a[a[i].st].height);
              else
                trasare_legatura(a[i].x+a[i].width/2, a[i].y+a[i].height, a[a[i].st].x+a[a[i].st].width/2, a[a[i].st].y,
                                 a[i].x+a[i].width, a[i].y+a[i].height, a[i].x+a[a[i].st].width, a[a[i].st].y+a[a[i].st].height);
            }
        if(a[i].dr!=-1)
            trasare_legatura(a[i].x+a[i].width, a[i].y+a[i].height, a[a[i].dr].x+a[a[i].dr].width/2, a[a[i].dr].y,
                             a[i].x+a[i].width, a[i].y+a[i].height, a[a[i].dr].x+a[a[i].dr].width, a[a[i].dr].y+a[a[i].dr].height);
       }
}

float applyOperator(char op,float nr1,float nr2)
{
    if(op == '+')
    {
        return (nr1 + nr2);
    }
    else if(op == '-')
    {
        return(nr2 - nr1);
    }
    else if(op == '*')
    {
        return (nr1 * nr2);
    }
    else if(op == '/')
    {
        return (nr2 / nr1);
    }
}

int precedence(char ch)
{
    if(ch == '-' || ch == '+')
        return 0;
    else if(ch == '*' || ch == '/')
        return 1;
    else return 2;
}

void operatieStiva(stack<char>& operatori, stack<float>& valori)
{
    char op = operatori.top();
    float nr1 = valori.top();
    valori.pop();
    float nr2 = valori.top();
    valori.pop();

    valori.push(applyOperator(op,nr1,nr2));

}

float evaluateExpresion(char sir[],int i,int j)
{
    stack<char> operatori;
    stack<float> valori;

    for(int k = i; k<=j; k++)
    {
        if(isalpha(sir[k]))
        {
            valori.push(variabile[sir[k] - 'a']);
        }
        else if(isdigit(sir[k]))
        {
            float numar = 0;

            int p = k;
            while(isdigit(sir[p]))
            {
                numar = numar * 10 + (sir[p] - '0');
                p++;
            }
            k = p - 1;
            valori.push(numar);
        }
        else if(sir[k] == '(')
        {
            operatori.push(sir[k]);
        }
        else if(sir[k] == ')')
        {
            while(!operatori.empty() && operatori.top() != '(')
            {
                operatieStiva(operatori,valori);
                operatori.pop();
            }
            operatori.pop();
        }
        else
        {
            while(!operatori.empty() && operatori.top() != '(' && precedence(sir[k]) <= precedence(operatori.top()))
            {
                operatieStiva(operatori,valori);
                operatori.pop();
            }
            operatori.push(sir[k]);
        }
    }

    while(!operatori.empty() && operatori.top() != ')')
    {
        operatieStiva(operatori,valori);
        operatori.pop();
    }
    return valori.top();
}

float evaluateEquality(char sir[],int i,int j)
{
    int variabila;
    char op = 'x';
    char aux[100];
    if(isalpha(sir[i-2]))
    {
        variabila = sir[i-2] - 'a';
    }
    else
    {
        op = sir[i-2];
        variabila = sir[i-3] - 'a';
    }


    float val = evaluateExpresion(sir,i,j);
    if(op != 'x')
    {
        variabile[variabila] = applyOperator(op,val,variabile[variabila]);
    }
    else
        variabile[variabila] = val;

    for(int k=i;k<=j;k++)
        cout<<sir[k];
    cout<<endl;

    return variabile[variabila];
}

float evaluateTerm(char sir[],int i,int j)
{
    //cout<<i<<" "<<j<<endl;
    float val = 0;
    int ok = 0;

    int n = strlen(sir);
    int last = j;
    for(int k = j ; k >= i; k--)
    {
        if(sir[k] == '=')
        {
            ok = 1;
            val = evaluateEquality(sir,k+1,last);

            k--;
        }
    }
    if(!ok)
        val = evaluateExpresion(sir,i,j);

    return val;
}

float evalExpresie(char sir[])
{
    // evalueaza operatii de forma x = expresie
    int n = strlen(sir);

    // "x =  x + 1"
    for(int i=0; i<n; i++)
    {
        if(sir[i] == ' ')
        {
            strcpy(sir+i,sir+i+1);
            i--;
        }
    }
    n = strlen(sir);

    int first = 0;
    float val = 0;

    for(int i = 0; i < n; i++)
    {
        if(sir[i] == ',' || i == n - 1)
        {
            if(sir[i] == ',')
                val = evaluateTerm(sir,first,i-1);
            else
                val = evaluateTerm(sir,first,i);

            first = i + 1;
        }
    }

    return val;
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

void inapoi()
{cleardevice();
 setbkcolor(BLACK);
 clearviewport();
 ecrane[0] = 1;
 ecrane[1] = 0;
 ecranCurent = 0;
 ecran_schimbat=1;
}

void buton(int i)
{ switch (i)
    { case 0: inapoi();

    }

}

void bara_meniu(int x, int y)
{ int width=getmaxwidth()/4;
  for(int i=0;i<1;i++)
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
 if(selectat>5 && a[selectat].tip>1 && apartine_text(x,y) ) {citeste_text(selectat);deseneaza_text(a[selectat].text); }
 if(bloc_nou!=-1)
    { x-=a[bloc_nou].width/2;
      bool inter=intersecteaza_alte_blocuri(x,y);
      ///if(inter)  se va afisa o eroare
      if(apartine_zona(x,y) && !inter)
            {if(bloc_nou<=1)
                {if(bloc_start==0 && bloc_nou==0) bloc_start=1,a[nr_blocuri++]={bloc_nou,1,x,y};
                 if(bloc_stop==0 && bloc_nou==1) bloc_stop=1,a[nr_blocuri++]={bloc_nou,1,x,y};
                }
             else if(bloc_nou<=5) a[nr_blocuri++]={bloc_nou,1,x,y};
             else a[bloc_nou].x=x,a[bloc_nou].y=y;
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
             nod_st=nod_dest=-1;
            }
          if(nod_dr!=-1)
            {a[nod_dr].dr=nod_dest;
             a[nod_dest].ant=nod_dr;
             nod_dr=nod_dest=-1;
            }
        }
    }
    else selectat=verifica_toate_blocurile(x,y);
 bara_meniu(x,y);
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

/*void click_handler(int x,int y)
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
*/
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
   if(event) deseneaza_schema(),event=0;

   if(selectat<=5) sterge_info();

   if(options && ismouseclick(WM_LBUTTONDOWN)) {click_optiuni(); continue ;}

   if(selectat>5)
        {   if(a[selectat].tip>1) deseneaza_text(a[selectat].text);
            if(ismouseclick(WM_RBUTTONDOWN)) {click_dreapta_pe_bloc(); continue ;}
        }

   if(bloc_nou!=-1 && ismouseclick(WM_RBUTTONDOWN))  {anuleaza(); }

   if(bloc_nou!=-1) mutare();

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
