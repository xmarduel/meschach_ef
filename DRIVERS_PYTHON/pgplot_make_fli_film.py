#! /usr/bin/env python
#
# create an animation "pgplot.fli"  from a list of file pgplot<n>.ppm 
#                                                       pgplot_gif.<n>
#
import os
import sys


class film_maker:
    """
    """
    def make(self):
        self.build_files_list()
        self.build_film()

    def build_film(self):
        """ """
        os.remove("pgplot.list")
        os.system("ls -1 pgplot*." + self.format + " > pgplot.list")
        os.remove("pgplot.fli")
        os.system("ppm2fli" + self. ppm2fli_filter + " -g " + self.im_size + " pgplot.list pgplot.fli")

    def build_files_list(self):
        """ """
        # first file
        os.rename("pgplot.%s" % self.format, "pgplot%05d.%s" % (1,self.format))
        # others files
        for i in range(3, int(self.nbfiles)+1):
            print(i)
            file1 = "pgplot%d.%s"   % (i,self.format)
            file2 = "pgplot%05d.%s" % (i,self.format)
            
            os.rename(file1, file2)
     

class film_maker_ppm_pict(film_maker):
    """  """
    def __init__(self, nbfiles, im_size):
        self.format  = "ppm"
        self.nbfiles = nbfiles
        self.im_size = im_size
        self.ppm2fli_filter = ""
    

class film_maker_gif_pict(film_maker):
    """  """
    def __init__(self, nbfiles, im_size):
        self.format  = "gif"
        self.nbfiles = nbfiles
        self.im_size = im_size
        self.ppm2fli_filter = " -f giftopnm "


def get_film_maker(nb_files, format, im_size):
    """ factory function """
    if ( format == "PPM" ): return film_maker_ppm_pict(nb_files, im_size)
    if ( format == "GIF" ): return film_maker_gif_pict(nb_files, im_size)

    print("bad picture format (PPM or GIF)")
    sys.exit(1)


def usage():
    """ """
    print("usage: make_film.py <nb_files> <pict_format>  <pict_size>")
    print("                    <nb_files>:	integer")
    print("                    <pict_format>:	\"PPM\" or \"GIF\"")
    print("                    <pict_size>:	dim1xdim2")
    sys.exit(1)


if __name__ == '__main__':
    #
    if ( len(sys.argv) != 4 ): usage()
    #
    nbfiles = sys.argv[1]
    format  = sys.argv[2]
    im_size = sys.argv[3]
    #
    film_maker = get_film_maker(nbfiles, format, im_size)
    film_maker.make()
