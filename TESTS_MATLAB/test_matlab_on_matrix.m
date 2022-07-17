%
% Pour comparer avec Meschach les produits A*A
%                                          A*A'
%                                          A'*A
%
A(9,9) = 0.0;
%
A1(3,3) = 0.0;
A2(3,3) = 0.0;
A3(3,3) = 0.0;
A4(3,3) = 0.0;
A5(3,3) = 0.0;
A6(3,3) = 0.0;
%
% Fill A1
A1(1,1)=1.0;
A1(1,2)=2.0;
A1(2,2)=3.0;
A1(2,3)=4.0;
A1(3,1)=5.0;
A1(3,3)=6.0;
%
% Fill A2
A2(1,1)=1.0;
A2(1,2)=2.0;
A2(1,3)=3.0;
A2(2,1)=4.0;
A2(2,3)=5.0;
A2(3,1)=6.0;
A2(3,3)=7.0;
%
%
 A1
 A2
%
% disp('A1^T ='); A1'
% disp('A2^T ='); A2'
%
%
disp('A1*A2 = '); A1*A2
%
disp('A1*A2^T = '); A1*(A2')
%
disp('A1^T*A2 = '); (A1')*A2
%
%
% Test FIll in A sub matrices
%
A(1:3,1:3) = A1;
A(1:3,4:6) = A1;
A(4:6,7:9) = A2;
A(7:9,1:3) = A2;
A(7:9,4:6) = A1;
A(7:9,7:9) = A2;
%
disp('A = '); A
%
%
% To compare with a test in LANGAGE_C/CONVDIFF_1D_MES
%