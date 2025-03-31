echo off
if not exist fex1.f goto 1
rename fex1.f fex1.for
:1
if not exist fex2.f goto 2
rename fex2.f fex2.for
:2
if not exist fex3.f goto 3
rename fex3.f fex3.for
:3
