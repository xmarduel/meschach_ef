
from PyIVEC import *
from PyVEC import *
from PyMAT import *
from PySPMAT import *
from PyPERM import *

import meschach

def CHfactor(a: Mat) -> Mat :
    return  Mat(meschach.CHfactor(a.cmat))

def CHsolve(a: Mat, v: Vec) -> Vec :
    return  Vec(meschach.CHsolve(a.cmat, v.cvec, None))