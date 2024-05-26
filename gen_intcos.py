from math import cos, pi

DEG_START = 0
DEG_END = 180

def deg_to_rad(deg: float):
    return (deg / 180) * pi

def main():
    with open('intcos.hpp', 'w') as file:
        file.write("#ifndef INTCOS_HPP\n")
        file.write("#define INTCOS_HPP\n")
        file.write("#include <stdint.h>\n")
        file.write("uint8_t intcos(uint16_t angle);\n")
        file.write("#endif // define INTCOS_HPP\n")

    with open('intcos.cpp', 'w') as file:
        file.write("#include \"intcos.hpp\"\n")
        file.write(f"static const uint8_t intcos_len = {DEG_END - DEG_START + 1};\n")
        file.write(f"static const uint8_t intcos_data[{DEG_END - DEG_START + 1}] = " + "{\n    ")
        
        line_counter = 0
        for degrees in range(DEG_START, DEG_END + 1):
            radians = deg_to_rad(degrees)
            cos_val = (cos(radians) / 2) + 0.5
            cos_val *= 0xFF
            cos_val = round(cos_val)

            file.write(f"{cos_val}, ")

            line_counter += 1
            if line_counter == 4:
                file.write("\n    ")
                line_counter = 0

        file.write("};\n")

        file.write("uint8_t intcos(uint16_t degrees)\n")
        file.write("{\n")
        file.write("    uint16_t degrees_copy = degrees % 360;\n")
        file.write("    if (degrees_copy <= 180)\n")
        file.write("        return intcos_data[degrees_copy];\n")
        file.write("    else\n")
        file.write("        return intcos_data[360 - degrees_copy];\n")
        file.write("}\n")      

if __name__ == '__main__':
    main()