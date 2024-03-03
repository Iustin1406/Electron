#include <iostream>
#include<fstream>
#include<cstring>
#include <graphics.h>
#include <winbgim.h>
#define WIDTH 1400
#define HEIGHT 700
#define MAX1 20
#define MAX2 40
#define FUNDAL 0
using namespace std;


char butoane[7][10], mesaj[30]=" INFO";
void adaugareButoane()
{
    strcpy(butoane[0],"DELETE");
    strcpy(butoane[1]," EDIT");
    strcpy(butoane[2],"ROTATE");
    strcpy(butoane[3]," INFO");
    strcpy(butoane[4],"Z+");
    strcpy(butoane[5],"Z-");
    strcpy(butoane[6],"MOVE");
    strcpy(butoane[7],"SAVE");
    strcpy(butoane[8],"OPEN");

    for(int i=0; i<3; ++i)
    {
        rectangle(i*200,HEIGHT-101,(i+1)*200,HEIGHT-1);
        outtextxy(i*200+30,HEIGHT-60,butoane[i]);
    }
    line(600,HEIGHT-101,WIDTH,HEIGHT-101);
    outtextxy(650,HEIGHT-60,butoane[3]);
    rectangle(900,HEIGHT-101,1000,HEIGHT-1);
    outtextxy(925,HEIGHT-60,butoane[4]);
    rectangle(1000,HEIGHT-101,1100,HEIGHT-1);
    outtextxy(1025,HEIGHT-60,butoane[5]);
    rectangle(1100,HEIGHT-101,1300,HEIGHT-1);
    outtextxy(1150,HEIGHT-60,butoane[6]);
    line(1300,HEIGHT-51,WIDTH,HEIGHT-51);
    outtextxy(1305,HEIGHT-80,butoane[7]);
    outtextxy(1305,HEIGHT-30,butoane[8]);
}

//float zoom=10.0;

struct descriere
{
    char comanda[MAX1];
    unsigned nComenzi;
    float x1[MAX1], y1[MAX1], x2[MAX1], y2[MAX1];
};

struct punctLegatura
{
    float x, y;
    float x1, y1;
    unsigned cateLeg=0;
    /*struct{
        unsigned bloc, nod;
    }leg[10];*/
};

void roteste (float x, float y, float & xnou, float & ynou)
{
    xnou = y;
    ynou = -x;
}

struct piesa
{
    unsigned id;
    char nume[MAX1];
    float zoom=10;
    int x,y;              ///patratul din jur de latura 120: (P.x-60,P.y-60), (P.x+60, P.y+60)
    unsigned orientare;
    char continut[MAX2];
    unsigned nrLegaturi;
    punctLegatura pLeg[MAX1];
    descriere descr;
    ///valori pt calcule
    unsigned nr_piesa;
    char unitate[10];
    char valoare[10];
};

struct legatura
{
    unsigned bloc1, bloc2, nod1, nod2;
    unsigned varianta=0;
    float x, y;
};
legatura Legatura[MAX2];
unsigned nrLegaturi=0;
piesa Meniu[MAX1];
unsigned nrPieseMeniu=12;

piesa Piesa[MAX2];
unsigned nrPiese=0;
void initializeaza(piesa& P)
{
    P.orientare=0;
    char numeFisier[MAX1],s[MAX1];
    strcpy(numeFisier,P.nume);
    strcat(numeFisier,".ps");
    FILE* f=fopen(numeFisier,"rt");
    fscanf(f,"%s",s);
    /*printf("%s\n",s);*/
    fscanf(f,"%d",&P.nrLegaturi);
    /*printf("%d\n",P.nrLegaturi); */
    for (unsigned i=1; i<=P.nrLegaturi; i++)
    {
        fscanf(f,"%f%f",&P.pLeg[i].x1,&P.pLeg[i].y1);
        //printf("%f %f\n",P.pLeg[i].x,P.pLeg[i].y);
    }
    fscanf(f,"%s",P.unitate);
    //cout<<P.nume<<' '<<P.unitate<<endl;
    fscanf(f,"%s",s);
    /* printf("%s\n",s); */
    fscanf(f,"%d\n",&P.descr.nComenzi);
    /* printf("%d comenzi\n",P.descr.nComenzi); */
    for (unsigned i=1; i<=P.descr.nComenzi; i++)
    {
        fscanf(f,"%c%f%f",&P.descr.comanda[i],&P.descr.x1[i],&P.descr.y1[i]);
        fscanf(f,"%f%f\n",&P.descr.x2[i],&P.descr.y2[i]);
        /* printf("%c %f %f %f %f\n",P.descr.comanda[i],P.descr.x1[i],P.descr.y1[i],P.descr.x2[i],P.descr.y2[i]); */
    }
    strcpy(P.valoare,"0");
    fclose(f);
}

void myLine(piesa P, unsigned i)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i]; ///din fisier
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch (P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        roteste(x2,y2,x_2,y_2);
        break;
    }
    line(P.x+P.zoom*x_1,P.y+P.zoom*y_1,P.x+P.zoom*x_2,P.y+P.zoom*y_2); ///P.x=cursor, x_1 din fisier
}

void myRectangle(piesa P, unsigned i)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch (P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        roteste(x2,y2,x_2,y_2);
        roteste(x_2,y_2,x2,y2);
        roteste(x2,y2,x_2,y_2);
        break;
    }
    rectangle(P.x+P.zoom*x_1,P.y+P.zoom*y_1,P.x+P.zoom*x_2,P.y+P.zoom*y_2);
}

void myEllipse(piesa P, unsigned i)
{
    float x_1,y_1,x_2,y_2;
    float x1,y1,x2,y2;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i];
    y2=P.descr.y2[i];
    switch (P.orientare)
    {
    case 0:
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        x_2=y2;
        y_2=x2;
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        x_1=x1;
        y_1=y1;
        x_2=x2;
        y_2=y2;
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        x_2=y2;
        y_2=x2;
        break;
    }
    ellipse(P.x+x_1*P.zoom,P.y+y_1*P.zoom,0,360,x_2*P.zoom,y_2*P.zoom);
}

void myArc(piesa P, unsigned i)
{
    float x1,y1,x2;
    float x_1,y_1;
    x1=P.descr.x1[i];
    y1=P.descr.y1[i];
    x2=P.descr.x2[i]; /* y2=P.descr.y2[i]; */
    switch (P.orientare)
    {
    case 0:
        arc(P.x+x1*P.zoom,P.y+y1*P.zoom,-90,90,x2*P.zoom);
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*P.zoom,P.y+y_1*P.zoom,0,180,x2*P.zoom);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        arc(P.x+x1*P.zoom,P.y+y1*P.zoom,90,270,x2*P.zoom);
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*P.zoom,P.y+y_1*P.zoom,-180,0,x2*P.zoom);
        break;
    }
}

void deseneaza(piesa P, int culoare)
{
    setcolor(culoare);
    for (unsigned i=1; i<=P.descr.nComenzi; i++)
        switch(P.descr.comanda[i])
        {
        case 'L':
            myLine(P,i);
            break;
        case 'R':
            myRectangle(P,i);
            break;
        case 'O':
            myEllipse(P,i);
            break;
        case 'A':
            myArc(P,i);
            break;
        }
}

void amplaseaza(piesa& P, unsigned x, unsigned y, unsigned orient, unsigned a)
{
    P.x=x;
    P.y=y;
    //cout<<'i';
    if(a==2)
        for(int j=1; j<=P.nrLegaturi; j++)
        {
            //cout<<'a';
            P.pLeg[j].x=P.x+P.zoom*P.pLeg[j].x1;
            P.pLeg[j].y=P.y-P.zoom*P.pLeg[j].y1;
        }
    P.orientare=orient;
    deseneaza(P, WHITE);
}

void roteste(piesa& P)
{
    deseneaza(P,FUNDAL);
    P.orientare=(P.orientare+1) % 4;
    deseneaza(P, WHITE);
    for(int i=1; i<=P.nrLegaturi; i++)
    {
        int k=P.pLeg[i].x1;
        P.pLeg[i].x1=-P.pLeg[i].y1;
        P.pLeg[i].y1=k;
        P.pLeg[i].x=P.x+P.zoom*P.pLeg[i].x1;
        P.pLeg[i].y=P.y-P.zoom*P.pLeg[i].y1;
    }

}
void drawLine(float x1, float y1, float x2, float y2, int cul, int l)
{
    setcolor(cul);
    switch(l){
    case 0:{
    line(x1,y1,(x1+x2)/2,y1);
    circle((x1+x2)/2,y1,5);
    line((x1+x2)/2,y1,(x1+x2)/2,y2);
    line((x1+x2)/2,y2,x2,y2);
    break; }
    case 1:{
    line(x1,y1,x2,y1);
    line(x2,y1,x2,y2);
    circle(x2,y1,5);
    break;
    }
    case 2:{
    line(x1,y1,x1,y2);
    line(x1,y2,x2,y2);
    circle(x1,y2,5);
    break;
    }
    }
     /*   case 1:
    {if((x2-x3)*(x3-x1)>0 && (x2-x3)*(x3-x1)>0)
    {
        line(x1,y1,x1,(y1+y2)/2);
        line(x1,(y1+y2)/2,x2,(y1+y2)/2);
        line(x2,(y1+y2)/2,x2,y2);
    }
    else
    if((x2-x3)*(x3-x1)>0) {
    if(y2>y1){
        line(x1,y1,x1,y1+1.5*(x3-x1));
        drawLine(x1,y1+1.5*(x3-x1),x2,y2,cul,0,0,0,0,0);}
        else
        {
            line(x1,y1,x1,y1-1.5*(x3-x1));
            drawLine(x1,y1-1.5*(x3-x1),x2,y2,cul,0,0,0,0,0);
        }
    }
     else
     if((x2-x4)*(x4-x1)>0){
         swap(x1,x2); swap(y1,y2); swap(x3,x4); swap(y3,y4);
         if(y2>y1){
        line(x1,y1,x1,y1+1.5*(x3-x1));
        drawLine(x1,y1+1.5*(x3-x1),x2,y2,cul,0,0,0,0,0);}
        else
        {
            line(x1,y1,x1,y1-1.5*(x3-x1));
            drawLine(x1,y1-1.5*(x3-x1),x2,y2,cul,0,0,0,0,0);
        }
     }
     else{
    line(x1,y1,(x1+x2)/2,y1);
    line((x1+x2)/2,y1,(x1+x2)/2,y2);
    line((x1+x2)/2,y2,x2,y2);}
     break;}
}*/
    /*line(x1,y1,x1,(y1+y2)/2);
    line(x1,(y1+y2)/2,x2,(y1+y2)/2);
    line(x2,(y1+y2)/2,x2,y2);*/
}

void deseneazaMeniul(piesa Meniu[MAX2],unsigned int nrPieseMeniu)
{
    setcolor(WHITE);
    rectangle(0,0,getmaxx(),getmaxy());
    strcpy(Meniu[1].nume,"DIODA");
    strcpy(Meniu[2].nume,"ZENNER");
    strcpy(Meniu[3].nume,"TRANZNPN");
    strcpy(Meniu[4].nume,"TRANZPNP");
    strcpy(Meniu[5].nume,"CONDENS");
    strcpy(Meniu[6].nume,"REZIST");
    strcpy(Meniu[7].nume,"BATERIE");
    strcpy(Meniu[8].nume,"POLARIZ");
    strcpy(Meniu[9].nume,"SINU");
    strcpy(Meniu[10].nume,"SERVOMOT");
    strcpy(Meniu[11].nume,"AMPLOP");
    strcpy(Meniu[12].nume,"STOP");
    rectangle(0,0,getmaxx(),100);
    int lat=getmaxx()/nrPieseMeniu;
    for (unsigned i=1; i<=nrPieseMeniu; i++)
    {
        initializeaza(Meniu[i]);
        amplaseaza(Meniu[i],lat*i-lat/2,50,0, 1);
    }
}
bool verif(float a_x,float a_y,float b_x,float b_y,float c_x,float c_y)
{
    cout<<"Se intersecteaza\n";
    return (c_y-a_y)*(b_x-a_x) > (b_y-a_y)*(c_x-a_x);
}
bool intersecteazaLegaturi(float a_x,float a_y,float b_x,float b_y,piesa Piesa[],unsigned nrLegaturi)
{
    for(int i=1; i<=nrLegaturi; i++)
    {
        float c_x=Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x;
        float c_y=Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y;
        float d_x=Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x;
        float d_y=Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y;
        if(verif(a_x,a_y,c_x,c_y,d_x,d_y)!= verif(b_x,b_y,c_x,c_y,d_x,d_y)
                && verif(a_x,a_y,b_x,b_y,c_x,c_y)!= verif(a_x,a_y,b_x,b_y,d_x,d_y))
            {cout<<"se intersecteaza\n";return true;}
    }
    return false;
}
bool seIntersecteaza(piesa Piesa[],unsigned nrLegaturi)
{
    for(int i=1; i<nrLegaturi; ++i)
    {
        float a_x=Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x;
        float a_y=Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y;
        float b_x=Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x;
        float b_y=Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y;

        for(int j=i+1; j<=nrLegaturi; j++)
        {
            float c_x=Piesa[Legatura[j].bloc1].pLeg[Legatura[j].nod1].x;
            float c_y=Piesa[Legatura[j].bloc1].pLeg[Legatura[j].nod1].y;
            float d_x=Piesa[Legatura[j].bloc2].pLeg[Legatura[j].nod2].x;
            float d_y=Piesa[Legatura[j].bloc2].pLeg[Legatura[j].nod2].y;
            if(verif(a_x,a_y,c_x,c_y,d_x,d_y)!= verif(b_x,b_y,c_x,c_y,d_x,d_y)
                    && verif(a_x,a_y,b_x,b_y,c_x,c_y)!= verif(a_x,a_y,b_x,b_y,d_x,d_y))
                return true;
        }
    }
    return false;
}
void deseneazaLegatura(piesa P[MAX2],int &idNod1, int &p1, int &idNod2, int &p2, int nrPiese)
{
    bool click=false;
    bool clickGol=false;
    int i, xMouse,yMouse, j;
    // aleg un punct de legatura (p1) de la nodul 1 (idNod1)
    // aleg un punct de legatura (p2) de la nodul 2 (idNod2)
    // pana aleg, desenez mereu o linie
    float x1,y1,x2,y2;
    i=idNod1;
    j=p1;
    //cout<<j<<' ';
    x1=P[i].pLeg[j].x;
    y1=P[i].pLeg[j].y;
    // cout<<"1:"<<x1<<' ';
    xMouse=mousex();
    yMouse=mousey();
    do
    {
        drawLine(x1,y1,xMouse,yMouse, 15-FUNDAL,0);
        /// redesenam muchiile in caz ca stergem cand umblam cu legatura
        for(i=1; i<=nrLegaturi; i++){
            drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);}

        /// sa deseneze din nou in caz ca sterge marginile
        if(yMouse<=100)
            deseneazaMeniul(Meniu,nrPieseMeniu);
        if(yMouse>=HEIGHT-100)
            adaugareButoane();

        // cout<<endl<<"2:"<<x1<<' ';
        for (i=1; i<=nrPiese; i++)
            //if(xMouse+60 < P[i].x-60 || xMouse-60 > P[i].x+60 || yMouse+60 < P[i].y-60 || yMouse-60 > P[i].y+60)
            if((P[i].x-xMouse)*(x1-P[i].x)>=0 || (P[i].y-yMouse)*(y1-P[i].y)>=0)
                deseneaza(P[i],15-FUNDAL);
        delay(50);
        //cout<<"3:"<<x1<<' ';
        drawLine(x1,y1,xMouse,yMouse,FUNDAL,0);
        //  cout<<"4:"<<x1<<endl;
        xMouse=mousex();
        yMouse=mousey();
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            Beep(1200,100);
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            for (i=1; i<=nrPiese; i++)
                for(j=1; j<=P[i].nrLegaturi; j++)
                    if(i!=idNod1 && abs(P[i].pLeg[j].x-xMouse)<=10 && abs(P[i].pLeg[j].y-yMouse)<=10)
                    {
                        //cout<<x1;
                        idNod2=i, p2=j;
                        x2=P[i].pLeg[j].x;
                        y2=P[i].pLeg[j].y;
                        click=true;
                        //cout<<"x2:"<<x2<<endl;
                    }
            //cout<<xMouse<<' '<<yMouse<<'\n';
            /// eliminam linia care nu are capat final
            if(!click)
            {
                drawLine(x1,y1,xMouse,yMouse,FUNDAL,0);
                clickGol=true;
                click=true;
            }
        }
    }

    while (!click);

    /// verificam daca linia are un corespondent
    if(!clickGol && !intersecteazaLegaturi(x1,y1,x2,y2,P,nrLegaturi))
    {
        //cout<<"da";
        drawLine(x1,y1,x2,y2, 15-FUNDAL,0);
        setcolor(RED);
        line(x1,y1,x2,y2);
        /// desenam toate legaturile
        for(i=1; i<=nrLegaturi; i++)
            drawLine(P[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,P[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                     P[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,P[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                     15-FUNDAL,Legatura[i].varianta);
        nrLegaturi++;
        Legatura[nrLegaturi].bloc1=idNod1;
        Legatura[nrLegaturi].bloc2=idNod2;
        Legatura[nrLegaturi].nod1=p1;
        Legatura[nrLegaturi].nod2=p2;
        switch(Legatura[nrLegaturi].varianta)
        {
        case 0:
            {
                Legatura[nrLegaturi].x=(x1+x2)/2;
                Legatura[nrLegaturi].y=y1;
                break;
            }
        case 1:
            {
                Legatura[nrLegaturi].x=x2;
                Legatura[nrLegaturi].y=y1;
                break;
            }
        case 2:
            {
                Legatura[nrLegaturi].x=x1;
                Legatura[nrLegaturi].y=y2;
                break;
            }
        }
        P[idNod2].pLeg[p2].cateLeg++;
        for (i=1; i<=nrPiese; i++)
                deseneaza(P[i],15-FUNDAL);
        for(i=1; i<=nrLegaturi; i++){
            drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);}

        /*P[idNod2].pLeg[p2].leg[P[idNod2].pLeg[p2].cateLeg].bloc=idNod1;
        P[idNod2].pLeg[p2].leg[P[idNod2].pLeg[p2].cateLeg].nod=p1;
        P[idNod1].pLeg[p1].leg[P[idNod1].pLeg[p1].cateLeg].bloc=idNod2;

        for(i=1;i<=nrLegaturi;i++)
            cout<<Legatura[i].bloc1<<'('<<Legatura[i].nod1<<')'<<'-'<<Legatura[i].bloc2<<'('<<Legatura[i].nod2<<')'<<' ';
        cout<<endl;*/
    }
    /// desenam legaturile in caz ca stergem cu muchia si nu avem capat final de leg
    for(i=1; i<=nrLegaturi; i++)
        drawLine(P[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,P[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                 P[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,P[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                 15-FUNDAL,Legatura[i].varianta);

}

unsigned alegePiesa(piesa vectorPiese[MAX2], unsigned nrPiese)
{
    bool click1=false, click2=false, click3=false;
    unsigned i, nrPiesaAleasa=0, j;
    int idNod1, idNod2, p1, p2, numarl;
    int xMouse,yMouse;
    //cout<<'p';
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click1 && !click2)
        {
            //cout<<mousey()<<'\n';
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex();
            yMouse=mousey();
            for (i=1; i<=nrPiese; i++)     ///alege piesa din meniu
                for(j=1; j<=vectorPiese[i].nrLegaturi; j++)
                    if (abs(vectorPiese[i].x - xMouse)<=15
                            && abs(vectorPiese[i].y - yMouse)<=15)
                    {
                        //cout<<j;
                        nrPiesaAleasa=i;
                        click1=true;
                        //cout<<'I'<<endl;
                        //cout<<"leg: "<<vectorPiese[i].pLeg[j].x<<' '<<vectorPiese[i].pLeg[j].y<<endl;
                    }
                    else if(abs(vectorPiese[i].pLeg[j].x-xMouse)<=5 && abs(vectorPiese[i].pLeg[j].y-yMouse)<=5)
                    {
                        //cout<<"leg: "<<vectorPiese[i].pLeg[j].x<<' '<<vectorPiese[i].pLeg[j].y<<endl;
                        idNod1=i, p1=j;
                        click2=true;
                        vectorPiese[i].pLeg[j].cateLeg++;
                        //cout<<"II"<<endl;
                        //cout<<vectorPiese[i].pLeg[j].x<<endl;
                    }
//cout<<'v';
                for(i=1;i<=nrLegaturi;i++){
                    if(abs(Legatura[i].x-xMouse)<=5 && abs(Legatura[i].y-yMouse)<=5){ //cout<<"III"<<endl;
                        click3=true;
                        numarl=i;
                    }
                    //cout<<i<<':'<<Legatura[i].x<<' '<<xMouse<<' '<<Legatura[i].y<<' '<<yMouse<<endl;
                    }
        }
    while (!click1 && !click2 && !click3);
    if(click1)
        return nrPiesaAleasa;
    else if(click2)
    {
        deseneazaLegatura(vectorPiese,idNod1,p1,idNod2,p2,nrPiese);
        return 0;
    }
    else if(click3)
    {   //cout<<"numarl:"<<numarl<<endl;
        drawLine(vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].x,vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].y,
                 vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].x,vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].y,
                 FUNDAL,Legatura[numarl].varianta);
        Legatura[numarl].varianta=(Legatura[numarl].varianta+1)%3;
        //cout<<"varianta:"<<Legatura[numarl].varianta<<endl;
        switch(Legatura[numarl].varianta)
        {
        case 0:
            {
                Legatura[numarl].x=(vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].x+vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].x)/2;
                Legatura[numarl].y=vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].y;
                break;
            }
        case 1:
            {
                Legatura[numarl].x=vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].x;
                Legatura[numarl].y=vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].y;
                break;
            }
        case 2:
            {
                Legatura[numarl].x=vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].x;
                Legatura[numarl].y=vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].y;
                break;
            }
        }
        drawLine(vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].x,vectorPiese[Legatura[numarl].bloc1].pLeg[Legatura[numarl].nod1].y,
                 vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].x,vectorPiese[Legatura[numarl].bloc2].pLeg[Legatura[numarl].nod2].y,
                 15-FUNDAL,Legatura[numarl].varianta);
        for(i=1; i<=nrLegaturi; i++)
        drawLine(vectorPiese[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,vectorPiese[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                 vectorPiese[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,vectorPiese[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                 15-FUNDAL,Legatura[i].varianta);
        return 0;
    }
}

unsigned alegeOPiesaDinMeniu(piesa Meniu[MAX2],unsigned nrPieseMeniu)
{
    unsigned nrPiesaAleasa=0;
    nrPiesaAleasa=alegePiesa(Meniu,nrPieseMeniu);
    // printf("%d, ",nrPiesaAleasa);
    return nrPiesaAleasa;
}

bool faraSuprapunere(int x1,int y1,piesa Piesa[], unsigned nrPiese)
{
    for(int i=1; i<nrPiese; i++)
        if(!(x1+60 < Piesa[i].x-60 || x1-60 > Piesa[i].x+60 || y1+60 < Piesa[i].y-60 || y1-60 > Piesa[i].y+60))
            return false;
    return true;

}

void punePiesa(piesa& P, piesa* Piesa, unsigned nrPiese)
{
    bool click=false;
    int xMouse,yMouse;
    do
        if(ismouseclick(WM_LBUTTONDOWN) && !click)
        {
            clearmouseclick(WM_LBUTTONDOWN);
            xMouse=mousex(); ///coordonatele pieselor
            yMouse=mousey();
            if(yMouse>=140 && yMouse<HEIGHT-135 && faraSuprapunere(xMouse,yMouse,Piesa,nrPiese)) ///sa nu se afle cursorul pe meniu sau pe alta piesa deja pusa
            {
                //printf("%d, %d\n",xMouse,yMouse);
                amplaseaza(P,xMouse,yMouse,0,2);
                click=true;
            }
        }
    while (!click);
}
void newWindow1(char nrfisier[],int mainWindow)
{
    int tempWindow=initwindow(600,250,"OPEN",0,701);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
    setcolor(WHITE);
    char Enter=13,Escape=27,BackSpace=8,tasta,mesaj[30]="Open circuit number ",tasta1[2];
    outtextxy(10,10,mesaj);
    do
    {
        tasta=getch();
        if(strchr("0123456789",tasta))
        {
            tasta1[0]=tasta;
            strcat(mesaj,tasta1);
        }
        else if(tasta==BackSpace)
        {
            setcolor(FUNDAL);
            outtextxy(10,10,mesaj);
            mesaj[strlen(mesaj)-1]='\0';
            setcolor(WHITE);
        }

        outtextxy(10,10,mesaj);

    }
    while (tasta!=Enter && tasta!=Escape);
    //cout<<mesaj+20;
    strcpy(nrfisier,mesaj+20);
    setcurrentwindow(mainWindow);
    closegraph(tempWindow);
}
void newWindow(piesa& P,int mainWindow)
{
    int tempWindow=initwindow(600,250,"INFO",0,701);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
    setcolor(WHITE);
    char Enter=13,Escape=27,BackSpace=8,tasta,mesaj[30]="Setati valoarea: ",tasta1[2];
    outtextxy(10,10,mesaj);
    do
    {
        tasta=getch();
        if(strchr("0123456789",tasta))
        {
            tasta1[0]=tasta;
            strcat(mesaj,tasta1);
        }
        else if(tasta==BackSpace)
        {
            setcolor(FUNDAL);
            outtextxy(10,10,mesaj);
            mesaj[strlen(mesaj)-1]='\0';
            setcolor(WHITE);
        }

        outtextxy(10,10,mesaj);

    }
    while (tasta!=Enter && tasta!=Escape);
    strcpy(P.valoare,mesaj+17);
    setcurrentwindow(mainWindow);
    closegraph(tempWindow);
}
void valoarePiesa(piesa& P,char mesaj[])
{
    setcolor(FUNDAL);
    outtextxy(650,HEIGHT-60,mesaj);
    setcolor(WHITE);
    strcpy(mesaj,P.valoare);
    strcat(mesaj, " ");
    strcat(mesaj,P.unitate);
    outtextxy(650,HEIGHT-60,mesaj);
}

int main()
{
    int mainWindow=initwindow(WIDTH,HEIGHT,"ELECTRON",0,0);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
    adaugareButoane();
    int i, j;
    //legatura Legatura[MAX2];
    //unsigned nrLegaturi=0;
    ifstream fCS("numarCircuiteSalvate.txt");
    int numarCS;
    fCS>>numarCS;
    //cout<<numarCS;
    fCS.close();
    unsigned nrPiesaAleasa;
    deseneazaMeniul(Meniu,nrPieseMeniu);
    bool prelucreaza=false;
//     int xm=0, ym=0;
    do
    {

        if (ismouseclick(WM_LBUTTONDOWN))
        {  // xm=mousex(); ym=mousey();
            if(mousey()<100)
            {
                nrPiesaAleasa=alegeOPiesaDinMeniu(Meniu,nrPieseMeniu);
                // printf("Am ales piesa %u\n",nrPiesaAleasa);
                if (nrPiesaAleasa!=nrPieseMeniu && nrPiesaAleasa!=0)
                {
                    nrPiese++;
                    Piesa[nrPiese]=Meniu[nrPiesaAleasa];
                    Piesa[nrPiese].id=nrPiesaAleasa;
                    punePiesa(Piesa[nrPiese], Piesa, nrPiese);
                }
            }
            else if(mousey()<=HEIGHT-135)
            {
                nrPiesaAleasa=alegePiesa(Piesa,nrPiese);
                //printf("Am ales piesa %u\n",nrPiesaAleasa);
                if(nrPiesaAleasa!=0)
                {
                    prelucreaza=true;
                    /// redesenam piesele pentru a ramane cu galben ultima piesa selectata
                    for(int i=1; i<=nrPiese; ++i)
                        deseneaza(Piesa[i],WHITE);

                    deseneaza(Piesa[nrPiesaAleasa],YELLOW); /// coloram piesa cu galben pentru a arata ca vrem sa o folosim
                    //cout<<nrPiesaAleasa<<' '<<Piesa[nrPiesaAleasa].pLeg[1].x<<' '<<Piesa[nrPiesaAleasa].pLeg[1].y<<' '<<Piesa[nrPiesaAleasa].x<<endl;
                }
            }
            else
            {
                if(prelucreaza)
                {
                    /// dupa ce am pus o piesa pe tabla nu va aparea nimic pe INFO
                    /// odata ce am mai dat inca o data click va aparea 0 cu unitatea ei de masura
                    int xMouse=mousex();
                    if(xMouse<200)                                                ///DELETE
                    {   //cout<<nrPiesaAleasa<<endl;
                        deseneaza(Piesa[nrPiesaAleasa],FUNDAL);
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1;i<=nrLegaturi;i++){
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                {
                                    drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                FUNDAL,Legatura[i].varianta);
                                     for(j=i; j<nrLegaturi; j++)
                                        Legatura[j]=Legatura[j+1];
                                    nrLegaturi--;
                                    i--;
                                }
                            }
                            for(i=nrPiesaAleasa; i<nrPiese; i++)
                            Piesa[i]=Piesa[i+1];
                            nrPiese--;
                            for(i=1; i<=nrLegaturi; i++){
                                if(Legatura[i].bloc1>nrPiesaAleasa) Legatura[i].bloc1--;
                                if(Legatura[i].bloc2>nrPiesaAleasa) Legatura[i].bloc2--;
                                drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);
                             }
                        }

                        for(i=1; i<=nrPiese; i++)
                            deseneaza(Piesa[i],15-FUNDAL);
                        //cout<<"nrPiese: "<<nrPiese<<endl;
                    }
                    if(xMouse>=200 && xMouse<400)      ///EDITEAZA valoarea intr-o ferastra
                    {
                        newWindow(Piesa[nrPiesaAleasa],mainWindow); /// avem nevoie de mainWindow pentru a o face prioritara dupa ce am terminat cu popup window
                    }
                    if(xMouse>=400 && xMouse<600)        ///ROTATE
                    {
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                  drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                FUNDAL,Legatura[i].varianta);
                         }
                        roteste(Piesa[nrPiesaAleasa]);
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                   drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);
                            for(i=1; i<=nrPiese; i++)
                                deseneaza(Piesa[i],15-FUNDAL);
                        }
                    }
                    if(xMouse>=600 && xMouse<900)        ///INFO
                        valoarePiesa(Piesa[nrPiesaAleasa],mesaj);
                    if(xMouse>=900 && xMouse<1000)      ///ZOOM IN
                    {
                        deseneaza(Piesa[nrPiesaAleasa],FUNDAL);
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                    drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                FUNDAL,Legatura[i].varianta);
                         }
                        Piesa[nrPiesaAleasa].zoom++;
                        for(int j=1; j<=Piesa[nrPiesaAleasa].nrLegaturi; j++)
                        {
                           Piesa[nrPiesaAleasa].pLeg[j].x=Piesa[nrPiesaAleasa].x+Piesa[nrPiesaAleasa].zoom*Piesa[nrPiesaAleasa].pLeg[j].x1;
                           Piesa[nrPiesaAleasa].pLeg[j].y=Piesa[nrPiesaAleasa].y-Piesa[nrPiesaAleasa].zoom*Piesa[nrPiesaAleasa].pLeg[j].y1;
                        }
                        deseneaza(Piesa[nrPiesaAleasa],WHITE);
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                  drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);
                           for(i=1; i<=nrPiese; i++)
                                deseneaza(Piesa[i],15-FUNDAL);
                        }
                    }
                    if(xMouse>=1000 && xMouse<1100)      ///ZOOM OUT
                    {
                        deseneaza(Piesa[nrPiesaAleasa],FUNDAL);
                         if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                       drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                FUNDAL,Legatura[i].varianta);
                        }
                        Piesa[nrPiesaAleasa].zoom--;
                        deseneaza(Piesa[nrPiesaAleasa],WHITE);
                        /// deseneaza legaturile deoarece cand micsoram se sterg capetele legaturii
                        /*for(i=1; i<=nrLegaturi; i++)
                            drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,
                                     Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                     Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,
                                     Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,15-FUNDAL);*/
                        for(int j=1; j<=Piesa[nrPiesaAleasa].nrLegaturi; j++)
                        {
                           Piesa[nrPiesaAleasa].pLeg[j].x=Piesa[nrPiesaAleasa].x+Piesa[nrPiesaAleasa].zoom*Piesa[nrPiesaAleasa].pLeg[j].x1;
                           Piesa[nrPiesaAleasa].pLeg[j].y=Piesa[nrPiesaAleasa].y-Piesa[nrPiesaAleasa].zoom*Piesa[nrPiesaAleasa].pLeg[j].y1;
                        }
                        deseneaza(Piesa[nrPiesaAleasa],WHITE);
                        if(Piesa[nrPiesaAleasa].nrLegaturi!=0)
                        {
                            for(i=1; i<=nrLegaturi; i++)
                                if(Legatura[i].bloc1==nrPiesaAleasa || Legatura[i].bloc2==nrPiesaAleasa)
                                     drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);
                             for(i=1; i<=nrPiese; i++)
                                deseneaza(Piesa[i],15-FUNDAL);
                        }
                    }
                    else if(xMouse>=1100 && xMouse<1300) /// MOVE
                    {
                        int x1=Piesa[nrPiesaAleasa].x,y1=Piesa[nrPiesaAleasa].y;
                        clearmouseclick(WM_LBUTTONDOWN);
                        while (!ismouseclick(WM_LBUTTONDOWN)) {}
                        //Piesa[nrPiesaAleasa].x=mousex();
                        //Piesa[nrPiesaAleasa].y=mousey();
                        if(mousey()>=130 && mousey()<=HEIGHT-130)
                        {

                            amplaseaza(Piesa[nrPiesaAleasa],mousex(),mousey(),Piesa[nrPiesaAleasa].orientare,2);
                            if(!seIntersecteaza(Piesa,nrLegaturi))
                            {
                                //cout<<"nu se intersecteaza\n";
                                //Piesa[nrPiesaAleasa].x=x1;
                                //Piesa[nrPiesaAleasa].y=y1;
                                amplaseaza(Piesa[nrPiesaAleasa],x1,y1,Piesa[nrPiesaAleasa].orientare,2);
                                deseneaza(Piesa[nrPiesaAleasa],FUNDAL);

                                for(i=1; i<=nrLegaturi; i++)
                                       drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                FUNDAL,Legatura[i].varianta);

                                amplaseaza(Piesa[nrPiesaAleasa],mousex(),mousey(),Piesa[nrPiesaAleasa].orientare,2);
                                for(i=1; i<=nrLegaturi; i++)
                                    drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);

                            }
                            else
                            {
                                //cout<<"se intersecteaza\n";
                                deseneaza(Piesa[nrPiesaAleasa],FUNDAL);
                                Piesa[nrPiesaAleasa].x=x1;
                                Piesa[nrPiesaAleasa].y=y1;
                            }
                        }
                    }
                }
                if(mousex()>1300)
                {
                        if(mousey()<HEIGHT-50)  ///SAVE
                        {
                            numarCS++;
                            //cout<<numarCS;
                            char sir[30];
                            strcpy(sir,"circuit");
                            char nrchar[5];
                            int p=numarCS, k=0;
                            while(p)
                            {
                                nrchar[k]='0'+p%10;
                                p/=10;
                                k++;
                            }
                            nrchar[k]='\0';
                            for(int m=0;m<k/2;m++)
                            {
                                char aux=nrchar[m];
                                nrchar[m]=nrchar[k-m-1];
                                nrchar[k-m-1]=aux;
                            }
                            strcat(sir,nrchar);
                            strcat(sir,".txt");
                            //cout<<sir;
                            ofstream fCS("numarCircuiteSalvate.txt", ofstream::trunc);
                            fCS << numarCS;
                            fCS.close();
                            ofstream fout(sir);
                            fout<<nrPiese<<endl;
                            for(i=1;i<=nrPiese;i++)
                            {
                                fout<<Piesa[i].id<<' '<<Piesa[i].x<<' '<<Piesa[i].y<<' '<<Piesa[i].orientare<<' '<<Piesa[i].zoom<<' '<<Piesa[i].nrLegaturi<<endl;
                                for(j=1;j<=Piesa[i].nrLegaturi;j++)
                                {
                                    fout<<Piesa[i].pLeg[j].x<<' '<<Piesa[i].pLeg[j].y<<' '<<Piesa[i].pLeg[j].x1<<' '<<Piesa[i].pLeg[j].y1<<endl;
                                }
                            }
                            fout<<nrLegaturi<<endl;
                            for(i=1;i<=nrLegaturi;i++)
                                fout<<Legatura[i].bloc1<<' '<<Legatura[i].bloc2<<' '<<Legatura[i].nod1<<' '<<Legatura[i].nod2<<' '<<Legatura[i].varianta<<' '<<Legatura[i].x<<' '<<Legatura[i].y<<endl;
                            fout.close();
                        }
                        if(mousey()>HEIGHT-50)  ///OPEN
                        {
                            char nrfisier[5];
                            newWindow1(nrfisier,mainWindow);
                            char sir[30];
                            strcpy(sir,"circuit");
                            strcat(sir,nrfisier);
                            strcat(sir,".txt");
                            //cout<<sir;
                            cleardevice();
                            deseneazaMeniul(Meniu,nrPieseMeniu);
                            adaugareButoane();
                            ifstream fin(sir);
                            fin>>nrPiese;
                            for(i=1;i<=nrPiese;i++)
                            {
                                fin>>j;
                                Piesa[i]=Meniu[j];
                                Piesa[i].id=j;
                                fin>>Piesa[i].x>>Piesa[i].y>>Piesa[i].orientare>>Piesa[i].zoom>>Piesa[i].nrLegaturi;
                                //cout<<Piesa[i].id<<' '<<Piesa[i].x<<' '<<Piesa[i].y<<' '<<Piesa[i].orientare<<' '<<Piesa[i].nrLegaturi<<endl;
                                for(j=1;j<=Piesa[i].nrLegaturi;j++)
                                    fin>>Piesa[i].pLeg[j].x>>Piesa[i].pLeg[j].y>>Piesa[i].pLeg[j].x1>>Piesa[i].pLeg[j].y1;
                                    //cout<<Piesa[i].pLeg[j].x<<' '<<Piesa[i].pLeg[j].y<<' '<<Piesa[i].pLeg[j].x1<<' '<<Piesa[i].pLeg[j].y1<<endl;}
                                deseneaza(Piesa[i],15-FUNDAL);
                            }
                            fin>>nrLegaturi;
                            for(i=1;i<=nrLegaturi;i++)
                            {
                                fin>>Legatura[i].bloc1>>Legatura[i].bloc2>>Legatura[i].nod1>>Legatura[i].nod2>>Legatura[i].varianta>>Legatura[i].x>>Legatura[i].y;
                                drawLine(Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].x,Piesa[Legatura[i].bloc1].pLeg[Legatura[i].nod1].y,
                                                Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].x,Piesa[Legatura[i].bloc2].pLeg[Legatura[i].nod2].y,
                                                15-FUNDAL,Legatura[i].varianta);
                            }
                            fin.close();
                        }
                    }
                //deseneaza(Piesa[nrPiesaAleasa],WHITE);
            }
            valoarePiesa(Piesa[nrPiesaAleasa],mesaj);
            clearmouseclick(WM_LBUTTONDOWN);
        }

    }while(nrPiesaAleasa!=20);
    //while (xm<=1300 || ym>100);
    closegraph(mainWindow);
    return 0;
}
