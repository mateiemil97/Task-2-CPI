/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_COMANDA                    2       /* control type: scale, callback function: (none) */
#define  PANEL_PERIOADA                   3       /* control type: scale, callback function: (none) */
#define  PANEL_AMPLITUDINE                4       /* control type: scale, callback function: (none) */
#define  PANEL_TIMER                      5       /* control type: timer, callback function: esantionare */
#define  PANEL_GRAPH_RREDR                6       /* control type: graph, callback function: (none) */
#define  PANEL_GRAPH_RSIN                 7       /* control type: graph, callback function: (none) */
#define  PANEL_PERESANT                   8       /* control type: scale, callback function: Setare_Perioada_Esantionare */
#define  PANEL_DECORATION                 9       /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               10      /* control type: deco, callback function: (none) */
#define  PANEL_PICTUREBUTTON_3            11      /* control type: pictButton, callback function: QuitCallback */
#define  PANEL_PICTUREBUTTON              12      /* control type: pictButton, callback function: Start_aplicatie_punte */
#define  PANEL_TEXTMSG                    13      /* control type: textMsg, callback function: (none) */
#define  PANEL_ALPHA                      14      /* control type: scale, callback function: (none) */
#define  PANEL_PICTUREBUTTON_2            15      /* control type: pictButton, callback function: refresh_grafice */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK esantionare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK refresh_grafice(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Setare_Perioada_Esantionare(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Start_aplicatie_punte(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
