# SeamCarving

Ahmed Darwich
Algorithms 535
Project 3 -- Seam Carving
Dr. Duan

This is my submission for Project 3 -- Seam Carving. This is for a school project. Do not copy my code.

Overview:
This programming assignment was to create a dynamic programming algorithm to resize images. It was written entirely in C++.
Seam carving changes the size of an image by removing the least visible pixels(least energetic pixels) in the image. The visibility
of a pixel can be defined using an energy function. Seam carving can be done by finding an one-pixel wide path of lowest energy
crossing the image from top to bottom (vertical path) or from left to right (horizontal path) and removing the path
(seam).

To complete seam carving of an image, the input must be read in the form of a PGM file. This form follows this trend:

"P2
# Comment
MaxGreyScaleNumber
ColumnNumber RowNumber
Values....."

In order to eliminate seams that do not effect the image uniqueness, an energy function was utilized. This energy function 
was programmed like e(i,j) = |v(i,j)-v(i-1,j)|+ |v(i,j)-v(i+1,j)|+ |v(i,j)-v(i,j-1)|+ |v(i,j)-v(i,j+1)|, where
v(i,j) = pixel value at (i,j) since the input of PMG values was read in as matrix. For boundary cases such as 
the edge of the image, the difference was coded as 0 by changing the pixel value of the outside range to the current pixel value.
This energy matrix was then transformed into a cumulative energy matrix where the path to remove the least energetic pixel was
revealed. The resulting matrix was then transposed and underwent the same algorithm to remove the horizontal seams of the image. 
The result was then written to another PMG file with the name "image_name_processed.pmg" and viewed to ensure the uniqueness of the image
remained the same, meaning the image was still the same just resized. 

To Run My Program:
My program is built using CMake. In order to run please follow steps below:
1.) Ensure that CMakeLists.txt is in current directory
2.) mkdir build
3.) cd build
4.) Run "cmake .."
5.) Run "make" in build directory
6.) To run program, run "./seamCarving535 filename verticalSeams horizontalSeams"
    Where filename is a PMG file of the correct form and
    verticalSeams and horizontalSeams are numbers that are less than the number of rows and columns of the PMG file.
7.) View resulting resized image according to outputted filename in build directory

Thank you.
