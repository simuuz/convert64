# convert64
Simple utility to convert a .n64/.v64 rom file to .z64 and viceversa
## Usage
Compile with a simple `make` and use as `./convert64 rom.n/v/z64 <optional flag, check below>`</br>
If a .n64 or a v64 rom is passed, the flag is assumed (so it automatically converts it to a .z64) and so it's optional, **however** if a .z64 rom is passed, it is **mandatory** to specify to what type you want to convert it. For example: `./convert64 rom.n/v64` will work as it is, where instead `./convert64 rom.z64` won't. It *needs* a `-n/-v64` flag, so like: `./convert64 rom.z64 -n/v64`

### But how does it work?
N64 files are little-endian, Z64 files are big-endian and V64 files are byte-swapped. What does this mean?
Let's take for example the number 0x12345678 in hexadecimal:

| Endianness    | Byte 1 | Byte 2 | Byte 3 | Byte 4 | Explanation |
| ------------- | ------ | ------ | ------ | ------ | ----------- |
| Big-endian    | 0x12   | 0x34   | 0x56   | 0x78   | Same order  |
| Little-endian | 0x78   | 0x56   | 0x34   | 0x12   | Last byte to first byte |
| Byte-swapped  | 0x34   | 0x12   | 0x78   | 0x56   | Swap bytes pair by pair |
