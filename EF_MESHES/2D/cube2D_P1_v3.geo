lc1 = 0.05;
lc2 = 0.01;

//    
//  4 *-----------* 3
//    |           |
//    |           |
//    |           |
//    |           |
//    |           |
//    *-----------*
//    1            2
//

Point(1) = {0.0, 0.0, 0, lc1};
Point(2) = {1.0, 0.0, 0, lc1};
Point(3) = {1.0, 1.0, 0, lc2};
Point(4) = {0.0, 1.0, 0, lc1};


Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line Loop(5) = {1,2,3,4};

Plane Surface(6) = {5};

Physical Line(1) = {1};
Physical Line(2) = {2};
Physical Line(3) = {3};
Physical Line(4) = {4};

Physical Surface(0) = {6};
