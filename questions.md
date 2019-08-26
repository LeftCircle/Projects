# Questions

## What's `stdint.h`?

stdint.h is the standard int header file in the standard C library that defines the structures of int.
It definitely does much more than that as well. 

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

It is shorthand for unsigned int with 8 bytes, ....
It sets the maximum value for char and int based on 
the number of bytes that can be used

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

a BYTE is one byte (0 - 255)
DWORD is 4 bytes ( 0-4294967295)
Long is also 4 bytes, but is signed
WORD is 2 bytes

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

B then M in ASCII, so 0x42 0x4D

## What's the difference between `bfSize` and `biSize`?

bfSize is the size in bytes of the bitmap file
biSize is the number of bytes required by the entire structure

## What does it mean if `biHeight` is negative?

if biHeight is negative then the image is top down, so the first pixel that is coded is the top left.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount determines the number of bytes per pixel 
and the depth of the color. 

## Why might `fopen` return `NULL` in `copy.c`?

If we are opening a pointer that points to NULL, or if a copy cannot be made. 

## Why is the third argument to `fread` always `1` in our code?

because size_t nmemb == 1 for our case. nmemb is equal to the number of items of data, and we are only copying one image. 

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

padding is used to work in multiples of 4. If the bi.biWidth == 3, this is equal to 3 pixels x 3 bytes per pixel = 9 bytes, so padding must add 3 bytes to reach 12. 

## What does `fseek` do?

moves the file pointer position by a given offset.

## What is `SEEK_CUR`?

Gives the current position of the file pointer
