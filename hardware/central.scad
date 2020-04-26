

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


difference(){
    cube([150,180,6]);
    translate([9.75,4,0]){
        grup();
    }

    translate([109.75,4,0]){
        grup();
    }
    
    translate([9.75,150,0]){
        grup();
    }
    
    translate([109.75,150,0]){
        grup();
    }

}