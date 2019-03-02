outer_dimensions = [200, 120, 100];
min_height = 40;
corner_radius = 5;
mode_button_x = [45, 155];
mode_button_y = 90;
mode_button_d = 24.5;
mode_button_kd = 3.5;
mode_button_ks = 40;
scene_button_x = [30, 65, 100, 135, 170];
scene_button_y = 35;
scene_button_d = 24.5;
led_hole_dia = 8;
led_hole_pos = [100, 80];
led_height = 10;
wall_thickness = 3;
screw_hole_diameter = 4.5;
screw_hole_distance = 10;
screw_nut_diameter = 10;
screw_nut_sides = 4;
skew = 15;
cable_hole_diameter = 5;
cable_hole_distance = 65;
version = "OBS CONTROL BOX BY RIDER.CZ v. 1.5";

fudge = 1;
$fn = 32;

inner_dimensions = [
    outer_dimensions[0] - 2 * wall_thickness, 
    outer_dimensions[1] - 2 * wall_thickness, 
    outer_dimensions[2] - wall_thickness + fudge
];

difference() {
    // Main shape
    union() {
        // Basic box
        difference() {
            rcube(outer_dimensions, corner_radius);
            translate([wall_thickness, wall_thickness, wall_thickness]) rcube(inner_dimensions, corner_radius - wall_thickness);
        }

        // LED shield
        translate(led_hole_pos) cylinder(d = led_hole_dia + 2 * wall_thickness, h = led_height);

        // Screw pillars
        for(x = [0, outer_dimensions[0] - screw_hole_distance * 2], y  = [0, outer_dimensions[1] - screw_hole_distance * 2]) translate([x, y]) rcube([screw_hole_distance * 2, screw_hole_distance * 2, outer_dimensions[2]], corner_radius);

        // Version
        translate([outer_dimensions[0] / 2, outer_dimensions[1] / 2, wall_thickness]) linear_extrude(.6) 
            text(text = version, font = "Arial:bold", size = 7.2, halign = "center", valign = "center");
    }

    // Screw holes
    for(x = [0, outer_dimensions[0] - screw_hole_distance * 2], y  = [0, outer_dimensions[1] - screw_hole_distance * 2]) translate([x, y]) {
        translate([screw_hole_distance, screw_hole_distance, -fudge]) cylinder(d = screw_hole_diameter, h = outer_dimensions[1] + 2 * fudge);
        translate([screw_hole_distance, screw_hole_distance, wall_thickness * 3]) rotate(180 / screw_nut_sides) cylinder(d = screw_nut_diameter, h = outer_dimensions[1], $fn = screw_nut_sides);
    }

    // Panel holes
    hh = wall_thickness + 2 * fudge;
    translate([0, 0, -fudge]) {
        // LED
        translate(led_hole_pos) cylinder(d = led_hole_dia, h = led_height + 2 * fudge);

        // Scene buttons
        for(x = scene_button_x) {
            translate([x, scene_button_y]) cylinder(d = scene_button_d, h = hh);
        }

        // Mode button
        for(x = mode_button_x) {
            translate([x, mode_button_y]) {
                cylinder(d = mode_button_d, h = hh);
                translate([-mode_button_ks / 2, 0]) cylinder(d = mode_button_kd, h = hh);
                translate([+mode_button_ks / 2, 0]) cylinder(d = mode_button_kd, h = hh);
            }
        }
    }

    // General skew
    translate([-fudge, -fudge, min_height]) rotate([skew, 0, 0]) cube(max(outer_dimensions) + 2 * fudge);

    // Cable hole
    translate([outer_dimensions[0] / 2, outer_dimensions[1] - wall_thickness - fudge, cable_hole_distance]) {
        rotate([-90, 0,0]) cylinder(d = cable_hole_diameter, h = wall_thickness + 2 * fudge);
        translate([-cable_hole_diameter / 2, 0]) cube([cable_hole_diameter, wall_thickness + 2 * fudge, outer_dimensions[2]]);
    }
}

module rcube(size, radius) {
    if(len(radius) == undef) {
        // The same radius on all corners
        rcube(size, [radius, radius, radius, radius]);
    } else if(len(radius) == 2) {
        // Different radii on top and bottom
        rcube(size, [radius[0], radius[0], radius[1], radius[1]]);
    } else if(len(radius) == 4) {
        // Different radius on different corners
        hull() {
            // BL
            if(radius[0] == 0) cube([1, 1, size[2]]);
            else translate([radius[0], radius[0]]) cylinder(r = radius[0], h = size[2]);
            // BR
            if(radius[1] == 0) translate([size[0] - 1, 0]) cube([1, 1, size[2]]);
            else translate([size[0] - radius[1], radius[1]]) cylinder(r = radius[1], h = size[2]);
            // TR
            if(radius[2] == 0) translate([size[0] - 1, size[1] - 1])cube([1, 1, size[2]]);
            else translate([size[0] - radius[2], size[1] - radius[2]]) cylinder(r = radius[2], h = size[2]);
            // TL
            if(radius[3] == 0) translate([0, size[1] - 1]) cube([1, 1, size[2]]);
            else translate([radius[3], size[1] - radius[3]]) cylinder(r = radius[3], h = size[2]);
        }
    } else {
        echo("ERROR: Incorrect length of 'radius' parameter. Expecting integer or vector with length 2 or 4.");
    }
}