%
clear all
%
%
N=8;
%
D = sparse(1:N,1:N   , 2, N,N);
E = sparse(2:N,1:N-1 ,-1, N,N);
Z = sparse(1:1,N:N   ,-1, N,N);
%
MAT_PER = Z + E + D + E' + Z' ;
%
full(MAT_PER)
%
% [L,U] = lu(MAT_PER)
%
% b = zeros(N,1)
% b(N) = 1
%
% x = U\(L\b)
%
[U,S,V] = svd(full(MAT_PER));
d = diag(S)
%
rang = rank(full(MAT_PER))
pause
%
%
EYE = eye(N)
PERM(1:N,1:N) = 0;

%
for i=1:floor(N/2)
  PERM(:,2*i-1) = EYE(:,  i  ) ;
  PERM(:,2*i  ) = EYE(:,N-i+1) ;
end;
if (rem(N,2)~=0) PERM(:,N) = EYE(:,floor(N/2)+1) ; end;
%
PERM
%
TRANS = PERM'*MAT_PER*PERM
%
[L,U] = lu(TRANS)
%
b = ones(N,1)
x = U\(L\b)
%
M = chol(TRANS)
%
x = M\(M'\b)
%