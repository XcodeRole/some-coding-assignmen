# distutils: language = c++
# cython: language_level = 3

from cpython cimport *
from algOmega cimport * #algOmega omegaStrategy omegaPair BR
cdef class pyalgOmega:
    cdef algOmega c_alg #存储algOmega对象
    def __cinit__(self,int _me, int *_A, double ***_M, double _decay):
        self.c_alg=algOmega(_me,_A, _M,_decay)
    def move(self):
        return self.c_alg.move()
    def update(self,int *acts):
        self.c_alg.update(acts)