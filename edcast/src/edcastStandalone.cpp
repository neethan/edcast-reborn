// EdcastStandalone.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "edcastStandalone.h"
#include "BassWindow.h"
#include "saneEdcastConfig.h"
//#include "Dummy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern CBassWindow *mainWindow;
CBassWindow *mainWindow;
CWnd *myWin;

char    logPrefix[255] = "edcaststandalone";

/////////////////////////////////////////////////////////////////////////////
// CedcastStandalone

BEGIN_MESSAGE_MAP(CedcastStandaloneApp, CWinApp)
	//{{AFX_MSG_MAP(CedcastStandaloneApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CedcastStandaloneApp construction

void inputMetadataCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->inputMetadataCallback(g->encoderNumber, pValue, FILE_LINE);
}

void outputStatusCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->outputStatusCallback(g->encoderNumber, pValue, FILE_LINE);
    mainWindow->outputMountCallback(g->encoderNumber,g->gMountpoint);

}

void writeBytesCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->writeBytesCallback(g->encoderNumber, pValue);
}

void outputServerNameCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->outputServerNameCallback(g->encoderNumber, pValue);
}

void outputBitrateCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->outputBitrateCallback(g->encoderNumber, pValue);
}

void outputStreamURLCallback(void *gbl, void *pValue) {
    edcastGlobals *g = (edcastGlobals *)gbl;
    mainWindow->outputStreamURLCallback(g->encoderNumber, pValue);
}

int edcast_init(edcastGlobals *g)
{
	int	printConfig = 0;
	


	setServerStatusCallback(g, outputStatusCallback);
	setGeneralStatusCallback(g, NULL);
	setWriteBytesCallback(g, writeBytesCallback);
	setBitrateCallback(g, outputBitrateCallback);
	setServerNameCallback(g, outputServerNameCallback);
	setDestURLCallback(g, outputStreamURLCallback);
    //strcpy(g->gConfigFileName, ".\\edcast_standalone");

	readConfigFile(g);
	
	setFrontEndType(g, FRONT_END_EDCAST_PLUGIN);
	
	return 1;
}


CedcastStandaloneApp::CedcastStandaloneApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CedcastStandaloneApp object

CedcastStandaloneApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CedcastStandaloneApp initialization
/*
bool done;
INT  nResult;

int RunModalWindow(HWND hwndDialog,HWND hwndParent)
{
  if(hwndParent != NULL)
    EnableWindow(hwndParent,FALSE);

  MSG msg;
  
  for(done=false;done==false;WaitMessage())
  {
    while(PeekMessage(&msg,0,0,0,PM_REMOVE))
    {
      if(msg.message == WM_QUIT)
      {
        done = true;
        PostMessage(NULL,WM_QUIT,0,0);
        break;
      }

      if(!IsDialogMessage(hwndDialog,&msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  if(hwndParent != NULL)
    EnableWindow(hwndParent,TRUE);

  DestroyWindow(hwndDialog);

  return nResult;
}
*/

void CedcastStandaloneApp::SetMainAfxWin(CWnd *pwnd) {
    m_pMainWnd = pwnd;
}

BOOL CedcastStandaloneApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#if _OLD_DIR_STUFF
	char currentDir[MAX_PATH] = ".";
	char tmpfile[MAX_PATH] = "";
	wsprintf(tmpfile, "%s\\.tmp", currentDir);

	FILE *filep = fopen(tmpfile, "w");
	if (filep == 0) {
		char path[MAX_PATH] = "";

		SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
		strcpy(currentDir, path);
	}
	else {
		fclose(filep);
		_unlink(tmpfile);
	}

    LoadConfigs(currentDir, "edcaststandalone");
#else
    LoadConfigs(".", "EdcastStandalone");
	const saneConfig * sc =	saneLoadConfigs(_T("edcaststandalone_0.cfg"));
	/*
	OutputDebugString("AppName: ");
	OutputDebugString(sc->appName);
	OutputDebugString("\nCurrentDir: ");
	OutputDebugString(sc->currentDir);
	OutputDebugString("\nExeName: ");
	OutputDebugString(sc->exename);
	OutputDebugString("\nFirstConfig: ");
	OutputDebugString(sc->firstConfig);
	OutputDebugString("\nFirstWritableFolder: ");
	OutputDebugString(sc->firstWritableFolder);
	OutputDebugString("\n");
	*/
#endif
    mainWindow = new CBassWindow(m_pMainWnd);

    theApp.SetMainAfxWin(mainWindow);

    mainWindow->InitializeWindow();

	mainWindow->DoModal();

	return FALSE;
}
