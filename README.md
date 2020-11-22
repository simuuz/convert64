# convert64
Simple utility to convert a .n64/.v64 rom file to .z64 and viceversa
## Usage
Compile with a simple `make` and use as `./convert rom.n64/v64/z64 <optional -type, check below>`</br>
If a .n64 or a v64 rom is passed, the type `-z64` is assumed and so it's optional, **however** if a .z64 rom is passed, it is **mandatory** to specify to what type you want to convert it. For example: `./convert64 rom.n64/v64` will work as it is, where instead `./convert64 rom.z64 -n64/-v64` *needs* the `-n64/-v64`
