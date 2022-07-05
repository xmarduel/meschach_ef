
import sys

from meschach      import *
from meschach_adds import *


m1 = Mat(3,3)
m2 = Mat(3,3)
m3 = Mat(3,3)

m1.ones()
m2.ones()

m3 = m1 + m2
print(m3)

print("after m3 = m1 + m2 ...")
mem_info_file(sys.stdout,0)

m4 = Mat(3,3)
m4 = +(m1 + m2)
print m4

print("after m4 = +(m1 + m2) ...")
mem_info_file(sys.stdout,0)


m5 = m4
print m5

print("after m5 = m4 ...")
mem_info_file(sys.stdout,0)

