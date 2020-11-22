#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdint>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,"Must specify a file. Usage: \"./byteswap rom.n64\"\n");
        return -1;
    } else {
        if(std::string(argv[1]).find("z64",std::string(argv[1]).size() - 3) != std::string::npos) {
            fprintf(stderr,"%s is already a .z64!\n", argv[1]);
            return -1;
        }
        FILE *fin = fopen(argv[1], "rb");
        if(fin == NULL) {
            fprintf(stderr,"Could not open file\n");
            return -1;
        }

        fseek(fin,0,SEEK_END);
        size_t size = ftell(fin);
        fseek(fin,0,SEEK_SET);
        std::vector<uint8_t> le_rom(size);
        std::vector<uint8_t> be_rom(size);
        std::string game_name;

        fread(le_rom.data(),1,size,fin);

        for(int i = 0; i < size; i+=2) {
            be_rom[i] = le_rom[i+1];
            be_rom[i+1] = le_rom[i];
        }

        for(uint8_t i = 0x20; i <= 0x33; i++) {
            game_name[i - 0x20] = be_rom[i];
        }

        printf("Size: %d MiB\tName: %s\tCountry code: %c\n", size/1048576, game_name.c_str(), be_rom[0x3e]);
        std::string new_filename(argv[1]); new_filename.erase(new_filename.size() - 3, 3); new_filename += "z64";
        FILE *foff = fopen(new_filename.c_str(),"wb");

        if(foff == NULL) {
            fprintf(stderr,"Could not open file\n");
            return -1;
        }

        fwrite(be_rom.data(),1,size,foff);

        printf("Swapped! Wrote to \"%s\"\n", new_filename.c_str());
        fclose(fin);
        fclose(foff);

        return 0;
    }
    return -1;
}