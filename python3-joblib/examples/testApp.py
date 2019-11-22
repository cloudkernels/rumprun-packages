from joblib import Parallel, delayed
from math import sqrt

a = Parallel(n_jobs=1)(delayed(sqrt)(i**2) for i in range(10))
print(a)
