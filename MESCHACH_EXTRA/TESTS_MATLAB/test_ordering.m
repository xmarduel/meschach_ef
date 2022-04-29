%
clear all
%
N = 10
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
T = sparse(2*N,2*N);
T(  1:N  ,  1:N  ) = A;
T(N+1:N+N,1+N:N+N) = A;
T(N+1:N+N,  1:N  ) = A;
T(  1:  N,N+1:N+N) = A;
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
%
% LU factorisation
%[L1,U1] = luinc(T,0.0);
%[L2,U2] = luinc(B,0.0);
%[L3,U3] = luinc(C,0.0);
%
%figure(2)
%subplot(2,2,1); spy(L1+U1);
%subplot(2,2,2); spy(L2+U2);
%subplot(2,2,4); spy(L3+U3);
