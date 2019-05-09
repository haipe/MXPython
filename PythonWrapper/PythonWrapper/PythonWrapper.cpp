// PythonWrapper.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Python.h"

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

int main(int argc, char* argv[])
{
    printf("app run path:%s.\n", argv[0]);

    // 开始PythonWrapper
    WCHAR filePath[MAX_PATH];
    ::GetModuleFileNameW(NULL, filePath, MAX_PATH);
    ::PathRemoveFileSpecW(filePath);
    std::wstring python_home(filePath);
    python_home += L"\\ph\\";

    Py_SetPythonHome(python_home.c_str());

    Py_Initialize();

    //检查初始化是否完成
    if (!Py_IsInitialized())
    {
        printf("python init fail:\n");
        PyErr_Print();
        printf("\n");

        ::system("pause");
        return -1;
    }

    const char* ver = Py_GetVersion();
    printf("python version:%s.\n", ver);

    //
StartRunPW:

    std::string pythonFile;
    if (argc <= 1)
    {
    InputPythonFile:
        printf("please input a python file or exit:\n");
        std::cin >> pythonFile;
        if (pythonFile.empty())
        {
            printf("input none !\n");
            goto InputPythonFile;
        }

        if (pythonFile == "exit")
        {
            //关闭Python解释器
            Py_Finalize();
            return 0;
        }

        int pos = pythonFile.rfind(".py");
        if (pos == std::string::npos || pos != (pythonFile.length() - 3))
        {
            printf("not a python file!\n");
            goto InputPythonFile;
        }

        if (!::PathFileExistsA(pythonFile.c_str()))
        {
            printf("file not exist!\n");
            goto InputPythonFile;
        }
    }
    else
    {
        pythonFile = argv[1];//第一个传入的python文件
    }

    FILE *ff = nullptr;
    ff = _Py_fopen(pythonFile.c_str(), "r+");
   
    PyCompilerFlags cf;
    cf.cf_flags = 0;
  
    int res = PyRun_SimpleFile(ff, pythonFile.c_str());
    
    PyErr_Print();
    
    argc = 0;
    goto StartRunPW;

    return 0;
}

