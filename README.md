# xz-fuzzing

Results of fuzz-testing the xz compression library (https://github.com/tukaani-project/xz).

Version tested: v5.4.6 ([https://github.com/tukaani-project/xz/releases/tag/v5.4.6](https://github.com/tukaani-project/xz/releases/download/v5.4.6/xz-5.4.6.tar.gz)). Compiled with instrumentation and tested with AflPlusPlus (https://aflplus.plus/).

# setup guide

## building with instrumentation
build xz distribution with AFL instrumentation.
1) Prerequisites: install C & C++ compilers, make, afl++; Download and unpack the distro.
2) In the distro dir: setup env variables to use AFL-instrumented compilers and run configure: `CC=path/to/afl/afl-gcc CXX=path/to/afl/afl-g++ ./configure --disable-shared` (in one line). Flag `--disable-shared` gets the binary installed into the building dir.
3) Assemble the binary by `make clean all`.
4) The target binary will appear in src/xz subfolder
![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/2c09c888-076c-41cc-91ae-8cf6715c739f)


## prepare initial set of exaples
The seeds_dir contains initial test cases to be compressed, seeds_xz_dir contains test cases of compressed files to be decompressed. They are: small code example, a binary executable, a text file containing all 256 ASCII characters, a picture, a voice recorder file and an already compressed file.

## fuzz-test 
To fuzz-test decompression wuth afl: `/path/to/afl/afl-fuzz -i seeds_xz_dir/ -o output_dir/ -- path/to/distro/src/xz/xz -d @@`. Flag -d stands for decompression.

# results
Fuzzing was stopped once no new paths were found in 15 minutes. In 2h 47min, 443 paths were found, and zero crashes.
![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/f3b0d5b0-44a1-4fe7-a662-40c1344f9e2f)
