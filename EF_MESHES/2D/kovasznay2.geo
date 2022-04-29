lc1 = 0.02;
lc2 = 0.05;

//    
//  4 *-----------* 3
//    |           |
//    |           |
//    |           |
//    *-----------*
//    1            2
//

Point(1) = {-0.5, -0.5, 0, lc1};
Point(2) = { 2.5, -0.5, 0, lc2};
Point(3) = { 2.5,  0.5, 0, lc2};
Point(4) = {-0.5,  0.5, 0, lc1};


Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line Loop(5) = {1,2,3,4};

Plane Surface(6) = {5};

Physical Line(1) = {4}; // u,v input
Physical Line(2) = {2}; // u,v output
Physical Line(3) = {1,3}; // v = 0, u != 0

Physical Surface(0) = {6};