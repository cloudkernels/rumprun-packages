#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif

extern PyObject *PyInit__minpack(void);
extern PyObject *PyInit__zeros(void);
extern PyObject *PyInit__group_columns(void);
extern PyObject *PyInit__cobyla(void);
extern PyObject *PyInit__bglu_dense(void);
extern PyObject *PyInit_minpack2(void);
extern PyObject *PyInit__slsqp(void);
extern PyObject *PyInit__trlib(void);
extern PyObject *PyInit__nnls(void);
extern PyObject *PyInit__lbfgsb(void);
extern PyObject *PyInit_moduleTNC(void);
extern PyObject *PyInit___zeros(void);
extern PyObject *PyInit_givens_elimination(void);
extern PyObject *PyInit__stats(void);
extern PyObject *PyInit_mvn(void);
extern PyObject *PyInit_statlib(void);
extern PyObject *PyInit_interpnd(void);
extern PyObject *PyInit__fitpack(void);
extern PyObject *PyInit__bspl(void);
extern PyObject *PyInit__ppoly(void);
extern PyObject *PyInit_dfitpack(void);
extern PyObject *PyInit__decomp_update(void);
extern PyObject *PyInit__fblas(void);
extern PyObject *PyInit__flinalg(void);
extern PyObject *PyInit__interpolative(void);
extern PyObject *PyInit_cython_lapack(void);
extern PyObject *PyInit__flapack(void);
extern PyObject *PyInit_cython_blas(void);
extern PyObject *PyInit__solve_toeplitz(void);
extern PyObject *PyInit__test_ccallback(void);
extern PyObject *PyInit__fpumode(void);
extern PyObject *PyInit_messagestream(void);
extern PyObject *PyInit__ccallback_c(void);
extern PyObject *PyInit__upfirdn_apply(void);
extern PyObject *PyInit__spectral(void);
extern PyObject *PyInit_sigtools(void);
extern PyObject *PyInit__peak_finding_utils(void);
extern PyObject *PyInit__max_len_seq_inner(void);
extern PyObject *PyInit_spline(void);
extern PyObject *PyInit___odrpack(void);
extern PyObject *PyInit__voronoi(void);
extern PyObject *PyInit__distance_wrap(void);
extern PyObject *PyInit__hausdorff(void);
extern PyObject *PyInit_qhull(void);
extern PyObject *PyInit_ckdtree(void);
extern PyObject *PyInit_convolve(void);
extern PyObject *PyInit__fftpack(void);
extern PyObject *PyInit__hierarchy(void);
extern PyObject *PyInit__optimal_leaf_ordering(void);
extern PyObject *PyInit__vq(void);
extern PyObject *PyInit__test_fortran(void);
extern PyObject *PyInit_mio_utils(void);
extern PyObject *PyInit_mio5_utils(void);
extern PyObject *PyInit_streams(void);
extern PyObject *PyInit__ni_label(void);
extern PyObject *PyInit__nd_image(void);
extern PyObject *PyInit__cytest(void);
extern PyObject *PyInit__ctest_oldapi(void);
extern PyObject *PyInit__test_round(void);
extern PyObject *PyInit__ufuncs(void);
extern PyObject *PyInit__comb(void);
extern PyObject *PyInit__ufuncs_cxx(void);
extern PyObject *PyInit_cython_special(void);
extern PyObject *PyInit__ellip_harm_2(void);
extern PyObject *PyInit_specfun(void);
extern PyObject *PyInit__test_multivariate(void);
extern PyObject *PyInit_vode(void);
extern PyObject *PyInit__test_odeint_banded(void);
extern PyObject *PyInit__dop(void);
extern PyObject *PyInit__quadpack(void);
extern PyObject *PyInit__odepack(void);
extern PyObject *PyInit_lsoda(void);
extern PyObject *PyInit__min_spanning_tree(void);
extern PyObject *PyInit__tools(void);
extern PyObject *PyInit__reordering(void);
extern PyObject *PyInit__traversal(void);
extern PyObject *PyInit__shortest_path(void);
extern PyObject *PyInit__arpack(void);
extern PyObject *PyInit__superlu(void);
extern PyObject *PyInit__iterative(void);
extern PyObject *PyInit__sparsetools(void);
extern PyObject *PyInit__csparsetools(void);

void add_scipy_builtin()
{
	PyImport_AppendInittab("scipy.optimize._minpack", &PyInit__minpack);
	PyImport_AppendInittab("scipy.optimize.cython_optimize._zeros", &PyInit__zeros);
	PyImport_AppendInittab("scipy.optimize._group_columns", &PyInit__group_columns);
	PyImport_AppendInittab("scipy.optimize._cobyla", &PyInit__cobyla);
	PyImport_AppendInittab("scipy.optimize._bglu_dense", &PyInit__bglu_dense);
	PyImport_AppendInittab("scipy.optimize.minpack2", &PyInit_minpack2);
	PyImport_AppendInittab("scipy.optimize._slsqp", &PyInit__slsqp);
	PyImport_AppendInittab("scipy.optimize._trlib._trlib", &PyInit__trlib);
	PyImport_AppendInittab("scipy.optimize._nnls", &PyInit__nnls);
	PyImport_AppendInittab("scipy.optimize._lbfgsb", &PyInit__lbfgsb);
	PyImport_AppendInittab("scipy.optimize.moduleTNC", &PyInit_moduleTNC);
	PyImport_AppendInittab("scipy.optimize.__zeros", &PyInit___zeros);
	PyImport_AppendInittab("scipy.optimize._lsq.givens_elimination", &PyInit_givens_elimination);
	PyImport_AppendInittab("scipy.stats._stats", &PyInit__stats);
	PyImport_AppendInittab("scipy.stats.mvn", &PyInit_mvn);
	PyImport_AppendInittab("scipy.stats.statlib", &PyInit_statlib);
	PyImport_AppendInittab("scipy.interpolate.interpnd", &PyInit_interpnd);
	PyImport_AppendInittab("scipy.interpolate._fitpack", &PyInit__fitpack);
	PyImport_AppendInittab("scipy.interpolate._bspl", &PyInit__bspl);
	PyImport_AppendInittab("scipy.interpolate._ppoly", &PyInit__ppoly);
	PyImport_AppendInittab("scipy.interpolate.dfitpack", &PyInit_dfitpack);
	PyImport_AppendInittab("scipy.linalg._decomp_update", &PyInit__decomp_update);
	PyImport_AppendInittab("scipy.linalg._fblas", &PyInit__fblas);
	PyImport_AppendInittab("scipy.linalg._flinalg", &PyInit__flinalg);
	PyImport_AppendInittab("scipy.linalg._interpolative", &PyInit__interpolative);
	PyImport_AppendInittab("scipy.linalg.cython_lapack", &PyInit_cython_lapack);
	PyImport_AppendInittab("scipy.linalg._flapack", &PyInit__flapack);
	PyImport_AppendInittab("scipy.linalg.cython_blas", &PyInit_cython_blas);
	PyImport_AppendInittab("scipy.linalg._solve_toeplitz", &PyInit__solve_toeplitz);
	PyImport_AppendInittab("scipy._lib._test_ccallback", &PyInit__test_ccallback);
	PyImport_AppendInittab("scipy._lib._fpumode", &PyInit__fpumode);
	PyImport_AppendInittab("scipy._lib.messagestream", &PyInit_messagestream);
	PyImport_AppendInittab("scipy._lib._ccallback_c", &PyInit__ccallback_c);
	PyImport_AppendInittab("scipy.signal._upfirdn_apply", &PyInit__upfirdn_apply);
	PyImport_AppendInittab("scipy.signal._spectral", &PyInit__spectral);
	PyImport_AppendInittab("scipy.signal.sigtools", &PyInit_sigtools);
	PyImport_AppendInittab("scipy.signal._peak_finding_utils", &PyInit__peak_finding_utils);
	PyImport_AppendInittab("scipy.signal._max_len_seq_inner", &PyInit__max_len_seq_inner);
	PyImport_AppendInittab("scipy.signal.spline", &PyInit_spline);
	PyImport_AppendInittab("scipy.odr.__odrpack", &PyInit___odrpack);
	PyImport_AppendInittab("scipy.spatial._voronoi", &PyInit__voronoi);
	PyImport_AppendInittab("scipy.spatial._distance_wrap", &PyInit__distance_wrap);
	PyImport_AppendInittab("scipy.spatial._hausdorff", &PyInit__hausdorff);
	PyImport_AppendInittab("scipy.spatial.qhull", &PyInit_qhull);
	PyImport_AppendInittab("scipy.spatial.ckdtree", &PyInit_ckdtree);
	PyImport_AppendInittab("scipy.fftpack.convolve", &PyInit_convolve);
	PyImport_AppendInittab("scipy.fftpack._fftpack", &PyInit__fftpack);
	PyImport_AppendInittab("scipy.cluster._hierarchy", &PyInit__hierarchy);
	PyImport_AppendInittab("scipy.cluster._optimal_leaf_ordering", &PyInit__optimal_leaf_ordering);
	PyImport_AppendInittab("scipy.cluster._vq", &PyInit__vq);
	PyImport_AppendInittab("scipy.io._test_fortran", &PyInit__test_fortran);
	PyImport_AppendInittab("scipy.io.matlab.mio_utils", &PyInit_mio_utils);
	PyImport_AppendInittab("scipy.io.matlab.mio5_utils", &PyInit_mio5_utils);
	PyImport_AppendInittab("scipy.io.matlab.streams", &PyInit_streams);
	PyImport_AppendInittab("scipy.ndimage._ni_label", &PyInit__ni_label);
	PyImport_AppendInittab("scipy.ndimage._nd_image", &PyInit__nd_image);
	PyImport_AppendInittab("scipy.ndimage._cytest", &PyInit__cytest);
	PyImport_AppendInittab("scipy.ndimage._ctest_oldapi", &PyInit__ctest_oldapi);
	PyImport_AppendInittab("scipy.special._test_round", &PyInit__test_round);
	PyImport_AppendInittab("scipy.special._ufuncs", &PyInit__ufuncs);
	PyImport_AppendInittab("scipy.special._comb", &PyInit__comb);
	PyImport_AppendInittab("scipy.special._ufuncs_cxx", &PyInit__ufuncs_cxx);
	PyImport_AppendInittab("scipy.special.cython_special", &PyInit_cython_special);
	PyImport_AppendInittab("scipy.special._ellip_harm_2", &PyInit__ellip_harm_2);
	PyImport_AppendInittab("scipy.special.specfun", &PyInit_specfun);
	PyImport_AppendInittab("scipy.integrate._test_multivariate", &PyInit__test_multivariate);
	PyImport_AppendInittab("scipy.integrate.vode", &PyInit_vode);
	PyImport_AppendInittab("scipy.integrate._test_odeint_banded", &PyInit__test_odeint_banded);
	PyImport_AppendInittab("scipy.integrate._dop", &PyInit__dop);
	PyImport_AppendInittab("scipy.integrate._quadpack", &PyInit__quadpack);
	PyImport_AppendInittab("scipy.integrate._odepack", &PyInit__odepack);
	PyImport_AppendInittab("scipy.integrate.lsoda", &PyInit_lsoda);
	PyImport_AppendInittab("scipy.sparse.csgraph._min_spanning_tree", &PyInit__min_spanning_tree);
	PyImport_AppendInittab("scipy.sparse.csgraph._tools", &PyInit__tools);
	PyImport_AppendInittab("scipy.sparse.csgraph._reordering", &PyInit__reordering);
	PyImport_AppendInittab("scipy.sparse.csgraph._traversal", &PyInit__traversal);
	PyImport_AppendInittab("scipy.sparse.csgraph._shortest_path", &PyInit__shortest_path);
	PyImport_AppendInittab("scipy.sparse.linalg.eigen.arpack._arpack", &PyInit__arpack);
	PyImport_AppendInittab("scipy.sparse.linalg.dsolve._superlu", &PyInit__superlu);
	PyImport_AppendInittab("scipy.sparse.linalg.isolve._iterative", &PyInit__iterative);
	PyImport_AppendInittab("scipy.sparse._sparsetools", &PyInit__sparsetools);
	PyImport_AppendInittab("scipy.sparse._csparsetools", &PyInit__csparsetools);
}

#ifdef __cplusplus
}
#endif
