#
# class IMat
#
import meschach
import meschach_adds

import sys
import PyIVEC


class IMatRow:
    """
    helper class be be able to type
        IMat MyIMat(3,3)
        a = MyIMat[2][1]

        the term MyIMat[2] return an instance of the class IMatRow,
        and the expression a = MyIMat[2][1] calls the method __getattr__ of IMatRow
    """
    def __init__(self, FromIMat, Row):
        self.FromIMat = FromIMat
        self.Row      = Row

    def __getitem__(self,key):
        return meschach_adds.im_get_val(self.FromIMat, self.Row, key)

    def __setitem__(self,key, val):
        meschach_adds.im_set_val(self.FromIMat, self.Row, key, val)


class IMat:
    """
    class for Int Matrix : self.this is a IMAT * structure
    """
    def __init__(self,m,n):
        """
        setup self.this
        """
        try:
            cimat = meschach_adds.im_get(m,n)
            self.__dict__["this"] = cimat
        except:
            raise IndexError("negative argument for <IMat> initialization")

    def __del__(self):
        """
        free mem of self.this
        """
        if hasattr(self,"this"):
            meschach_adds.im_free(self.this)
        self.__dict__["this"] = meschach_adds.im_null()

    def resize(self,m,n):
        """
        """
        if hasattr(self,"this"):
            meschach_adds.im_resize(self.this,m,n)

    #
    # Get the dimension of the matrix
    #

    def _get_m(self):
        return meschach_adds.im_dim1(self)
    def _get_n(self):
        return meschach_adds.im_dim2(self)

    m = property( _get_m, doc="1rst dim")
    n = property( _get_n, doc="2nd dim")


    def __setattr__(self,name, value):
        if ( name == "this" ):
            raise AttributeError("not allowed to change self.this in this way")
        else:
            pass
            #raise AttributeError("not allowed to add an attribute to an IMat instance")

    #
    # Getting the data
    #
    def __setitem__(self,key,ival):
        if ( not isinstance(key, int) ):
            raise TypeError("must be a integer")
        if ( not isinstance(ival, int) ):
            raise TypeError("must be a integer")

        IMatRowInstance = IMatRow(self.this, key)
        return IMatRowInstance

    def __getitem__(self,key):
        if ( not isinstance(key, int) ):
            raise TypeError("must be a integer")

        IMatRowInstance = IMatRow(self.this, key)
        return IMatRowInstance

    #
    # Input - Output diagnostics
    #
    def out(self,file=None):
        """
        """
        if (file==None):
            meschach_adds.im_foutput(sys.stdout,self.this)
        else:
            meschach_adds.im_foutput(file,self.this)

    def dump(self,file=None):
        """
        """
        if (file==None):
            meschach_adds.im_dump(sys.stdout,self.this)
        else:
            meschach_adds.im_dump(file,self.this)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        meschach_adds.im_foutput(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        meschach_adds.im_dump(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    #
    # Members functions
    #

    def ones(self):
        """
        """
        meschach_adds.im_ones(self.this)

    def zero(self):
        """
        """
        meschach_adds.im_zero(self.this)

    def rand(self):
        """
        """
        meschach_adds.im_rand(self.this)

    def identity(self):
        """
        """
        meschach_adds.im_ident(self.this)

    def __invert__(self):
        """
        transpose of the matrix At = ~A
        """
        ret = IMat(self.n, self.m)
        meschach_adds.im_transp(self.this, ret.this)
        return ret

    #
    # operators creating temporary
    #
    def __add__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            ret = IMat(self.m, self.n)
            meschach_adds.im_add(self.this, a.this, ret.this)
            return ret
        # a is a integer
        elif ( isinstance(a, int) ):
            ret = IMat(self.m,self.n)
            meschach_adds.im_sadd(a, self.this, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __radd__(self,a):
        return self.__add__(a)

    def __sub__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            ret = IMat(self.m, self.n)
            meschach_adds.im_sub(self.this, a.this, ret.this)
            return ret
        # a is a integer
        elif ( isinstance(a, int) ):
            ret = IMat(self.m,self.n)
            meschach_adds.im_ssub(a, self.this, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __rsub__(self,a):
        return self.__sub__(a)

    def __mul__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            ret = IMat(self.m, a.n)
            meschach_adds.im_mlt(self.this, a.this, ret.this)
            return ret
        # a is a IVec
        if ( isinstance(a, PyIVEC.IVec) ):
            ret = PyIVEC.IVec(self.n)
            meschach_adds.imv_mlt(self.this, a.this, ret.this)
            return ret
        # a is a integer
        elif ( isinstance(a, int) ):
            ret = IMat(self.m, self.n)
            meschach_adds.im_smlt(self.this, a, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __rmul__(self,a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            meschach_adds.im_add(self.this, a.this, self.this)
            return self
        # a is a integer
        elif ( isinstance(a, int) ):
            meschach_adds.im_sadd(a, self.this, self.this)
            return self
        else:
            raise TypeError("wrong type")

    def __isub__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            meschach_adds.im_sub(self.this, a.this, self.this)
            return self
        # a is a integer
        elif ( isinstance(a, int) ):
            meschach_adds.im_ssub(a, self.this, self.this)
            return self
        else:
            raise TypeError("wrong type")

    def __imul__(self,a):
        # a is a IMat
        if ( isinstance(a, IMat) ):
            tmp = + self
            meschach_adds.im_mlt(tmp.this, a.this, self.this)
            return self
        # a is a integer
        elif ( isinstance(a, int) ):
            meschach_adds.im_smlt(self.this, a, self.this)
            return self
        else:
            raise TypeError("wrong type")

    #
    # assignation operator
    #
    def __pos__(self):
        """
        to clone a matrix : m1 = + m2   --- memory is not shared ---
        """
        ret = IMat(self.m, self.n)
        meschach_adds.im_copy(self.this, ret.this)
        return ret

    # A = - B
    def __neg__(self):
        """
        """
        ret = IMat(self.m, self.n)
        meschach_adds.im_copy(self.this, ret.this)
        meschach_adds.im_smlt(ret.this, -1, ret.this)
        return ret
