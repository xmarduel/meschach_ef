//
clear all
//
N = 5
//
//
D = 6*sparse(diag(ones(N,1)));
E = -1*sparse(diag(1*ones(N-1,1), 1  ));
//F = sparse(3:N,1:N-2, 1,N,N);
//
//A = D + E + E' + F + F' ;
A = D + E + E'  ;

//
//X = sparse(2*N,2*N);
X(  1:N  ,  1:N  ) = A;
X(N+1:N+N,1+N:N+N) = A;
X(N+1:N+N,  1:N  ) = A;
X(  1:  N,N+1:N+N) = A;
//
UPPER = X - tril(X,-1) ;
LOWER = X - tril(X,-1)';
X = sparse(UPPER);
//
FX = full(X);
FX = FX+tril(FX',-1)+tril(FX,-1)
//xset('window',0);
//hist3d(FX,52,85);// show all
//
// BANDWIDTH REDUCTION FOR THE MATRIX
[iperm,mrepi,profile,ierr]=bandwr(X,0);
//max(profile(2:$)-profile(1:($-1)))
//
//
FX2 = FX(mrepi,mrepi);
//xset('window',1);
//hist3d(FX2,52,85);// show all
//
FX2