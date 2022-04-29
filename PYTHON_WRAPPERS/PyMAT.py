#
# class Mat
#
#from libmeschach import *
import libmeschach
#from libmeschach_adds import *
import libmeschach_adds
#
import sys
#
import PyVEC
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2004/07/30 06:52:01 $
#  $Revision: 1.14 $
#  ----------------------------------------------------------------
#
class MatRow(object):
    """
    helper class be be able to type
        Mat MyMat(3,3)
        a = MyMat[2][1]
        MyMat[2][2] = b
        
        the term MyMat[2] return an instance of the class MatRow,
        -> the expression a = MyMat[2][1] calls the method __getitem__ of MatRow
        -> the expression MyMat[2][2] = b calls the method __setitem__ of MatRow
    """
    def __init__(self, FromMat, Row):
        self.FromMat = FromMat
        self.Row     = Row
    #
    def __getitem__(self,key):
        return libmeschach.m_get_val(self.FromMat, self.Row, key)
    #
    def __setitem__(self,key, val):
        libmeschach.m_set_val(self.FromMat, self.Row, key, val)
#
#
#
#
#
class Mat(object):
    """
    class for Dense Matrix : self.this is a MAT * structure
    """
    #
    def __init__(self,m,n): 
        """
        setup self.this
        """
        try:
            cmat = libmeschach.m_get(m,n)
            self.__dict__["this"] = cmat
        except:
            raise IndexError, "negative argument for <Mat> initialization"
    #
    #
    def __del__(self):
        """
        free mem of self.this
        """
        if hasattr(self,"this"):
            libmeschach.m_free(self.this)
        self.__dict__["this"] = libmeschach.m_null()
        #
    #
    #
    #
    # Getting the data
    #
    # Get the dimension of the matrix
    #
    def get_m(self):
        return libmeschach_adds.m_dim1(self)
    def get_n(self):
        return libmeschach_adds.m_dim2(self)
    #
    m = property( get_m, doc="1rst dim")
    n = property( get_n, doc="2nd dim")
    #
    #
    def __setattr__(self,name, value):
        #
        if ( name == "this" ):
            #
            raise AttributeError, "not allowed to change self.this in this way"
            #
        else:
            pass;
            #raise AttributeError, "not allowed to add an attribute to an Mat instance"
    #
    #
    # Getting the data
    #
    def __setitem__(self,key,ival):
        if ( not isinstance(key, int) ):
            raise TypeError, "must be a integer"
        if ( not isinstance(ival, float) ):
            raise TypeError, "must be a float"
        #
        MatRowInstance = MatRow(self.this, key)
        return MatRowInstance
    #
    def __getitem__(self,key):
        if ( not isinstance(key, int) ):
            raise TypeError, "must be a integer"
        #
        MatRowInstance = MatRow(self.this, key)
        return MatRowInstance
    #
    #
    #
    # Input - Output diagnostics
    #
    def out(self,file=None):
        """
        """
        if (file==None):
            libmeschach.m_foutput(sys.stdout,self.this)
        else:
            libmeschach.m_foutput(file,self.this)
    #
    def dump(self,file=None):
        """
        """
        if (file==None):
            libmeschach.m_dump(sys.stdout,self.this)
        else:
            libmeschach.m_dump(file,self.this)
    #
    #
    def __str__(self): # as "out"
        f_tmp = file("tmp.dat", "w+")
        libmeschach.m_foutput(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()
        #
        return str
    #
    def __repr__(self): # as "dump"
        f_tmp = file("tmp.dat", "w+")
        libmeschach.m_dump(f_tmp, self.this)
        f_tmp.seek(0) # rewind
        str = f_tmp.read()
        f_tmp.close()
        #
        return str
    #
    #
    #
    # playing with storage
    #
    def free(self):
        """
        """
        libmeschach.m_free(self.this)
        self.__dict__["this"] = m_null()
    #
    def resize(self,m,n):
        """
        """
        libmeschach.m_resize(self.this,m,n)
    #
    #
    #
    # Members functions
    #
    def ones(self):
        """
        """
        libmeschach.m_ones(self.this)
    #
    def zero(self):
        """
        """
        libmeschach.m_zero(self.this)
    #
    def rand(self):
        """
        """
        libmeschach.m_rand(self.this)
    #
    def identity(self):
        """
        """
        libmeschach.m_ident(self.this)	
    #
    #
    #
    #
    def __invert__(self):
        """
        transpose of the matrix At = ~A
        """
        ret = Mat(self.n, self.m)
        libmeschach.m_transp(self.this, ret.this)
        return ret
    #
    #
    def norm_frob(self):
        """
        """
        return libmeschach.m_norm_frob(self.this)
    #
    #
    #
    # operators creating temporary
    #
    def __add__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            ret = Mat(self.m, self.n)
            libmeschach.m_add(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Mat(self.m,self.n)
            libmeschach_adds.m_sadd(a, self.this, ret.this)
            return ret
        #
        else:
            raise TypeError, "wrong type"
        #
    #
    #
    def __radd__(self,a):
        return self.__add__(a)
    #
    #   
    def __sub__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            ret = Mat(self.m, self.n)
            libmeschach.m_sub(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Mat(self.m,self.n)
            libmeschach_adds.m_ssub(a, self.this, ret.this)
            return ret
        #
        else:
            raise TypeError, "wrong type"
        #
    #
    #
    def __rsub__(self,a):
        return self.__sub__(a)
    #
    #
    def __mul__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            ret = Mat(self.m, a.n)
            libmeschach.m_mlt(self.this, a.this, ret.this)
            return ret
        # a is a Vec
        elif ( isinstance(a, PyVEC.Vec) ):
            ret = PyVEC.Vec(self.n)
            libmeschach.mv_mlt(self.this, a.this, ret.this)
            return ret
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            ret = Mat(self.m, self.n)
            libmeschach.sm_mlt(a, self.this, ret.this)
            return ret
        #
        else:
            raise TypeError, "wrong type"
        #
    #
    #
    def __rmul__(self,a):
        return self.__mul__(a)	
    #
    #
    #
    # operator using and returning base object
    #
    def __iadd__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            libmeschach.m_add(self.this, a.this, self.this)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            libmeschach_adds.m_sadd(a, self.this, self.this)
            return self
        #
        else: 
            raise TypeError, "wrong type"
        #
    #
    #
    def __isub__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            libmeschach.m_sub(self.this, a.this, self.this)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            libmeschach_adds.m_ssub(a, self.this, self.this)
            return self
        #
        else: 
            raise TypeError, "wrong type"
        #
    #
    #
    def __imul__(self,a):
        # a is a Mat
        if ( isinstance(a, Mat) ):
            tmp = + self
            libmeschach.m_mlt(tmp.this, a.this, self.this)
            return self
        # a is a integer or a double
        elif ( isinstance(a, int) or isinstance(a, float) ):
            libmeschach.sm_mlt(a, self.this, self.this)
            return self
        #
        else: 
            raise TypeError, "wrong type"
        #
    #
    #
    #
    # assignation operator
    #
    def __pos__(self):
        """
        to clone a matrix : m1 = + m2   --- memory is not shared ---
        """
        ret = Mat(self.m, self.n)
        libmeschach.m_copy(self.this, ret.this)
        return ret
    #
    #
    # A = - B
    def __neg__(self):
        """
        """
        ret = Mat(self.m, self.n)
        libmeschach.m_copy(self.this, ret.this)
        libmeschach.sm_mlt(-1.0, ret.this, ret.this)
        return ret
    #
    #  

	

