#include <iostream>
#include <cstdint>
#include <vector>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr,"Must specify a file. Usage: \"./convert64 rom.n/v/z64 [options]\"\n\nOptions:\n\t-n/v/z64\n");
        return -1;
    } else {
        bool ton64 = false, tov64 = false, toz64 = false;
        //argument parsing
        if(argc >= 3) {
            ton64 = (strcmp(argv[2],"-n64") == 0);
            tov64 = (strcmp(argv[2],"-v64") == 0);
            toz64 = (strcmp(argv[2],"-z64") == 0);
        }

        if (argc < 3) {
            printf("No extension specified, assuming ->.z64\n");
            toz64 = true;
        }

        //opens the rom
        FILE *fin = fopen(argv[1], "rb");
        if(fin == NULL) {
            fprintf(stderr,"Could not open file\n");
            return -1;
        }

        fseek(fin, 0, SEEK_END);
        size_t size = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        std::vector<uint8_t> in_rom(size);
        std::vector<uint8_t> out_rom(size);
        std::string game_name;

        fread(in_rom.data(), 1, size, fin);
        //detecting the orignal file's endianness
        uint32_t endianness = ((in_rom[0] << 24) | (in_rom[1] << 16) | (in_rom[2] << 8) | in_rom[3]);
        bool isz64 = (endianness == 0x80371240);
        bool isn64 = (endianness == 0x40123780);
        bool isv64 = (endianness == 0x37804012);

        //checking if it's a valid rom
        if(!isz64 && !isn64 && !isv64) {
            fprintf(stderr,":( i couldn't recognize this rom. Make sure it's valid!\n");
            return -1;
        }

        //Swapping routines: n64
        if (isn64) {
            printf("Detected n64!\n");
            if (tov64) {
                for(int i = 0; i < size; i+=4) {
                    out_rom[i] = in_rom[i+2];
                    out_rom[i+1] = in_rom[i+3];
                    out_rom[i+2] = in_rom[i];
                    out_rom[i+3] = in_rom[i+1];
                }
            } else if (ton64) {
                fprintf(stderr,"Already a .n64 file\n");
                return -1;
            } else if (toz64) {
                for(int i = 0; i < size; i+=4) {
                    out_rom[i]   = in_rom[i+3];
                    out_rom[i+1] = in_rom[i+2];
                    out_rom[i+2] = in_rom[i+1];
                    out_rom[i+3] = in_rom[i];
                }
            } else {
                fprintf(stderr,"%s is not a valid option\n", argv[2]);
                return -1;
            }
        }

        //v64
        if (isv64) {
            printf("Detected v64!\n");
            if (ton64) {
                for(int i = 0; i < size; i+=4) {
                    out_rom[i] = in_rom[i+2];
                    out_rom[i+1] = in_rom[i+3];
                    out_rom[i+2] = in_rom[i];
                    out_rom[i+3] = in_rom[i+1];
                }
            } else if (tov64) {
                fprintf(stderr,"Already a .v64 file\n");
                return -1;
            } else if (toz64) {
                for(int i = 0; i < size; i+=4) {    
                    out_rom[i] = in_rom[i+1];
                    out_rom[i+1] = in_rom[i];
                    out_rom[i+2] = in_rom[i+3];
                    out_rom[i+3] = in_rom[i+2];
                }
            } else {
                fprintf(stderr,"%s is not a valid option\n", argv[2]);
                return -1;
            }
        }

        //z64
        if (isz64) {
            printf("Detected z64!\n");
            if(argc < 3) {
                fprintf(stderr,"You passed a .z64 file, so you need also to specify to what extension you want to convert it. Usage: \"./convert64 rom.z64 -n/v64\"\n");
                return -1;
            }
            if (ton64) {
                for(int i = 0; i < size; i+=4) {
                    out_rom[i] = in_rom[i+3];
                    out_rom[i+1] = in_rom[i+2];
                    out_rom[i+2] = in_rom[i+1];
                    out_rom[i+3] = in_rom[i];
                }
            } else if (tov64) {
                for(int i = 0; i < size; i+=4) {
                    out_rom[i] = in_rom[i+1];
                    out_rom[i+1] = in_rom[i];
                    out_rom[i+2] = in_rom[i+3];
                    out_rom[i+3] = in_rom[i+2];
                }
            } else if (toz64) {
                fprintf(stderr,"Already a .z64 file\n");
                return -1;
            } else {
                fprintf(stderr,"%s is not a valid option\n", argv[2]);
                return -1;
            }
        }

        //getting the rom's internal name to print info
        for(uint8_t i = 0x20; i <= 0x33; i++) {
            game_name[i - 0x20] = out_rom[i];
        }
        
        //printing info
        printf("Size: %lu MiB\tName: %s\tCountry code: %c\n", size/1048576, game_name.c_str(), out_rom[0x3e]);

        //stripping and changing the extension
        std::string new_filename(argv[1]); new_filename.erase(new_filename.size() - 3, 3); 
        if(ton64)
            new_filename += "n64";
        else if(tov64)
            new_filename += "v64";
        else if(toz64)
            new_filename += "z64";

        //writing the new file and checking errors
        FILE *foff = fopen(new_filename.c_str(),"wb");
        if(foff == NULL) {
            fprintf(stderr,"Could not open file\n");
            return -1;
        }

        fwrite(out_rom.data(),1,size,foff);

        //print the last message and close the files, thus returning 0 -> success
        printf("Swapped! Wrote to \"%s\"\n", new_filename.c_str());
        fclose(fin);
        fclose(foff);

        return 0;
    }
    return -1;
}