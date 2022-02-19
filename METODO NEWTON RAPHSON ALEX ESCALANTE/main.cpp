#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include "resource.h"
using namespace std;
HINSTANCE hInst;
HBRUSH brush;
HPEN greenPen;
COLORREF actual;

double P_ANTERx=9999,P_ANTERy=9999;
bool glFUN=0,Cierre=0,InserTT=0;

void PREPROCESO_ENTRADA_DATO(HWND,char *);
void CALCULAR_PUNTOS_FUNCION(HDC);
void GRAF_LINE_COORD(HDC);
void GRAF_LINE_PUNTO(HDC,double,double);
void GRAF_PUNTO(HDC,double,double);
void GRAF_LINE_FUNCION(HDC,double,double);

///CONCATENAR
void InConcatINT(int,int);
void InConcatDOUBLE(double,int);
void InConcatCHAR(char,int);
void InConcatSTRING(string,int);

int PRINTF_NUM_INTERACCIONES=0;
double INTERACIONES_RES[6];
class Concat{
    private:
        char *A;
        char *C;
    public:
    Concat(){
        A=new char[3];
        A[0]='\0';
        C=new char[3];
        C[0]='\0';
    }
    void CONCATENAR(char *B){
        int i=0;for(;*(A+i);i++);
        int j=0;for(;*(B+j);j++);
        int S=i+j+4;C=new char[S];
        for(i=0;*(A+i);i++){
            *(C+i)=*(A+i);
        }for(j=0;*(B+j);j++,i++){
            *(C+i)=*(B+j);
        }*(C+i)='\0';
        A=C;
    }
    void Mostrar(HWND hwndDlg,int Posis){
        if(Posis==0){
            SetDlgItemText(hwndDlg,EDIT11,C);
        }
        if(Posis==1){
            SetDlgItemText(hwndDlg,EDIT12,C);
        }
        if(Posis==2){
            SetDlgItemText(hwndDlg,EDIT13,C);
        }
        if(Posis==3){
            SetDlgItemText(hwndDlg,EDIT14,C);
        }
    }
    char *MostrarIN(){
        return C;
    }
}INTERACIONES[5];

void InConcatINT(int X,int Pos){
    char cad[10];
    itoa(X,cad,10);
    INTERACIONES[Pos].CONCATENAR(cad);
}

void InConcatDOUBLE(double BDTy,int Pos){
    double NUM=BDTy;double NUMRES_DOUBLE;
    int NUM_A=NUM;
    NUM*=100000;
    NUMRES_DOUBLE=NUM-NUM_A*100000;
    if(BDTy<0&&NUM_A==0){
        InConcatCHAR('-',Pos);
    }
    if(NUMRES_DOUBLE!=0){
        char cad1[10],cad2[10];
        itoa(NUM_A,cad1,10);
        if(NUMRES_DOUBLE<0){
            NUMRES_DOUBLE*=-1;
        }
        itoa(NUMRES_DOUBLE,cad2,10);
        for(int g=0;*(cad2+g);g++){
            if(*(cad2+g)=='0'){bool CNT=1;
                for(int h=g;*(cad2+h);h++){
                    if(*(cad2+h)!='0'){
                        CNT=0;
                    }
                }if(CNT){
                    *(cad2+g)='\0';break;
                }
            }
        }
        INTERACIONES[Pos].CONCATENAR(cad1);
        InConcatCHAR('.',Pos);
        INTERACIONES[Pos].CONCATENAR(cad2);
    }else{
        char cad1[10];
        itoa(NUM_A,cad1,10);
        InConcatINT(NUM_A,Pos);
    }
}

void InConcatCHAR(char X,int Pos){
    char Var[2];
    Var[0]=X;
    Var[1]='\0';
    INTERACIONES[Pos].CONCATENAR(Var);
}

void InConcatSTRING(string X,int Pos){
    char *Var;int i=0;
    for(;X[i];i++);
    Var=new char[i+1];
    for(i=0;X[i];i++){
        Var[i]=X[i];
    }Var[i]='\0';
    INTERACIONES[Pos].CONCATENAR(Var);
}

///CONCATENAR


///METODO DE NEWTON

///OPERA ALGE
struct DATOS_ALGEBRAICOS{
    double  NumARR[30];
    char    VarARR[30];
    int     PotARR[30];
    int     CANT_COMBIN;
}D[2];

void IntroDATOS(double NUM,char VAR,int POT){
    D[0].NumARR[D[0].CANT_COMBIN]=NUM;
    D[0].VarARR[D[0].CANT_COMBIN]=VAR;
    D[0].PotARR[D[0].CANT_COMBIN]=POT;
    D[0].CANT_COMBIN++;D[1].CANT_COMBIN++;
}

void DERIVADA(){
    for(int i=0;i<D[0].CANT_COMBIN;i++){
        if(D[0].NumARR[i]!=0){
            if(D[0].VarARR[i]=='x'){
                D[1].NumARR[i]=D[0].NumARR[i]*D[0].PotARR[i];
                D[1].PotARR[i]=D[0].PotARR[i]-1;
                if(D[1].PotARR[i]==0){
                    D[1].VarARR[i]='1';
                }else{
                    D[1].VarARR[i]='x';
                }
            }else{
                D[1].NumARR[i]=0;
                D[1].PotARR[i]=0;
                D[1].VarARR[i]='1';
            }
        }
    }
}

double OPERACIONRESUL(bool NIVEL,double ValDATO){
    double RESULTADO_Y=0;
    for(int i=0;i<D[NIVEL].CANT_COMBIN;i++){
        if(D[NIVEL].NumARR[i]!=0){
            double RPOT=ValDATO;
            if(D[NIVEL].VarARR[i]=='x'){
                for(int S=1;S<D[NIVEL].PotARR[i];S++){
                    RPOT*=ValDATO;
                }
                RESULTADO_Y+=RPOT*D[NIVEL].NumARR[i];
            }else{
                RESULTADO_Y+=D[NIVEL].NumARR[i];
            }
        }
    }
    return RESULTADO_Y;
}

///OPERA ALGE

int METODO_DE_NEWTON(double X_INI=1){
    double X_ANT,fX,fpX;
    DERIVADA();
    cout<<"f(x): \n";
    for(int i=0;i<D[0].CANT_COMBIN;i++){
        cout<<D[0].NumARR[i];
        cout<<D[0].VarARR[i]<<"^";
        cout<<D[0].PotARR[i]<<endl;
    }
    cout<<"\nf`(x): \n";
    for(int i=0;i<D[1].CANT_COMBIN;i++){
        cout<<D[1].NumARR[i];
        cout<<D[1].VarARR[i]<<"^";
        cout<<D[1].PotARR[i]<<endl;
    }

    //cout<<"\n R:: "<<X_INI<<"\n\n";
    INTERACIONES_RES[0]=X_INI;
    for(int N=0;N<4;N++){
        fX=OPERACIONRESUL(0,X_INI);
        fpX=OPERACIONRESUL(1,X_INI);
        X_ANT=X_INI;
        X_INI=X_ANT-fX/fpX;

        InConcatDOUBLE(X_ANT,N);
        InConcatSTRING("-(",N);
        InConcatDOUBLE(fX,N);
        InConcatSTRING("/",N);
        InConcatDOUBLE(fpX,N);
        InConcatSTRING(") = ",N);
        InConcatDOUBLE(X_INI,N);

        //cout<<"\n R:: "<<X_ANT<<"-("<<fX<<"/"<<fpX<<") = "<<X_INI<<"\n\n";
        INTERACIONES_RES[N+1]=X_INI;
    }
/*
    for(int N=0;N<5;N++){
        ///cout<<INTERACIONES[N].MostrarIN()<<endl;
        cout<<endl<<INTERACIONES_RES[N];
    }
*/
    return 0;
}

///METODO DE NEWTON

///PREPROCESO

void GENERAR_DATO(int inicio,int fin,char *CAD){
    Concat TIPOS_D_DATOS[3];
    char NewCHAR_VAR[3];NewCHAR_VAR[0]='1';NewCHAR_VAR[1]='\0';
    char NewCHAR_ONE[3];NewCHAR_ONE[0]='1';NewCHAR_ONE[1]='\0';
    for(int i=inicio;i<fin;i++){
        if(*(CAD+i)!='x'){
            for(;i<fin;i++){
                if(*(CAD+i)=='x'){i++;
                    NewCHAR_VAR[0]='x';
                    if(*(CAD+i)=='^'){i++;
                        for(;i<fin;i++){
                            char NewCHAR_POT[3];NewCHAR_POT[0]=*(CAD+i);NewCHAR_POT[1]='\0';
                            TIPOS_D_DATOS[1].CONCATENAR(NewCHAR_POT);
                        }
                    }
                    break;
                }
                char NewCHAR[3];NewCHAR[0]=*(CAD+i);NewCHAR[1]='\0';
                TIPOS_D_DATOS[0].CONCATENAR(NewCHAR);
            }
        }else{
            if(*(CAD+i)=='x'){i++;
                NewCHAR_VAR[0]='x';
                TIPOS_D_DATOS[0].CONCATENAR(NewCHAR_ONE);
                if(*(CAD+i)=='^'){i++;
                    for(;i<fin;i++){
                        char NewCHAR_POT[3];NewCHAR_POT[0]=*(CAD+i);NewCHAR_POT[1]='\0';
                        TIPOS_D_DATOS[1].CONCATENAR(NewCHAR_POT);
                    }
                }
            }
        }
    }

    double NUMERO_DOUBLE;
    char   VAR_CHAR;
    int    POT_INT;

    if(TIPOS_D_DATOS[0].MostrarIN()[0]){
        NUMERO_DOUBLE=atof(TIPOS_D_DATOS[0].MostrarIN());
    }else{
        NUMERO_DOUBLE=atof(NewCHAR_ONE);
    }
    VAR_CHAR=*(NewCHAR_VAR+0);
    if(TIPOS_D_DATOS[1].MostrarIN()[0]){
        POT_INT=atoi(TIPOS_D_DATOS[1].MostrarIN());
    }else{
        POT_INT=atoi(NewCHAR_ONE);
    }
    //cout<<NUMERO_DOUBLE<<" "<<VAR_CHAR<<" "<<POT_INT;
    IntroDATOS(NUMERO_DOUBLE,VAR_CHAR,POT_INT);

}

bool PREPROCESO_ENTRADA_DATO(HWND hwndDlg,char *CADENA,double X_EVALUADO){
    if(*(CADENA+0)!='\0'){
        SetDlgItemText(hwndDlg,EDIT8,"CORRECTO...");
        D[0].CANT_COMBIN=0;
        D[1].CANT_COMBIN=0;
        for(int i=0,j=0;;i++){
            if(*(CADENA+i)=='-'||*(CADENA+i)=='+'||*(CADENA+i)=='\0'){
                GENERAR_DATO(j,i,CADENA);j=i;
            }
            if(*(CADENA+i)=='\0'){
                break;
            }
        }
        METODO_DE_NEWTON(X_EVALUADO);
    }else{
        SetDlgItemText(hwndDlg,EDIT8,"NO EXISTE ENTRADA...");
        return 0;
    }
    return 1;
}

///PREPROCESO

///APARTADO GRAFICO

void CALCULAR_PUNTOS_FUNCION(HDC hdc){
    if(glFUN){
        for(double DTx=-45.0,DTy;DTx<44;DTx+=0.1){
            DTy=OPERACIONRESUL(0,DTx);cout<<"";
            GRAF_LINE_FUNCION(hdc,DTx,DTy);
        }P_ANTERx=9999,P_ANTERy=9999;
    }
}

void GRAF_LINE_COORD(HDC hdc){
    MoveToEx(hdc,20,590,NULL);
    LineTo(hdc,880,590);
    MoveToEx(hdc,450,300,NULL);
    LineTo(hdc,450,900);

    greenPen=CreatePen(PS_SOLID, 4, RGB(0, 50, 50));
    SelectObject(hdc,greenPen);
        for(int i=20;i<=880;i+=10){
            MoveToEx(hdc,i,590,NULL);
            LineTo(hdc,i,590);
        }
        for(int i=300;i<=900;i+=10){
            MoveToEx(hdc,450,i,NULL);
            LineTo(hdc,450,i);
        }
    DeleteObject(greenPen);

    SetBkMode(hdc, TRANSPARENT);
    actual = SetTextColor(hdc, RGB(0,255,0));
        TextOut(hdc,435,295,"y",1);
        TextOut(hdc,428,885,"- y",3);
        TextOut(hdc,20,595,"- x",3);
        TextOut(hdc,875,595,"x",1);
    SetTextColor(hdc, actual);
}

void GRAF_LINE_PUNTO(HDC hdc,double x,double y){
    greenPen=CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
    SelectObject(hdc,greenPen);
        if(x>0&&y>0){
            x*=10;y*=10;
            x+=450;y*=-1;y+=590;
            for(int i=450;i<=x;i+=5){
                MoveToEx(hdc,i,y,NULL);
                LineTo(hdc,i,y);
            }
            for(int i=y;i<=590;i+=5){
                MoveToEx(hdc,x,i,NULL);
                LineTo(hdc,x,i);
            }
        }
        if(x<0&&y>0){
            x*=10;y*=10;
            x+=450;y*=-1;y+=590;
            for(int i=x;i<=450;i+=5){
                MoveToEx(hdc,i,y,NULL);
                LineTo(hdc,i,y);
            }
            for(int i=y;i<=590;i+=5){
                MoveToEx(hdc,x,i,NULL);
                LineTo(hdc,x,i);
            }
        }
        if(x<0&&y<0){
            x*=10;y*=10;
            x+=450;y*=-1;y+=590;
            for(int i=x;i<=450;i+=5){
                MoveToEx(hdc,i,y,NULL);
                LineTo(hdc,i,y);
            }
            for(int i=y;i>=590;i-=5){
                MoveToEx(hdc,x,i,NULL);
                LineTo(hdc,x,i);
            }
        }
        if(x>0&&y<0){
            x*=10;y*=10;
            x+=450;y*=-1;y+=590;
            for(int i=x;i>=450;i-=5){
                MoveToEx(hdc,i,y,NULL);
                LineTo(hdc,i,y);
            }
            for(int i=y;i>=590;i-=5){
                MoveToEx(hdc,x,i,NULL);
                LineTo(hdc,x,i);
            }
        }
    DeleteObject(greenPen);
}

void GRAF_PUNTO(HDC hdc,double x,double y){
    GRAF_LINE_PUNTO(hdc,x,y);
    x*=10;y*=10;
    x+=450;y*=-1;y+=590;

    greenPen=CreatePen(PS_SOLID, 5, RGB(0, 255, 0));
    SelectObject(hdc,greenPen);
        MoveToEx(hdc,x,y,NULL);
        LineTo(hdc,x,y);
    DeleteObject(greenPen);
}

void GRAF_LINE_FUNCION(HDC hdc,double x,double y){
    x*=10;y*=10;
    x+=450;y*=-1;y+=590;
    greenPen=CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    SelectObject(hdc,greenPen);
        if(x>10&&x<910&&y>280&&y<920){
            if(P_ANTERx!=9999&&P_ANTERy!=9999){
                MoveToEx(hdc,P_ANTERx,P_ANTERy,NULL);
                LineTo(hdc,x,y);
            }
            P_ANTERx=x;
            P_ANTERy=y;
        }else{
            P_ANTERx=9999;
            P_ANTERy=9999;
        }
    DeleteObject(greenPen);
}

///APARTADO GRAFICO

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    switch(uMsg)
    {
        case WM_PAINT:
        {   HDC hdc;
            PAINTSTRUCT ps;
            hdc=BeginPaint(hwndDlg,&ps);
            greenPen=CreatePen(PS_SOLID, 2, RGB(51, 204, 255));
            SelectObject(hdc,greenPen);
            Rectangle(hdc,5,5,897,275);

            brush=CreateSolidBrush(RGB(0, 0, 0));
            SelectObject(hdc,brush);
            Rectangle(hdc,5,285,897,925);
            DeleteObject(brush);

            GRAF_LINE_COORD(hdc);
            CALCULAR_PUNTOS_FUNCION(hdc);

            for(int i=0;i<PRINTF_NUM_INTERACCIONES+1;i++){
                double YPOINT;
                YPOINT=OPERACIONRESUL(0,INTERACIONES_RES[i]);
                //cout<<INTERACIONES_RES[i]<<" "<<YPOINT<<endl;
                if(YPOINT<100&&YPOINT>-100){
                    GRAF_PUNTO(hdc,INTERACIONES_RES[i],YPOINT);///OJO COORDENADAS FUERA DE RANGO CHASHEA
                }
            }//cout<<endl;

            DeleteObject(greenPen);
            EndPaint(hwndDlg,&ps);
        }
        case WM_INITDIALOG:
        {
        }
        return TRUE;

        case WM_CLOSE:
        {   EndDialog(hwndDlg, 0);
        }
        return TRUE;

        case WM_COMMAND:
        {   switch(LOWORD(wParam))
            {
                case BTN_SIGUIENT:
                    if(Cierre){
                        EndDialog(hwndDlg, 0);
                    }else{
                        for(int i=0;i<=PRINTF_NUM_INTERACCIONES;i++){
                            INTERACIONES[i].Mostrar(hwndDlg,i);
                        }
                        if(PRINTF_NUM_INTERACCIONES<3){
                            PRINTF_NUM_INTERACCIONES++;
                        }else{
                            Cierre++;
                            PRINTF_NUM_INTERACCIONES++;
                            SetDlgItemText(hwndDlg,BTN_SIGUIENT,"SALIR...");
                        }
                    }
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
                case BTN_INSERTAR:
                    if(InserTT){
                    }else{
                        double X_EVALUADO;
                        char CADENA_EDIT1[20],CADENA_EDIT2[20];
                        GetDlgItemText(hwndDlg,EDIT1,CADENA_EDIT1,20);//capturar evento
                        GetDlgItemText(hwndDlg,EDIT2,CADENA_EDIT2,20);//capturar evento
                        X_EVALUADO=atof(CADENA_EDIT2);glFUN=1;
                        if(PREPROCESO_ENTRADA_DATO(hwndDlg,CADENA_EDIT1,X_EVALUADO)){
                            InserTT++;PRINTF_NUM_INTERACCIONES=0;
                        }
                    }
                    InvalidateRect(hwndDlg,NULL,true);
                    return TRUE;
            }
        }return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
