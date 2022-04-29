%
clear all
%
N = 100
%
A = sparse(N,N);
%
D = sparse(1:N,1:N,   4,N,N);
E = sparse(2:N,1:N-1, 2,N,N);
%F = sparse(3:N,1:N-2, 1,N,N);
%
%A = D + E + E' + F + F' ;
A = D + E + E'  ;
%
%
T = sparse(2*N,2*N);
T(  1:N  ,  1:N  ) = A;
T(N+1:N+N,1+N:N+N) = A;
T(N+1:N+N,  1:N  ) = A;
T(  1:  N,N+1:N+N) = A;
%
T = T + 5*sparse(1:2*N,1:2*N,   4,2*N,2*N);
T = T + 5*sparse(2:2*N,1:2*N-1, 2,2*N,2*N);
T = T + 5*sparse(1:2*N-1,2:2*N, 2,2*N,2*N);
%
spy(T);
%
figure(1);
subplot(2,2,1)
spy(T); title('initial matrix');
%
p2 = symmmd(T); B = T(p2,p2);
%
subplot(2,2,2);
spy(B); title('with minimum degre'); 
%
%
p3 = symrcm(T); C = T(p3,p3);
%
subplot(2,2,4);
spy(C); title('with cuthill-mackee');
%
%
subplot(2,2,3);
spy(T(:,p3)); title('just with colperm');
%
%
% LU factorisation
%[L1,U1] = luinc(T,0.0);
%[L2,U2] = luinc(B,0.0);
%[L3,U3] = luinc(C,0.0);
[L1,U1] = lu(T);
[L2,U2] = lu(B);
[L3,U3] = lu(C);
%
%
figure(2)
subplot(2,2,1); spy(L1+U1); title('LU from initial matrix');
subplot(2,2,2); spy(L2+U2); title('LU from minimum degre'); 
subplot(2,2,4); spy(L3+U3); title('LU from cuthill-mackee');
%
b = (1:2*N)' ;
% solve with the reduced banded matrix
Pb = b(p3);
Psol = C\Pb;
%
% inverse permutation
for i=1:2*N
   inv_p3(p3(i)) = i
end
%%P3 = zeros(2*N,2*N);
%%for i=1:2*N
%%  P3(i,p3(i)) = 1;
%%end;
%%
%%INV_P3 = inv(P3);
%%
%%for i=1:2*N
%%for j=1:2*N
%%  if INV_P3(i,j) == 1 inv_p3(i)=j; end;
%%end;
%%end;
%
% Compare sol_b and sol ...
sol_b = Psol(inv_p3);
sol_a = T\b;
%
%
