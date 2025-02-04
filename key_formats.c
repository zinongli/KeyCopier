#include "key_formats.h"
// all lengths in inches since it's all American formats
// angle is in degrees
const KeyFormat all_formats[] = {
    {.manufacturer = "Kwikset",
     .format_name = "KW1",
     .format_link = "https://lsamichigan.org/Tech/Kwikset_KeySpecs.pdf", // C31
     .first_pin_inch = 0.247,
     .last_pin_inch = 0.847,
     .pin_increment_inch = 0.15,
     .pin_num = 5,
     .pin_width_inch = 0.084,
     .elbow_inch = 0.15,
     .drill_angle = 90,
     .uncut_depth_inch = 0.329,
     .deepest_depth_inch = 0.191,
     .depth_step_inch = 0.023,
     .min_depth_ind = 1,
     .max_depth_ind = 7,
     .macs = 4,
     .clearance = 3},

    {.manufacturer = "Schlage",
     .format_name = "SC4",
     .format_link = "https://lsamichigan.org/Tech/SCHLAGE_KeySpecs.pdf", // C45
     .first_pin_inch = 0.231,
     .last_pin_inch = 1.012,
     .pin_increment_inch = 0.1562,
     .pin_num = 6,
     .pin_width_inch = 0.031,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.335,
     .deepest_depth_inch = 0.2,
     .depth_step_inch = 0.015,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 8},

    {.manufacturer = "Weiser",
     .format_name = "WR5",
     .format_link = "C50",
     .first_pin_inch = 0.237,
     .last_pin_inch = 0.861,
     .pin_increment_inch = 0.156,
     .pin_num = 5,
     .pin_width_inch = 0.079,
     .elbow_inch = 0.15,
     .drill_angle = 90,
     .uncut_depth_inch = 0.315,
     .deepest_depth_inch = 0.153,
     .depth_step_inch = 0.018,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 3},

    {.manufacturer = "Arrow",
     .format_name = "AR4",
     .format_link = "C2",
     .first_pin_inch = 0.265,
     .last_pin_inch = 1.040,
     .pin_increment_inch = 0.155,
     .pin_num = 6,
     .pin_width_inch = 0.060,
     .elbow_inch = 0.1,
     .drill_angle = 90, // This should actually be 100 but the current resolution will
     // make 100 degrees very ugly and unsuable
     .uncut_depth_inch = 0.312,
     .deepest_depth_inch = 0.186,
     .depth_step_inch = 0.014,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 7},

    {.manufacturer = "Elzett",
     .format_name = "EL10",
     .format_link = "2274",
     .first_pin_inch = 0.206,
     .last_pin_inch = 0.836,
     .pin_increment_inch = 0.158,
     .pin_num = 5,
     .pin_width_inch = 0.051,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.339,
     .deepest_depth_inch = 0.173,
     .depth_step_inch = 0.024,
     .min_depth_ind = 0,
     .max_depth_ind = 7,
     .macs = 6,
     .clearance = 3},

    {.manufacturer = "Abus",
     .format_name = "AB3",
     .format_link = "C112",
     .first_pin_inch = 0.157,
     .last_pin_inch = 0.768,
     .pin_increment_inch = 0.122,
     .pin_num = 6,
     .pin_width_inch = 0.035,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     .uncut_depth_inch = 0.254,
     .deepest_depth_inch = 0.152,
     .depth_step_inch = 0.025,
     .min_depth_ind = 1,
     .max_depth_ind = 5,
     .macs = 5,
     .clearance = 3},

    {.manufacturer = "Master Lock",
     .format_name = "M1",
     .format_link = "C35",
     .first_pin_inch = 0.185,
     .last_pin_inch = 0.689,
     .pin_increment_inch = 0.126,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     .uncut_depth_inch = 0.276,
     .deepest_depth_inch = 0.171,
     .depth_step_inch = 0.015,
     .min_depth_ind = 0,
     .max_depth_ind = 7,
     .macs = 7,
     .clearance = 6},

    {.manufacturer = "Master Lock",
     .format_name = "M2",
     .format_link = "C34",
     .first_pin_inch = 0.136,
     .last_pin_inch = 0.508,
     .pin_increment_inch = 0.124,
     .pin_num = 4,
     .pin_width_inch = 0.031,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     .uncut_depth_inch = 0.210,
     .deepest_depth_inch = 0.120,
     .depth_step_inch = 0.015,
     .min_depth_ind = 0,
     .max_depth_ind = 6,
     .macs = 5,
     .clearance = 5},

    {.manufacturer = "American Lock",
     .format_name = "AM7",
     .format_link = "C80",
     .first_pin_inch = 0.157,
     .last_pin_inch = 0.781,
     .pin_increment_inch = 0.125,
     .pin_num = 6,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     .uncut_depth_inch = 0.283,
     .deepest_depth_inch = 0.173,
     .depth_step_inch = 0.016,
     .min_depth_ind = 1,
     .max_depth_ind = 8,
     .macs = 7,
     .clearance = 5},

    {.manufacturer = "Yale",
     .format_name = "Y2",
     .format_link = "C57",
     .first_pin_inch = 0.200,
     .last_pin_inch = 1.025,
     .pin_increment_inch = 0.165,
     .pin_num = 6,
     .pin_width_inch = 0.054,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.320,
     .deepest_depth_inch = 0.149,
     .depth_step_inch = 0.019,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 9,
     .clearance = 4},

    {.manufacturer = "Yale",
     .format_name = "Y11",
     .format_link = "C55",
     .first_pin_inch = 0.124,
     .last_pin_inch = 0.502,
     .pin_increment_inch = 0.095,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.246,
     .deepest_depth_inch = 0.167,
     .depth_step_inch = 0.020,
     .min_depth_ind = 1,
     .max_depth_ind = 5,
     .macs = 5,
     .clearance = 3},

    {.manufacturer = "Sargent",
     .format_name = "S22",
     .format_link = "C44",
     .first_pin_inch = 0.216,
     .last_pin_inch = 0.996,
     .pin_increment_inch = 0.156,
     .pin_num = 6,
     .pin_width_inch = 0.063,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.328, // double check
     .deepest_depth_inch = 0.148,
     .depth_step_inch = 0.020,
     .min_depth_ind = 1,
     .max_depth_ind = 10,
     .macs = 7,
     .clearance = 5},

    {.manufacturer = "National",
     .format_name = "RO3",
     .format_link = "C37",
     .first_pin_inch = 0.156,
     .last_pin_inch = 0.525,
     .pin_increment_inch = 0.093,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.250,
     .deepest_depth_inch = 0.175,
     .depth_step_inch = 0.025,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 5,
     .clearance = 1},

    {.manufacturer = "National",
     .format_name = "NA25",
     .format_link = "C40",
     .first_pin_inch = 0.250,
     .last_pin_inch = 0.874,
     .pin_increment_inch = 0.156,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.304,
     .deepest_depth_inch = 0.191,
     .depth_step_inch = 0.012,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 8},

    {.manufacturer = "Corbin",
     .format_name = "CO88",
     .format_link = "C14",
     .first_pin_inch = 0.250,
     .last_pin_inch = 1.030,
     .pin_increment_inch = 0.156,
     .pin_num = 6,
     .pin_width_inch = 0.047,
     .elbow_inch = 0.1,
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.343,
     .deepest_depth_inch = 0.217,
     .depth_step_inch = 0.014,
     .min_depth_ind = 1,
     .max_depth_ind = 10,
     .macs = 7,
     .clearance = 8},

    {.manufacturer = "Lockwood",
     .format_name = "L1",
     .format_link = "C28",
     .first_pin_inch = 0.276,
     .last_pin_inch = 0.902,
     .pin_increment_inch = 0.153,
     .pin_num = 5,
     .pin_width_inch = 0.063,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.320,
     .deepest_depth_inch = 0.158,
     .depth_step_inch = 0.018,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 9,
     .clearance = 4},

    {.manufacturer = "Lockwood",
     .format_name = "LW4",
     .format_link = "CX57",
     .first_pin_inch = 0.240,
     .last_pin_inch = 0.865,
     .pin_increment_inch = 0.156,
     .pin_num = 5,
     .pin_width_inch = 0.047,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.337,
     .deepest_depth_inch = 0.202,
     .depth_step_inch = 0.015,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 9,
     .clearance = 8},

    {.manufacturer = "Lockwood",
     .format_name = "LW5",
     .format_link = "CX57",
     .first_pin_inch = 0.240,
     .last_pin_inch = 1.0222,
     .pin_increment_inch = 0.1562,
     .pin_num = 6,
     .pin_width_inch = 0.031,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.337,
     .deepest_depth_inch = 0.202,
     .depth_step_inch = 0.015,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 9,
     .clearance = 8},

    {.manufacturer = "National",
     .format_name = "NA12",
     .format_link = "C39",
     .first_pin_inch = 0.150,
     .last_pin_inch = 0.710,
     .pin_increment_inch = 0.140,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.270,
     .deepest_depth_inch = 0.157,
     .depth_step_inch = 0.013,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 8},

    {.manufacturer = "Russwin",
     .format_name = "RU45",
     .format_link = "CX6",
     .first_pin_inch = 0.250,
     .last_pin_inch = 1.030,
     .pin_increment_inch = 0.156,
     .pin_num = 6,
     .pin_width_inch = 0.053,
     .elbow_inch = 0.1,
     .drill_angle = 90,
     .uncut_depth_inch = 0.343,
     .deepest_depth_inch = 0.203,
     .depth_step_inch = 0.028,
     .min_depth_ind = 1,
     .max_depth_ind = 6,
     .macs = 5,
     .clearance = 3},

    {.manufacturer = "Ford",
     .format_name = "H75",
     .sides = 2,
     .stop = 2,
     .format_link = "CX101",
     .first_pin_inch = 0.201,
     .last_pin_inch = 0.845,
     .pin_increment_inch = 0.092,
     .pin_num = 8,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.201, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.354,
     .deepest_depth_inch = 0.254,
     .depth_step_inch = 0.025,
     .min_depth_ind = 1,
     .max_depth_ind = 5,
     .macs = 5,
     .clearance = 2},

    {.manufacturer = "Chevrolet",
     .format_name = "B102",
     .sides = 2,
     .stop = 2,
     .format_link = "CF215",
     .first_pin_inch = 0.205,
     .last_pin_inch = 1.037,
     .pin_increment_inch = 0.093,
     .pin_num = 10,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.205, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.315,
     .deepest_depth_inch = 0.238,
     .depth_step_inch = 0.026,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 5,
     .clearance = 2},

    {.manufacturer = "Dodge",
     .format_name = "Y159",
     .sides = 2,
     .stop = 2,
     .format_link = "CX102",
     .first_pin_inch = 0.297,
     .last_pin_inch = 0.941,
     .pin_increment_inch = 0.092,
     .pin_num = 8,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.297, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.339,
     .deepest_depth_inch = 0.268,
     .depth_step_inch = 0.024,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 4,
     .clearance = 2},

    {.manufacturer = "Nissan",
     .format_name = "DA34",
     .sides = 2,
     .stop = 2,
     .format_link = "CF304",
     .first_pin_inch = 0.169,
     .last_pin_inch = 0.913,
     .pin_increment_inch = 0.082,
     .pin_num = 10,
     .pin_width_inch = 0.031,
     .elbow_inch = 0.169, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.334,
     .deepest_depth_inch = 0.257,
     .depth_step_inch = 0.026,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 4,
     .clearance = 2},

    {.manufacturer = "KIA",
     .format_name = "KK8",
     .sides = 2,
     .stop = 2,
     .format_link = "3003",
     .first_pin_inch = 0.217,
     .last_pin_inch = 0.795,
     .pin_increment_inch = 0.082,
     .pin_num = 8,
     .pin_width_inch = 0.031,
     .elbow_inch = 0.217, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.331,
     .deepest_depth_inch = 0.252,
     .depth_step_inch = 0.020,
     .min_depth_ind = 1,
     .max_depth_ind = 5,
     .macs = 5,
     .clearance = 3},

    {.manufacturer = "Kawasaki",
     .format_name = "KA14",
     .sides = 2,
     .format_link = "CMC50",
     .first_pin_inch = 0.098,
     .last_pin_inch = 0.591,
     .pin_increment_inch = 0.098,
     .pin_num = 6,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.258,
     .deepest_depth_inch = 0.198,
     .depth_step_inch = 0.020,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 4,
     .clearance = 3},

    {.manufacturer = "Yamaha",
     .format_name = "YM63",
     .sides = 2,
     .format_link = "CMC71",
     .first_pin_inch = 0.157,
     .last_pin_inch = 0.748,
     .pin_increment_inch = 0.098,
     .pin_num = 7,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.1, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.295,
     .deepest_depth_inch = 0.236,
     .depth_step_inch = 0.020,
     .min_depth_ind = 1,
     .max_depth_ind = 4,
     .macs = 4,
     .clearance = 3},

    {.manufacturer = "Best (A2)",
     .format_name = "SFIC",
     .stop = 2,
     .format_link = "C3",
     .first_pin_inch = 0.250,
     .last_pin_inch = 0.998,
     .pin_increment_inch = 0.149,
     .pin_num = 6,
     .pin_width_inch = 0.051,
     .elbow_inch = 0.081, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.318,
     .deepest_depth_inch = 0.206,
     .depth_step_inch = 0.013,
     .min_depth_ind = 0,
     .max_depth_ind = 9,
     .macs = 7,
     .clearance = 3},

    {.manufacturer = "RV (FIC,GL,Bauer)",
     .format_name = "RV",
     .sides = 2,
     .format_link = "4013",
     .first_pin_inch = 0.126,
     .last_pin_inch = 0.504,
     .pin_increment_inch = 0.094,
     .pin_num = 5,
     .pin_width_inch = 0.039,
     .elbow_inch = 0.126, // this should be equal to first pin inch for tip
     // stopped key line
     .drill_angle = 90, 
     
     .uncut_depth_inch = 0.272,
     .deepest_depth_inch = 0.224,
     .depth_step_inch = 0.024,
     .min_depth_ind = 1,
     .max_depth_ind = 3,
     .macs = 3,
     .clearance = 1}};
