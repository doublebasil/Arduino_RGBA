#include "intcos.hpp"
static const uint8_t intcos_len = 181;
static const uint8_t intcos_data[181] = {
    255, 255, 255, 255, 
    255, 255, 254, 254, 
    254, 253, 253, 253, 
    252, 252, 251, 251, 
    250, 249, 249, 248, 
    247, 247, 246, 245, 
    244, 243, 242, 241, 
    240, 239, 238, 237, 
    236, 234, 233, 232, 
    231, 229, 228, 227, 
    225, 224, 222, 221, 
    219, 218, 216, 214, 
    213, 211, 209, 208, 
    206, 204, 202, 201, 
    199, 197, 195, 193, 
    191, 189, 187, 185, 
    183, 181, 179, 177, 
    175, 173, 171, 169, 
    167, 165, 163, 160, 
    158, 156, 154, 152, 
    150, 147, 145, 143, 
    141, 139, 136, 134, 
    132, 130, 128, 125, 
    123, 121, 119, 116, 
    114, 112, 110, 108, 
    105, 103, 101, 99, 
    97, 95, 92, 90, 
    88, 86, 84, 82, 
    80, 78, 76, 74, 
    72, 70, 68, 66, 
    64, 62, 60, 58, 
    56, 54, 53, 51, 
    49, 47, 46, 44, 
    42, 41, 39, 37, 
    36, 34, 33, 31, 
    30, 28, 27, 26, 
    24, 23, 22, 21, 
    19, 18, 17, 16, 
    15, 14, 13, 12, 
    11, 10, 9, 8, 
    8, 7, 6, 6, 
    5, 4, 4, 3, 
    3, 2, 2, 2, 
    1, 1, 1, 0, 
    0, 0, 0, 0, 
    0, };
uint8_t intcos(uint16_t degrees)
{
    uint16_t degrees_copy = degrees % 360;
    if (degrees_copy <= 180)
        return intcos_data[degrees_copy];
    else
        return intcos_data[360 - degrees_copy];
}
