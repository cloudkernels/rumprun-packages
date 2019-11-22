
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

extern PyObject *PyInit__multiarray_umath(void);
extern PyObject *PyInit__multiarray_tests(void);
extern PyObject *PyInit__pocketfft_internal(void);
extern PyObject *PyInit__umath_linalg(void);
extern PyObject *PyInit__umath_tests(void);
extern PyObject *PyInit_lapack_lite(void);
extern PyObject *PyInit_common(void);
extern PyObject *PyInit_mtrand(void);
extern PyObject *PyInit_mt19937(void);
extern PyObject *PyInit_pcg64(void);
extern PyObject *PyInit_sfc64(void);
extern PyObject *PyInit_philox(void);
extern PyObject *PyInit_generator(void);
extern PyObject *PyInit_bit_generator(void);
extern PyObject *PyInit_bounded_integers(void);

void add_numpy_builtin()
{
  PyImport_AppendInittab("numpy.core._multiarray_umath", &PyInit__multiarray_umath);
  PyImport_AppendInittab("numpy.core._multiarray_tests", &PyInit__multiarray_tests);
  PyImport_AppendInittab("numpy.core._umath_tests", &PyInit__umath_tests);
  PyImport_AppendInittab("numpy.fft._pocketfft_internal", &PyInit__pocketfft_internal);
  PyImport_AppendInittab("numpy.linalg._umath_linalg", &PyInit__umath_linalg);
  PyImport_AppendInittab("numpy.linalg.lapack_lite", &PyInit_lapack_lite);
  PyImport_AppendInittab("numpy.random.common", &PyInit_common);
  PyImport_AppendInittab("numpy.random.mtrand", &PyInit_mtrand);
  PyImport_AppendInittab("numpy.random.mt19937", &PyInit_mt19937);
  PyImport_AppendInittab("numpy.random.pcg64", &PyInit_pcg64);
  PyImport_AppendInittab("numpy.random.sfc64", &PyInit_sfc64);
  PyImport_AppendInittab("numpy.random.philox", &PyInit_philox);
  PyImport_AppendInittab("numpy.random.generator", &PyInit_generator);
  PyImport_AppendInittab("numpy.random.bit_generator", &PyInit_bit_generator);
  PyImport_AppendInittab("numpy.random.bounded_integers", &PyInit_bounded_integers);
}

#ifdef __cplusplus
}
#endif
