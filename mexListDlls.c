#include <stdio.h>
#include <string.h>
#include "mex.h"

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
}