#
# class SpMat
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2014/11/22 12:24:10 $
#  $Revision: 1.13 $
#  ----------------------------------------------------------------
#
import sys

#from libmeschach import *
import libmeschach
#from libmeschach_adds import *
import libmeschach_adds

import PyVEC


class SpMatRow:
    """
    utility class to enable calls like A[2][1]
    """
    def __init__(self, FromSpMat, Row):
        self.FromSpMat = FromSpMat
        self.Row       = Row

    def __getitem__(self,key):
        return libmeschach.sp_get_val(self.FromSpMat, self.Row, key)
    def __setitem__(self,key, val):
        libmeschach.sp_set_val(self.FromSpMat, self.Row, key, val)


class SpMat(object):
    """
    class for  Sparse Matrix : self.this is a SPMAT * structure
    """
    def __init__(self,m,n): 
        """
        setup self.this ; this is a SPMAT* structure 
        """
        try:
            cspmat = libmeschach.sp_get(m,n,10)
            self.__dict__["this"] = cspmat
        except:
            raise IndexError, "negative argument for <SpMat> initialization"

    def __del__(self):
        """
        free the storage in self.this
        """
        if hasattr(self,"this"):
            libmeschach.sp_free(self.this)
        self.__dict__["this"] = libmeschach.sp_null()

    #
    # Get the dimension of the matrix
    #
    def _get_m(self):
        return libmeschach_adds.sp_dim1(self)
    def _get_n(self):
        return libmeschach_adds.sp_dim2(self)

    m = property( _get_m, doc="1rst dim")
    n = property( _get_n, doc="2nd dim")


    def __setattr__(self, name, value):

        if name == "this" :
            raise AttributeError, "not allowed to change self.this in this way"
        else:
            pass;
            #raise AttributeError, "not allowed to add an attribute to an IMat instance"

    #
    # Getting/Setting  the data
    #
    def __getitem__(self, key):
        SpMatRowInstance = SpMatRow(self.this, key)
        return SpMatRowInstance
        
    def __setitem__(self, key, val):
        SpMatRowInstance = SpMatRow(self.this, key)
        return SpMatRowInstance

    #
    # Input - Output diagnostics
    #
    def out(self, stream=sys.stdout):
        """
        """
        libmeschach.sp_foutput(stream, self.this)

    def dump(self,stream=sys.stdout):
        """
        """
        ibmeschach.sp_dump(stream, self.this)


    def __str__(self): # as "out"
        f_tmp = file("tmp.dat", "w+")
        libmeschach.sp_foutput(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = file("tmp.dat", "w+")
        libmeschach.sp_dump(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    #
    # playing with storage
    #
    def free(self):
        """
        """
        libmeschach.sp_free(self.this)
        self.__dict__["this"] = libmeschach.sp_null()

    def resize(self,m,n):
        """
        """
        libmeschach.sp_resize(self.this,m,n)

    #
    # Members functions
    #
    def ones(self):
        """
        """
        libmeschach_adds.sp_ones(self.this)

    def zero(self):
        """
        """
        libmeschach.sp_zero(self.this)
        libmeschach.sp_compact(self.this, 0.0) # because sp_zero don't

    def rand(self):
        """
        """
        libmeschach_adds.sp_rand(self.this)

    def identity(self):
        """
        """
        libmeschach_adds.sp_ident(self.this)

    def eye(self):
        """
        """
        libmeschach_adds.sp_eye(self.this)

    def tridiag(self, lower_val, diag_val, upper_val):
        """
        """
        libmeschach_adds.sp_tridiag(self.this, lower_val, diag_val, upper_val)

    #
    # operators creating temporary
    #
    def __add__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, self.n)
            libmeschach.sp_add(self.this, a.this, ret.this)
            libmeschach.sp_compact(ret.this, 0.0)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError, "wrong type"
        #    ret = SpMat(self.m,self.n)
        #    sp_sadd(a, self.this, ret.this)
        #    sp_compact(ret.this, 0.0)
        #    return ret
        else:
            raise TypeError, "wrong type"

    def __radd__(self,a):
        return self.__add__(a)
 
    def __sub__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, self.n)
            libmeschach.sp_sub(self.this, a.this, ret.this)
            libmeschach.sp_compact(ret.this, 0.0)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError, "wrong type"
        #    ret = SpMat(self.m,self.n)
        #    sp_ssub(a, self.this, ret.this)
        #    sp_compact(ret.this, 0.0)
        #    return ret
        else:
            raise TypeError, "wrong type"

    def __rsub__(self,a):
        return self.__sub__(a)

    def __mul__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, a.n)
            libmeschach_adds.sp_m_mlt(self.this, a.this, ret.this)
            libmeschach.sp_compact(ret.this, 0.0)
            return ret
        # a is a Vec
        elif ( isinstance(a, PyVEC.Vec) ):
            ret = PyVEC.Vec(self.n)
            libmeschach.sp_mv_mlt(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = SpMat(self.m, self.n)
            libmeschach.sp_smlt(self.this, a, ret.this)
            libmeschach.sp_compact(ret.this, 0.0)
            return ret
        #
        else:
            raise TypeError, "wrong type"

    def __rmul__(self,a):
        return self.__mul__(a)	

    #
    # operator using and returning base object
    #
    def __iadd__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            libmeschach.sp_add(self.this, a.this, self.this)
            libmeschach.sp_compact(self.this, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError, "wrong type"
        #    sp_sadd(a, self.this, self.this)
        #    sp_compact(self.this, 0.0)
        #    return self
        else: 
            raise TypeError, "wrong type"
        #
    #
    #
    def __isub__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            libmeschach.sp_sub(self.this, a.this, self.this)
            libmeschach.sp_compact(self.this, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError, "wrong type"
        #    sp_ssub(a, self.this, self.this)
        #    sp_compact(self.this, 0.0)
        #    return self
        else: 
            raise TypeError, "wrong type"
    
    def __imul__(self,a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            tmp = + self
            libmeschach_adds.sp_m_mlt(tmp.this, a.this, self.this)
            libmeschach.sp_compact(self.this, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            libmeschach.sp_smlt(self.this, a, self.this)
            libmeschach.sp_compact(self.this, 0.0)
            return self
        #
        else: 
            raise TypeError, "wrong type"

    #
    # assignation operator ??
    #
    # A = + B
    def __pos__(self):
        """
        to clone a Sparse Matrix
        """
        ret = SpMat(self.m, self.n)
        libmeschach.sp_copy2(self.this, ret.this)
        libmeschach.sp_compact(ret.this, 0.0)
        return ret

    # A = - B
    def __neg__(self):
        """
        create a copy of the matrix , multiplied with (-1)
        """
        ret = SpMat(self.m, self.n)
        libmeschach.sp_copy2(self.this, ret.this)
        libmeschach.sp_smlt(ret.this, -1.0, ret.this)
        libmeschach.sp_compact(ret.this, 0.0)
        return ret
