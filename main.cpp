#include <iostream>
#include<fstream>
#include<cstring>
#include <graphics.h>
#include <winbgim.h>
# define WIDTH 1400
# define HEIGHT 700
#define MAX1 20
#define MAX2 40
#define FUNDAL 0
using namespace std;

char butoane[5][10], mesaj[30]="  INFO";
void adaugareButoane()
{
    strcpy(butoane[0],"DELETE");
    strcpy(butoane[1],"  EDIT");
    strcpy(butoane[2],"ROTATE");
    strcpy(butoane[3],"  INFO");
    for(int i=0; i<4; ++i)
    {
        rectangle(i*350,HEIGHT-101,(i+1)*350-1,HEIGHT-1);
        outtextxy(i*350+100,HEIGHT-60,butoane[i]);
    }
}

float zoom=10.0;

struct descriere
{
    char comanda[MAX1];
    unsigned nComenzi;
    float x1[MAX1], y1[MAX1], x2[MAX1], y2[MAX1];
};

struct punctLegatura
{
    float x, y;
    unsigned cateLeg=0;
    struct{
        unsigned bloc, nod;
    }leg[10];
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
    struct
    {
        unsigned bloc1, bloc2, nod1, nod2;
    } bloc;
    unsigned varianta;
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
        fscanf(f,"%f%f",&P.pLeg[i].x,&P.pLeg[i].y);
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
    line(P.x+zoom*x_1,P.y+zoom*y_1,P.x+zoom*x_2,P.y+zoom*y_2); ///P.x=cursor, x_1 din fisier
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
    rectangle(P.x+zoom*x_1,P.y+zoom*y_1,P.x+zoom*x_2,P.y+zoom*y_2);
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
    ellipse(P.x+x_1*zoom,P.y+y_1*zoom,0,360,x_2*zoom,y_2*zoom);
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
        arc(P.x+x1*zoom,P.y+y1*zoom,-90,90,x2*zoom);
        break;
    case 1:
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*zoom,P.y+y_1*zoom,0,180,x2*zoom);
        break;
    case 2:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        arc(P.x+x1*zoom,P.y+y1*zoom,90,270,x2*zoom);
        break;
    case 3:
        roteste(x1,y1,x_1,y_1);
        roteste(x_1,y_1,x1,y1);
        roteste(x1,y1,x_1,y_1);
        arc(P.x+x_1*zoom,P.y+y_1*zoom,-180,0,x2*zoom);
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
            P.pLeg[j].x=P.x+zoom*P.pLeg[j].x;
            P.pLeg[j].y=P.y-zoom*P.pLeg[j].y;
        }
    P.orientare=orient;
    deseneaza(P, WHITE);
}

void roteste(piesa& P)
{
    deseneaza(P,FUNDAL);
    P.orientare=(P.orientare+1) % 4;
    deseneaza(P, WHITE);
}
void drawLine(float x1, float y1, float x2, float y2, int cul)
{
    setcolor(cul);
    line(x1,y1,(x1+x2)/2,y1);
    line((x1+x2)/2,y1,(x1+x2)/2,y2);
    line((x1+x2)/2,y2,x2,y2);
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
        drawLine(x1,y1,xMouse,yMouse, 15-FUNDAL);
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
        drawLine(x1,y1,xMouse,yMouse,FUNDAL);
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
                drawLine(x1,y1,xMouse,yMouse,FUNDAL);
                clickGol=true;
                click=true;
            }
        }
    }

    while (!click);

    /// verificam daca linia are un corespondent
    if(!clickGol)
    {
        //cout<<"da";
        drawLine(x1,y1,x2,y2, RED);
        for(i=1; i<=nrLegaturi; i++)
            drawLine(P[Legatura[i].bloc.bloc1].pLeg[Legatura[i].bloc.nod1].x,P[Legatura[i].bloc.bloc1].pLeg[Legatura[i].bloc.nod1].y,P[Legatura[i].bloc.bloc2].pLeg[Legatura[i].bloc.nod2].x,P[Legatura[i].bloc.bloc2].pLeg[Legatura[i].bloc.nod2].y,RED);
        nrLegaturi++;
        Legatura[nrLegaturi].bloc.bloc1=idNod1;
        Legatura[nrLegaturi].bloc.bloc2=idNod2;
        Legatura[nrLegaturi].bloc.nod1=p1;
        Legatura[nrLegaturi].bloc.nod2=p2;
        P[idNod2].pLeg[p2].cateLeg++;
        P[idNod2].pLeg[p2].leg[P[idNod2].pLeg[p2].cateLeg].bloc=idNod1;
        P[idNod2].pLeg[p2].leg[P[idNod2].pLeg[p2].cateLeg].nod=p1;
        P[idNod1].pLeg[p1].leg[P[idNod1].pLeg[p1].cateLeg].bloc=idNod2;
    }

}

unsigned alegePiesa(piesa vectorPiese[MAX2], unsigned nrPiese)
{
    bool click1=false, click2=false;
    unsigned i, nrPiesaAleasa=0, j;
    int idNod1, idNod2, p1, p2;
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
                        //cout<<"leg: "<<vectorPiese[i].pLeg[j].x<<' '<<vectorPiese[i].pLeg[j].y<<endl;
                    }
                    else if(abs(vectorPiese[i].pLeg[j].x-xMouse)<=20 && abs(vectorPiese[i].pLeg[j].y-yMouse)<=20)
                    {
                        //cout<<"leg: "<<vectorPiese[i].pLeg[j].x<<' '<<vectorPiese[i].pLeg[j].y<<endl;
                        idNod1=i, p1=j;
                        click2=true;
                        vectorPiese[i].pLeg[j].cateLeg++;
                        //cout<<vectorPiese[i].pLeg[j].x<<endl;
                    }
        }
    while (!click1 && !click2);
    if(click1)
        return nrPiesaAleasa;
    else if(click2)
    {

        deseneazaLegatura(vectorPiese,idNod1,p1,idNod2,p2,nrPiese);
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
    outtextxy(1150,HEIGHT-60,mesaj);
    setcolor(WHITE);
    strcpy(mesaj,P.valoare);
    strcat(mesaj, " ");
    strcat(mesaj,P.unitate);
    outtextxy(1150,HEIGHT-60,mesaj);
}

int main()
{
    int mainWindow=initwindow(WIDTH,HEIGHT,"ELECTRON",0,0);
    settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
    adaugareButoane();

    //legatura Legatura[MAX2];
    //unsigned nrLegaturi=0;

    unsigned nrPiesaAleasa;
    deseneazaMeniul(Meniu,nrPieseMeniu);
    bool prelucreaza=false;

    do
    {

        if (ismouseclick(WM_LBUTTONDOWN))
        {
            if(mousey()<100)
            {
                nrPiesaAleasa=alegeOPiesaDinMeniu(Meniu,nrPieseMeniu);
                // printf("Am ales piesa %u\n",nrPiesaAleasa);
                if (nrPiesaAleasa!=nrPieseMeniu && nrPiesaAleasa!=0)
                {
                    nrPiese++;
                    Piesa[nrPiese]=Meniu[nrPiesaAleasa];
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
                    ///cout<<nrPiesaAleasa<<' '<<Piesa[nrPiesaAleasa].pLeg[1].x<<' '<<Piesa[nrPiesaAleasa].pLeg[1].y<<' '<<Piesa[nrPiesaAleasa].x<<endl;
                }
            }
            else
            {
                if(prelucreaza)
                {
                    /// dupa ce am pus o piesa pe tabla nu va aparea nimic pe INFO
                    /// odata ce am mai dat inca o data click va aparea 0 cu unitatea ei de masura
                    int xMouse=mousex();
                    if(xMouse<350)                                                ///DELETE
                    {
                        deseneaza(Piesa[nrPiesaAleasa],FUNDAL);
                        for(int i=nrPiesaAleasa; i<nrPiese; i++)
                            Piesa[i]=Piesa[i+1];
                        nrPiese--;
                        //cout<<"nrPiese: "<<nrPiese<<endl;
                    }
                    if(xMouse>=350 && xMouse<700)      ///EDITEAZA valoarea intr-o ferastra
                    {
                        newWindow(Piesa[nrPiesaAleasa],mainWindow); /// avem nevoie de mainWindow pentru a o face prioritara dupa ce am terminat cu popup window
                        /*setcolor(FUNDAL);
                        outtextxy(1150,HEIGHT-60,mesaj);
                         //editeaza(Piesa[nrPiesaAleasa])
                         setcolor(WHITE);
                         strcpy(mesaj,Piesa[nrPiesaAleasa].valoare);
                         strcat(mesaj, " ");
                         strcat(mesaj,Piesa[nrPiesaAleasa].unitate);
                         outtextxy(1150,HEIGHT-60,mesaj);*/
                    }
                    if(xMouse>=700 && xMouse<1050)        ///ROTATE
                    {
                        roteste(Piesa[nrPiesaAleasa]);
                    }
                    if(xMouse>=1050)        ///INFO
                        valoarePiesa(Piesa[nrPiesaAleasa],mesaj);
                }
            }
            valoarePiesa(Piesa[nrPiesaAleasa],mesaj);
            clearmouseclick(WM_LBUTTONDOWN);
        }
    }while (nrPiesaAleasa!=nrPieseMeniu);
    /*for(int i=1;i<=nrPiese;i++)
    cout<<Piesa[i].x<<' '<<Piesa[i].y<<' '<<Piesa[i].nume<<endl;*/
    // getch(); 
    closegraph(mainWindow);
    return 0;
}
