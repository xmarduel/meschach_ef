#
#
#  ----------------------------------------------------------------
#  $Author: xavier $
#  $Date: 2005/10/30 12:59:14 $
#  $Revision: 1.4 $
#
#
#  Revision 1.4  2003/12/20 17:13:43  xavier
#  No log message.
#
#  Revision 1.3  2003/12/20 16:21:33  xavier
#  No log message.
#
#  Revision 1.2  2003/12/20 14:14:29  xavier
#  No log message.
#
#  ----------------------------------------------------------------
#
#
from string import *
from math import *
from copy import *
#
def parse_string_for_monome1D(the_str):
    """
    transform a "monome" string in a list [degree,coeff]
    
    "2"        -> monome = [ 0, 2]
    "-2"       -> monome = [ 0,-2]
    "3*X"      -> monome = [ 1, 3]
    "-3*X"     -> monome = [ 1,-3]
    "4*X^2"    -> monome = [ 2, 4]
    "-4*X^2"   -> monome = [ 2,-4]
    "5*X^3"    -> monome = [ 3, 5]
    "-5*X^3"   -> monome = [ 3,-5]
    "6*X^11"   -> monome = [11, 6]
    "-6*X^11"  -> monome = [11,-6]
    """
    #
    if ( not isinstance(the_str, str) ):
        raise TypeError, "zut"
    #    
    # find degree
    deg = the_str.count('X')
    #
    if (deg > 1):  # not allowed in a monome !
        raise exception  
    #    
    if (deg == 1):
        cursor = the_str.find("^")
        if ( cursor != -1 ):
            deg = atoi(the_str[cursor+1:])
    #
    # find coeff
    coeff = 0.0 
    #
    the_str = the_str.replace(' ','')
    #
    the_str = the_str.replace('*','')
    the_str = the_str.replace('X','')
    the_str = the_str.replace('^','')
        #
    is_neg = True
    #
    if ( the_str[0] == '-' ):
        is_neg = True
    else :
        is_neg = False
    #
    the_str = the_str.replace('-','')
    the_str = the_str.replace('+','')
    #
    #
    try:
        coeff = atof(the_str)
    except:  # exception -> no real coeff in the string ... coeff = 1.0 or 0.0
        if ( deg==0 ):
            coeff = 0.0
        else:
            coeff = 1.0
    #
    if ( is_neg ) :
        coeff *=  -1.0
    #
    return [deg,coeff]
    #
#
#
#
def parse_string_for_polynome1D(str):
    """
    transform a string in a list  of monomes [degree,coeff]
    """
    list_data = [] # the list of monome1D
    #
    str1 = copy(str)
    str1 = str1.replace(' ','') # remove the empty space
    #
    #
    if ( str1.startswith('+') or str1.startswith('-') ):
        pass
    else:
        str1 = '+' + str1
    #
    nbMonomes  = 0
    nbMonomes += str1.count('+')
    nbMonomes += str1.count('-')
    #
    #
    #loop over the monomes
    while ( nbMonomes > 0 ):
        #
        idx1 = str1.find('+',1)
        idx2 = str1.find('-',1)
        #
        idx = 0
        #
        #print "next + at ", idx1
        #print "next - at ", idx2
        #
        if ( idx1 == -1 and idx2 == -1 ):
            #
            if ( nbMonomes  > 1 ):
                raise TypeError,"idx1 and idx2 = -1"
            else:
                idx = len(str1)
        elif ( idx1 == -1 ):
            idx = idx2
        elif ( idx2 == -1 ):
            idx = idx1
        else :
            if ( idx1 < idx2 ):
                idx = idx1
            else :
                idx = idx2
        #
        #
        strMonome = str1[0:idx]
        #
        list_data.append(Monome1D(parse_string_for_monome1D(strMonome)))
        #
        # remove strMonome from str1
        str1 = str1[idx:]
        #
        nbMonomes -= 1
    #
    return list_data
    #
#
#
def sort_fun( m1, m2 ):
    #
    res  =  m1.__cmp__(m2)
    #
    return res
    #
#    
#
def fact(n):
    #
    if ( n>1 ):
        return n*fact(n-1)
    else:
        return 1
#
#
#################################################################################################
#
class Monome1D(object):
    #
    """
    the main class : a structure with two field :
        self.deg
        self.coeff
    """
    #
    def __init__(self, data):
        """
        1- init with a string :  MyMonome = Monome1D("-3.0*X^2")
        2- init with a list of two elements[deg, coeff] )
        3- init with a other Monome ( copy constructor )
        """
        #
        #print "__init__ : data =", data
        if ( isinstance(data, str) ):
            [self.deg, self.coeff] = parse_string_for_monome1D(data)
        elif ( isinstance(data, Monome1D) ):
            self.deg  = copy(data.deg)
            self.coeff = copy(data.coeff)
        elif ( isinstance(data, list ) ):
            self.deg  = data[0] 
            self.coeff = data[1] 
        else :
            print "__init__ : data =", data
            raise TypeError, "bad init"
        #
        #
    #
    #
    def __str__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.deg
    #
    def __repr__(self):
        return '%lf'%self.coeff + '*X^%1d'%self.deg
    #
    #
    #
    def __mul__(self,other):
        """
        multiplication of two monomes
        """
        deg   = 0
        coeff = 0.0
        #
        if ( isinstance(other, Monome1D) ):
            deg   = self.deg   + other.deg   # deg in X
            coeff = self.coeff * other.coeff # coeff
        elif (isinstance(other, int)) :
            deg   = self.deg # deg in X
            coeff = self.coeff * other # coeff
        elif (isinstance(other, float)) :
            deg   = self.deg # deg in X
            coeff = self.coeff * other # coeff
        else :
           raise typeError,"must  multiply with a int, a float or a Monome1D"
        #
        return Monome1D( [deg, coeff] )
        #
    #
    #
    #
    def __cmp__(self,other):
        """
        """
        if ( self.deg < other.deg ):
            return -1
        elif ( self.deg > other.deg ):
            return  1
        else:
            return 0
                
    #
    #
    def array_to_string(self):
        """
        """
        str = ' %+lf'%self.coeff
        #
        if ( self.deg == 1 ):
            str += "*X"
        elif ( self.deg > 1 ):
            str += '*X^' + itoa(self.deg)
        #
        return str
    #
#
#
#
#
#################################################################################################
#
class Poly1D(object):
    """
    the main class : Polynome 
    """
    #
    def __init__(self, input):
        """
        usage: pol = Poly1D("1 + 2*X - 3*X^2")
       
        private constructor : pol = Poly1D(polynome.data) -> copy
        """
        if ( isinstance(input, str) ):
            self.data = parse_string_for_polynome1D(input)  # data = [ Monome1 ,Monome2 , ... ]
        elif ( isinstance(input, list) ):
            self.data = copy(input)
        else :
            print "__init__ : input =", input
            raise TypeError, "bad init"
        #
        #print self.data
        #
        self.simplify_list_of_monomes()
        #
    #
    #
    #
    def __str__(self):
        """
        to output  a poly
        """
        str = ''
        #
        if ( len(self.data) == 0 ):
            str = " 0"
        else:
            for i in range(len(self.data)):
                str += self.data[i].array_to_string()
        #
        return str
    #
    #
    def __repr__(self):
        """
        to output  a poly
        """
        str = ''
        #
        if ( len(self.data) == 0 ):
            str = " 0"
        else:
            for i in range(len(self.data)):
                str += self.data[i].array_to_string()
        #
        return str
    #
    #
    #
    def simplify_list_of_monomes ( self  ):
        """
        remove or concatenate similar monomes
        """
        data = []
        #
        for i in range (len(self.data)):
        #
            #
            deg   = self.data[i].deg
            coeff = self.data[i].coeff
            #
            is_same_monome = False
            #
            for j in range(len(data)):
                #
                other_deg = data[j].deg
                if ( deg == other_deg ):
                    data[j].coeff +=  coeff
                    is_same_monome = True
                    #
                    # remove the monome ?
                    if ( data[j].coeff == 0.0 ):
                        del data[j]
                    #
                    break
                #
            #
            if ( is_same_monome == False ):
                data.append( copy(self.data[i]) )
            #
        #
        # sort
        data.sort(sort_fun)
        #
        self.data = data;
    #
    #
    # Operations
    #
    def __mul__(self, other):
        """
        return the multiplication of two polynomes
        usage: res = a * b
        """
        data = []
        #
        for i in range(len(self.data)):
            for j in range(len(other.data)):
                #
                deg   = self.data[i].deg + other.data[j].deg # the degree
                coeff = self.data[i].coeff * other.data[j].coeff # the coeff
                #
                data.append( Monome1D([deg, coeff]) )
        #
        return  Poly1D(data)
        #
    #
    #
    def __add__(self, other):
        """
        return the summ of two polynomes
        usage: res = a + b
        """
        data = []
        #
        for i in range(len(self.data)):
            data.append( self.data[i] )
        #
        for i in range(len(other.data)):
            data.append( other.data[i] )
        #
        return Poly1D(data)
        #
    #
    #
    def __sub__(self, other):
        """
        return the difference between two polynomes 
        usage: res = a - b
        """
        data = []
        #
        for i in range(len(self.data)):
            data.append( self.data[i] )
        #
        for i in range(len(other.data)):
            data_tmp = copy(other.data[i])
            data_tmp.coeff *= -1
            data.append( data_tmp )
        #
        return  Poly1D(data)
        #
    #
    #
    #
    def __pos__(self):
        """
        return a copy of the polynome ( not a reference )
        """
        data = copy(self.data)
        #
        return Poly1D(data)
        #
    #
    #
    def __neg__(self):
        """
        return   - polynome  (minus polynome) ( not a reference )
        """
        data = copy(self.data)
        #
        for i in range(len(self.data)):
            data[i].coeff *= -1
        #
        return Poly1D(data)
    #
    #
    #
    def integr_01(self):
        """
        integrate a polynome on 0:1
        """
        val = 0.0
        #
        for i in range(len(self.data)):
        #
            #
            deg   = self.data[i].deg
            coeff = self.data[i].coeff
            #
            num   = fact(deg)
            denom = fact(deg + 1)
            #
            val_tmp = ( 1.0*coeff*num ) / denom
            #
            val += val_tmp
        #
        return val
    #
    #
    #
    def diff(self):
        """
        differentiate a polynome
        """
        data = []
        #
        for i in range(len(self.data)):
            #
            deg   = copy(self.data[i].deg)
            coeff = copy(self.data[i].coeff)
            #
            if ( deg > 0 ):
                coeff *= deg
                deg   -= 1
                #
                data.append( Monome1D([deg, coeff]) )
        #
        print data
        return Poly1D(data)
        #
    #
    #
