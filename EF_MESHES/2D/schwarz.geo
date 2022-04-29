lc = 0.111;

//
//    
//  6 *-----------* 5
//    |           |
//  7 *           * 4
//    |         /  
//    |       /    
//    |     /     
//    |    |      # 9
//    |     \     
//    |       \    
//    |         \   
//    |           * 3
//    |           |
//    *-----------*
//    1            2
//
//
//
//

Point(9) = {5, 5, 0, lc};

Point(1) = {0, 0, 0, lc};
Point(2) = {5, 0, 0, lc};
Point(3) = {5, 2, 0, lc};
Point(4) = {5, 8, 0, lc};
Point(5) = {5,10, 0, lc};
Point(6) = {0,10, 0, lc};
Point(7) = {0, 8, 0, lc};

Line(1) = {1,2};
Line(2) = {2,3};
Circle(9) = {4, 9, 3};
Line(3) = {4,5};
Line(4) = {5,6};
Line(5) = {6,7};
Line(6) = {7,1};

Line Loop(5) = {1,2,-9,3,4,5,6};

Plane Surface(6) = {5};

Physical Line(1) = {5}; // u = 0
Physical Line(2) = {1,2,3,4,6}; // du/dn = 0
Physical Line(3) = {9}; // du/dn + 2u = 0

Physical Surface(0) = {6};