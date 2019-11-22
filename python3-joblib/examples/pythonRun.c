//github.com/patmarion/NumpyBuiltinExample
#include <Python.h>
#include <stdio.h>
#include "numpy_builtin.h"
#include "frozen.h"

int main(int argc, char **argv)
{

  FILE* inFile = 0;
  if (argc > 1) {
      inFile = fopen(argv[1], "r");
      if (!inFile) {
        fprintf(stderr,"Failed to open file: %s\n", argv[1]);
        return 1;
      }
  }

  wchar_t **wargv = (wchar_t **)PyMem_Malloc(sizeof(wchar_t*)*argc);
  for (int i=0; i<argc; i++) {
      wchar_t *arg = Py_DecodeLocale(argv[i], NULL);
      if (arg == NULL) {
          fprintf(stderr, "Fatal error: cannot decode argv[%d]\n", i);
          return 1;
      }
      wargv[i] = arg;
  }

  add_numpy_builtin();
  PyImport_FrozenModules = _PyImport_FrozenModules;
  Py_FrozenFlag = 1;
  Py_NoSiteFlag = 1;

  Py_SetProgramName(wargv[0]);
  Py_Initialize();

  int retCode;

  if (inFile) {
    retCode = PyRun_SimpleFile(inFile, argv[1]);
    fclose(inFile);
  } 
  else {
    retCode = Py_Main(argc, wargv);
  }
  
  Py_Finalize();

  for (int i=0; i<argc; i++)
      PyMem_RawFree(wargv[i]);

  PyMem_Free(wargv);

  return retCode;
}
