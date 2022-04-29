//
clear all
//
N = 6
//
// A = 4*sparse(diag(ones(N,1)));
//
D = 4*sparse(diag(ones(N,1)));
E = (-1)*sparse(diag(1*ones(N/2,1), N/2  ));
//F = sparse(3:N,1:N-2, 1,N,N);
//
//A = D + E + E' + F + F' ;
A = D + E  ; // upper triangle !! (for scilab)
AA = D + E + E' ; // the true matrix
//
xset('window',0)
FA = full(A);
//hist3d((FA+tril(FA',-1)+tril(FA,-1)'),52,85);// show all
//
// BANDWIDTH REDUCTION FOR THE MATRIX
[iperm,mrepi,profile,ierr]=bandwr(A);
//max(profile(2:$)-profile(1:($-1)))
//
// NEW MATRIX
//n=g('node_number');
B=FA(mrepi,mrepi);
//xset('window',1)
//hist3d(B+tril(B',-1)+tril(B,-1)'),52,85);
//B
//
BB = AA(mrepi,mrepi); // the true permuted matrix
//
[L1,U1] = lu(full(AA))
[L2,U2] = lu(full(BB))
//
[M1,V1] = lufact(AA);  // lufact on sparse matrices performs a permutation
[M2,V2] = lufact(BB);  // lufact on sparse matrices performs a permutation

[N1,O1,P1,Q1] = luget(M1);
[N2,O2,P2,Q2] = luget(M2); // see Q2 the permutation ! Is mrepi !
//
b = ones(1:N)';
b(2) = 4;
b(3) = 2;
b
//
x1 = lusolve(AA,b)  // solution
//
Pb = b(mrepi)
Py = lusolve(BB,Pb)
//
x2 = Py(iperm)     // solution
//
/////////////////////////////////////////////////
// MATLAB :
// p3 = symrcm(T); C = T(p3,p3);
// "p3" is the equivalent of "mrepi" od SCILAB
/////////////////////////////////////////////////
//
// for non-symetrix systems NS = L+D+U:
// --> make a symetric syytem with all SS = L+D+U+D'+U'
// --> cal the permutation with  SSS = L'+U+D (upper matrix) 
////////////////////////////////////////////////////////////