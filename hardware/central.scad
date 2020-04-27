

$fa = 1;
$fs = 0.4;

module grup(){
    
    size_hole=2.75;
    
    translate([0,0,-10]){
        cylinder(h=30,r=size_hole);
        translate([30.5,0,0]){
            cylinder(h=30,r=size_hole);
        }
        translate([0,26,0]){
            cylinder(h=30,r=size_hole);
        }
        translate([30.5,26,0]){
            cylinder(h=30,r=size_hole);
        }
    }
    
}

module square_hole(){
    
    translate([0,0,-10]){
        
        cube([30,30,30]);
 
        
    }
    
}


difference(){
    cube([150,150,3]);
    translate([9.75,4,0]){
        grup();
    }

    translate([109.75,4,0]){
        grup();
    }
    
    translate([9.75,120,0]){
        grup();
    }
    
    translate([109.75,120,0]){
        grup();
    }
    
    
    translate([60,5,0]){
        square_hole();
    }
    
    translate([60,115,0]){
        square_hole();
    }
    
    translate([10,60,0]){
        square_hole();
    }
    
    translate([115,60,0]){
        square_hole();
    }
    

}