//
clear all
//
// Beispiel in 2D :
// Kinematic mit 5 rotatorische Gelenke
//
// Laenge von den Axis
//
L = [ 1 ; 2 ; 2 ; 1 ; 1 ];
//
// Fuer mich,  x sind die Gelenke
//
function pt = ROTAT(x)
pt = [ [ cos(x) , -sin(x) , 0 ] ;
       [ sin(x) ,  cos(x) , 0 ] ;
       [ 0      ,   0     , 1 ] ];
return pt;
//
function pt = TRANS(x)
pt = [ [ 1 , 0 , x ] ;
       [ 0 , 1 , 0 ] ;
       [ 0 , 0 , 1 ] ];
return pt;
//
// 
function pt = D_ROTAT(x)
pt = [ [ -sin(x) , -cos(x) , 0 ] ;
       [  cos(x) , -sin(x) , 0 ] ;
       [ 0      ,   0      , 0 ] ];
return pt;
//
function pt = D_TRANS(x)
pt = [ [ 0 , 0 , 1 ] ;
       [ 0 , 0 , 0 ] ;
       [ 0 , 0 , 0 ] ];
return pt;
//
//
//
function pt = GET_TRANSLATION_PART(f)
pt = [ f(1,3) ; f(2,3) ];
//
// x ist der vektor von Gelenke
function pt = GET_END_EFFECTOR(x)
pt = GET_TRANSLATION_PART(ROTAT(x(1))*TRANS(L(1))* ...
                          ROTAT(x(2))*TRANS(L(2))* ...
                          ROTAT(x(3))*TRANS(L(3))* ...
                          ROTAT(x(4))*TRANS(L(4))* ...
                          ROTAT(x(5))*TRANS(L(5)) );
return pt
// 
function pt = GET_END_EFFECTOR_TR(x)
pt = GET_END_EFFECTOR(x);
return pt'
//
//
function pt = D_GET_END_EFFECTOR(x)
pt = [ GET_TRANSLATION_PART(D_ROTAT(x(1))*TRANS(L(1))* ...
                              ROTAT(x(2))*TRANS(L(2))* ...
                              ROTAT(x(3))*TRANS(L(3))* ...
                              ROTAT(x(4))*TRANS(L(4))* ...
                              ROTAT(x(5))*TRANS(L(5)) ); ...

       GET_TRANSLATION_PART(  ROTAT(x(1))*TRANS(L(1))* ...
                            D_ROTAT(x(2))*TRANS(L(2))* ...
                              ROTAT(x(3))*TRANS(L(3))* ...
                              ROTAT(x(4))*TRANS(L(4))* ...
                              ROTAT(x(5))*TRANS(L(5)) ); ...

       GET_TRANSLATION_PART(  ROTAT(x(1))*TRANS(L(1))* ...
                              ROTAT(x(2))*TRANS(L(2))* ...
                            D_ROTAT(x(3))*TRANS(L(3))* ...
                              ROTAT(x(4))*TRANS(L(4))* ...
                              ROTAT(x(5))*TRANS(L(5)) ); ...

       GET_TRANSLATION_PART(  ROTAT(x(1))*TRANS(L(1))* ...
                              ROTAT(x(2))*TRANS(L(2))* ...
                              ROTAT(x(3))*TRANS(L(3))* ...
                            D_ROTAT(x(4))*TRANS(L(4))* ...
                              ROTAT(x(5))*TRANS(L(5)) ); ...

       GET_TRANSLATION_PART(  ROTAT(x(1))*TRANS(L(1))* ...
                              ROTAT(x(2))*TRANS(L(2))* ...
                              ROTAT(x(3))*TRANS(L(3))* ...
                              ROTAT(x(4))*TRANS(L(4))* ...
                            D_ROTAT(x(5))*TRANS(L(5)) ) ];
return pt
//
//
//
// Target
target = [ 3 ; 2 ]
target_TR = tg'
//
// Ini Gelenke
x_ini = [0 ; 0; 0; 0; 0]
// Ini pos
p_ini = GET_END_EFFECTOR(j_ini);
//
//
// x_target : die Gelenke so dass der end-effector ist am target
//
// ich habe eine bissen vergessen die argumente ... aber  ist gegeben f und Df als vor-letzte und letzte argumente
[ cost, x_tg ] = optim( p_ini, ????xxxxxx??? , 'qn' , '0.5*norm(target-GET_END_EFFECTOR(x))','(target_TR-GET_END_EFFECTOR_TR(x))*D_GET_END_EFFECTOR(x)' )
//
// cost ist einfach f(x_target) = 0.5*norm(target - GET_END_EFFECTOR(x_target)) : sollte sehr klein sein
//
// Man kann die constraints einfugen in den argumenten (see man page)
//
//
// plot
//
//
function pt = GET_EFFECTOR_FRAME(x)
eff_frame = INIT_FRAME();
for (i=1; i<x.length(); i++)
   eff_frame = ini_frame * ( ROTAT(x(i))*TRANS(L(i)) );
end
pt = GET_TRANSLATION_PART(eff_frame)
return pt
//
//
//
X1 = GET_END_EFFECTOR_1(x_tg(1:1));
X2 = GET_END_EFFECTOR_2(x_tg(1:2));
X3 = GET_END_EFFECTOR_3(x_tg(1:3));
X4 = GET_END_EFFECTOR_4(x_tg(1:4));
X5 = GET_END_EFFECTOR_5(x_tg(1:5));
//
//
Kette_X = [ p_ini(1) , X1(1) , X2(1) , X3(1) , X4(1), X5(1) ];
Kette_Y = [ p_ini(2) , X1(2) , X2(2) , X3(2) , X4(2), X5(2) ];
//
// ich habe ein bissen vergessen die argumente fuer plotting
plot2D( Kette_X , Kette_Y )
xgrid(1)
//