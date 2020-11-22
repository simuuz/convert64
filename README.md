# convert64
Simple utility to convert a .n64/.v64 rom file to .z64 and viceversa
## Usage
Compile with a simple `make` and use as `./convert rom.n/v/z64 <optional flag, check below>`</br>
If a .n64 or a v64 rom is passed, the flag is assumed (so it automatically converts it to a .z64) and so it's optional, **however** if a .z64 rom is passed, it is **mandatory** to specify to what type you want to convert it. For example: `./convert64 rom.n/v64` will work as it is, where instead `./convert64 rom.z64` Won't. It *needs* a `-n/-v64` flag, so like: `./convert64 rom.z64 -n/v64`
