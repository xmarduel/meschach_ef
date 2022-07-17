%-------------------------------- 
% MATLAB visualisation ('WORK.m') 
%-------------------------------- 
% 
load coordinates.dat; coordinates(:,1)=[]; 
load solution.dat;  solution(:,1)=[]; 
load elements3.dat; elements3(:,1)=[]; 
% 
figure(1) 
set(1,'Position',[750 450 500 500]) 
trisurf(elements3,coordinates(:,1),coordinates(:,2),solution,... 
        'facecolor','interp','LineStyle','none'); 
hold on;           
view(10,40);       
rotate3d;          
% 
