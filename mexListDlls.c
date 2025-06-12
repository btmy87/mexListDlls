#include <stdio.h>
#include <string.h>
#include "mex.h"
#include "windows.h"
#include "psapi.h"

#define NMODULE 4096

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[])
{
  if (nrhs != 0)
  {
    mexErrMsgIdAndTxt("mexListDlls:nrhs", "Zero inputs required.");
  }
  if (nlhs != 0)
  {
    mexErrMsgIdAndTxt("mexListDlls:nlhs", "Zero outputs required.");
  }

  HANDLE hProcess = GetCurrentProcess();
  HMODULE *lphModule;
  lphModule = calloc(NMODULE, sizeof(HMODULE));
  DWORD lpcbNeeded;
  DWORD cb = NMODULE * sizeof(HMODULE);
  BOOL status = EnumProcessModules(hProcess, lphModule,
                                   cb, &lpcbNeeded);
  if (status == 0)
  {
    mexErrMsgIdAndTxt("mexListDlls:EnumProcessModules", "EnumProcessModules failed");
  }

  DWORD countModules = lpcbNeeded / sizeof(HMODULE);
  char *pFilename;
  pFilename = calloc(MAX_PATH + 1, sizeof(char));

  for (int i = 0; i < countModules; i++)
  {
    memset(pFilename, 0, MAX_PATH + 1);
    GetModuleFileNameA(lphModule[i], pFilename, MAX_PATH + 1);
    mexPrintf(pFilename);
    mexPrintf("\n");
  }
}