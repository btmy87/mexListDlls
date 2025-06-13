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
    const std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    if (status == 0)
    {
      matlabPtr->feval(u"error", 0,
                       std::vector<matlab::data::Array>({factory.createScalar("EnumProcessModules failed")}));
    }

    DWORD countModules = lpcbNeeded / sizeof(HMODULE);
    char *pFilename;
    pFilename = (char *)calloc(MAX_PATH + 1, sizeof(char));

    matlab::data::TypedArray<matlab::data::MATLABString> out =
        factory.createArray<matlab::data::MATLABString>({countModules, 1});
    for (int i = 0; i < countModules; i++)
    {
      memset(pFilename, 0, MAX_PATH + 1);
      GetModuleFileNameA(lphModule[i], pFilename, MAX_PATH + 1);
      std::string sFilename8 = std::string(pFilename);
      std::u16string sFilename16(sFilename8.begin(), sFilename8.end());

      // Assign as MATLAB string using factory.createScalar
      out[i] = sFilename16;
    }

    outputs[0] = std::move(out);
  }
  void checkArguments(matlab::mex::ArgumentList &outputs, matlab::mex::ArgumentList &inputs)
  {
    const std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    if (inputs.size() != 0)
    {
      matlabPtr->feval(u"error", 0,
                       std::vector<matlab::data::Array>({factory.createScalar("Expected 0 inputs")}));
    }

    if (outputs.size() > 1)
    {
      matlabPtr->feval(u"error", 0,
                       std::vector<matlab::data::Array>({factory.createScalar("Maximum one output supported")}));
    }
  };
};