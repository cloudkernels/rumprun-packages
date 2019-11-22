import numpy as np
import random
from scipy.sparse import bsr_matrix
from scipy.odr import Model,RealData,ODR
from scipy.optimize import minimize,rosen,rosen_der

print(">> Sparse example\n")
print(bsr_matrix((3, 4), dtype=np.int8).toarray())

print("\n>> ODR example\n")
def f(B, x):
    return B[0]*x + B[1]

x = np.array([0, 1, 2, 3, 4, 5])
y = np.array([i**2 + random.random() for i in x])

linear = Model(f)
mydata = RealData(x, y)
myodr = ODR(mydata, linear, beta0=[1., 2.])
myoutput = myodr.run()
myoutput.pprint()

print("\n>> Minimize example\n")
x0 = [1.3, 0.7, 0.8, 1.9, 1.2]

res = minimize(rosen, x0, method='BFGS', jac=rosen_der,
               options={'gtol': 1e-6, 'disp': True})
print(res.x)
print(res.message)
print(res.hess_inv)
