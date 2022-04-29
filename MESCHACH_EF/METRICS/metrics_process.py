#! /usr/bin/env python

from math   import *
from string import *

import sys
import os

# ------------------------------------------------------------------------

class item_cyclo:
    """
    a class to order items consisting of:
        1- function name 
        2- function cyclomatic complexity
        
    the __eq__, __gt__ and __lt__  serve to sort an list of items
    """
    def __init__(self, name, val):
	     """
		  """
        self.name = name
        self.val  = val

        self.filter_name()

    def __lt__(self, other):
        return self.val > other.val

    def __gt__(self, other):
        return self.val < other.val

    def __eq__(self, other):
        return self.val == other.val

    def __repr__(self):
        return "func name = %-50s -> %d" % (self.name, self.val)

    def __str__(self):
        return "%-50s -> %d" % (self.name, self.val)


    def filter_name(self):
        """
        """
        self.name = self.name.replace("static" , "") 
        self.name = self.name.replace("int"    , "")
        self.name = self.name.replace("Real"   , "")
		  self.name = self.name.replace("real"   , "")
        self.name = self.name.replace("double" , "")
        self.name = self.name.replace("void"   , "")

        # remove empty space at the beginning
        self.name = self.name.strip()

        # bug in cyclo : it can keep the returned type of the function in the function name ! -> remove it
        leer_char = self.name.find(" ") # name in 2 parts: "returned_type function"
        if ( leer_char > 0 and self.name != "Total complexity" ):
            self.name = self.name[leer_char:]
            self.name = self.name.strip()


def get_statistics(metrics):

    statistics = []

    for line in metrics :
        # read all the line of metrics, store instances of "item_cyclo" in a array 
        if ( len(line) > 1 ):
            # extract name of function and value
            k1 = line.find('\t') #  a line with \t is of the form : func_name  \t cyclomatic_number

            if ( k1 == -1 ): # exception : a line like "Total complexity:    34"
                k2 = line.find(':')
                func_name  = line[0:k2]
                func_cyclo = atoi(line[k2+1:-1]) # remove the \n at the end of the line
            else:
                func_name  = line[0:k1]
                func_cyclo = atoi(line[k1+1:-1])

            statistics.append( item_cyclo(func_name,func_cyclo) )
        else:
            pass # empty line
    
    return statistics


def get_statistics_ordered(statistics):

    statistics_ordered = []

    # remove the line with "Total_complexity"
    for k in range(len(statistics)) :
        if ( statistics[k].name == "Total complexity" ):
            continue
        statistics_ordered.append( statistics[k] )

    statistics_ordered.sort() # descending ordering

    return statistics_ordered

# -----------------------------------------------------------------------

if __name__ == '__main__':
   
    # call metrics.sh -> get all outputs in "metrics"

    USE_LC = False

    metrics = os.popen("sh metrics.sh")

    if USE_LC :
        stats   = os.popen("lc ../SRCS/*.c")
    else:
        stats  = os.popen("wc -l ../SRCS/*.c") # nb of lines only

    # put all the lines in the "metrics" text in our structure
    statistics         = get_statistics(metrics)
    statistics_ordered = get_statistics_ordered(statistics)

    # -----------------------------------------------
    #  CYCLOMATIC COMPLEXITY STATISTICS
    #
    # I- make output a little more beautiful

    f1 = open("metrics1.txt", "w+")

    f1.write("\n\t\t\t  CYCLOMATIC COMPLEXITY   \n\n")

    for k in range(len(statistics) ) :
        f1.write( str(statistics[k])  + '\n')
        if ( statistics[k].name == "Total complexity" ): f1.write('\n')

    f1.close()


    # II- now order the results

    f2 = open("metrics2.txt", "w+")

    f2.write("\n\t\t\t  CYCLOMATIC COMPLEXITY   \n\n")

    for k in range(len(statistics_ordered) ) :
        f2.write( str(statistics_ordered[k] ) + '\n' )

    f2.close()

    # -----------------------------------------------
    #  LINES STATISTICS
    #
    # I- make output a little more beautiful

    f4 = open("lines_stats.txt", "w")

    i = 0
    first_lines = []

    for line in stats :
        if USE_LC:
            i = i+1
            if ( i<4 ):
                line = "\t\t\t\t" + line
                f4.write(line)
                first_lines.append(line)
                continue

        func_name = line[:line.find("\t")]
        func_stat = line[line.find("\t"):]

        if ( func_name.startswith("ALL") ):  func_stat = func_stat[1:]

        new_item = "%-42s %s" % (func_name, func_stat) 

        f4.write(new_item)

    if USE_LC:
        # repeat the firsts lines
        for line in first_lines :
            f4.write(line)

    f4.close()

