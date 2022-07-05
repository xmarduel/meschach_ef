
import sys

from meschach        import *
from meschach_lapack import *
from meschach_adds   import *



A = sp_get(5,5,3)
A = sp_tridiag(A, 1,4,3)

M = m_get(5,5)

sp_m2dense(A,M)

m_foutput(sys.stdout, M)


B = bd_get(5,1,1)
B = mat2band(M,1,1,B)
b_foutput(sys.stdout, B)

C = sp2band(A)
b_foutput(sys.stdout, C)

D = sp2lowerband(A)
b_foutput(sys.stdout, D)


"""
Ld /Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Products/Debug/liblibmeschach_threaded.dylib normal x86_64 (in target: libmeschach_threaded)
    cd /Users/xavier/DEVELOPMENT/MESCHACH_WORK
    export MACOSX_DEPLOYMENT_TARGET=10.13
    /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang -arch x86_64 -dynamiclib
       -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk
       -L/Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Products/Debug
       -L. -F/Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Products/Debug
       -filelist /Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Intermediates.noindex/MESCHACH_PROJECT.build/Debug/libmeschach_threaded.build/Objects-normal/x86_64/libmeschach_threaded.LinkFileList
       -exported_symbols_list MESCHACH_THREADED/libmeschach_threaded_export.txt
       -install_name /usr/local/lib/liblibmeschach_threaded.dylib
       -mmacosx-version-min=10.13
        -Xlinker
        -object_path_lto
        -Xlinker /Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Intermediates.noindex/MESCHACH_PROJECT.build/Debug/libmeschach_threaded.build/Objects-normal/x86_64/libmeschach_threaded_lto.o -Xlinker -export_dynamic -Xlinker -no_deduplicate
        -lmeschach
        -compatibility_version 1 -current_version 1
        -Xlinker -dependency_info
        -Xlinker /Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Intermediates.noindex/MESCHACH_PROJECT.build/Debug/libmeschach_threaded.build/Objects-normal/x86_64/libmeschach_threaded_dependency_info.dat
        -o /Users/xavier/Library/Developer/Xcode/DerivedData/MESCHACH_PROJECT-dzpravmpjydxhmfdlrtntildzgwc/Build/Products/Debug/liblibmeschach_threaded.dylib
"""