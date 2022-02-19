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

int DIMENC_LIST=0,CONT_NIVEL_DIVIC=0;
double P_ANTERx=9999,P_ANTERy=9999;
bool glFUN=0,Cierre=0,BUSQ_DATo=1;
double PRIMERAS_RESP[30],XComB[30];

void MULTIPLICA_BASE(double [2],char [2],int [2],int);
void MULTIPLIC_VAR_IGUALES(double [2],char [2],int [2],int);
void MULTIPLICACION(int,bool,double,char,int,double,char,int);
void SuMA();
void CONCATENAR_RESUL();
void MOSTRAR_RESUL_FINAL();
double OPERACIONRESUL(double);
void OPER_DIVICION(int);
void ALMACENA_RESUL(int);
void INSERT_NEW_DATO(double,double,double);
void OPERAR_DIF_DIVIDID(HWND);
void PREPROCESO_ENTRADA_DATO(HWND,char *);
void CALCULAR_PUNTOS_FUNCION(HDC);
void GRAF_LINE_COORD(HDC);
void GRAF_LINE_PUNTO(HDC,double,double);
void GRAF_PUNTO(HDC,double,double);
void GRAF_LINE_FUNCION(HDC,double,double);
void InsertDATO();

///CONCATENAR
void InConcatINT(int,int);
void InConcatDOUBLE(double,int);
void InConcatCHAR(char,int);
void InConcatSTRING(string,int);

class Concat{
    private:
        char *A;
        char *C;
    public:
    Concat(){
        A=new char[3];
        A[0]='\0';
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
        if(Posis==1){
            SetDlgItemText(hwndDlg,EDIT11,C);
        }
        if(Posis==2){
            SetDlgItemText(hwndDlg,EDIT12,C);
        }
        if(Posis==3){
            SetDlgItemText(hwndDlg,EDIT13,C);
        }cout<<C;
    }
}Res[3];

void InConcatINT(int X,int Pos){
    char cad[10];
    itoa(X,cad,10);
    Res[Pos].CONCATENAR(cad);
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

        Res[Pos].CONCATENAR(cad1);
        InConcatCHAR('.',Pos);
        Res[Pos].CONCATENAR(cad2);
        //SetDlgItemText(hwndDlg,EDIT11,CONCATENAR(CONCATENAR(cad1,cadP),cad2));
    }else{
        char cad1[10];
        itoa(NUM_A,cad1,10);
        InConcatINT(NUM_A,Pos);
        //SetDlgItemText(hwndDlg,EDIT11,cad1);

    }
}
/*
void InConcatDOUBLE(double X,int Pos){
    int Xint=X;
    char cad[10];
    char cadDec[10];
    itoa(Xint,cad,10);
    Xint*=10;
    X*=10;
    if(Xint==0&&X<0){
        InConcatCHAR('-',Pos);
    }
    if(X<0){
        Xint=Xint-X;
    }else{
        Xint=X-Xint;
    }itoa(Xint,cadDec,10);

    Res[Pos].CONCATENAR(cad);
    if(Xint!=0){
        InConcatCHAR('.',Pos);
        Res[Pos].CONCATENAR(cadDec);
    }

}
*/
void InConcatCHAR(char X,int Pos){
    char Var[2];
    Var[0]=X;
    Var[1]='\0';
    Res[Pos].CONCATENAR(Var);
}

void InConcatSTRING(string X,int Pos){
    char *Var;int i=0;
    for(;X[i];i++);
    Var=new char[i+1];
    for(i=0;X[i];i++){
        Var[i]=X[i];
    }Var[i]='\0';
    Res[Pos].CONCATENAR(Var);
}

///CONCATENAR

///OPERA ALGE
struct DATOS_ALGEBRAICOS{
    double  NumARR[30];
    char    VarARR[30];
    int     PotARR[30];
    int     CANT_COMBIN;
}D[3];

void MULTIPLICA_BASE(double Num[2],char VaR[2],int PoT[2],int Cantidad){
    for(int i=0;i<Cantidad;i++){
        D[0].NumARR[i]=*(Num+i);
        D[0].VarARR[i]=*(VaR+i);
        D[0].PotARR[i]=*(PoT+i);
        D[0].CANT_COMBIN=Cantidad;
    }
}

void MULTIPLIC_VAR_IGUALES(double Num[2],char VaR[2],int PoT[2],int Cantidad){
    for(int i=0,P=0;i<D[0].CANT_COMBIN;i++){
        for(int j=0;j<Cantidad;j++){
            D[1].NumARR[P]=D[0].NumARR[i]*Num[j];
            if(D[0].VarARR[i]=='x'&&VaR[j]=='x'){
                D[1].VarARR[P]=D[0].VarARR[i];
                D[1].PotARR[P]=D[0].PotARR[i]+PoT[j];
            }else if(D[0].VarARR[i]=='1'&&VaR[j]=='1'){
                D[1].VarARR[P]=D[0].VarARR[i];
                D[1].PotARR[P]=D[0].PotARR[i];
            }
            else{
                if(D[0].VarARR[i]!='1'){
                    D[1].VarARR[P]=D[0].VarARR[i];
                    D[1].PotARR[P]=D[0].PotARR[i];
                }
                if(VaR[j]!='1'){
                    D[1].VarARR[P]=VaR[j];
                    D[1].PotARR[P]=PoT[j];
                }
            }
            P++;
        }
    }D[0].CANT_COMBIN*=Cantidad;
}
//MULTIPLICA TERMINOS ALGEBRAICOS
void MULTIPLICACION(int Cantidad=1,bool Entrada=1,double Num1=1,char Var1='1',int PT1=1,double Num2=1,char Var2='1',int PT2=1){
    double NUmeric[2];
    char   Variable[2];
    int    Pot[2];

    if(Entrada==1){
        if(Cantidad==1){
            NUmeric[0]=Num1;Variable[0]=Var1;Pot[0]=PT1;
        }
        if(Cantidad==2){
            NUmeric[0]=Num1;Variable[0]=Var1;Pot[0]=PT1;
            NUmeric[1]=Num2;Variable[1]=Var2;Pot[1]=PT2;
        }
        MULTIPLICA_BASE(NUmeric,Variable,Pot,Cantidad);
    }else{
        if(Cantidad==1){
            NUmeric[0]=Num1;Variable[0]=Var1;Pot[0]=PT1;
        }
        if(Cantidad==2){
            NUmeric[0]=Num1;Variable[0]=Var1;Pot[0]=PT1;
            NUmeric[1]=Num2;Variable[1]=Var2;Pot[1]=PT2;
        }

        MULTIPLIC_VAR_IGUALES(NUmeric,Variable,Pot,Cantidad);
        for(int i=0;i<D[0].CANT_COMBIN;i++){
            D[0].NumARR[i]=D[1].NumARR[i];
            D[0].VarARR[i]=D[1].VarARR[i];
            D[0].PotARR[i]=D[1].PotARR[i];
        }
        for(int i=0;i<D[0].CANT_COMBIN;i++){
            for(int j=1;j<D[0].CANT_COMBIN;j++){
                if(i!=j){
                    if(D[0].VarARR[i]==D[0].VarARR[j]&&D[0].PotARR[i]==D[0].PotARR[j]){
                        D[0].NumARR[i]+=D[0].NumARR[j];
                        for(int jk=j+1;jk<D[0].CANT_COMBIN;jk++){
                            D[0].NumARR[jk-1]=D[0].NumARR[jk];
                            D[0].VarARR[jk-1]=D[0].VarARR[jk];
                            D[0].PotARR[jk-1]=D[0].PotARR[jk];
                            D[1].NumARR[jk-1]=D[1].NumARR[jk];
                            D[1].VarARR[jk-1]=D[1].VarARR[jk];
                            D[1].PotARR[jk-1]=D[1].PotARR[jk];
                        }
                        D[0].CANT_COMBIN--;
                    }
                }
            }
        }

    }
}
//SUMA TERMINOS ALGEBRAICOS
void SuMA(){
    for(int i=0,j=D[2].CANT_COMBIN;i<D[0].CANT_COMBIN;i++,j++){
        D[2].NumARR[j]=D[0].NumARR[i];
        D[2].VarARR[j]=D[0].VarARR[i];
        D[2].PotARR[j]=D[0].PotARR[i];
    }D[2].CANT_COMBIN+=D[0].CANT_COMBIN;

    for(int i=0;i<D[2].CANT_COMBIN;i++){
            for(int j=1;j<D[2].CANT_COMBIN;j++){
                if(i!=j){
                    if(D[2].VarARR[i]==D[2].VarARR[j]&&D[2].PotARR[i]==D[2].PotARR[j]){
                        D[2].NumARR[i]+=D[2].NumARR[j];
                        for(int jk=j+1;jk<D[2].CANT_COMBIN;jk++){
                            D[2].NumARR[jk-1]=D[2].NumARR[jk];
                            D[2].VarARR[jk-1]=D[2].VarARR[jk];
                            D[2].PotARR[jk-1]=D[2].PotARR[jk];
                        }
                        D[2].CANT_COMBIN--;
                    }
                }
            }
    }
}

void CONCATENAR_RESUL(){
    for(int i=0;i<D[0].CANT_COMBIN;i++){
        if(D[0].NumARR[i]!=0){
            if(D[0].NumARR[i]>0){
                InConcatCHAR('+',1);cout<<"+";
                if(D[0].VarARR[i]=='x'){
                    if(D[0].NumARR[i]!=1){
                        InConcatDOUBLE(D[0].NumARR[i],1);cout<<D[0].NumARR[i];
                    }
                }else{
                    InConcatDOUBLE(D[0].NumARR[i],1);cout<<D[0].NumARR[i];
                }
            }else{
                if(D[0].VarARR[i]=='x'){
                    if(D[0].NumARR[i]!=1){
                        InConcatDOUBLE(D[0].NumARR[i],1);cout<<D[0].NumARR[i];
                    }
                }else{
                    InConcatDOUBLE(D[0].NumARR[i],1);cout<<D[0].NumARR[i];
                }
            }
            if(D[0].VarARR[i]!='1'){
                InConcatCHAR(D[0].VarARR[i],1);cout<<D[0].VarARR[i];
            }
            if(D[0].PotARR[i]!=1){
                InConcatCHAR('^',1);
                InConcatINT(D[0].PotARR[i],1);cout<<"^"<<D[0].PotARR[i];
            }
        }
    }
}

void MOSTRAR_RESUL_FINAL(){///resultado final
    for(int i=0;i<D[2].CANT_COMBIN;i++){
        if(D[2].NumARR[i]!=0){
            if(D[2].NumARR[i]>0&&i!=0){
                InConcatCHAR('+',2);cout<<"+";
                if(D[2].VarARR[i]=='x'){
                    if(D[2].NumARR[i]!=1){
                        InConcatDOUBLE(D[2].NumARR[i],2);cout<<D[2].NumARR[i];
                    }
                }else{
                    InConcatDOUBLE(D[2].NumARR[i],2);cout<<D[2].NumARR[i];
                }
            }else{
                if(D[2].VarARR[i]=='x'){
                    if(D[2].NumARR[i]!=1){
                        InConcatDOUBLE(D[2].NumARR[i],2);cout<<D[2].NumARR[i];
                    }
                }else{
                    InConcatDOUBLE(D[2].NumARR[i],2);cout<<D[2].NumARR[i];
                }
            }
            if(D[2].VarARR[i]!='1'){
                InConcatCHAR(D[2].VarARR[i],2);cout<<D[2].VarARR[i];
            }
            if(D[2].PotARR[i]!=1){
                InConcatCHAR('^',2);
                InConcatINT(D[2].PotARR[i],2);cout<<"^"<<D[2].PotARR[i];
            }
        }
    }cout<<"\n\n\n";
}

double OPERACIONRESUL(double ValDATO){
    double RESULTADO_Y=0;
    for(int i=0;i<D[2].CANT_COMBIN;i++){
        if(D[2].NumARR[i]!=0){
            double RPOT=ValDATO;
            if(D[2].VarARR[i]=='x'){
                for(int S=1;S<D[2].PotARR[i];S++){
                    RPOT*=ValDATO;
                }
                RESULTADO_Y+=RPOT*D[2].NumARR[i];
            }else{
                RESULTADO_Y+=D[2].NumARR[i];
            }
        }
    }
    return RESULTADO_Y;
}

///OPERA ALGE

///APARTADO OPERACIONAL DIV

class NODO{
    public:
    double Res;
    double x;
    double y;
    NODO *izq,*PaDr;
    NODO(double RES=0,double X=0,double Y=0,NODO *Padr=NULL,NODO *iz=NULL){
         Res=RES;
         x=X;
         y=Y;
         izq=iz;
         PaDr=Padr;
    }
};

class LISTA{
    public:
    NODO *raiz,*padr;
    LISTA(){
        raiz=NULL;
        padr=NULL;
    }
    void INSERTAR_COORD(double Res,double x,double y){
         INS_COORD(Res,x,y,raiz,padr);
    }
    void INS_COORD(double Res,double x,double y,NODO *&R,NODO *padr){
         if(R==NULL)
            R=new NODO(Res,x,y,padr);
         else
            INS_COORD(Res,x,y,R->izq,R);
    }
    void BUSQUEDA_DATO(double X_BUSQ,double Y_BUSQ){BUSQ_DATo=1;
         BUSQ_D(raiz,X_BUSQ,Y_BUSQ);
    }
    void BUSQ_D(NODO *R,double X_BUSQ,double Y_BUSQ){
         if(R!=NULL){
            if(R->x==X_BUSQ&&R->y==Y_BUSQ){
                BUSQ_DATo=0;
            }else{
                BUSQ_D(R->izq,X_BUSQ,Y_BUSQ);
            }
         }
    }
    void OPER_NR_1(){
         OP_1(raiz->izq);
    }
    void OP_1(NODO *R){
         if(R!=NULL){
            INSERT_NEW_DATO((R->y-R->PaDr->y)/(R->x-R->PaDr->x),R->x,R->PaDr->x);
            OP_1(R->izq);
         }
    }
    void OPER_NR_2(){
         OP_2(raiz->izq);
    }
    void OP_2(NODO *R){
         if(R!=NULL){
            INSERT_NEW_DATO((R->Res-R->PaDr->Res)/(R->x-R->PaDr->y),R->x,R->PaDr->y);
            OP_2(R->izq);
         }
    }
    void PRINTF_PUNTOS(HDC hdc){
         PFPTS(hdc,raiz);
    }
    void PFPTS(HDC hdc,NODO *R){
         if(R!=NULL){
            GRAF_PUNTO(hdc,R->x,R->y);
            PFPTS(hdc,R->izq);
         }
    }
    void MOSTRAR_NR1(){
         MOS1(raiz->izq);
         DIMENC_LIST++;
         cout<<"\n\n\n===================================\n\n\n\n";
    }
    void MOS1(NODO *R){
         if(R!=NULL){
            cout<<R->y<<" - "<<R->PaDr->y<<"\n--------\n";
            cout<<R->x<<" - "<<R->PaDr->x<<"\n";
            cout<<"=============== "<<(R->y-R->PaDr->y)/(R->x-R->PaDr->x)<<endl<<endl;
            MOS1(R->izq);CONT_NIVEL_DIVIC++;
         }
    }
    void MOSTRAR_NR2(){
         MOS2(raiz->izq,0);
         cout<<"\n\n\n===================================\n\n\n\n";
    }
    void MOS2(NODO *R,int C){
         if(R!=NULL){
            cout<<R->Res<<" - "<<R->PaDr->Res<<"\n--------\n";
            cout<<R->x<<" - "<<R->PaDr->y<<"\n";
            cout<<"=============== "<<((R->Res)-(R->PaDr->Res))/((R->x)-(R->PaDr->y))<<endl<<endl;
            MOS2(R->izq,C+1);
         }
    }
    void ALMACENAR_COORD_XS_INI(){
         DRIVE_X(raiz,0);
    }
    void DRIVE_X(NODO *R,int C){
         if(R->izq!=NULL){
            XComB[C]=R->x;
            DRIVE_X(R->izq,C+1);
         }
    }
    void ALMACENAR_RESP_DIVID(int C){
        PRIMERAS_RESP[C]=raiz->Res;
    }
    void ALMACENAR_COORD_Y_INI(){
        PRIMERAS_RESP[0]=raiz->y;
    }
}A[20];

void OPER_DIVICION(int LIMIT_DIV){
    for(int i=1;i<LIMIT_DIV;i++){
        A[DIMENC_LIST].OPER_NR_2();
        A[DIMENC_LIST].MOSTRAR_NR2();
        DIMENC_LIST++;
    }
}

void ALMACENA_RESUL(int LIMIT_DIV){
    for(int i=0;i<=LIMIT_DIV;i++){
        if(i!=0){
            A[i].ALMACENAR_RESP_DIVID(i);
        }else{
            A[i].ALMACENAR_COORD_Y_INI();
        }
    }
}

void INSERT_NEW_DATO(double Res,double x,double y){
    A[DIMENC_LIST+1].INSERTAR_COORD(Res,x,y);
}

void OPERAR_DIF_DIVIDID(HWND hwndDlg){
    A[DIMENC_LIST].OPER_NR_1();
    A[DIMENC_LIST].MOSTRAR_NR1();
    OPER_DIVICION(CONT_NIVEL_DIVIC);
    cout<<"Cantida de secuencias: "<<CONT_NIVEL_DIVIC<<"\n\n";
    cout<<"P"<<CONT_NIVEL_DIVIC<<"(X) = ";

    ALMACENA_RESUL(CONT_NIVEL_DIVIC);
    A[0].ALMACENAR_COORD_XS_INI();

    cout<<PRIMERAS_RESP[0];
    InConcatDOUBLE(PRIMERAS_RESP[0],0);
    for(int I=1;I<=CONT_NIVEL_DIVIC;I++){
        if(PRIMERAS_RESP[I]!=0){
            if(PRIMERAS_RESP[I]>0){
                InConcatCHAR('+',0);cout<<"+";
            }
            InConcatDOUBLE(PRIMERAS_RESP[I],0);cout<<PRIMERAS_RESP[I]<<"";
            for(int K=0;K<I;K++){
                InConcatSTRING("(X",0);
                InConcatDOUBLE(XComB[K]*-1,0);
                InConcatSTRING(")",0);cout<<"(X"<<XComB[K]*-1<<")";
            }
        }
    }

    cout<<"\n\n";
    D[2].CANT_COMBIN=0;
    cout<<"P"<<CONT_NIVEL_DIVIC<<"(X) = ";
    cout<<PRIMERAS_RESP[0];
    MULTIPLICACION(1,1  ,PRIMERAS_RESP[0],'1',1);

    InConcatDOUBLE(PRIMERAS_RESP[0],1);
    SuMA();

    for(int I=1;I<=CONT_NIVEL_DIVIC;I++){
        if(PRIMERAS_RESP[I]!=0){
           MULTIPLICACION(1,1  ,PRIMERAS_RESP[I],'1',1);
            for(int K=0;K<I;K++){
                MULTIPLICACION(2,0  ,1,'x',1, XComB[K]*-1,'1',1);
            }
            CONCATENAR_RESUL();
            SuMA();
        }
    }

    cout<<"\n\nRESULTADO\n\n";
    cout<<"P"<<CONT_NIVEL_DIVIC<<"(X) = ";
    MOSTRAR_RESUL_FINAL();

    Res[0].Mostrar(hwndDlg,1);
    cout<<endl;
    Res[1].Mostrar(hwndDlg,2);
    cout<<endl;
    Res[2].Mostrar(hwndDlg,3);
}

///APARTADO OPERACIONAL DIV

void PREPROCESO_ENTRADA_DATO(HWND hwndDlg,char *Num){
    char *NewCHAR1,*NewCHAR2;
    double FX_IN=0,FY_IN=0;
    int i=0,j=0;for(;*(Num+i);i++){
        if(*(Num+i)==','){j=i;}
    }if(j){
        NewCHAR1=new char[j+1];
        NewCHAR2=new char[i-j];

        int I=0;for(;*(Num+I)!=',';I++){
            *(NewCHAR1+I)=*(Num+I);
        }*(NewCHAR1+I)='\0';I++;

        int K=0;for(;*(Num+I);I++,K++){
            *(NewCHAR2+K)=*(Num+I);
        }*(NewCHAR2+K)='\0';

        FX_IN=atof(NewCHAR1);
        FY_IN=atof(NewCHAR2);

        A[DIMENC_LIST].BUSQUEDA_DATO(FX_IN,FY_IN);
        if(BUSQ_DATo){
            cout<<"("<<FX_IN<<","<<FY_IN<<")\n";
            SetDlgItemText(hwndDlg,EDIT8,"INSERTADO...");
            A[DIMENC_LIST].INSERTAR_COORD(0,FX_IN,FY_IN);
        }else{
            SetDlgItemText(hwndDlg,EDIT8,"COORDENADA EXISTENTE");
        }
    }else{
        SetDlgItemText(hwndDlg,EDIT8,"ERROR, NO EXISTE DOS COORDENADAS");
        cout<<"ERROR, NO EXISTE DOS COORDENADAS\n";
    }
}

///APARTADO GRAFICO

void CALCULAR_PUNTOS_FUNCION(HDC hdc){
    if(glFUN){
        for(double DTx=-45.0,DTy;DTx<44;DTx+=0.1){
            DTy=OPERACIONRESUL(DTx);cout<<"";
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

    greenPen=CreatePen(PS_SOLID, 7, RGB(0, 255, 0));
    SelectObject(hdc,greenPen);
        MoveToEx(hdc,x,y,NULL);
        LineTo(hdc,x,y);
    DeleteObject(greenPen);
}

void GRAF_LINE_FUNCION(HDC hdc,double x,double y){
    x*=10;y*=10;
    x+=450;y*=-1;y+=590;
    greenPen=CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
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

void InsertDATO(){
/*
    A[DIMENC_LIST].INSERTAR_COORD(0,1,4);
    A[DIMENC_LIST].INSERTAR_COORD(0,2,3.5);
    A[DIMENC_LIST].INSERTAR_COORD(0,3,4);
    A[DIMENC_LIST].INSERTAR_COORD(0,5,5.6);
*/
}

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

            InsertDATO();

            GRAF_LINE_COORD(hdc);
            A[0].PRINTF_PUNTOS(hdc);
            CALCULAR_PUNTOS_FUNCION(hdc);

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
                case BTN_CALCULAR:
                    if(Cierre){
                        EndDialog(hwndDlg, 0);
                    }else{
                        OPERAR_DIF_DIVIDID(hwndDlg);glFUN=1;Cierre++;
                        SetDlgItemText(hwndDlg,BTN_CALCULAR,"SALIR...");
                        InvalidateRect(hwndDlg,NULL,true);
                    }
                    return TRUE;
                case BTN_INSERTAR:
                    char Cad[20];
                    SetDlgItemText(hwndDlg,EDIT8,"...");
                    GetDlgItemText(hwndDlg,EDIT1,Cad,20);//capturar evento

                    PREPROCESO_ENTRADA_DATO(hwndDlg,Cad);

                    SetDlgItemText(hwndDlg,EDIT1,"");
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
