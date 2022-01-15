#include <iostream>
#include <stdlib.h>
#include <winbgim.h>
#include <string>
#include <stack>
#include <list>
#include<set>
#include<queue>
#include<fstream>
#include<dirent.h>
#include<math.h>
#include<time.h>

#define MARGINE_SUPERIOARA 0.20
#define DISTANTA_INTRE_COMPONENTE 0.05
#define INALTIME_ZONA_MENIU 0.60
#define LUNGIME_ZONA_MENIU 0.2
#define MARGINE_STANGA 0.40
#define FONT_STYLE DEFAULT_FONT
#define FONT_SIZE 0
#define distanta_max_leg 25
#define NMAX 501
#define NR_ERORI 30
#define max_zoom 8
#define option_width 160
#define option_height 22
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

///coordonatele butoanelor viteza executie
#define minusV_x 1220
#define minusV_y 630
#define plusV_x 1470
#define plusV_y 630
#define latura_v 20

///coordonatele butoanelor left-right de eroare
#define left_x 1450
#define left_y 90
#define latura_left 20
#define dist_left 40

///coordonatele butonului pentru codul c++
#define cod_x 1250
#define cod_y 500
#define cod_width 230
#define cod_height 40

///zona de informatii
#define info_x 1200
#define info_y 50
#define info_width 400
#define info_height 100
#define max_text_height 75

///
#define set_raza 10
//#define set_dist 10
#define set_nod_x 280

/// culorile folosite
#define nr_culori 15
#define nr_limbaje 2
#define cod_color LIGHTGRAY
#define culoare_text RED

using namespace std;

#include "evaluareExp.h"

int ecrane[11]; // Numarul de ecrane al programului
int nrOptiuni;  //Cate optiuni are meniul ( poate fi modificat prin apelarea de un numar dorit de ori a functiei creeazaOptiune )
int ecranCurent; //Ecranul deschis in acest moment

int nr_blocuri=6,bloc_nou=-1,selectat=-1,raza=7,dist_leg=distanta_max_leg,distanta_intre_blocuri=20,continuare = 0,start_main=0, nod_dest=-1,nod_st=-1,nod_dr=-1;;
bool event=1,options=0,ecran_schimbat=1,rezult;
int culori_butoane[6]= {COLOR(249,5,5),COLOR(250,255,77),COLOR(95,255,77),LIGHTBLUE,COLOR(46,255,193),COLOR(46,165,255)},nr_butoane=6;
int zoom_ratio=5,indice_zoom=4; /// zoom_ratio=5 va insemna 1/5 adica 20%
int ecran_x,ecran_y;
int viteza=200;
char mesaje_butoane[6][NMAX]= {"Inapoi","Salvare","Executa","Undo","Redo","Ajutor"};
bool error[NR_ERORI];
int indice_info,indice_undo;
char mesaje_eroare[NR_ERORI][NMAX]= {"Programul nu este   legat la nici un bloc de tip stop\n",
                                     "Programul nu are niciun bloc de tip start\n","Valoare gresita\n","Blocul % nu este legat\n","Blocul % nu are o expresie corecta\n","Expresia din blocul % are simboluri egale consecutive\n", "Expresia din blocul % nu se afla peste alfabetul corect\n", "Operatorii unari din blocul % nu sunt bine asociati\n",
                                     "Expresia din blocul % nu este corect parantetizata\n", "Expresia din blocul % nu incepe sau se termina corect\n", "Expresia din blocul % nu poate sa contina doua puncte unul dupa altul\n", "O cifra cu virgula nu este corect scrisa in blocul % \n",
                                     "Exista un operator  la finalul expresiei in blocul % \n", "Exista doua variabile una dupa alta in blocul % \n", "Exista doi operatori de acelasi fel unul langa altul in blocul % \n", "Operatie binara gresita in blocul % \n","Impartire la zero in blocul % \n",
                                     "In blocul %, Operatiile binare pot fi doar numere intregi\n", "Operatie nedefinita in blocul % \n", "Operatorii de incrementare/decrementare nu are o variabila acociata in blocul % \n", "Expresia necesita un lvalue in blocul % \n", "Virgula nu poate sa apara la inceput in blocul % \n","Expresia are un operator la inceput in blocul % \n"
                                     ,"Paranteza nu are continut in blocul % \n", "Blocul % citire nu poate citi aceleasi doua variabile in acelasi timp \n", "Exista variabile de lungime mai mare de 2 in blocul % \n", "Virgulele nu respecta regulile de sintaxa in blocul % \n"
                                     , "Apar simboluri invalide in blocul % \n"
                                    };
char mesaje_rezultat[3][100]= {"Variabila % are valoarea % ","Rezultatul blocului % este % "};
int nr_setari=7;
char mesaje_limbaje[4][NMAX]= {"C/C++","JAVA"};
char mesaje_setari[9][NMAX]= {"CULOARE FUNDAL","CULOARE BLOCURI","CULOARE NODURI","CULOARE LEGATURI","CULOARE TEXT","CULOARE BLOCURI","LIMBAJ","IN TIMPUL EXECUTIEI"};
//int culori[11]={COLOR(),COLOR(),COLOR(),COLOR(),COLOR(),COLOR(),COLOR(),COLOR(),COLOR(),COLOR()};
int culori[20]= {0x330033,0xFFFFFF,0xF00000,0xFF8000,0xFFFF33,0x66FFB2,0x009900,0x33FFFF,0x99FF99,COLOR(76,0,153),0x6666FF,0x0080FF,0x3333FF,0x7F00FF,COLOR(178,103,252),COLOR(153,51,255)};
int fundal=culori[1],text_color=culori[0],color=culori[5],nod_color=culori[6]-2,option_color=culori[5],line_color=culori[12],mark_color=COLOR(76,0,153),limbaj=0;
int indice_culori[8]= {1,5,6,12,0,9,0};
char fisiere[20][NMAX];
int fisier_curent=-1,nr_fisiere = 0;
int nr_fisier[20];
char cod[100][NMAX] = {{'\0'}};
int linieCod,indexCaracter,linieCaracter,cursorPozitie;
char codCorect[1000];
bool executat=0;
int depth = 0;

void zoom_bloc(int,int);
void ecran1();
void schimba_coord();
void refacere_coord();
void zoom_bloc();
void number_to_text(int,char*);
void seteaza_culori();

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
    int bloc,nr;            // bloc=0 insemana o eroare in care nu se afiseaza si nr blocului
    erori(int a, int b)
    {
        bloc=a;
        nr=b;
    }
};
vector<erori> S;

struct afisari
{
    int bloc,nr; // nr=1 inseamna ca se afiseaza doar un rezultat, nr=0 inseamna ca se va afisa si numele variabilei, nr=-1 inseamna mesaj simplu
    float val;
    char var;
    afisari(int a, int b, float c, char d='0' )
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
    double x1,y1;   // coordonatele blocului daca nu ar avea zoom
    int width=100,height=50;
    int w1=100,h1=50;
    char text[NMAX];
    bool ok=0;
    int st=-1,dr=-1;
    int bucla;
    list<int> ant;      // nodurile care intra in blocul curent
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

#include "genereazaSchema.h"

struct schimbari
{
    int tip;
    int bloc1,bloc2,x1,y1,x2,y2;
    bool dreapta;
    int leg=0;
    char text1[NMAX],text2[NMAX];
    blocuri x;
    vector<bool> viz;
    schimbari(int a=0, int b=0, int c=0, int d=0, int e=0, int f=0, int g=0, bool h=0, char s1[]="", char s2[]="")
    {
        tip=a;
        bloc1=b;
        bloc2=c;
        x1=d;
        y1=e;
        x2=f;
        y2=g;
        dreapta=h;
        strcpy(text1,s1);
        strcpy(text2,s2);
    }
};
vector<schimbari> undo;

bool esteSirDeCaractere(char sir[])
{
    int i = 0, j = strlen(sir) - 1, n = strlen(sir);
    while(i < n-1 && sir[i] == ' ')
        i++;
    while(j > 0 && sir[j] == ' ')
        j--;

    if(j - i == 2 && sir[i] == '\'' && sir[j] == '\'')
        return true;

    return sir[i] == '"' && sir[j] == '"';
}

void listFile(char v[][NMAX],int& n)
{
    DIR *pDIR;
    struct dirent *entry;
    n = 0;

    if( pDIR=opendir("salvate") )
    {
        while(entry = readdir(pDIR))
        {
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                strcpy(v[++n], entry->d_name);
        }
        closedir(pDIR);
    }
}


int nextFileName(int& cifre) // retuneaza urmatorul numar care nu apare in numele sablou din folderul salvate si numar de cifre al acestuia
{
    DIR *pDIR;
    struct dirent *entry;

    vector<int> numere;
    char base[] = "schema";
    int numarLipsa = 0,p = 10;
    cifre = 1;

    if( pDIR=opendir("salvate") )
    {
        while(entry = readdir(pDIR))
            if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ) // parent directory
            {
                int nr = 0,ok = 1,i;
                int len = strlen(entry->d_name);

                for(i=0; i<len && i<6; i++)
                    if(base[i] != entry->d_name[i])
                        ok = 0;

                for(i=6; i<len; i++)
                    if(!isdigit(entry->d_name[i]))
                        break;
                    else
                        nr = nr * 10 + entry->d_name[i] - '0';

                if(i < len && entry->d_name[i] != '.' || i+4 != len || entry->d_name[i+1] != 't' || entry->d_name[i+2] != 'x' || entry->d_name[i+3] != 't' )
                    ok = 0;

                if(ok)
                    numere.push_back(nr);
            }

        closedir(pDIR);
    }

    sort(numere.begin(),numere.end());

    for(int i=0; i<numere.size(); i++)
    {
        if(numere[i] != numarLipsa)
            return numarLipsa;
        numarLipsa++;

        if(numarLipsa >= p)
        {
            cifre++;
            p *= 10;
        }
    }
    return numarLipsa;
}

void deseneaza_start(int x, int y, char s[NMAX]="START", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-10)
    {
        int n=(width-10)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setbkcolor(color);
    setcolor(text_color);
    rectangle(x,y,x+width,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y+height,raza);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y+height,raza,raza);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
}


void deseneaza_stop(int x, int y, char s[NMAX]="STOP", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-10)
    {
        int n=(width-10)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setbkcolor(color);
    setcolor(text_color);
    rectangle(x,y,x+width,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y,raza);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y,raza,raza);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
}
void deseneaza_intrare(int x, int y, char s[NMAX]="INTRARE", int width=100, int height=50)
{
    int dist=width/5,lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-dist)    // in cazul in care textul nu incape in bloc
    {
        int n=(width-dist)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setcolor(text_color);
    line(x,y,x+width,y);
    line(x+width,y,x+width-dist,y+height);
    line(x+width-dist,y+height,x+dist,y+height);
    line(x+dist,y+height,x,y);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y+height,raza);    // nod jos
    circle(x+width/2,y,raza);           // nod sus
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y+height,raza,raza);
    fillellipse(x+width/2,y,raza,raza);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
}

void deseneaza_iesire(int x, int y, char s[NMAX]="IESIRE", int width=100, int height=50)
{
    int dist=width/5,lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-dist)
    {
        int n=(width-dist)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setcolor(text_color);
    line(x+dist,y,x+width-dist,y);
    line(x+width-dist,y,x+width,y+height);
    line(x+width,y+height,x,y+height);
    line(x,y+height,x+dist,y);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y+height,raza);
    circle(x+width/2,y,raza);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y+height,raza,raza);
    fillellipse(x+width/2,y,raza,raza);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
}

void deseneaza_decizie(int x, int y, char s[NMAX]="DECIZIE", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-45)  // daca nu incape textul in bloc
    {
        int n=(width-45)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setcolor(text_color);
    line(x,y+height,x+width/2,y);
    line(x+width/2,y,x+width,y+height);
    line(x+width,y+height,x,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y,raza);
    circle(x,y+height,raza);
    circle(x+width,y+height,raza);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y,raza,raza);
    fillellipse(x,y+height,raza,raza);
    fillellipse(x+width,y+height,raza,raza);
    int adjust=0;               // se muta textul mai sus sau mai jos in functie de zoom
    if(indice_zoom>4) adjust+=5;
    else adjust-=2*(4-indice_zoom);
    outtextxy(x+width/2-lung_text/2,y+height/2+adjust,s1);
}

void deseneaza_calcul(int x, int y, char s[NMAX]="CALCUL", int width=100, int height=50)
{
    int lung_text=textwidth(s);
    char s1[NMAX];
    strcpy(s1,s);
    setbkcolor(color);
    if(lung_text>width-10)
    {
        int n=(width-10)/textwidth("M");
        s1[n]='\0';
        strcat(s1,"...");
        lung_text=textwidth(s1);
    }
    setcolor(text_color);
    rectangle(x,y,x+width,y+height);
    setfillstyle(SOLID_FILL,color);
    floodfill(x+width/2,y+height-1,text_color);
    circle(x+width/2,y+height,raza);
    circle(x+width/2,y,raza);
    setfillstyle(SOLID_FILL,nod_color);
    fillellipse(x+width/2,y+height,raza,raza);
    fillellipse(x+width/2,y,raza,raza);
    outtextxy(x+width/2-lung_text/2,y+height/2-textheight("W")/2,s1);
}

void desenare_bloc(int i)
{
    int x=a[i].x,y=a[i].y;
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
    return abs(x1*(y2-y3)+x2*(y3-y1)+x3*(y1-y2))/2;     // arie prin determinant
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

void verifica_butoane(int x, int y) // se verifica blocurile de jos
{
    for(int i=0; i<=5; i++)
        if(verifica_bloc(x,y,i)) bloc_nou=a[i].tip;
}

void adauga_undo()
{
    while(undo.size()>indice_undo)  // ne asiguram ca se vor pune corect in vector noile mutari in cazul in care am dat undo
        undo.pop_back();
    indice_undo++;
}

void adauga(int i, int k, blocuri y)
{
    list<int>::iterator it;
    if(start_main<6) start_main=i;
    for(int j=6; j<nr_blocuri; j++)     // nr tuturor blocurilor mai mari decat blocul adaugat va creste cu 1
    {
        if(a[j].st!=-1 && a[j].st>=i)
            a[j].st++;
        if(a[j].tip==4 && a[j].dr!=-1 && a[j].dr>=i)
            a[j].dr++;
        for(it=a[j].ant.begin(); it!=a[j].ant.end(); it++)
            if((*it)>=i) (*it)++;
    }
    nr_blocuri++;
    for(int j=nr_blocuri; j>i; j--) // mutam blocurile cu o pozitie la dreapta
        a[j]=a[j-1];
    refacere_coord();   // refacem blocurile fara blocul adaugat
    a[i]=y;
    zoom_bloc(indice_zoom-4,i);
    schimba_coord();    // schimbam toate blocurile inclusiv cel adaugat
    if(a[i].st!=-1)  a[a[i].st].ant.push_back(i);
    if(a[i].tip==4 && a[i].dr!=-1)  a[a[i].dr].ant.push_back(i);
    for(it=a[i].ant.begin(); it!=a[i].ant.end(); it++)  // parcurgem lista de blocuri anterioare ale blocului adaugat si refacem legaturile daca este cazul
    {
        if(a[*it].tip==4 && undo[k].viz[*it])   // undo[k].viz[*it]=1 daca inainte de a fi sters, blocul era legat printr-o legatura dreapta de blocul anterior
            a[*it].dr=i;
        else a[*it].st=i;
    }
}

void sterge(int i, int ind=-1)
{
    if(ind==-1)   // stergere din optiuni
    {
        adauga_undo();
        undo.push_back({6,i});
        ind=undo.size()-1;
    }
    refacere_coord();
    undo[ind].x=a[i];
    schimba_coord();

    int k=undo[ind].bloc1;
    list<int>::iterator it1;
    undo[ind].viz.assign(nr_blocuri,0);
    for(it1=a[k].ant.begin(); it1!=a[k].ant.end(); it1++) // vizitam toate blocurile decizie care au legatura dreapta cu blocul de sters
        if(a[*it1].tip==4 && a[*it1].dr==k) undo[ind].viz[*it1]=1;

    list<int>::iterator it;
    if(i==start_main) start_main=0;
    for(it=a[i].ant.begin(); it!=a[i].ant.end(); it++)  // parcurgem nodurile anterioare pentru a sterge si legaturile
    {
        if(a[*it].st==i) a[*it].st=-1;
        if(a[*it].tip==4 && a[*it].dr==i) a[*it].dr=-1;
    }
    for(int j=6; j<nr_blocuri; j++) // stergem blocul si din listele de blocuri anterioare
        a[j].ant.remove(i);
    for(int j=i; j<nr_blocuri-1; j++)   // mutam blocurile cu o pozitie la stanga
        a[j]=a[j+1];
    nr_blocuri--;
    for(int j=6; j<nr_blocuri; j++) // toate blocurile mai mari decat i se scad cu 1 in cadrul legaturilor
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
    x1=((x+ecran_x)*zoom_ratio)/(indice_zoom-4+zoom_ratio); // ecuatia de la zoom inversata
    y1=((y+ecran_y-colt_y)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
    a[i].x=x;
    a[i].y=y;
    a[i].x1=x1;
    a[i].y1=y1;
}

void verifica_optiuni(int i, int a1, int b, int x, int y)   // optiunile de la click dreapta
{
    if(x+option_width>colt_x+lungime) x-=option_width+100;  // atunci cand optiunile nu incap in ecran si se deseneaza in mod simetric
    if(y+option_height>colt_y+inaltime) y-=y+option_height-colt_y-inaltime;
    int nr=4;
    if(a[selectat].tip!=4) nr--;    // numarul de optiuni
    if(a1>x+option_width || a1<x || b>y+nr*option_height || b<y) return ;
    nr=(b-y)/option_height;
    if(nr==1)   // sterge tot blocul
    {
        sterge(i);
        return ;
    }
    if(nr==2)   // sterge nod stanga
    {
        if(a[selectat].st==-1) return ;
        adauga_undo();
        undo.push_back({5,selectat,a[selectat].st});
        sterge_legatura(selectat,a[selectat].st,0);
        return ;
    }
    if(nr==3 && a[selectat].tip==4) // sterge nod dreapta
    {
        if(a[selectat].dr==-1) return ;
        adauga_undo();
        undo.push_back({5,selectat,a[selectat].dr,0,0,0,0,1});
        sterge_legatura(selectat,a[selectat].dr,1);
        return ;
    }
    bloc_nou=i; // retinem blocul pentru mutare
}

void desenare_optiuni(int x, int y, int nr, char s1[]="", char s2[]="", char s3[]="", char s4[]="" )
{
    char mesaje[5][100];
    strcpy(mesaje[0],s1);
    strcpy(mesaje[1],s2);
    strcpy(mesaje[2],s3);
    strcpy(mesaje[3],s4);
    if(selectat!=-1 && a[selectat].tip!=4) nr--;    // in acest caz nu se desenaza si sterge nod dreapta
    setcolor(BLACK);
    setbkcolor(option_color);
    rectangle(x,y,x+option_width,y+nr*option_height);
    setfillstyle(SOLID_FILL,option_color);
    floodfill(x+10,y+10,BLACK);
    for(int i=1; i<=nr; i++)
    {
        int lung_text=textwidth(mesaje[i-1]);
        outtextxy(x+option_width/2-lung_text/2,y+5+(i-1)*option_height,mesaje[i-1]);
        line(x,y+i*option_height,x+option_width,y+i*option_height);
    }
}

void afiseaza_optiuni(int x, int y)
{
    int lung_text;
    if(x+option_width>colt_x+lungime) x-=option_width+100;
    if(y+option_height>colt_y+inaltime) y-=y+option_height-colt_y-inaltime;
    desenare_optiuni(x,y,4,"MUTA","STERGE","STERGE NOD STANGA","STERGE NOD DREAPTA");
}

void deseneaza_info()
{
    int x1=info_x+10,y1=info_y+10;
    setcolor(BLUE);
    setbkcolor(fundal);
    rectangle(info_x,info_y-10,info_x+info_width,info_y+info_height);
    outtextxy(x1,y1,"INFORMATII");
    line(info_x,info_y+30,info_x+info_width,info_y+30);
}

void desenare_buton(int left, int right, int culoare, char text[])  // butoanele de meniu de sus
{
    int width=right-left,height=50,lung_text=textwidth(text);
    setcolor(BLUE);
    setbkcolor(culoare);
    setfillstyle(SOLID_FILL,culoare);
    bar(left,0,right,40);
    rectangle(left,0,right,40);
    settextstyle(3,0,2);
    setusercharsize(3,2,3,2);
    outtextxy(left+width/2-lung_text/2,height/3,text);
    settextstyle(3,0,0);
    setusercharsize(25,100,25,100);
}

void marcheaza_bloc(int i, int culoare=mark_color)
{
    setfillstyle(SOLID_FILL,culoare);
    floodfill(a[i].x+a[i].width/2+raza,a[i].y+a[i].height-1,text_color);
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
    for(int i=6; i<nr_blocuri; i++) // nr inseamna semnul, va schimba daca se aduna sau se scade 20%
        zoom_bloc(nr,i);

}

void verifica_zoom(int x, int y)
{
    refacere_coord();
    if(apartine_dreptungi(x,y,minus_x,minus_y,latura_zoom,latura_zoom) && indice_zoom>2)
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
        dist_leg=distanta_max_leg+distanta_max_leg/zoom_ratio;
    }
    schimba_coord();
}

void deseneaza_butoane_zoom()
{
    setbkcolor(fundal);
    setcolor(GREEN);
    outtextxy(minus_x-70,minus_y,"ZOOM");
    setcolor(BLACK);
    rectangle(minus_x,minus_y,minus_x+latura_zoom,minus_y+latura_zoom);
    line(minus_x+latura_zoom/10,minus_y+latura_zoom/2,minus_x+latura_zoom-latura_zoom/10,minus_y+latura_zoom/2);
    rectangle(plus_x,plus_y,plus_x+latura_zoom,plus_y+latura_zoom);
    line(plus_x+latura_zoom/2,plus_y+latura_zoom/10,plus_x+latura_zoom/2,plus_y+latura_zoom-latura_zoom/10);
    line(plus_x+latura_zoom/10,plus_y+latura_zoom/2,plus_x+latura_zoom-latura_zoom/10,plus_y+latura_zoom/2);
}

void sterge_butoane_viteza()
{
    setfillstyle(SOLID_FILL,fundal);
    bar(minusV_x-5,minusV_y-5,plusV_x+30,plusV_y+30);
}

void deseneaza_butoane_viteza()
{
    setcolor(BLACK);
    rectangle(minusV_x,minusV_y,minusV_x+latura_v,minusV_y+latura_v);
    line(minusV_x+latura_v/10,minusV_y+latura_v/2,minusV_x+latura_v-latura_v/10,minusV_y+latura_v/2);
    rectangle(plusV_x,plusV_y,plusV_x+latura_v,plusV_y+latura_v);
    line(plusV_x+latura_v/2,plusV_y+latura_v/10,plusV_x+latura_v/2,plusV_y+latura_v-latura_v/10);
    line(plusV_x+latura_v/10,plusV_y+latura_v/2,plusV_x+latura_v-latura_v/10,plusV_y+latura_v/2);

    setcolor(GREEN);
    setbkcolor(fundal);
    outtextxy(minusV_x,minusV_y-2*latura_v,"VITEZA DE EXECUTIE");
    setcolor(BLACK);
    int nr=6,poz=(1000-viteza)/200,latura=(plusV_x-minusV_x-latura_v)/7;
    for(int i=1; i<=nr; i++)
    {
        if(i-1==poz)    // se deseneaza patratul la pozitia corecta
        {
            rectangle(minusV_x+i*latura+latura_v/2,minusV_y,minusV_x+i*latura+latura_v/2+latura_v,minusV_y+latura_v);
            continue ;
        }
        line(minusV_x+latura_v+i*latura,minusV_y+5,minusV_x+latura_v+i*latura,minusV_y+latura_v-5);
    }
}

void verifica_butoane_viteza(int x, int y)
{
    if(apartine_dreptungi(x,y,minusV_x,minusV_y,latura_v,latura_v) && viteza<1000)
    {
        viteza+=200;
        sterge_butoane_viteza();
        deseneaza_butoane_viteza();
    }
    if(apartine_dreptungi(x,y,plusV_x,plusV_y,latura_v,latura_v) && viteza>0)
    {
        viteza-=200;
        sterge_butoane_viteza();
        deseneaza_butoane_viteza();
    }
    if(y>=minusV_y && y<=minusV_y+latura_v && x>=minusV_x+latura_v && x<=plusV_x-latura_v)  // daca am apasat intre plus si minus la viteza de executie
    {
        int latura=(plusV_x-minusV_x-latura_v)/7,nr=(x-minusV_x-2*latura_v)/latura;
        viteza=(5-nr)*200;
        sterge_butoane_viteza();
        deseneaza_butoane_viteza();
    }
}

void obtine_cod()
{
    FILE *fptr;
    char nume[50];
    if(!executat || eroare || strlen(codCorect) == 0) return ;

    if(limbaj == 0) // C/C++
        strcpy(nume,"schemaMain.cpp");
    else if(limbaj == 1) // Java
        strcpy(nume,"schemaMain.java");

    fptr = fopen(nume,"w");
    fprintf(fptr,"%s",codCorect);
    fclose(fptr);
    system(nume);
}

void verifica_buton_cod(int x, int y)
{
    if(apartine_dreptungi(x,y,cod_x,cod_y,cod_width,cod_height))
        obtine_cod();
}

void desenare_buton_cod()
{
    setcolor(BLACK);
    setbkcolor(cod_color);
    setfillstyle(SOLID_FILL,cod_color);
    bar(cod_x,cod_y,cod_x+cod_width,cod_y+cod_height);
    rectangle(cod_x,cod_y,cod_x+cod_width,cod_y+cod_height);
    outtextxy(cod_x+10,cod_y+cod_height/3,"OBTINERE COD");
    //setfillstyle(SOLID_FILL,cod_color);
    //floodfill(cod_x+1,cod_y+1,BLACK);
}

void verifica_mutare_ecran()
{
    char c;
    if(getch()==0) c=getch();   // getch()==0 daca e caracter special, cum ar fi sagetile
    if(c==72 || c==75 || c==77 || c==80)
    {
        c=(c-70)/3;/// 1-stanga 0-sus 2-dreapta 3-jos
        int di[4]= {0,-1,1,0},dj[4]= {-1,0,0,1},d=distanta_mutare_ecran;
        if(ecran_x+di[c]*d>=min_x_ecran && ecran_x+di[c]*d<=max_x_ecran && ecran_y+dj[c]*d>=min_y_ecran && ecran_y+dj[c]*d<=max_y_ecran )
        {
            refacere_coord();
            ecran_x+=di[c]*d,ecran_y+=dj[c]*d;
            event=1;    // pentru a redesena ecranul
            schimba_coord();
        }
    }
}

void deseneaza_meniu()
{
    int width=getmaxwidth()/nr_butoane;
    for(int i=0; i<nr_butoane; i++)
        desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);
}

void desenare_margine()
{
    for(int i=0; i<=5; i++)
        desenare_bloc(i);
    deseneaza_meniu();
    deseneaza_butoane_zoom();
    deseneaza_butoane_viteza();
    desenare_buton_cod();

}

void incarca_blocuri()
{
    a[0]= {0,0,50,710,"START"};
    a[1]= {1,0,200,710,"STOP"};
    a[2]= {2,0,350,710,"INTRARE"};
    a[3]= {3,0,500,710,"IESIRE"};
    a[4]= {4,0,650,710,"DECIZIE"};
    a[5]= {5,0,800,710,"CALCUL"};
}

void init()
{
    int y=710,x=50;
    if(!continuare) // cand se creeaza o schema noua
    {
        viteza=200;
        nr_blocuri=6;
        start_main=0;
        indice_zoom=4;
        raza=7;
        fisier_curent=-1;
        ecran_x=min_x_ecran;
        ecran_y=min_y_ecran;
    }

    continuare=1;
    setbkcolor(fundal);
    clearviewport();
    desenare_margine();
    deseneaza_info();
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

bool apartine_left_right(int x, int y)  // butoanele din informatii prin care se trece prin mai multe mesaje
{
    int d=dist_left,nr;
    if(apartine_dreptungi(x,y,left_x,left_y,latura_left,latura_left))
    {
        indice_info=max(indice_info-1,0);
        return true;
    }
    if(apartine_dreptungi(x,y,left_x+d,left_y,latura_left,latura_left))
    {
        if(eroare) nr=S.size()-1;
        else nr=Rezultat.size()-1;
        indice_info=min(indice_info+1,nr);
        return true;
    }
    return false;
}

void deseneaza_butoane_left_right(int culoare, int nr)
{
    int d=dist_left;
    setcolor(BLACK);
    if(indice_info>0)  rectangle(left_x,left_y,left_x+latura_left,left_y+latura_left);  // nu se deseneaza butoanele daca nu sunt mai multe mesaje in stanga
    if(indice_info<nr-1) rectangle(left_x+d,left_y,left_x+d+latura_left,left_y+latura_left);
    setcolor(culoare);
    setlinestyle(SOLID_LINE,1,2);
    if(indice_info>0)
    {
        line(left_x,left_y+latura_left/2,left_x+latura_left,left_y);
        line(left_x,left_y+latura_left/2,left_x+latura_left,left_y+latura_left);
    }
    if(indice_info<nr-1)
    {
        line(left_x+latura_left+d,left_y+latura_left/2,left_x+d,left_y);
        line(left_x+latura_left+d,left_y+latura_left/2,left_x+d,left_y+latura_left);
    }
    setlinestyle(SOLID_LINE,0,1);
}

void deseneaza_text(char text[], int caz=0, char mesaj[]="")
{
    setcolor(BLACK);
    setbkcolor(fundal);
    if(caz==0 || caz==3)    // caz=0 inseamna introducere text cand se da click pe bloc, caz=1 inseamna introducere text cand se executa un bloc intrare
    {
        textbox.p1.x=info_x+10;
        textbox.p2.x=info_x+300;
        textbox.p1.y=info_y+50;
    }
    else    // caz=1 inseamna erori, caz=2 inseamna rezultate
    {
        textbox.p1.x=info_x+10;
        textbox.p2.x=info_x+300;
        textbox.p1.y=info_y+60;
        textbox.p2.y=info_y+info_height;
    }
    int width=textwidth("m"),height=textheight("W"),lung=textwidth(text);
    if(lung+10>textbox.p2.x-textbox.p1.x)   // blocul in care se introduce textul se mareste
    {
        sterge_info();
        char s[NMAX];
        int n=20;//numar_caractere(text,textbox.p2.x-textbox.p1.x-10);
        strncpy(s,text,n);
        s[n]='\0';
        outtextxy(textbox.p1.x+10,textbox.p1.y,s);
        outtextxy(textbox.p1.x+10,textbox.p1.y+height,text+n);
        height*=2;
    }
    else   outtextxy(textbox.p1.x+10,textbox.p1.y,text);
    if(caz==0 || caz==3)
    {
        setcolor(BLACK);
        outtextxy(info_x+10,info_y+35,mesaj);
        textbox.p2.y=textbox.p1.y+height;
        rectangle(textbox.p1.x,textbox.p1.y,textbox.p2.x,textbox.p2.y);
    }
    else
    {
        int culoare=GREEN,nr=Rezultat.size();
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

void sterge_caseta_text()
{
    int height=2*textheight("W");
    textbox.p1.x=info_x+10;
    textbox.p2.x=info_x+300;
    textbox.p1.y=info_y+50;
    textbox.p2.y=textbox.p1.y+height;
    setfillstyle(SOLID_FILL,fundal);
    bar(textbox.p1.x,textbox.p1.y,textbox.p2.x,textbox.p2.y);
}

void citeste_text(int i)
{
    char c,s[NMAX];
    int n;
    strcpy(s,a[i].text);
    n=strlen(s);
    while(c!=13)
    {
        c=getch();
        if(c==8 && n>0) n--,s[n]='\0',sterge_caseta_text(); // sterge un caracter
        if(!verifica_textbox(s))    // daca nu mai incap caractere
        {
            continue;
        }
        if(c!=8 && c!=13)  s[n++]=c,s[n]='\0'; // punem caracterul
        deseneaza_text(s);
    }

    strcpy(a[i].text,s); // punem textul in blocul respectiv
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
        if(nod_st!=-1 && nod_dr!=-1)    // ne asiguram ca la un moment dat numai nod_st sau nod_dr este setat
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

void aducere_in_plan(punct &pStart, punct &pStop)
{
    pStart.x=max(0,pStart.x);
    pStart.x=min(lungime,pStart.x);

    pStop.x=max(0,pStop.x);
    pStop.x=min(lungime,pStop.x);

    pStart.y=max(colt_y,pStart.y);
    pStart.y=min(colt_y+inaltime,pStart.y);

    pStop.y=max(colt_y,pStop.y);
    pStop.y=min(colt_y+inaltime,pStop.y);
}

bool testLinie(linie l,int px) // testeaza linia daca se afla la margini, px este o valoare mica ce face diferenta intre coordonate din ecranul principal si ecranul invizibil
{
    px = abs(px);
    return !(l.p1.x <= px && l.p2.x <= px || l.p1.y <= colt_y+px && l.p2.y <= colt_y+px || l.p1.x >= colt_x + lungime && l.p2.x >= colt_x + lungime || l.p1.y >= colt_y + inaltime && l.p2.y >= colt_y + inaltime );
}

void trasareLinie(linie l,int px)
{
    aducere_in_plan(l.p1,l.p2);
    if(testLinie(l,px)) // trasam linia doar daca este vizibila in ecran
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);

}

int trasareLinie(int a,int b,int c,int d,int px) // trasarea unor linii folosita pentru procedura trageSageata
{
    linie l;
    l.p1.x = a;
    l.p1.y = b;
    l.p2.x = c;
    l.p2.y = d;
    aducere_in_plan(l.p1,l.p2);
    if(testLinie(l,px))
    {
        line(l.p1.x,l.p1.y,l.p2.x,l.p2.y);
        return 1;
    }
    return 0;
}

void trasareSageata(linie l,int tip)
{
    int midx = (l.p1.x + l.p2.x) / 2;
    int midy = (l.p1.y + l.p2.y) / 2;

    int lungimeX = abs(l.p1.x-l.p2.x); // lungimea pe orizontala
    int lungimeY = abs(l.p1.y-l.p2.y); // lungimea pe verticala

    int px = a[6].width / 20; // valoare relativa la dimensiunea celorlalte blocuri

    if(tip == 1) // sageata spre dreapta sau in jos
    {
        if(lungimeX <= 15) // daca i o linie foarte mica atunci nu i mai trasam sagetile
            return ;

        if(l.p1.x < l.p2.x) // linie de la stanga spre dreapta
        {
            int ok = trasareLinie(midx,midy,midx-px,midy-px,px);

            if(ok)
                trasareLinie(midx,midy,midx-px,midy+px,px);

        }
        else // linie in jos
        {
            int ok = trasareLinie(midx,midy,midx+px,midy+px,px);

            if(ok)
                trasareLinie(midx,midy,midx+px,midy-px,px);
        }
    }
    else // sageata spre stanga sau in sus
    {
        if(lungimeY <= 15)  // daca i o linie foarte mica atunci nu o mai trasam
            return;
        if(l.p1.y < l.p2.y) // linie in sus
        {
            int ok = trasareLinie(midx,midy,midx-px,midy-px,px);
            if(ok)
                trasareLinie(midx,midy,midx+px,midy-px,px);

        }
        else // linie spre stanga
        {
            int ok = trasareLinie(midx,midy,midx+px,midy+px,px);
            if(ok)
                trasareLinie(midx,midy,midx-px,midy+px,px);
        }
    }
}

bool punctInterior(linie& l,int i) // folosit pentru detectarea unei linii care se termina intr-un bloc
{
    return (a[i].x <= l.p2.x && l.p2.x <= a[i].x+a[i].width && a[i].y <= l.p2.y && l.p2.y <= a[i].y+a[i].height);
}

void ocolireVertical(linie& l,linie& aux,int px,int i)
{
    aux.p2.x = aux.p1.x; // ramanem pe acelasi x

    if(px > 0) // daca linia vine din jos in sus
        aux.p2.y = (a[i].y + a[i].height) + px;
    else // daca linia vine din sus in jos
        aux.p2.y = a[i].y + px;

    trasareLinie(aux,px); // trasam prima parte, linia verticala dinaintea blocului
    trasareSageata(aux,2);

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

    trasareLinie(aux,px); // trasam partea a doua, linia orizontala care ocoleste blocul
    trasareSageata(aux,1);

    aux.p1 = aux.p2;
    if(px > 0) // daca linia vine din jos in sus
        aux.p2.y = a[i].y - px;
    else
        aux.p2.y = a[i].y + a[i].height - px;

    trasareLinie(aux,px); // trasam partea a treia, linia verticala care ocoleste blocul
    trasareSageata(aux,2);

    aux.p1 = aux.p2;
    aux.p2.y = aux.p1.y; // ramanem pe aceeasi orizontala
    aux.p2.x = l.p2.x;  // schimbam x-ul

    trasareLinie(aux,px); // trasam ultima parte, care leaga linia de ocolire, cu punctul prin care ar fi trebuit sa treaca linia, daca nu exista blocul i
    trasareSageata(aux,1);

    aux.p1 = aux.p2;
}

void ocolireVerticalaDesteapta(linie& l1,int i,linie* next,int px,int y)
{
    l1.p2.x = l1.p1.x; // ramanem pe aceasi verticala
    if(px > 0) // daca mergem din jos in sus
        l1.p2.y = a[i].y + a[i].height + px;
    else // daca mergem din sus in jos
        l1.p2.y = a[i].y + px;

    trasareLinie(l1,px); // trasam prima parte, pana la inceputul blocului
    trasareSageata(l1,2);

    l1.p1 = l1.p2;
    if(next->p1.x >= next->p2.x)
        if(px > 0)
            l1.p2.x = a[i].x - px;
        else
            l1.p2.x = a[i].x + px;
    else if(px > 0)
        l1.p2.x = a[i].x + a[i].width + px;
    else
        l1.p2.x = a[i].x + a[i].width - px;
    trasareLinie(l1,px); // trasam a doua parte, pana ce ocolim pe orizontala
    trasareSageata(l1,1);

    l1.p1 = l1.p2;
    l1.p2.x = l1.p1.x; // ramanem pe aceasi verticala
    l1.p2.y = y; // mentinem y-ul punctului in care ar fi trebuit sa ajungem, care s-ar fi aflat in interiorul ultimului bloc

    trasareLinie(l1,px); // trasam a treia parte, ca sa ajungem in punctul de coordonate y corespunzator, dar in afara blocului
    trasareSageata(l1,2);
}

void ocolireOrizontal(linie& l,linie& aux,int px,int i)
{
    aux.p2.y = aux.p1.y;
    if(px > 0)
        aux.p2.x = a[i].x - px;
    else
        aux.p2.x = a[i].x + a[i].width - px;

    trasareLinie(aux,px);
    trasareSageata(aux,1);

    aux.p1 = aux.p2;
    if(px > 0)
        if(l.p1.y <= a[i].y + a[i].height / 2 )
            aux.p2.y = a[i].y - px;
        else
            aux.p2.y = a[i].y + a[i].height + px;
    else if(l.p1.y <= a[i].y + a[i].height / 2)
        aux.p2.y = a[i].y + px;
    else
        aux.p2.y = a[i].y + a[i].height - px;

    trasareLinie(aux,px);
    trasareSageata(aux,2);

    aux.p1 = aux.p2;
    aux.p2.y = aux.p1.y;
    if(px > 0)
        aux.p2.x = a[i].x + a[i].width + px;
    else
        aux.p2.x = a[i].x + px;
    trasareLinie(aux,px);
    trasareSageata(aux,1);

    aux.p1 = aux.p2;
    aux.p2.x = aux.p1.x;
    aux.p2.y = l.p1.y;

    trasareLinie(aux,px);
    trasareSageata(aux,2);

    aux.p1 = aux.p2;
}

void ocolireOrizontalaDesteapta(linie& l1,int i,linie* next,int px,int x)
{
    l1.p2.y = l1.p1.y; // ramanem pe aceasi verticala

    if(px < 0) // daca mergem din jos in sus
        l1.p2.x = a[i].x + a[i].width - px;
    else // daca mergem din sus in jos
        l1.p2.x = a[i].x - px;
    trasareLinie(l1,px); // trasam prima parte, pana la inceputul blocului
    trasareSageata(l1,1);

    l1.p1 = l1.p2;

    if(next->p1.y >= next->p2.y)
        if(px < 0)
            l1.p2.y = a[i].y + px;
        else
            l1.p2.y = a[i].y - px;
    else if(px < 0)
        l1.p2.y = a[i].y + a[i].height - px;
    else
        l1.p2.y = a[i].y + a[i].height + px;

    trasareLinie(l1,px); // trasam a doua parte, pana ce ocolim pe orizontala
    trasareSageata(l1,2);

    l1.p1 = l1.p2;
    l1.p2.y = l1.p1.y; // ramanem pe aceasi verticala
    l1.p2.x = x; // mentinem y-ul punctului in care ar fi trebuit sa ajungem, care s-ar fi aflat in interiorul ultimului bloc

    trasareLinie(l1,px); // trasam a treia parte, ca sa ajungem in punctul de coordonate y corespunzator, dar in afara blocului
    trasareSageata(l1,1);

}

void trasareVertical(linie& l,linie* next,int nod1,int nod2) //traseaza o linie pe verticala, verificand si evitand eventuale blocuri aflate pe aceasta dreapta
{
    int v[10]; // vector in care se vor stoca indicele blocurilor care s-ar afla peste linia trasata
    int len = 0; // initial nu exista blocuri care se suprapun cu linia
    int px = a[5].width / 10;
    linie aux,aux2;

    aducere_in_plan(l.p1,l.p2);

    for(int i=6; i<nr_blocuri; i++) // parcurgem toate blocurile care nu se afla in meniul de jos
        if(a[i].ok)
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
    {
        trasareLinie(l,px);
        trasareSageata(l,2);
    }
    else if(next != NULL && punctInterior(l,v[len]))  // verificam daca i o linie care are punctul destinatie intr-un bloc
    {
        ocolireVerticalaDesteapta(aux,v[len],next,px,l.p2.y); // ocoleste destept acest obstacol (punctul se va afla pe acelasi y cu punctul in care ar fi trebuit sa fie, dar x ul este diferit in functie de linia urmatoare)
        l = aux; // actualizam inceputul liniei urmatoare
    }
    else // daca nu are punct interior atunci il ocolim ca celelalte blocuri
    {
        ocolireVertical(l,aux,px,v[len]);
        aux2.p1.x = aux.p2.x, aux2.p1.y = aux.p2.y, aux2.p2.x = l.p2.x, aux2.p2.y = l.p2.y;
        trasareLinie(aux2,px); // il legam cu punctul in care ar trebui sa ajunga linia
    }
    l.p1 = l.p2; // copiem p2 in p1, din acest punct incepem urmatoarea linie
}

void trasareOrizontal(linie& l,linie* next,int nod1,int nod2)
{
    int v[10]; // vector in care se vor stoca indicele blocurilor care s-ar afla peste linia trasata
    int len = 0; // initial nu exista blocuri care se suprapun cu linia
    int px = a[5].width / 10; //
    linie aux,aux2;

    aducere_in_plan(l.p1,l.p2);

    for(int i=6; i<nr_blocuri; i++)
        if(a[i].ok)
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
    {
        trasareLinie(l,px);
        trasareSageata(l,1);
    }
    else if(next != NULL && punctInterior(l,v[len])) // verificam daca i este o linie care are punctul destinatie intr-un bloc
    {
        ocolireOrizontalaDesteapta(aux,v[len],next,px,l.p2.x); // ocoleste destept acest obstacol (punctul se va afla pe acelasi y cu punctul in care ar fi trebuit sa fie, dar x ul este diferit in functie de linia urmatoare)
        l = aux; // actualizam inceputul liniei urmatoare
    }
    else // daca nu are punct interior atunci il ocolim ca celelalte blocuri
    {
        ocolireOrizontal(l,aux,px,v[len]);
        aux2.p1.x = aux.p2.x, aux2.p1.y = aux.p2.y, aux2.p2.x = l.p2.x, aux2.p2.y = l.p2.y;
        trasareLinie(aux2,px); // il legam cu punctul in care ar trebui sa ajunga linia
    }
    l.p1 = l.p2;
}

void trasare_legatura(int nod1,int nod2,int tip,int dr)
{
    blocuri n1=a[nod1],n2=a[nod2];
    punct pStart,pStop;  // punctele de inceput si final
    setcolor(line_color);

    linie l[5],aux, aux1, aux2; // legatura este impartita in mai multe linii orizontale/verticale

    aux1.p1.x = a[nod1].x;
    aux1.p1.y = a[nod1].y;
    aux1.p2.x = a[nod1].x + a[nod1].width;
    aux1.p2.y = a[nod1].y + a[nod1].height;

    aux2.p1.x = a[nod2].x;
    aux2.p1.y = a[nod2].y;
    aux2.p2.x = a[nod2].x + a[nod2].width;
    aux2.p2.y = a[nod2].y + a[nod2].height;

    aducere_in_plan(aux1.p1,aux1.p2);
    aducere_in_plan(aux2.p1,aux2.p2);

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
    int cx1 = aux1.p1.x, cx2 = aux1.p2.x;
    int cy1 = aux2.p1.x, cy2 = aux2.p2.x;


    if(!a[nod1].ok && !a[nod2].ok && a[nod1].x < 0 && a[nod2].x < 0) // daca ambele noduri nu mai sunt deloc vizibile, inclusiv legatura lor
        return ;

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

        trasareVertical(aux,&l[1],nod1,nod2);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p1.y;

        trasareOrizontal(aux,&l[2],nod1,nod2);

        aux.p2 = l[2].p2;

        trasareVertical(aux,NULL,nod1,nod2);
    }

    else if(pStart.y > pStop.y && cx2 >= cy1 && cy2 >= cx1)
    {
        l[0].p1 = pStart;
        l[0].p2.x = pStart.x;
        l[0].p2.y = pStart.y + px;

        l[1].p1 = l[0].p2;
        if(pStop.x <= pStart.x)
            l[1].p2.x = min(aux1.p1.x,aux2.p1.x) - px;
        else
            l[1].p2.x = max(aux1.p2.x,aux2.p2.x) + px;

        l[1].p2.y = l[0].p2.y;

        l[2].p1 = l[1].p2;
        l[2].p2.x = l[1].p2.x;
        l[2].p2.y = a[nod2].y - px;

        l[3].p1 = l[2].p2;
        l[3].p2.x = pStop.x;
        l[3].p2.y = l[2].p2.y;

        l[4].p1 = l[3].p2;
        l[4].p2 = pStop;

        aux = l[0];
        trasareVertical(aux,&l[1],nod1,nod2);

        if(pStop.x <= pStart.x)
            aux.p2.x = min(aux1.p1.x,aux2.p1.x) - px;
        else
            aux.p2.x = max(aux1.p2.x,aux2.p2.x) + px;
        aux.p2.y =  l[0].p2.y;;

        trasareOrizontal(aux,&l[2],nod1,nod2);

        aux.p2.x = aux.p1.x;
        aux.p2.y = a[nod2].y - px;
        trasareVertical(aux,&l[3],nod1,nod2);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p2.y;
        trasareOrizontal(aux,&l[4],nod1,nod2);

        aux.p2 = pStop;
        trasareVertical(aux,NULL,nod1,nod2);
    }
    else
    {
        int dist = abs((min(aux1.p2.x,aux2.p2.x) + max(aux1.p1.x, aux2.p1.x)) / 2 );

        l[0].p1 = pStart;
        l[0].p2.x = pStart.x;
        l[0].p2.y = pStart.y + px;

        l[1].p1 = l[0].p2;
        l[1].p2.x = dist;
        l[1].p2.y = l[0].p1.y;

        l[2].p1 = l[1].p2;
        l[2].p2.x = l[1].p1.x;
        l[2].p2.y = pStop.y - px;

        l[3].p1 = l[2].p2;
        l[3].p2.x = pStop.x;
        l[3].p2.y = l[2].p2.y;

        l[4].p1 = l[3].p2;
        l[4].p2 = pStop;

        aux = l[0];
        trasareVertical(aux,&l[1],nod1,nod2);

        aux.p2.x = dist;
        aux.p2.y = aux.p1.y;
        trasareOrizontal(aux,&l[2],nod1,nod2);

        aux.p2.x = aux.p1.x;
        aux.p2.y = pStop.y - px;
        trasareVertical(aux,&l[3],nod1,nod2);

        aux.p2.x = pStop.x;
        aux.p2.y = aux.p2.y;
        trasareOrizontal(aux,&l[4],nod1,nod2);

        aux.p2 = pStop;
        trasareVertical(aux,NULL,nod1,nod2);
    }
    a[nod1]=n1;
    a[nod2]=n2;
}

void deseneaza_legaturi()
{
    for(int i=6; i<nr_blocuri; i++) // parcurgem toate blocurile care nu se afla in meniul de jos
    {
        if(a[i].st !=-1)// && a[a[i].st].ok)
            trasare_legatura(i,a[i].st,a[i].tip,0); // legatura stanga

        if(a[i].dr!=-1)// && a[a[i].dr].ok)
            trasare_legatura(i,a[i].dr,a[i].tip,1); // legatura dreapta
    }
}

void deseneaza_schema()
{
    setcolor(BLUE);
    rectangle(0,colt_y-10,lungime,colt_y+inaltime);
    setfillstyle(SOLID_FILL,fundal);
    floodfill(500,100,BLUE);
    deseneaza_legaturi();
    /// nu se mai redeseneaza marginea daca liniile astea sunt comentate
    /*setcolor(BLUE);
    rectangle(0,colt_y,lungime,colt_y+inaltime);
    setfillstyle(SOLID_FILL,fundal);
    floodfill(801,711,BLUE);
    desenare_margine();*/
    //
    for(int i=6; i<nr_blocuri; i++)
        if(a[i].ok)
            desenare_bloc(i);
}


int verifica_toate_blocurile(int x, int y) // verificam daca am dat click pe un bloc
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
    a[nr_blocuri+1]= {a[i].tip,1,x,y,a[i].text};// nr_blocuri+1 este un bloc fictiv
    x-=a[nr_blocuri+1].width/2; // pentru a afisa blocul cu mijlocul laturii de sus la pozitia mouse-ului
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
    for(int i=1; i<7; i++)
        ecrane[i] = 0;

    nr_butoane = 6;
    ecranCurent = 0;
    ecran_schimbat=1;
}

void text_to_number(float &n, char* s)
{
    bool negativ=0;
    int i=0;
    if(s[0]=='-') negativ=1,i++;
    n=0;
    for( ;i<strlen(s); i++)
        n=n*10+s[i]-'0';
    if(negativ) n*=-1;
}

void number_to_text(int n, char* s)
{
    int nr=0;
    bool negativ=0;
    if(n<0) negativ=1,n*=-1;
    do
    {
        s[nr++]='0'+(n%10);
        n/=10;
    }
    while(n);
    if(negativ) s[nr++]='-';
    s[nr]='\0';
    for(int i=0; i<nr/2; i++)
    {
        int aux=s[i];
        s[i]=s[nr-i-1];
        s[nr-i-1]=aux;
    }
}

void replace_var(int n, char text[],int zecimale=0)
{
    char *p,*p1,s[NMAX],aux[NMAX];
    number_to_text(n,aux);
    strcpy(s,text);
    p=strchr(s,'%');
    p[0]='\0';
    strcpy(text,s);
    strcat(text,aux);
    if(zecimale<0) zecimale*=-1;
    if(zecimale>0)
    {
        while(zecimale%10==0) zecimale/=10;
        strcat(text,".");
        aux[0]='\0';
        number_to_text(zecimale,aux);
        strcat(text,aux);
    }
    strcat(text,p+1);

}

void afiseaza_erori()
{
    event=0;
    sterge_info();
    int j=indice_info;
    char aux[NMAX],text[NMAX];
    strcpy(text,"EROARE   ");
    number_to_text(j+1,aux);
    strcat(text,aux);
    strcat(text,"/");
    number_to_text(S.size(),aux);
    strcat(text,aux);
    if(S[j].bloc==-1)
    {
        deseneaza_text("Nu pot exista doua blocuri de tip start",1,text);
    }
    else if(S[j].bloc==0)   // text fara variabila
    {
        deseneaza_text(mesaje_eroare[S[j].nr],1,text);
    }
    else    // text cu variabila
    {
        int bloc=S[j].bloc,nr=S[j].nr;
        if(nr>=3)
        {
            char text1[NMAX];
            strcpy(text1,mesaje_eroare[S[j].nr]);
            replace_var(bloc,text1);
            deseneaza_text(text1,1,text);
        }
    }
}

bool verifica_text_cin(char text[]) // cand se introduc caractere prin blocul intrare
{
    if ( text[0]!='-' && text[0]!='+' && ( text[0]<'0' || text[0]>'9') ) return false;
    for(int i=1; i<strlen(text); i++)
        if( text[i]<'0' || text[i]>'9') return false;
    return true;
}

void sterge_variabile()
{
    setcolor(fundal);
    bar(info_x+10,info_y+info_height+10,info_x+10+400,info_y+info_height+10+300);
}

void afiseaza_rezultat()
{
    /*for(int j=0;j<Rezultat.size();j++)
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
    {
        int bloc=Rezultat[j].bloc,nr=Rezultat[j].nr;
        float rez=Rezultat[j].val;cout<<"AAA"<<rez<<"AAA\n";
        char *p,text1[NMAX];
        strcpy(text1,mesaje_rezultat[nr]);
        if(nr==-1)  // text simplu
        {
            strcpy(text1,a[bloc+5].text);
            deseneaza_text(text1,1,text);
        }
        else
        {
            if(nr==0)   // text cu variabila
            {
                p=strchr(text1,'%');
                p[0]=Rezultat[j].var;
            }
            if(nr==1) replace_var(bloc,text1);  // textul cu rezultat dintr-un anumit bloc
            if((rez-(int)(rez))==0)
                replace_var(rez,text1);
            else replace_var(rez,text1,(int)(rez*1000)%1000); // rezultat cu zecimale
            deseneaza_text(text1,1,text);   // rezultatul, atat pentru text cu variabila cat si cu rezultat
        }
    }
    setcolor(GREEN);
    setbkcolor(fundal);
    outtextxy(info_x+10,info_y+info_height+10,"VARIABILE");
    setcolor(BLACK);
    for(int j=0; j<26; j++)
    {
        char text[NMAX];
        text[0]='a'+j;
        text[1]=':';
        text[2]='\0';
        number_to_text(variabile[j],text+2);
        float rez=variabile[j];
        if( (rez-(int)(rez)) )
        {
            strcat(text,".");
            int n=(int)(rez*1000)%1000;
            while(n%10==0) n/=10;
            if(n<0) n*=-1;
            number_to_text(n,text+strlen(text));
        }
        outtextxy(info_x+10+j/13*(info_width/2),info_y+10+info_height+(j%13+1)*20,text);
    }

}

void verifica_erori_desen()
{
    if(start_main<6) S.push_back({0,1});    // fara bloc de start
    bool stop=0;
    for(int i=6; i<nr_blocuri; i++)
        if(a[i].tip!=0)
        {
            int nr_leg=0;
            int nr_total_leg=2;
            if(a[i].tip==1)
            {
                if(!a[i].ant.empty()) stop=1;
                continue ;
            }
            if(a[i].tip>1)
            {
                if(a[i].st!=-1) nr_leg++;
                if(!a[i].ant.empty()) nr_leg++;
            }
            if(a[i].tip==4)
            {
                nr_total_leg=3;
                if(a[i].dr!=-1) nr_leg++;
            }
            if(nr_total_leg!=nr_leg && nr_leg!=0) S.push_back({i-5,3}); // daca un bloc conectat cu macar o legatura nu are toate legaturile
        }
    if(!stop) S.push_back({0,0}); // daca nu avem stop
    if(S.size()) eroare=1;
}

void salvare(int numar=fisier_curent,char numeVechi[]="",int caz=0)
{
    refacere_coord();
    zoom(0,0);

    char folder[NMAX] = "salvate/schema";
    int i,cifre;

    if(caz==0) // nu-i o redenumire, vrem sa salvam o schema noua
    {
        if(numar != -1) // modificam un fisier aflat deja in salvate
        {
            strcpy(folder+8,fisiere[numar]);
            i=strlen(folder);
        }
        else if(numar == -1) // salvam in fisier nou
        {
            numar = nextFileName(cifre); // functia returneaza urmatorul numar din sablon
            int aux = pow(10,cifre-1);
            for(i=14; cifre != 0; i++,cifre--)
            {
                folder[i] = '0' + numar / aux;
                numar = numar % aux;
                aux /= 10;
            }
            folder[i] = '.';
            folder[i+1] = 't';
            folder[i+2] = 'x';
            folder[i+3] = 't';
            folder[i+4] = '\0';

            nr_fisiere++;
            fisier_curent=nr_fisiere;

        }

        FILE* fptr = fopen(folder,"w");
        fwrite(&nr_blocuri,sizeof(int),1,fptr);
        fwrite(&start_main,sizeof(int),1,fptr);

        for(int i=6; i<nr_blocuri; i++)
        {
            int lenSir = strlen(a[i].text);
            int lenAnt = a[i].ant.size();
            fwrite(&a[i].tip,sizeof(int),1,fptr);
            fwrite(&a[i].nr,sizeof(int),1,fptr);
            fwrite(&a[i].x,sizeof(double),1,fptr);
            fwrite(&a[i].y,sizeof(double),1,fptr);
            fwrite(&a[i].x1,sizeof(double),1,fptr);
            fwrite(&a[i].y1,sizeof(double),1,fptr);
            fwrite(&a[i].width,sizeof(int),1,fptr);
            fwrite(&a[i].height,sizeof(int),1,fptr);
            fwrite(&a[i].h1,sizeof(int),1,fptr);
            fwrite(&a[i].w1,sizeof(int),1,fptr);
            fwrite(&lenSir,sizeof(int),1,fptr);
            fwrite(&a[i].text,lenSir,1,fptr);
            fwrite(&a[i].ok,sizeof(bool),1,fptr);
            fwrite(&a[i].bucla,sizeof(bool),1,fptr);
            fwrite(&a[i].st,sizeof(int),1,fptr);
            fwrite(&a[i].dr,sizeof(int),1,fptr);
            fwrite(&lenAnt,sizeof(int),1,fptr);
            list<int>::iterator iter;
            for(iter = a[i].ant.begin(); iter != a[i].ant.end(); iter++)
            {
                int val=*iter;
                fwrite(&val,sizeof(int),1,fptr);
            }
        }
        fclose(fptr);
    }
    else if(caz==1 && numar!=-1)    // redenumire
    {
        char path[100] = "salvate/";
        char aux[100] = "";


        strcat(path,numeVechi);
        strcat(aux,"salvate/");
        strcat(aux,fisiere[numar]);
        rename(path,aux);

    }

    int semn=1;
    if(indice_zoom<4) semn=-1;
    zoom(semn,indice_zoom-4);
    schimba_coord();
}

void salvare_setari()
{
    FILE* fptr=fopen("setari.txt","w");
    for(int i=0; i<nr_setari; i++)
        fwrite(&indice_culori[i],sizeof(int),1,fptr);
    fclose(fptr);
}

void restaurare_setari()
{
    FILE* fptr=fopen("setari.txt","r");
    for(int i=0; i<nr_setari; i++)
        fread(&indice_culori[i],sizeof(int),1,fptr);
    seteaza_culori();
    fclose(fptr);
}

void cautaVariabile(int variabileSetate[],int& nrVar,set<int>& vizitate,int nod)
{
    if(vizitate.find(nod) == vizitate.end()) // daca n am mai vizitat nodul curend/blocul curent
    {
        vizitate.insert(nod); // il vizitam

        if(a[nod].tip > 1 && !esteSirDeCaractere(a[nod].text)) // blocurile start si stop nu pot contine variabile, dar nici sirurile de caractere
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

        if(a[nod].st != -1) // mergem la stanga
            cautaVariabile(variabileSetate,nrVar,vizitate,a[nod].st);
        if(a[nod].dr != -1) // mergem la dreapta
            cautaVariabile(variabileSetate,nrVar,vizitate,a[nod].dr);
    }
}

void adauga_variabile(string& cod,int nod)
{
    int variabileSetate[26] = {0};
    set<int> vizitate;
    int nrVar = 0;

    cautaVariabile(variabileSetate,nrVar,vizitate,nod); // functia principala, care parcurge schema si marcheaza toate variabilele prezente in vectorul de frecventa variabileSetate

    if(nrVar) // daca avem variabile
    {
        if(limbaj == 1)
            cod += "public ";
        cod += "float ";
    }

    for(int i=0; i<26; i++)
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

void adaugaTabs(string& cod)
{
    for(int i=1; i<=2*depth; i++)
        cod += "  ";
}

bool varificaBucla(int nod,int start,set<int>& vizitate)
{
    if(vizitate.find(nod) == vizitate.end()) // daca nu am mai vizitat blocul curent
    {
        vizitate.insert(nod); // il vizitam

        if(a[nod].bucla) // daca intalnesc o bucla inseamna ca nodul de la care am inceput a mers pe un back-edge la un parinte/bunic/strabunic etc, adica se afla intr o bucla, deci nu de aici se incepe bucla
            return false;

        if(a[nod].tip == 1) // daca blocul ii stop, atunci ii clar ca toate nodurile aflate pe drumul de la nodul start la nodul stop nu se afla intr o bucla
            return false;
        else
        {
            bool left = false, right = false;
            if(a[nod].st != -1) // merg la stanga
                left = varificaBucla(a[nod].st,start,vizitate);

            if(a[nod].dr != -1) // merg la dreapta
                right = varificaBucla(a[nod].dr,start,vizitate);
            return left || right;
        }
    }
    else if(nod == start) // daca am mai vizitat blocul curent, verific daca de aici am si inceput apelul functiei verificaBucla
    {
        a[nod].bucla++;
        return true;
    }
    else
        return false;
}

void adaugaDoWhile(string& cod,int nod)
{
    set<int> vizitate;
    if(varificaBucla(nod,nod,vizitate)) // verific daca se incepe bucla
        for(int i=1; i<=a[nod].bucla; i++) // o anumita intructiune poate fi startul a mai multor instructiuni do while
        {
            adaugaTabs(cod);
            depth++;
            cod += "do {\n\n";
        }
}

void adaugaCin(string& cod,int nod)
{
    adaugaDoWhile(cod,nod); // ar putea fi inceputul unui dowhile

    int len = strlen(a[nod].text),i = 0;
    int variabile[26] = {0};
    queue<char> q;

    while(i < len && a[nod].text[i] == ' ')
        i++;

    for(; i<len; i++)
    {
        char ch = a[nod].text[i];
        if(isalpha(ch))
        {
            if(variabile[ch-'a']) // daca am mai vizitat o
            {
                eroare = 24;
                S.push_back({nod-5,24});
            }
            else
            {
                variabile[ch-'a'] = 1;
                q.push(ch);
            }
            if(i > 1 && isalpha(a[nod].text[i-1]))
            {
                eroare = 25; // nu pot aparea variabile de lungime mai mare decat 2;
                S.push_back({nod-5,25});
            }
        }
        else if(ch == ',' && (i == 0 || a[nod].text[i-1] == ',' || i == len - 1)) // doua virgule una dupa alta, sau virgula la inceput sau sfarsit
        {
            eroare = 26;
            S.push_back({nod-5,26});
        }
        else if(ch != ',' && ch != ' ' && !isalpha(ch))
        {
            eroare = 27;
            S.push_back({nod-5,27});
        }
    }

    if(!q.empty())
    {
        adaugaTabs(cod);
        if(limbaj == 0)
            cod += "cin>>";

        while(!q.empty())
        {
            cod += q.front();
            q.pop();
            if(limbaj == 0)
            {
                if(!q.empty())
                    cod += ">>";
                else
                    cod += ";";
            }
            else if(limbaj == 1)
            {
                cod += " = ";
                cod += "reader.nextFloat();";
                if(!q.empty())
                {
                    cod += "\n";
                    adaugaTabs(cod);
                }
            }
        }
        cod += "\n\n";
    }
}

bool formaConsecutiva(char s[]) // variabila,variabila,....,variabila (sintaxa fixa), folosit pentru a distinge niste cazuri la interpretarea blocului afisare
{
    int i = 0;
    int n = strlen(s);
    while(i < n && s[i] == ' ')
        i++;
    i++;
    while(i < n)
    {
        if(s[i] == ',' && !esteVariabila(s[i-1]))
            return false;
        if(esteVariabila(s[i]) && s[i-1] != ',')
            return false;
        if(i == n-1 && !esteVariabila(s[i]))
            return false;
        if(!esteVariabila(s[i]) && s[i] != ',')
            return false;
        i++;
    }
    return true;
}

void formatareSir(char sir[],string& nou) // folosit pentru siruri de caractere ca acestea sa fie bine formatate
{
    int i = 0, j = strlen(sir) - 1, n = strlen(sir);
    nou = "\"";
    while(i < n-1 && sir[i] == ' ')
        i++;
    i++;
    while(j > 0 && sir[j] == ' ')
        j--;
    j--;
    while(i <= j)
    {
        if(sir[i] == '\'' || sir[i] == '\"' || sir[i] == '\?' || sir[i] == '\\')
        {
            nou += "\\";
        }
        nou += sir[i];
        i++;
    }
    nou += "\"";
}

void adaugaCout(string& cod,int nod)
{
    adaugaDoWhile(cod,nod); // verific daca de aici se incepe un do while

    int len = strlen(a[nod].text);
    int paranteze = 0;

    if(len && !esteSirDeCaractere(a[nod].text))
    {
        adaugaTabs(cod);
        for(int i=0; i<len; i++)
        {
            char ch = a[nod].text[i];
            if((ch == '>' || ch == '<') && !paranteze )
            {
                eroare = 28;
                S.push_back({nod-5,28});
            }
            else if(ch == '(')
                paranteze++;
            else if(ch == ')')
                paranteze--;
        }

        if(limbaj == 0)
        {
            cod += "cout<<";
            int i = 0;
            if(formaConsecutiva(a[nod].text)) //variabila,variabila,....,variabila (sintaxa fixa)
            {
                while(i < len && a[nod].text[i] == ' ')
                    i++;
                while(i < len)
                {
                    cod += a[nod].text[i];
                    if(i+2 < len)
                        cod += "<<";
                    i += 2;
                }
            }
            else
                cod += a[nod].text;
            cod += ";\n\n";
        }
        else if(limbaj == 1)
        {
            int i = 0;
            if(formaConsecutiva(a[nod].text))
            {
                cod += "System.out.print(";

                while(i < len && a[nod].text[i] == ' ')
                    i++;
                while(i < len)
                {
                    cod += a[nod].text[i];
                    cod+=");\n";

                    if(i+2 < len)
                    {
                        adaugaTabs(cod);
                        cod += "System.out.print(";
                    }
                    i += 2;
                }
            }
            else
            {
                cod += "System.out.print(";
                string expresie = "";
                for(int i=0; i<len; i++)
                {
                    if(a[nod].text[i] != ',')
                    {
                        expresie += a[nod].text[i];
                    }
                    else
                    {
                        cod += expresie;
                        cod += ")\n";
                        adaugaTabs(cod);
                        cod += "System.out.print(";
                        expresie = "";
                    }
                }
                cod += expresie;
                cod += ");\n\n";
            }
        }
    }
    else
    {
        string nou;
        formatareSir(a[nod].text,nou);
        adaugaTabs(cod);
        if(limbaj == 0)
        {
            cod += "cout<<";
            cod += nou;
            cod += ";\n\n";
        }
        else if(limbaj == 1)
        {
            cod += "System.out.print(";
            cod += nou;
            cod += ");\n\n";
        }
    }
}

void adaugaBloc(string& cod,int nod)
{
    set<int> vizitate;
    if(varificaBucla(nod,nod,vizitate)) // verific daca blocul este un while
    {
        adaugaTabs(cod);
        depth++;
        cod += "while(";
        cod += a[nod].text;
        cod += ") {\n\n";
    }
    else // daca nu i while atunci ii un if
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
    adaugaDoWhile(cod,nod); // verifica daca un bloc calcul este inceputul unui do-while

    if( strlen(a[nod].text) > 0) // daca are continut blocul
    {
        adaugaTabs(cod);
        cod += a[nod].text;
        cod += ";";
        cod += "\n\n";
    }

}

void genereaza_mesaj(string& cod,set<int>& vizitate,int nod)
{
    if(vizitate.find(nod) != vizitate.end()) // daca am mai vizitat blocul, atunci am terminat o bucla, ii adaug acolada
    {
        depth--;
        adaugaTabs(cod);
        cod += "}\n\n";
    }
    else
    {
        vizitate.insert(nod); // vizitam blocul curent
        if(a[nod].tip == 0) // daca este un bloc start
        {
            if(limbaj == 0)
                cod = "#include<iostream>\n\nusing namespace std;\n\n";
            else if(limbaj == 1)
            {
                cod = "import java.util.Scanner;\n\npublic class Main {\n";
                depth++;
            }

            adauga_variabile(cod,nod);

            if(limbaj == 0)
            {
                cod += "\n\nint main() {\n\n";
                depth++;
            }
            else if(limbaj == 1)
            {
                cod += "\n\npublic static void main(String[] args) {\n\n";
                depth++;
                adaugaTabs(cod);
                cod += "Scanner reader = new Scanner(System.in);\n";
            }

            if(a[nod].st != -1) // mergem mai departe
                genereaza_mesaj(cod,vizitate,a[nod].st);
        }
        else if(a[nod].tip == 1) // daca i un bloc stop
        {

            if(limbaj == 0)
            {
                if(depth == 1) // daca a fost ultimul stop
                {
                    cod += "return 0;\n\n";
                    depth--;
                    adaugaTabs(cod);
                    cod += "}\n\n";
                }
                else
                {
                    depth--;
                    adaugaTabs(cod);
                    cod += "}\n";
                }

            }
            else if(limbaj == 1)
            {
                if(depth == 2) // daca a fost ultimul stop
                {
                    depth--;
                    adaugaTabs(cod);
                    cod += "}\n\n";
                    depth--;
                    adaugaTabs(cod);
                    cod += "}\n\n";
                }
                else
                {
                    depth--;
                    adaugaTabs(cod);
                    cod += "}\n";
                }
            }
        }
        else if(a[nod].tip == 2) // daca i un bloc de citire
        {
            adaugaCin(cod,nod);
            if(a[nod].st != -1) // merg mai departe
                genereaza_mesaj(cod,vizitate,a[nod].st);
        }
        else if(a[nod].tip == 3) // daca i un bloc de afisare
        {
            adaugaCout(cod,nod);
            if(a[nod].st != -1) // merg mai departe
                genereaza_mesaj(cod,vizitate,a[nod].st);
        }
        else if(a[nod].tip == 4) // daca i un bloc decizie
        {
            if(a[nod].st != -1 && a[a[nod].st].bucla && a[a[nod].st].tip != 4) // daca blocul de decizie curent
            {
                depth--;
                adaugaTabs(cod);
                cod += "}while(";
                cod += a[nod].text;
                cod += ");\n\n";
            }
            else
            {
                adaugaBloc(cod,nod); // adaug blocul decizie
                if(a[nod].st != -1) // merg mai departe
                    genereaza_mesaj(cod,vizitate,a[nod].st);
            }

            if(a[nod].dr != -1) // pot merge si la dreapta
                genereaza_mesaj(cod,vizitate,a[nod].dr);
        }
        else // daca i bloc calcul
        {
            adaugaExpresie(cod,nod);
            if(a[nod].st != -1)
                genereaza_mesaj(cod,vizitate,a[nod].st);
        }
    }
}

bool apartine_stiva(int i, stack<int> S)
{
    stack<int> aux;
    bool ok=0;
    while(!S.empty())
    {
        int x=S.top();
        S.pop();
        if(x==i) ok=1;
        aux.push(x);
    }
    while(!aux.empty())
    {
        S.push(aux.top());
        aux.pop();
    }
    return ok;
}

void executa()
{
    for(int i=0; i<26; i++) variabile[i]=0;
    sterge_info();
    eroare=rezult=0;
    S.clear();
    Rezultat.clear();

    verifica_erori_desen();
    int i=start_main;
    float rez;
    stack<int>rad;

    string cod;
    set<int> vizitate;
    for(int i=6; i<nr_blocuri; i++)
        a[i].bucla = false;
    genereaza_mesaj(cod,vizitate,start_main);

    while(a[i].tip!=1 && !eroare)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            eroare=0;
            rezult=0;
            S.clear();
            Rezultat.clear();
            return ;
        }
        eroare=0;
        int i1=i;
        if(a[i1].ok)
            marcheaza_bloc(i1);
        delay(viteza);
        if(strcmp(a[i].text,"")==0)
        {
            S.push_back({i-5,4});   // bloc fara expresie
            eroare=-1;
        }
        else if(a[i].tip == 3 && esteSirDeCaractere(a[i].text))
        {
            Rezultat.push_back({i-5,-1,0}); // afisare text simplu
            if(a[i].st!=-1)
                i=a[i].st;
            else eroare=1;
            if(a[i].tip==1 && !rad.empty())
            {
                i=a[rad.top()].dr;
                rad.pop();
            }
            if(a[i1].ok)
                marcheaza_bloc(i1,color);
            continue ;

        }
        else if(a[i].tip>1)
            rez=evalueazaExpresie(a[i].text);
        if(!eroare)
        {
            if(a[i].tip==3 || a[i].tip==2)
            {
                char aux[NMAX];
                int nr=0;
                for(int j=0; j<=strlen(a[i].text); j++)     // eliminam spatiile
                    if(a[i].text[j]!=' ') aux[nr++]=a[i].text[j];
                aux[nr]='\0';
                char *p=strtok(aux,",");
                while(p)
                {
                    if(a[i].tip==3)  // expresii de iesire permise : a   7*a    a*7   a,b,c     a,7*b,c    8*a,b,c      a=b=c=2,7*a,d
                    {
                        if(p[0]>='a' && p[0]<='z' && (strlen(p)==1 || (p[1]=='=' && p[2]!='=')) )
                            Rezultat.push_back({i-5,0,variabile[p[0]-'a'],p[0]});
                        else Rezultat.push_back({i-5,1,evalueazaExpresie(p)});
                    }
                    else    // luam variabilele de intrare
                    {
                        deseneaza_text("",3,"Introduceti o valoare");
                        strcpy(a[NMAX-9].text,"");
                        citeste_text(NMAX-9);
                        clearmouseclick(WM_LBUTTONDOWN);
                        sterge_info();
                        if(verifica_text_cin(a[NMAX-9].text))   // se introduc doar valori
                            text_to_number(variabile[p[0]-'a'],a[NMAX-9].text);
                        else eroare=1,S.push_back({0,2});
                    }
                    p=strtok(NULL,",");
                }
            }
            if(a[i].st!=-1)
            {
                if(a[i].tip==4)
                {
                    if(rez)
                    {
                        if(!a[i].bucla && !apartine_stiva(i,rad))  // pentru a reveni pe ramura din dreapta in decizie
                            rad.push(i);
                        i=a[i].st;
                    }
                    else
                    {
                        if(a[i].dr!=-1)
                        {
                           // if(!rez)
                                i=a[i].dr;
                        }
                        else eroare=1;
                    }
                }
                else i=a[i].st;
                if(a[i].tip==1 && !rad.empty())
                {
                    i=a[rad.top()].dr;
                    rad.pop();
                }
            }
            else eroare=1;
        }
        else if(eroare!=-1)  S.push_back({i-5,eroare});
        if(a[i1].ok)
            marcheaza_bloc(i1,color);
    }


    if(a[i].tip==1 && a[i].ok)
    {
        marcheaza_bloc(i,mark_color);
        delay(viteza);
        marcheaza_bloc(i,color);
    }

    if(S.empty()) rezult=1,event=0,executat=1;
    else eroare=1,event=0;

    //genereaza_mesaj(cod,vizitate,start_main);
    if(rezult==1) eroare=0;

    strcpy(codCorect, cod.c_str());
}

void Undo(int Redo=0)
{
    int j=indice_undo-1;
    if(j+1)
    {
        int tip=undo[j].tip;
        if(Redo) tip=7-tip;
        switch (tip)
        {
        case 0:
        {
            mutare_bloc(undo[j].bloc1,undo[j].x1,undo[j].y1);
            break;
        }
        case 1:
        {
            sterge(undo[j].bloc1,j);
            break;
        }
        case 2:
        {
            sterge_legatura(undo[j].bloc1,undo[j].bloc2,undo[j].dreapta);
            if(undo[j].leg)
            {
                if(undo[j].leg<0) adauga_legatura(undo[j].bloc1,-undo[j].leg,1);
                else adauga_legatura(undo[j].bloc1,undo[j].leg,0);
            }
            break;
        }
        case 3:
        {
            schimba_text(undo[j].bloc1,undo[j].text1);
            break;
        }
        case 4:
        {
            schimba_text(undo[j].bloc1,undo[j].text2);
            break;
        }
        case 5:
        {
            if(undo[j].leg)
            {
                if(undo[j].leg<0) sterge_legatura(undo[j].bloc1,-undo[j].leg,1);
                else sterge_legatura(undo[j].bloc1,undo[j].leg,0);
            }
            adauga_legatura(undo[j].bloc1,undo[j].bloc2,undo[j].dreapta);
            break;
        }
        case 6:
        {
            adauga(undo[j].bloc1,j,undo[j].x);
            break;
        }
        case 7:
        {
            mutare_bloc(undo[j].bloc1,undo[j].x2,undo[j].y2);
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

void afiseaza_text_ajutor()
{
    int i=NMAX-1;
    int r=raza,z=indice_zoom,e_x=ecran_x,e_y=ecran_y;
    continuare=0;
    init();
    a[i]= {4,1,600,300,"a=(i+"};
    a[i].st=i;
    trasare_legatura(i,i,4,0);
    desenare_bloc(i);
    afiseaza_optiuni(a[i].x+a[i].width,a[i].y);
    deseneaza_text(a[i].text);
    setcolor(culoare_text);
    line(a[i].x+a[i].width/2-5,a[i].y,a[i].x-100,a[i].y);
    line(a[i].x,a[i].y+a[i].height+5,a[i].x+a[i].width/2,a[i].y+100);
    line(a[i].x+a[i].width,a[i].y+a[i].height+5,a[i].x+a[i].width/2,a[i].y+100);
    settextstyle(3,0,2);
    setusercharsize(3,2,3,2);
    outtextxy(20,100,"Puteti folosi sagetile pentru a muta schema");
    outtextxy(info_x-430,info_y+25,"Dati click pe chenar, introduceti un text si apasati");
    outtextxy(info_x-450,info_y+45,"tasta enter pentru a introduce textul in blocul selectat");
    outtextxy(100,200,"Dati click pe doua noduri pentru a le uni printr-o legatura, un nod din partea superioara se poate uni doar cu un nod");
    outtextxy(80,220,"din partea inferioara si invers. De asemenea, un nod superior poate fi legat de mai multe ori, iar unul inferior doar o data");
    outtextxy(a[i].x+2*a[i].width+90,a[i].y,"Selectati un bloc plasat si dati click");
    outtextxy(a[i].x+2*a[i].width+70,a[i].y+20,"dreapta pentru a vedea optiunile");
    outtextxy(a[i].x-200,a[i].y-10,"Nod superior");
    outtextxy(a[i].x,a[i].y+100,"Noduri inferioare");
    outtextxy(50,530,"Selectati si plasati un bloc pentru a-l adauga in schema (Click dreapta pentru a anula)");
    outtextxy(70,550,"Blocurile Start si Stop nu pot fi modificate");
    outtextxy(70,570,"Blocul decizie va evalua expresia si va returna o valoare de adevar, daca expresia este adevarata, executia");
    outtextxy(50,590,"va continua prin legatura din stanga, apoi prin dreapta, altfel va continua doar prin dreapta");
    outtextxy(70,610,"Blocul intrare poate primi doar variabile separate prin virgula");
    outtextxy(70,630,"Blocul iesire poate primi variabile sau expresii separate prin virgula si va afisa rezultatul in zona informatii");
    outtextxy(70,650,"Blocul calcul poate primi variabile sau expresii separate prin virgula");
    outtextxy(cod_x+20,cod_y-70,"Dati click pentru a obtine");
    outtextxy(cod_x,cod_y-50,"codul in limbajul selectat a ultimei");
    outtextxy(cod_x,cod_y-30,"scheme executate cu succes");
    settextstyle(3,0,0);
    setusercharsize(25,100,25,100);
    raza=r;indice_zoom=z;
    ecran_x=e_x;ecran_y=e_y;
}

void help()
{
    cleardevice();
    setbkcolor(fundal);
    clearviewport();
    nr_butoane=1;
    int aux=nr_blocuri;
    afiseaza_text_ajutor();
    while(1)
    {
        delay(100);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            int y=mousey();
            clearmouseclick(WM_LBUTTONDOWN);
            if(y<=50) break;
        }
    }
    nr_butoane=6;
    nr_blocuri=aux;
    continuare=1;
    ecran1();
}

void buton(int i)
{
    switch (i)
    {
    case 0:
    {
        inapoi();
        break ;
    }
    case 1:
    {
        salvare();
        break ;
    }
    case 2:
    {
        executa();
        break ;
    }
    case 3:
    {
        Undo();
        break ;
    }
    case 4:
    {
        redo();
        break ;
    }
    case 5:
    {
        help();
        break ;
    }
    }

}

void bara_meniu(int x, int y)
{
    int width=getmaxwidth()/nr_butoane;
    for(int i=0; i<nr_butoane; i++)
        if(y>=0 && y<50 && x>=i*width && x<=(i+1)*width)
        {
            buton(i);
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
    if(selectat>5 && a[selectat].tip>1 && apartine_text(x,y) )  // citim text daca am dat click pe chenar
    {
        adauga_undo();
        undo.push_back({3,selectat,0,0,0,0,0,0,a[selectat].text});
        citeste_text(selectat);
        strcpy(undo.back().text2,a[selectat].text);
    }
    if(bloc_nou!=-1)
    {
        if(bloc_nou>5) x-=a[bloc_nou].width/2;  // pentru a aduce mijlocul laturii de sus la pozitia mouse-ului, indiferent de zoom
        else
        {
            int w1=a[bloc_nou].width;
            x-=(w1+((indice_zoom-4)*w1)/zoom_ratio)/2;
        }
        bool inter=intersecteaza_alte_blocuri(x,y);
        if(apartine_zona(x+ecran_x,y+ecran_y-colt_y,a[bloc_nou].width,a[bloc_nou].height) && !inter && nr_blocuri<NMAX-10) // plasare blocuri
        {
            int x1=x+ecran_x,y1=y+ecran_y-colt_y;
            if(indice_zoom!=4) //  calculam coordonatele initiale fara zoom
            {
                x1=((x+ecran_x)*zoom_ratio)/(indice_zoom-4+zoom_ratio); // ecuatia inversa de la zoom
                y1=((y+ecran_y-colt_y)*zoom_ratio)/(indice_zoom-4+zoom_ratio);
            }
            if(bloc_nou==0)
            {
                if(start_main>5)    // daca avem deja un bloc start
                {
                    S.push_back({-1,0});
                    eroare=1;
                    afiseaza_erori();
                    event=1;
                }
                else
                {
                    adauga_undo();
                    undo.push_back({1,nr_blocuri});
                    start_main=nr_blocuri;
                    a[nr_blocuri]= {bloc_nou,1,x,y};
                    a[nr_blocuri].x1=x1;
                    a[nr_blocuri++].y1=y1;
                }
            }
            else if(bloc_nou<=5 && nr_blocuri<NMAX-10)    // adaugam blocuri noi
            {
                adauga_undo();
                undo.push_back({1,nr_blocuri});
                a[nr_blocuri]= {bloc_nou,1,x,y};
                a[nr_blocuri].x1=x1;
                a[nr_blocuri++].y1=y1;
            }
            else    // mutam blocuri existente
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
        if(nod_dest!=-1 && (nod_st!=-1 || nod_dr!=-1))
        {
            if(nod_st!=-1)
            {
                adauga_undo();
                undo.push_back({2,nod_st,nod_dest});
                if(a[nod_st].st!=-1) // daca avem deja o legatura, o adaugam la undo pentru a o putea restaura
                {
                    a[a[nod_st].st].ant.remove(nod_st);
                    undo.back().leg=a[nod_st].st;
                }
                a[nod_st].st=nod_dest;
                a[nod_dest].ant.push_back(nod_st);
                nod_st=nod_dr=nod_dest=-1;
            }
            if(nod_dr!=-1)
            {
                adauga_undo();
                undo.push_back({2,nod_dr,nod_dest,0,0,0,0,1});
                if(a[nod_dr].dr!=-1)
                {
                    a[a[nod_dr].dr].ant.remove(nod_dr);
                    undo.back().leg=-a[nod_st].dr;
                }
                a[nod_dr].dr=nod_dest;
                a[nod_dest].ant.push_back(nod_dr);
                a[nod_dest].ant.unique();
                nod_dr=nod_st=nod_dest=-1;
            }
        }
    }
    else selectat=verifica_toate_blocurile(x,y);
    verifica_zoom(x,y);
    verifica_butoane_viteza(x,y);
    verifica_buton_cod(x,y);
}

void seteaza_culori()
{
    fundal=culori[indice_culori[0]];
    color=culori[indice_culori[1]]-1;
    nod_color=culori[indice_culori[2]]-2;
    line_color=culori[indice_culori[3]]-3;
    text_color=culori[indice_culori[4]]-4;
    mark_color=culori[indice_culori[5]]-5;
    setbkcolor(fundal);
    limbaj=indice_culori[6];
    clearviewport();
}

void deseneaza_triunghi(int x1, int y1, int x2, int y2, int x3, int y3)
{
    setcolor(BLACK);
    line(x1,y1,x2,y2);
    line(x1,y1,x3,y3);
    line(x3,y3,x2,y2);
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    floodfill(x1+(x3-x1)/2,y1+(y2-y1)/2,BLACK);

}

void desenare_optiuni_setari()
{
    setbkcolor(fundal);
    for(int i=0; i<nr_setari; i++)  // butoanele gri
    {
        setcolor(culoare_text);
        outtextxy(50,100+i*100,mesaje_setari[i]);
        deseneaza_triunghi(set_nod_x-25,100+i*100,set_nod_x-25,120+i*100,set_nod_x-35,110+i*100);
        deseneaza_triunghi(set_nod_x+25,100+i*100,set_nod_x+25,120+i*100,set_nod_x+35,110+i*100);
    }
    setcolor(culoare_text);
    outtextxy(50,100+(nr_setari-2)*100+20,mesaje_setari[nr_setari]);
    for(int i=0; i<nr_setari; i++)  // textul, respectiv culoarea selectata sau numele limbajului
    {
        if(i==nr_setari-1)
            {outtextxy(set_nod_x-25+10,100+i*100,mesaje_limbaje[limbaj]);
                continue;
            }
        setfillstyle(SOLID_FILL,culori[indice_culori[i]]-i);
        fillellipse(set_nod_x,110+i*100,set_raza,set_raza);
    }
}

void incarca_preview()
{
    int i=NMAX-8;
    a[i]= {0,1,970,100,"TEXT"};
    a[i].st=i+1;
    a[i+1].ant.push_back(i);
    a[i+1]= {2,1,970,200,"TEXT"};
    a[i+1].st=i+2;
    a[i+2].ant.push_back(i+1);
    a[i+2]= {4,1,870,300,"TEXT"};
    a[i+2].st=i+3;
    a[i+2].dr=i+4;
    a[i+3].ant.push_back(i+2);
    a[i+4].ant.push_back(i+2);
    a[i+3]= {1,1,720,400,"TEXT"};
    a[i+4]= {5,1,1020,400,"TEXT"};
    a[i+4].st=i+5;
    a[i+5].ant.push_back(i+4);
    a[i+5]= {3,1,1100,500,"TEXT"};
    a[i+5].st=i+6;
    a[i+6].ant.push_back(i+5);
    a[i+6]= {1,1,1020,600,"TEXT"};
}


void desenare_setari()
{
    setbkcolor(fundal);
    cleardevice();
    seteaza_culori();
    setcolor(culoare_text);
    settextstyle(3,0,2);
    setusercharsize(3,2,3,2);
    outtextxy(700,100,"Preview");
    settextstyle(3,0,0);
    setusercharsize(25,100,25,100);
    for(int i=NMAX-8; i<=NMAX-2; i++)
    {
        if(a[i].st !=-1)
            trasare_legatura(i,a[i].st,a[i].tip,0);
        if(a[i].dr!=-1)
            trasare_legatura(i,a[i].dr,a[i].tip,1);
        desenare_bloc(i);
    }
    marcheaza_bloc(NMAX-6);
    deseneaza_meniu();
    desenare_optiuni_setari();
}

void init_setari()
{
    setbkcolor(fundal);
    cleardevice();
    clearviewport();
    nr_butoane=1;
    incarca_preview();
    desenare_setari();


}
bool verifica_schimbari_setari(int x, int y)
{
    if( (x>=set_nod_x-35 && x<=set_nod_x-25) || (x>=set_nod_x+25 && x<=set_nod_x+35) )
        {
            for(int i=0; i<nr_setari; i++)
            if(y>=(100+i*100) && y<=(120+i*100) )
            {
                if(i==6)
                {
                     if(x>set_nod_x)
                     {
                         if(indice_culori[i]<nr_limbaje-1) indice_culori[i]++,executat=0;
                     }
                     else if(indice_culori[i]>0) indice_culori[i]--,executat=0;
                }
                else
                {
                if(x>set_nod_x) indice_culori[i]=min(indice_culori[i]+1,nr_culori);
                else indice_culori[i]=max(indice_culori[i]-1,0);
                }
                return true ;
            }
        }

    return false;
}


void click_stanga_setari()
{
    int x=mousex(),y=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
    bara_meniu(x,y);
    if(verifica_schimbari_setari(x,y)) desenare_setari();

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
    setcolor(culoare_text);
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
    if(continuare)
    {
        creeazaOptiune(widthEcran,heightEcran,1,6,"CREEAZA SCHEMA");
        creeazaOptiune(widthEcran,heightEcran,2,6,"CONTINUA");
        creeazaOptiune(widthEcran,heightEcran,3,6, "SALVATE");
        creeazaOptiune(widthEcran,heightEcran,4,6, "SCRIE COD");
        creeazaOptiune(widthEcran,heightEcran,5,6, "SETARI");
        creeazaOptiune(widthEcran,heightEcran,6,6, "IESIRE");
    }
    else
    {
        creeazaOptiune(widthEcran,heightEcran,1,5,"CREEAZA SCHEMA");
        creeazaOptiune(widthEcran,heightEcran,2,5, "SALVATE");
        creeazaOptiune(widthEcran,heightEcran,3,5, "SCRIE COD");
        creeazaOptiune(widthEcran,heightEcran,4,5, "SETARI");
        creeazaOptiune(widthEcran,heightEcran,5,5, "IESIRE");
    }

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
                    ecran_schimbat = 1;
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

        if(selectat>5) sterge_info();
        if(selectat<=5 && !eroare && !rezult) sterge_info(),sterge_variabile();

        if(options && ismouseclick(WM_LBUTTONDOWN))    // verificam optiunile sterge, muta, ...
        {
            click_optiuni();
            continue ;
        }

        if(selectat>5)
        {
            if(a[selectat].tip>1) // afisam chenarul in informatii
            {
                char aux[10],text[20]="Blocul ";
                number_to_text(selectat-5,aux);
                strcat(text,aux);
                deseneaza_text(a[selectat].text,0,text);
            }
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
void deschideFisier(int nr)
{
    char cale[NMAX]="salvate/";
    strcat(cale,fisiere[nr]);

    FILE* fptr = fopen(cale,"r");
    fread(&nr_blocuri,sizeof(int),1,fptr);
    fread(&start_main,sizeof(int),1,fptr);

    for(int i=6; i<nr_blocuri; i++)
    {
        int lenSir,lenAnt=0;

        fread(&a[i].tip,sizeof(int),1,fptr);
        fread(&a[i].nr,sizeof(int),1,fptr);
        fread(&a[i].x,sizeof(double),1,fptr);
        fread(&a[i].y,sizeof(double),1,fptr);
        fread(&a[i].x1,sizeof(double),1,fptr);
        fread(&a[i].y1,sizeof(double),1,fptr);
        fread(&a[i].width,sizeof(int),1,fptr);
        fread(&a[i].height,sizeof(int),1,fptr);
        fread(&a[i].h1,sizeof(int),1,fptr);
        fread(&a[i].w1,sizeof(int),1,fptr);
        fread(&lenSir,sizeof(int),1,fptr);
        fread(&a[i].text,lenSir,1,fptr);
        a[i].text[lenSir] = '\0';
        fread(&a[i].ok,sizeof(bool),1,fptr);
        fread(&a[i].bucla,sizeof(bool),1,fptr);
        fread(&a[i].st,sizeof(int),1,fptr);
        fread(&a[i].dr,sizeof(int),1,fptr);
        fread(&lenAnt,sizeof(int),1,fptr);

        for(int j=1; j<=lenAnt; j++)
        {
            int val;
            fread(&val,sizeof(int),1,fptr);
            a[i].ant.push_back(val);
        }
    }

    fclose(fptr);
    ecran_x=ecran_y=0;
    schimba_coord();
    indice_zoom=4;
    raza=7;

}

void redeseneaza(int curEcranY)
{
    setbkcolor(fundal);
    cleardevice();
    int width=getmaxwidth();
    for(int i=0; i<nr_butoane; i++)
        desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);

    int curEcranX = 0;
    int widthEcran =  getmaxwidth();
    int heightEcran = getmaxheight()-100;
    int spatiuStanga = 100;
    int spatiuSus = 0;
    int widthBloc = 200;
    int heightBloc = 100;
    int cnt = 0;

    for(int i=1; i<=nr_fisiere && cnt != 4; i++)
    {
        char *str = &fisiere[i][0];
        if(curEcranY + spatiuSus >= 100)
        {
            cnt++;
            rectangle(curEcranX + spatiuStanga,curEcranY + spatiuSus,curEcranX + spatiuStanga + widthBloc,curEcranY + spatiuSus + heightBloc);
            setbkcolor(fundal);
            setcolor(text_color);
            outtextxy(curEcranX + spatiuStanga + 50,curEcranY + spatiuSus + 35,str);
        }
        spatiuSus += 150;
    }
}

void Deschide(int i)
{
    int nr = 0;
    deschideFisier(i);

    cleardevice();
    setbkcolor(BLACK);
    clearviewport();

    for(int i=0; i<7; i++)
        ecrane[i] = 0;

    ecrane[1] = 1;
    nr_butoane = 6;
    ecranCurent = 1;
    ecran_schimbat=1;
    continuare=2;
    fisier_curent=i;
}

void Sterge(int bloc)
{
    char fisierStergere[NMAX] = "";
    strcat(fisierStergere,"salvate/");
    strcat(fisierStergere,fisiere[bloc]);
    remove(fisierStergere);
    for(int i=bloc; i<nr_fisiere; i++)
        strcpy(fisiere[i],fisiere[i+1]);
    nr_fisiere--;
    fisier_curent=-1;
}

void Redenumeste(int bloc, int curEcranY)
{
    char numeVechi[NMAX];
    strcpy(numeVechi,fisiere[bloc]);
    strcpy(fisiere[bloc],"");
    redeseneaza(curEcranY);

    bool ok=1;
    int nr=0;
    while(ok)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            ok=0;
        }
        if(kbhit())
        {
            char c=getch();
            if(c==13 || nr==10)
            {
                ok=0;
                break;
            }
            if(c==8)
            {
                if(nr>0) nr--;
            }
            else  fisiere[bloc][nr++]=c;
            fisiere[bloc][nr]='\0';

            redeseneaza(curEcranY);
        }
    }
    strcat(fisiere[bloc],".txt");
    salvare(bloc,numeVechi,1);

}

bool verifica_optiuni_salvate(int x, int y, int bloc, int curEcranY)
{
    int a=mousex(),b=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
    int nr=(b-y)/option_height;
    if(a<x || a>x+option_width || b<y || b>y+3*option_height) return false ;
    if(nr==0) Deschide(bloc);
    if(nr==1) Sterge(bloc);
    if(nr==2) Redenumeste(bloc,curEcranY);
    return true;
}

int click_stanga_salvate(int curBloc,int nr_fisiere)
{
    int x=mousex(),y=mousey();
    clearmouseclick(WM_LBUTTONDOWN);
    bara_meniu(x,y);
    event=1;
    int xs = 100,ys = 100;
    for(int i=1+curBloc; i<=nr_fisiere; i++)
    {
        if(xs <= x && x <= xs + 200 && ys <= y && y <= ys+100)
            return i;
        ys += 150;
    }
    return -1;

}

void init_salvate()
{
    cleardevice();
    setbkcolor(fundal);
    clearviewport();
    nr_butoane=1;
    deseneaza_meniu();

}

void desenare_salvate()
{
    cleardevice();
    setbkcolor(fundal);
    clearviewport();
    deseneaza_meniu();
}


void ecran2()
{

    cleardevice();
    listFile(fisiere,nr_fisiere);
    nr_butoane = 1;

    int curEcranY = 100;
    int curBloc = 0;
    int bloc=-1;
    bool event=1,ok=0;

    while(!ecran_schimbat)
    {
        delay(100);
        if(event) redeseneaza(curEcranY),event=0;
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            if(bloc!=-1)
                verifica_optiuni_salvate(300,(bloc-curBloc-1)*150+100,bloc,curEcranY);
            bloc=click_stanga_salvate(curBloc,nr_fisiere);
            if(ok) event=1,ok=0;

        }
        if(bloc!=-1 && ismouseclick(WM_RBUTTONDOWN))
        {
            clearmouseclick(WM_RBUTTONDOWN);
            desenare_optiuni(300,(bloc-curBloc-1)*150+100,3,"Deschide","Sterge","Redenumeste");
            ok=1;
        }
        if(kbhit())
        {
            char c=getch();

            if(c == 72)
            {
                if(curBloc > 0)
                {
                    curEcranY += 150;
                    event=1;  //redeseneaza(curEcranY,nr_fisiere,fisiere);
                    curBloc--;
                }

            }
            else if(c == 80)
            {
                if(curBloc + 4 < nr_fisiere)
                {
                    curEcranY -= 150;
                    event=1;//redeseneaza(curEcranY,nr_fisiere,fisiere);
                    curBloc++;
                }

            }

        }
    }

}

void afisareCursor()
{
    int len;

    cleardevice();
    setfillstyle(SOLID_FILL,fundal);
    floodfill(1,51,BLUE);
    setbkcolor(fundal);

    int width=getmaxwidth() / 2;
    for(int i=0; i<1; i++)
        desenare_buton(i*width,(i+1)*width,culori_butoane[i],mesaje_butoane[i]);
    desenare_buton(1*width,(1+1)*width,culori_butoane[1],mesaje_butoane[2]);

    for(int i=0; i<=linieCod; i++)
    {
        setbkcolor(fundal);
        setcolor(text_color);
        outtextxy(100,100+50*i,cod[i]);
    }

    len = textwidth(cod[linieCaracter]) - textwidth(cod[linieCaracter]+indexCaracter);
    line(100+len,100+50*linieCaracter,100+len,125+50*linieCaracter);
}

void scrieInFisier(char s[][NMAX],int n)
{
    FILE* ptr = fopen("cod.txt", "w");
    for(int i=0; i<=n; i++)
        fprintf(ptr, "%s", s[i]);
    fclose(ptr);
}


void ecran3()
{
    cleardevice();
    setbkcolor(fundal);
    char codCaracter,pred = -1;
    nr_butoane = -1;

    afisareCursor();
    while(!ecran_schimbat)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            click_stanga();
            int x = mousex();
            int y = mousey();
            if(x >= 0 && x <= getmaxwidth() / 2 && y >= 0 && y <= 50)
                inapoi();
            else if(x >= getmaxwidth() / 2 && x <= getmaxwidth() && y >= 0 && y <= 50)
            {
                schemaGresita = 0;
                startInceput = 0;

                scrieInFisier(cod,linieCod);
                nod* obiect = generare("cod.txt");

                if(obiect== NULL)
                    schemaGresita = 1;

                else if(strcmp(obiect->eticheta,"start") != 0)
                    schemaGresita = 1;

                FILE *fd=fopen("output.txt","r+");

                if(!schemaGresita)
                {
                    int xstart = 200,ystart = 200;
                    continuare = 0;
                    init();
                    creeazaSchema(obiect,xstart,ystart,-1);
                    for(int i=0; i<7; i++)
                        ecrane[i] = 0;
                    ecrane[1] = 1;
                    nr_butoane = 6;
                    ecran_schimbat = 1;
                    continuare = 2;
                }

                for(int i=6; i<nr_blocuri; i++)
                {
                    cout<<a[i].tip<<" "<<a[i].x<<" "<<a[i].y<<endl;
                }
                schimba_coord();

                fclose(fd);

            }
        }

        if(kbhit())
        {
            int nrElemente = strlen(cod[linieCaracter]);
            codCaracter = getch();


            if(codCaracter == 8) // backspace
            {

                if(indexCaracter != 0)
                {
                    for(int i=indexCaracter-1; i<nrElemente; i++)
                        cod[linieCaracter][i] = cod[linieCaracter][i+1];

                    indexCaracter--;
                }
                else
                {
                    linieCaracter--;

                    if(linieCaracter != -1)
                    {
                        indexCaracter = strlen(cod[linieCaracter]);
                        strcat(cod[linieCaracter],cod[linieCaracter+1]);
                        for(int i = linieCaracter+1; i<=linieCod; i++)
                            strcpy(cod[i],cod[i+1]);

                        linieCod = linieCod-1;
                    }
                    else
                    {
                        linieCaracter = 0;
                    }


                }

            }
            else if(codCaracter == 13) // enter
            {

                linieCaracter++;
                for(int i=linieCod+1; i>=linieCaracter; i--)
                    strcpy(cod[i],cod[i-1]);
                linieCod = max(linieCod,linieCaracter);

                char aux[100] = {'\0'};
                strcat(aux,cod[linieCaracter-1]+indexCaracter);
                strcat(aux,"\0");
                strcpy(cod[linieCaracter],aux);
                cod[linieCaracter-1][indexCaracter] = '\0';
                indexCaracter = 0;
                linieCod++;

            }
            else if(codCaracter == 75 && pred == 0)
            {
                indexCaracter = max(0,indexCaracter-1);
            }
            else if(codCaracter == 77 && pred == 0)
            {
                indexCaracter = min(nrElemente,indexCaracter+1);
            }
            else if(codCaracter == 72 && pred == 0)
            {
                linieCaracter--;
                if(linieCaracter != -1)
                {
                    int len = strlen(cod[linieCaracter]);
                    indexCaracter = min(indexCaracter,len);
                }
                else
                {
                    linieCaracter = 0;
                }
            }
            else if(codCaracter == 80 && pred == 0)
            {
                linieCaracter++;
                if(linieCaracter > linieCod)
                {
                    linieCaracter = linieCod;
                }
                else
                {
                    int len = strlen(cod[linieCaracter]);
                    indexCaracter = min(indexCaracter,len);
                }
            }
            else if(codCaracter != 0)
            {

                cod[linieCaracter][nrElemente+1] = '\0';

                for(int i = nrElemente; i > indexCaracter; i--)
                {
                    cod[linieCaracter][i] = cod[linieCaracter][i-1];
                }
                cod[linieCaracter][indexCaracter] = codCaracter;
                indexCaracter++;
            }
            pred = codCaracter;
            afisareCursor();
        }
    }

}

void ecran4()
{
    init_setari();
    while(!ecran_schimbat)
        if(ismouseclick(WM_LBUTTONDOWN))  click_stanga_setari();
}
void ecran5()
{
    cleardevice();
}

void launch()
{
    incarca_blocuri();
    setlinestyle(SOLID_LINE,0,1);
    restaurare_setari();
}

int main()
{
    ecrane[0] = 1; //Meniul este primul ecran apelat
    ecranCurent = 0;

    // int page = 1;  // Initializarea animatiei

    //initwindow(1500,768);
    initwindow(getmaxwidth(),getmaxheight()-25,"InterSchem");///1530 810

    // registermousehandler(WM_LBUTTONUP,click_handler); //Click Handlerul
    launch();
    do
    {
        settextstyle(3,0,0);
        setusercharsize(25,100,25,100);
        if(ecran_schimbat)
        {
            ecran_schimbat=0;
            delay(100);
            if(ecrane[0])
                ecran0();
            else if(!continuare)
            {


                if(ecrane[1])
                    ecran1();
                else if(ecrane[2] )
                    ecran2();
                else if(ecrane[3])
                    ecran3();
                else if(ecrane[4])
                    ecran4();
                else if(ecrane[5])
                {
                    salvare_setari();
                    exit(1);
                }
            }
            else
            {


                if(ecrane[1]) // butonul continuare
                {
                    if(continuare != 2)
                        continuare=0;
                    else
                        continuare = 1;
                    ecran1();
                }
                else if(ecrane[2])
                    ecran1();
                else if(ecrane[3])
                    ecran2();
                else if(ecrane[4])
                    ecran3();
                else if(ecrane[5])
                    ecran4();
                else if(ecrane[6])
                {
                    salvare_setari();
                    exit(1);
                }
            }
        }

        //page = 1-page;

    }
    while(true);

    getch();
    closegraph();

    return 0;
}

