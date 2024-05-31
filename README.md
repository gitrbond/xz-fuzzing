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


## prepare initial set of examples
The seeds_dir contains initial test cases to be compressed, seeds_xz_dir contains test cases of compressed files to be decompressed. They are: small code example, a binary executable, a text file containing all 256 ASCII characters, a picture, a voice recorder file and an already compressed file.

## fuzz-test 
To fuzz-test decompression wuth afl: `/path/to/afl/afl-fuzz -i seeds_xz_dir/ -o output_dir/ -- path/to/distro/src/xz/xz -d @@`. Flag -d stands for decompression.

## fuzzing results
Fuzzing was stopped once no new paths were found in 15 minutes. In 2h 47min, 443 paths were found, and zero crashes.

![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/cb95ddb1-ed11-4e9d-9e5a-1d93ce5f7c2e)

# calculating code coverage with lcov

To calculate code coverage, another distribution is compiled from sources with --coverage flags. Then, corpuses from `output_dir/default/queue` which were generated during fuzzing phase, are executed on a compiled binary.
1) `CC="gcc --coverage" CXX="g++ --coverage" ./configure --disable-shared`
2) `make clean all`
3) `cd path/to/src/xz` (directory with xz binary compiled with coverage)
4) `for file in path/to/output_dir/default/queue/*; do ./xz -d $file; done` (execute all paths)

When executing, .gcda files are created, they can be found in a distribution dir by `find -name *.gcda` - they are bitmaps with data about executed paths.

![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/f80d4ffa-29e1-4846-aca3-7ab053dd9c0a)

Lastly, generate coverage report with 

5) `lcov -c -d . -o xz_coverage` (from same distr dir, generates file with line coverage by file data)
6) `genhtml -o coverage_report xz_coverage` (generates pretty html file)

![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/8da6d54f-900a-457a-8e85-49eccf238592)

## coverage result
overall line coverage: 15.4%, overall function coverage: 23.0%

![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/9ded2f22-b11f-4028-b82a-561e4f8714f5)

When navigating into file, blue lines are executed ones, red are not. White are not-executable lines.

![image](https://github.com/gitrbond/xz-fuzzing/assets/61554885/9c77cb52-e011-4072-9540-de99ebdb3443)


## P.S
as there are sources in the resulting report, you may not want to include them. To delete them, run:

7) `find -name *.gcov.html | xargs rm`
