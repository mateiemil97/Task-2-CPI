//###################################################################################
//@@@@@@@@@@@@@@@@@Functii Utilitare - setare o noua perioada de esantionare la TIMER
//###################################################################################
int CVICALLBACK Setare_Perioada_Esantionare (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//Setare o noua perioada de esantionare
				GetCtrlVal (panelHandle, PANEL_PERESANT, &Tep);  
			//Setare a noului interval de timp la Timer
				SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_INTERVAL,Tep);
			//Pornire Timer cu noua perioada de timp 
				SetCtrlAttribute (panelHandle,PANEL_TIMER, ATTR_ENABLED,1);  
			break;
	}
	return 0;
}
//###################################################################################
//@@@@@@@@@@@@Functii Utilitare - Stergere continut grafice si pornire o noua afisare
//###################################################################################
int CVICALLBACK refresh_grafice (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//refreshul=1;
			//stergere grafice desenate pe obiectele GRAPH
			//	DeleteGraphPlot (panelHandle, PANEL_GRAPH1, -1, VAL_IMMEDIATE_DRAW );
			//	DeleteGraphPlot (panelHandle, PANEL_GRAPH2, -1, VAL_IMMEDIATE_DRAW ); 
			//Reinitializare timp de afisare
			//	tk_1=0;
			//		tk=Te;
			break;
	}
	return 0;
}
