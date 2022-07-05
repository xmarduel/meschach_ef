#
# class SpMat
#

import sys

import meschach
import meschach_adds

import PyVEC


class SpMatRow:
    """
    utility class to enable calls like A[2][1]
    """
    def __init__(self, FromSpMat, Row):
        self.FromSpMat = FromSpMat
        self.Row       = Row

    def __getitem__(self,key):
        return meschach.sp_get_val(self.FromSpMat, self.Row, key)
    def __setitem__(self,key, val):
        meschach.sp_set_val(self.FromSpMat, self.Row, key, val)


class SpMat(object):
    """
    class for  Sparse Matrix : self.cspmat is a SPMAT * structure
    """
    def __init__(self, m, n):
        """
        setup self.cspmat ; this is a SPMAT* structure
        """
        self.cspmat = meschach.sp_get(m, n, 10)

    #
    # Get the dimension of the matrix
    #
    def _get_m(self):
        return meschach_adds.sp_dim1(self.cspmat)
    def _get_n(self):
        return meschach_adds.sp_dim2(self.cspmat)

    m = property( _get_m, doc="1rst dim")
    n = property( _get_n, doc="2nd dim")


    #def __setattr__(self, name, value):

    #    if name == "this" :
    #        raise AttributeError("not allowed to change self.cspmat in this way")
    #    else:
    #        pass
     #       #raise AttributeError("not allowed to add an attribute to an IMat instance")

    #
    # Getting/Setting  the data
    #
    def __getitem__(self, key):
        SpMatRowInstance = SpMatRow(self.cspmat, key)
        return SpMatRowInstance

    def __setitem__(self, key, val):
        SpMatRowInstance = SpMatRow(self.cspmat, key)
        return SpMatRowInstance

    #
    # Input - Output diagnostics
    #
    def out(self, stream=sys.stdout):
        """
        """
        meschach.sp_foutput(stream, self.cspmat)

    def dump(self,stream=sys.stdout):
        """
        """
        meschach.sp_dump(stream, self.cspmat)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach.sp_foutput(f_tmp, self.cspmat)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach.sp_dump(f_tmp, self.cspmat)
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
        meschach.sp_free(self.cspmat)
        self.cspmat = meschach.sp_null()

    def resize(self, m, n):
        """
        """
        meschach.sp_resize(self.cspmat, m, n)

    #
    # Members functions
    #
    def ones(self):
        """
        """
        meschach_adds.sp_ones(self.cspmat)

    def zero(self):
        """
        """
        meschach.sp_zero(self.cspmat)
        meschach.sp_compact(self.cspmat, 0.0) # because sp_zero don't

    def rand(self):
        """
        """
        meschach_adds.sp_rand(self.cspmat)

    def identity(self):
        """
        """
        meschach_adds.sp_ident(self.cspmat)

    def eye(self):
        """
        """
        meschach_adds.sp_eye(self.cspmat)

    def tridiag(self, lower_val, diag_val, upper_val):
        """
        """
        meschach_adds.sp_tridiag(self.cspmat, lower_val, diag_val, upper_val)

    #
    # operators creating temporary
    #
    def __add__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, self.n)
            meschach.sp_add(self.cspmat, a.cspmat, ret.cspmat)
            meschach.sp_compact(ret.cspmat, 0.0)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError("wrong type")
        #    ret = SpMat(self.m,self.n)
        #    sp_sadd(a, self.cspmat, ret.cspmat)
        #    sp_compact(ret.cspmat, 0.0)
        #    return ret
        else:
            raise TypeError("wrong type")

    def __radd__(self, a):
        return self.__add__(a)

    def __sub__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, self.n)
            meschach.sp_sub(self.cspmat, a.cspmat, ret.cspmat)
            meschach.sp_compact(ret.cspmat, 0.0)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError("wrong type")
        #    ret = SpMat(self.m,self.n)
        #    sp_ssub(a, self.cspmat, ret.cspmat)
        #    sp_compact(ret.cspmat, 0.0)
        #    return ret
        else:
            raise TypeError("wrong type")

    def __rsub__(self, a):
        return self.__sub__(a)

    def __mul__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            ret = SpMat(self.m, a.n)
            meschach_adds.sp_m_mlt(self.cspmat, a.cspmat, ret.cspmat)
            meschach.sp_compact(ret.cspmat, 0.0)
            return ret
        # a is a Vec
        elif ( isinstance(a, PyVEC.Vec) ):
            ret = PyVEC.Vec(self.n)
            meschach.sp_mv_mlt(self.cspmat, a.cspmat, ret.cvec)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = SpMat(self.m, self.n)
            meschach.sp_smlt(self.cspmat, a, ret.cspmat)
            meschach.sp_compact(ret.cspmat, 0.0)
            return ret
        #
        else:
            raise TypeError("wrong type")

    def __rmul__(self, a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            meschach.sp_add(self.cspmat, a.cspmat, self.cspmat)
            meschach.sp_compact(self.cspmat, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError("wrong type")
        #    sp_sadd(a, self.cspmat, self.cspmat)
        #    sp_compact(self.cspmat, 0.0)
        #    return self
        else:
            raise TypeError("wrong type")
        #
    #
    #
    def __isub__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            meschach.sp_sub(self.cspmat, a.cspmat, self.cspmat)
            meschach.sp_compact(self.cspmat, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            raise TypeError("wrong type")
        #    sp_ssub(a, self.cspmat, self.cspmat)
        #    sp_compact(self.cspmat, 0.0)
        #    return self
        else:
            raise TypeError("wrong type")

    def __imul__(self, a):
        # a is a SpMat
        if ( isinstance(a, SpMat) ):
            tmp = + self
            meschach_adds.sp_m_mlt(tmp.cspmat, a.cspmat, self.cspmat)
            meschach.sp_compact(self.cspmat, 0.0)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            meschach.sp_smlt(self.cspmat, a, self.cspmat)
            meschach.sp_compact(self.cspmat, 0.0)
            return self
        #
        else:
            raise TypeError("wrong type")

    #
    # assignation operator ??
    #
    # A = + B
    def __pos__(self):
        """
        to clone a Sparse Matrix
        """
        ret = SpMat(self.m, self.n)
        meschach.sp_copy2(self.cspmat, ret.cspmat)
        meschach.sp_compact(ret.cspmat, 0.0)
        return ret

    # A = - B
    def __neg__(self):
        """
        create a copy of the matrix , multiplied with (-1)
        """
        ret = SpMat(self.m, self.n)
        meschach.sp_copy2(self.cspmat, ret.cspmat)
        meschach.sp_smlt(ret.cspmat, -1.0, ret.cspmat)
        meschach.sp_compact(ret.cspmat, 0.0)
        return ret
