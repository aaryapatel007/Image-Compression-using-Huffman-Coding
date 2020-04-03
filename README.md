# Lossless Image Compression using Huffman Coding

## Introduction

Huffman coding is a lossless data compression algorithm. Using this algorithm we can assign variable-length bit codes to the data that is being compressed. The characters are assigned codes based on their frequency of occurrence and the characters with the highest frequency are assigned smallest length bit codes and the characters with the least frequency are assigned the longest length bit codes. 

Every 24-bit .bmp file starts with a 54-byte header, the 18th byte specifies the width of the image and the 22nd byte specifies the height of the image. After the header, the color of each pixel is specified by its RGB values. The color of the bottom right pixel is stored first. The colors are stored in the order of blue, green and red and each color takes 1 byte each. Therefore, each pixel requires 3 bytes.

## Algorithm
1. Read .bmp image and store its width, height, size and information of pixels in the Struct `imageInfo`.
1. The information of each pixel is saved in an object of the Class `Pixel` containing information about the RGB values of the pixel, and the corresponding hex values.
1. Number of distinct colors are calculated and the frequency of each distinct color and its hex value is stored in an object of Class `color`.
1. A Huffman tree is created using the information in step 3 and the bit codes are saved with their respective hex values and frequency in the Class `color`.
1. The header of the new file is written.
1. The entire Image Matrix is traversed and the corresponding bit code of the color of each pixel is written in the file.
1. The written file is saved with an extension of .cmpbmp.
