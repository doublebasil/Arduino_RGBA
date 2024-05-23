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
        file.write(f"uint8_t intcos[{DEG_END - DEG_START + 1}] = " + "{\n    ")
        
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
        file.write("#endif // define INTCOS_HPP\n")

if __name__ == '__main__':
    main()