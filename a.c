#include "toolbox.h"
#include <cvirte.h>
#include <math.h>	
#include <userint.h>
#include "a.h"

//##########  Variabile globale  ###################
//handler pentru ferestre;
static int panelHandle;
//Perioada de esantionare;  
double Tep;
// perioada implementata la Timer pentru calcul direct a unei perioade de semnal sinusoidal formata din 20 de puncte
double T_timer;
//momentele de timp de esantionare:
	//>>>>la momentul actual k>>>tk
	//>>>>la momentul anterior(cu un pas de esantionare in urma tk-Te) avem k_1>>>tk_1  
double tpk,tpk_1;
//Momentul de sincronizare
double 	R_t_sincronizare;
//Parametrii semnalului A-amplitudine=220V, T-perioada=0.02sec; 
double A,T;
//selectie alternanta 1-pozitiva ; 0-negativa 
int R_altenanta_p_n;

//Esantioanele semnalelor la momentul tk>>>xk, yk / si la momentul tk_1>>>xk_1, yk_1;   
double R_fazak,R_fazak_1,R_fazak_2,R_yk,R_yk_1;
//unghiul de comanda tiristoare
int K_alpha;
double pi;

//nr.de alternante care se vor afisa
int Nr,cnt;

//Comanda primita de punte redresoare in domeniul [0..10]V
double com_punte;
int Nr_Te_intarziere; 

//Filtru pt. Puntea redresoare monofazata
double K_filtru, T_filtru, C_filtru;
double U_filtratk_1, U_filtratk;

//Includere fisiere din proiect care contin functii de prelucrare
/*OBS: Fisierul trebuie inclus in acest punct deoarece 
functiile folosesc variabile globale declarate anterior*/
#include "Functii_Utilitare.h" 

//###################################################################################
//					Functia main 
//###################################################################################
int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					   LPSTR lpszCmdLine, int nCmdShow)
{
	if (InitCVIRTE (hInstance, 0, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "a.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();  //pornire run-time engine
	DiscardPanel (panelHandle);
	return 0;
}
//###################################################################################
//						Functia Start Aplicatie
//###################################################################################
int CVICALLBACK Start_aplicatie_punte (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Setare perioada de esantionare initiala
				Tep=0.001; // perioada de esantionare este de 100ms
				T_timer=0.02;
			//Setare parametrii semnale ==>>relatii iterative
				tpk_1=0;//moment de esantionare anterior
				tpk=0;
					
			R_fazak_1=-1;//Valoare semnal la momentul de esantionare anterior 
			R_fazak_2=-1;
					R_yk_1=0;////Valoare semnal la momentul de esantionare anterior semnal avansat//intarziat
		
			R_t_sincronizare=0;
			//valoarea intoarsa de functia Pi() o salvam in variabila pi
				pi=Pi();
				T=0.020;//frecventa 50 Hz => T=1/50=0.02 sec
				A=220; 
				K_alpha=20; //Domeniu [0...100] K_alpha=((T/2)/Te)
	//Parametrii Filtrului pentru tensiunea redresata
	K_filtru=1; 
		T_filtru=0.03;//
			C_filtru=Tep/(Tep+T_filtru);
			//nr.de alternante care se vor afisa
				Nr=3;//Nr. alternante pentru afisare
				cnt=1;
				SetAxisRange (panelHandle,PANEL_GRAPH_RSIN, VAL_MANUAL, 0, Nr*20*Tep, VAL_NO_CHANGE,-220, 220);   
				SetAxisRange (panelHandle,PANEL_GRAPH_RREDR, VAL_MANUAL, 0, Nr*20*Tep, VAL_NO_CHANGE,-220, 220);

				
			//Setare interval de timp si pornire Timer
				SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_INTERVAL,T_timer);
					SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_ENABLED,1); 
		break;
	}
	return 0;
}
//###################################################################################
//					Functia ce implementeaza perioada de esantionare 
//###################################################################################
int CVICALLBACK esantionare (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			//citire de pe interfata grafica valoare Amplitudine Semnal
			//   	GetCtrlVal (panelHandle, PANEL_AMPLITUDINE, &A);
			//citire de pe interfata grafica valoare Perioada Semnal
			//   	GetCtrlVal (panelHandle, PANEL_PERIOADA, &T);

//Formatare K_DCG trensfomare de la [0..10]V ->> NR_cuante_intarziere 
			GetCtrlVal (panelHandle, PANEL_COMANDA, &com_punte);//semnalul de comanda a puntii redresoare[0..10]V
				Nr_Te_intarziere=(int)(10-com_punte);
			   		SetCtrlVal (panelHandle, PANEL_ALPHA, Nr_Te_intarziere);
						GetCtrlVal (panelHandle, PANEL_ALPHA, &K_alpha); 
//#####################################**************************************************########################################### 
//#####################################**************************************************########################################### 
for (int i=0; i<20; i++)   //calcul/implementare perioada semnal intr-o singura intrerupere de la Timer
{
//Sistemul monofazat faza R,de la Retea >>>>Generare semnale sinusoidal
					R_fazak=A*sin( ((2*pi)/T)*tpk ); //FAZA R
					
//######### R Transformator de Sincronizare cu trecerile prin "0" #######################################
			if ( (R_fazak>=0)&&(R_fazak_1<0)&&(R_fazak_2<0) )
				{		R_t_sincronizare=tpk; //Memorare >>Momentul trecerii prin "0" al fazei R de jos in sus 
						R_altenanta_p_n=1;//1 pt.altenanta pozitiva
					//Afisare moment trecere prin "0" a fazei
						PlotPoint (panelHandle, PANEL_GRAPH_RSIN, R_t_sincronizare, 0.5*A, VAL_ASTERISK, VAL_BLUE); 
				}
			else
				{
					if( (R_fazak<=0)&&(R_fazak_1>0)&&(R_fazak_2>0) )
					{		R_t_sincronizare=tpk; //Memorare >>Momentul trecerii prin "0" al fazei R de sus in jos  
							R_altenanta_p_n=0;//0 pt.altenanta negativa 
						//Afisare moment trecere prin "0" a fazei 
							PlotPoint (panelHandle, PANEL_GRAPH_RSIN, R_t_sincronizare, -0.5*A, VAL_ASTERISK, VAL_BLUE);
					}
				}


//#########  Redresare tensiune  #######################################
		
		//Faza R >> Redresare tensiune + Implemetarea GTO # comanda stare blocat sau saturat Transzistoare de mare putere
			//Formatare Semnal redresat FAZA R de la Retea 
			R_yk=A*sin( ((2*pi)/T)*tpk );//Generare sinusoida pt. redresare faza R
				if (R_yk<0)
					{   R_yk=-R_yk;  //Semnal Redresat
					}
			//Implementare comanda intarziata cu unghiul ALPHA pe fircare alternanta
				if ( tpk<(R_t_sincronizare+K_alpha*Tep) )
					{   R_yk=0;		 //Semnal Trunchiat cu unghiul K_alpha
					}

//Filtrare pulsuri pentru a obtine o tensiune medie ce se va aplica motorului
				U_filtratk=U_filtratk_1+C_filtru*(R_yk-U_filtratk_1); //teansiunea efectiva furnizata de puntea redresoare
		
//#########//plotare Faza R semnal sinusoidal #######################################  
		//R>>>Afisare Intarziere pulsuri cu valoarea K_alpha	
			if ( (tpk>=(R_t_sincronizare+K_alpha*Tep))&&(tpk<(R_t_sincronizare+K_alpha*Tep+Tep)) )
				{
					if ( R_altenanta_p_n==1 )
						{   PlotLine (panelHandle, PANEL_GRAPH_RSIN, tpk, 0, tpk, 0.5*A, VAL_BLUE);
						}
					else
						{   PlotLine (panelHandle, PANEL_GRAPH_RSIN, tpk, 0, tpk, -0.5*A, VAL_BLUE);
						}
					if(R_yk_1==0)
						{PlotLine (panelHandle, PANEL_GRAPH_RREDR, tpk_1, R_yk_1, tpk, R_yk_1, VAL_BLUE);//Faza R semnal Redresat
						}
					else
						{PlotLine (panelHandle, PANEL_GRAPH_RREDR, tpk_1, R_yk_1, tpk, R_yk, VAL_BLUE);//Faza R semnal Redresat 
						}
					PlotLine (panelHandle, PANEL_GRAPH_RREDR, tpk, 0, tpk, R_yk, VAL_BLUE);//Faza R semnal Redresat   
				}
			else
			{
				PlotLine (panelHandle, PANEL_GRAPH_RREDR, tpk_1, R_yk_1, tpk, R_yk, VAL_BLUE);//Faza R semnal Redresat 
			}
			PlotLine (panelHandle, PANEL_GRAPH_RSIN, tpk_1, R_fazak_1, tpk, R_fazak, VAL_RED);
			PlotLine (panelHandle, PANEL_GRAPH_RREDR, tpk_1, U_filtratk_1, tpk, U_filtratk, VAL_RED);//Faza R semnal Redresat si Filtrat  
		//reactualizare timp
					tpk_1=tpk; 
						tpk=tpk+Tep;
		//reactualizare esantioane==>>cele actuale devin vechi pt o noua intrerupere de la TIMER  
			R_fazak_2=R_fazak_1;		
				R_fazak_1=R_fazak;
					R_yk_1=R_yk;
			//reactualizare Filtru
			U_filtratk_1=U_filtratk;
} //END FOR
//#####################################**************************************************###########################################
//#####################################**************************************************########################################### 

//Refresh automat pentru a afisa "Oarecum" in timp real forma de unda a semnalului Redresat
						if(tpk>(Nr*20*Tep))
						{   
							DeleteGraphPlot (panelHandle, PANEL_GRAPH_RSIN, -1, VAL_IMMEDIATE_DRAW );
								DeleteGraphPlot (panelHandle, PANEL_GRAPH_RREDR, -1, VAL_IMMEDIATE_DRAW ); 
							tpk_1=0;
								tpk=Tep;
								R_t_sincronizare=0;
						}
		
		
		
		break;
	}
	return 0;
}

//###################################################################################
//@@@@@@@@@@@@@@@@@@@@@@@Functii Utilitare - Oprire aplicatie==Oprire Run-Time Engine
//###################################################################################
int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);  //oprire run-time engine  
			break;
	}
	return 0;
}


