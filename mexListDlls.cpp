#include "mex.hpp"

#include "windows.h"
#include "psapi.h"
#include <string>

#include "mexAdapter.hpp"
// #include "String.hpp"

#define NMODULE 4096

class MexFunction : public matlab::mex::Function
{
  matlab::data::ArrayFactory factory;
  const std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();

public:
  void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
  {
    checkArguments(outputs, inputs);
    HANDLE hProcess = GetCurrentProcess();
    HMODULE *lphModule = (HMODULE *)calloc(NMODULE, sizeof(HMODULE));
    DWORD lpcbNeeded;
    DWORD cb = NMODULE * sizeof(HMODULE);
    BOOL status = EnumProcessModules(hProcess, lphModule,
                                     cb, &lpcbNeeded);
    if (status == 0)
      mexPrintError("EnumProcessModules failed");

    DWORD countModules = lpcbNeeded / sizeof(HMODULE);
    WCHAR *pFilename;
    DWORD nbytesFilename = (MAX_PATH + 1) * sizeof(WCHAR);
    pFilename = (WCHAR *)calloc(MAX_PATH + 1, sizeof(WCHAR));

    matlab::data::TypedArray<matlab::data::MATLABString> out =
        factory.createArray<matlab::data::MATLABString>({countModules, 1});
    for (DWORD i = 0; i < countModules; i++)
    {
      memset(pFilename, 0, nbytesFilename);
      GetModuleFileNameW(lphModule[i], pFilename, nbytesFilename);
      std::u16string sFilename16((char16_t *)pFilename);
      out[i] = sFilename16;
    }
    free(lphModule);
    free(pFilename);

    outputs[0] = std::move(out);
  }

  void checkArguments(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs)
  {
    if (inputs.size() != 0)
      mexPrintError("Expected 0 inputs");
    if (outputs.size() > 1)
      mexPrintError("Expected at most 1 output");
  };

  void mexPrintError(std::string errMsg)
  {
    std::string errHeader("Error using mexListDlls:\n");
    std::vector<matlab::data::Array> msgArray = {factory.createScalar(errHeader + errMsg)};
    matlabPtr->feval(u"error", 0, msgArray);
  };
};