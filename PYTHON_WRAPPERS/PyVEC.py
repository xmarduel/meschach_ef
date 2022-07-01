#
# class Vec
#

import meschach
import meschach_adds

import sys


class Vec:
    """
    class for Vector : self.this is a VEC * structure
    """
    def __init__(self,m):
        """
        setup self.this
        """
        try:
            cvec = meschach.v_get(m)
            self.__dict__["this"] = cvec
        except:
            #raise IndexError
            raise IndexError("negative argument for <Vec> initialization")

    def __del__(self):
        """
        free mem of self.this
        """
        if hasattr(self,"this"):
            meschach.v_free(self.this)
        self.__dict__["this"] = meschach.v_null()



    @classmethod
    def Memory(cls, stream = sys.stdout):
        meschach.mem_info_file_onetype(stream, 0, meschach.TYPE_VEC)

    #
    #  Get the dimension of the matrix
    #
    def _get_m(self):
        return meschach_adds.v_dim(self)

    m = property( _get_m, doc="dim")
    dim = property( _get_m, doc="dim")

    #
    # Getting the data
    #
    def __len__(self):
        return meschach_adds.v_dim(self)

    def __setitem__(self,key,ival):
        if ( not isinstance(key, int) ):
            raise TypeError("must be a integer")
        if ( not  ( isinstance(ival, float) or isinstance(ival, int) ) ):
            raise TypeError("must be a float")
        return meschach.v_set_val(self.this,key,ival)

    def __getitem__(self,key):
        if ( not isinstance(key, int) ):
            raise TypeError("must be a integer")
        return meschach.v_get_val(self.this,key)

    def __setattr__(self,name, value):
        if name == "this" :
            raise AttributeError("not allowed to change self.this in this way")
        else:
            pass
            #raise AttributeError("not allowed to add an attribute to an IMat instance")

    #
    # Input - Output diagnostics
    #
    def out(self,file=None):
        """
        """
        if (file==None):
            meschach.v_foutput(sys.stdout, self.this)
        else:
            meschach.v_foutput(file,self.this)

    def dump(self,file=None):
        """
        """
        if (file==None):
            meschach.v_dump(sys.stdout,self.this)
        else:
            meschach.v_dump(file,self.this)

    def __str__(self): # as "out"
        f_tmp = open("tmp.dat", "w+")
        self.out(f_tmp)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()

        return str

    def __repr__(self): # as "dump"
        f_tmp = open("tmp.dat", "w+")
        self.dump(f_tmp)
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
        meschach.v_free(self.this)
        self.__dict__["this"] = meschach.v_null()

    def resize(self,m):
        """
        """
        meschach.v_resize(self.this, m)
    #
    #
    # Members functions
    #
    def ones(self):
        """
        """
        meschach.v_ones(self.this)

    def zero(self):
        """
        """
        meschach.v_zero(self.this)

    def rand(self):
        """
        """
        meschach.v_rand(self.this)

    def norm(self):
        """
        """
        return meschach.v_norm2(self.this)

    #
    # operators creating temporary
    #
    def __add__(self,a):
        # a is a Vec
        if ( isinstance(a, Vec) ):
            ret = Vec(self.m)
            meschach.v_add(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Vec(self.m)
            meschach_adds.v_sadd(a, self.this, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __radd__(self,a):
        return self.__add__(a)

    def __sub__(self,a):
        # a is a Vec
        if ( isinstance(a, Vec) ):
            ret = Vec(self.m)
            meschach.v_sub(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Vec(self.m)
            meschach_adds.v_ssub(a, self.this, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __rsub__(self,a):
        return self.__sub__(a)

    def __mul__(self,a):
        # a is a Vec
        if ( isinstance(a, Vec) ):
            ret = Vec(self.m)
            ret = meschach.in_prod(self.this, a.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Vec(self.m)
            meschach.sv_mlt(a, self.this, ret.this)
            return ret
        else:
            raise TypeError("wrong type")

    def __rmul__(self,a):
        return self.__mul__(a)

    #
    # operator using and returning base object
    #
    def __iadd__(self,a):
        # a is a Vec
        if ( isinstance(a, Vec) ):
            meschach.v_add(self.this, a.this, self.this)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            meschach_adds.v_sadd(a, self.this, self.this)
            return self
        else:
            raise TypeError("wrong type")

    def __isub__(self,a):
        # a is a Vec
        if ( isinstance(a, Vec) ):
            meschach.v_sub(self.this, a.this, self.this)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            meschach_adds.v_ssub(a, self.this, self.this)
            return self
        else:
            raise TypeError("wrong type")

    def __imul__(self,a):
        # a is a integer or a double
        if ( isinstance(a, int) or isinstance(a, float) ):
            meschach.sv_mlt(a, self.this, self.this)
            return self
        else:
            raise TypeError("wrong type")

    #
    # assignation operator : with __pos__
    #

    # A = + B
    def __pos__(self): # unary op.
        """
        to clone a Vector
        """
        ret = Vec(self.m)
        meschach.v_copy(self.this, ret.this)
        return ret

    # A = - B
    def __neg__(self): # unary op.
        """
        """
        ret = Vec(self.m)
        meschach.v_copy(self.this, ret.this)
        meschach.sv_mlt(-1.0, ret.this, ret.this)
        return ret
