# Starleaf Challenge

## LZW Decompresor (C++/Python)

Lempel-Ziv-Welch decompressor written both in C++ and Python. 

The algorithm uses 12 bit codes read from the input files and a 256 entry dictionary (integer to ASCII characters) to decompress the encoded messages in the input files unde `LzwInputData/` directory.

Run using Python

    python3 lzw_decoder.py

Run using C++

    g++ lzw_decoder.cpp -o lzw_decoder

Some comparations between the two implementations were made and the results are shown below.

Python (seconds)

    Execution time - compressedfile1.z: 0.0009844303131103516 seconds...
    Execution time - compressedfile2.z: 0.0008404254913330078 seconds...
    Execution time - compressedfile3.z: 0.09557032585144043 seconds...
    Execution time - compressedfile4.z: 1.855008840560913 seconds...

C++ (milliseconds)

    Execution time - compressedfile1.z: 0 milliseconds ... 
    Execution time - compressedfile2.z: 0 milliseconds ... 
    Execution time - compressedfile3.z: 24 milliseconds ... 
    Execution time - compressedfile4.z: 473 milliseconds ...

### More information

E-mail: horiapavel@yahoo.com

GitHub: https://github.com/Horiapavel98

LinkedIn: https://linkedin.com/in/hpavel