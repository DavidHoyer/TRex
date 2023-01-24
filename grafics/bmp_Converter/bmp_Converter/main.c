#include <stdio.h>
#include <stdlib.h>

int main()
{
    //file names
    char filename[]     = "C:\\ST\\Workspace\\TRex\\grafics\\bmp_Converter\\bmp_Converter\\Resum_Button.bmp";   //input bmp file
    char txtFilename[]  = "C:\\ST\\Workspace\\TRex\\grafics\\bmp_Converter\\bmp_Converter\\Resum_Button.txt"; //output txt file

    short int bfType;  //specifies the file type
    typedef struct tagBITMAPFILEHEADER
    {
        unsigned long int bfSize;  //specifies the size in bytes of the bitmap file
        short int bfReserved1;  //reserved; must be 0
        short int bfReserved2;  //reserved; must be 0
        unsigned long int bfOffBits;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
    }BITMAPFILEHEADER;

    typedef struct tagBITMAPINFOHEADER
    {
        unsigned long int biSize;  //specifies the number of bytes required by the struct
        long int biWidth;  //specifies width in pixels
        long int biHeight;  //specifies height in pixels
        short int biPlanes;  //specifies the number of color planes, must be 1
        short int biBitCount;  //specifies the number of bits per pixel
        unsigned long int biCompression;  //specifies the type of compression
        unsigned long int biSizeImage;  //size of image in bytes
        long int biXPelsPerMeter;  //number of pixels per meter in x axis
        long int biYPelsPerMeter;  //number of pixels per meter in y axis
        unsigned long int biClrUsed;  //number of colors used by the bitmap
        unsigned long int biClrImportant;  //number of colors that are important
    }BITMAPINFOHEADER;

    unsigned char bitmapImage;  //store image data
    int bitsize = 0;    //byte size of image
    long posIndicator = 0;

    BITMAPFILEHEADER bitmapFileHeader;  //bitmap file header
    BITMAPINFOHEADER bitmapInfoHeader;  //bitmap Info header
    FILE *filePtr;  //file pointer to bmp file
    FILE *filePtrWrite; //file pointer to output txt file

    //open file
    filePtr = fopen(filename,"r");

    if(filePtr == NULL)
        printf("error at file open!\n");

    //read the bitmap file type
    fread(&bfType, 2, 1, filePtr);

    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

    //read the bitmap info header
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

    //move file pointer and position indicator to the beginning of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    posIndicator = bitmapFileHeader.bfOffBits;

    //define bitsize of the image when image size in info header is 0
    if ((bitsize = bitmapInfoHeader.biSizeImage) == 0)
        bitsize = (bitmapInfoHeader.biWidth *
                   bitmapInfoHeader.biBitCount + 7) / 8 *
               abs(bitmapInfoHeader.biHeight);
    else
        bitsize = bitmapInfoHeader.biSizeImage;

    //setup output txt file
    filePtrWrite = fopen( txtFilename, "wb" ); //open file to write
    /*//write bmp file header into it
    fprintf_s( filePtrWrite, "Bitmap File Header\n"
                             "File type:        %s\n"
                             "Bitmap file size: %d\n"
                             "Reserved 1:       %d\n"
                             "Reserved 2:       %d\n"
                             "Data start:       %d\n\n",
               &bfType, bitmapFileHeader.bfSize, bitmapFileHeader.bfReserved1,
               bitmapFileHeader.bfReserved2, bitmapFileHeader.bfOffBits );
    //write bmp info header into it
    fprintf_s( filePtrWrite, "Bitmap Info Header\n"
                             "Size of Header:       %d\n"
                             "Width in pixel:       %d\n"
                             "Height in pixel:      %d\n"
                             "Nr. of clr planes:    %d\n"
                             "Bits per pixel:       %d\n"
                             "Compression method:   %d\n"
                             "Image size:           %d\n"
                             "X pixel per meter:    %d\n"
                             "Y pixel per meter:    %d\n"
                             "Number of colors:     %d\n"
                             "Important colors:     %d\n\n",
               bitmapInfoHeader.biSize, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight,
               bitmapInfoHeader.biPlanes, bitmapInfoHeader.biBitCount, bitmapInfoHeader.biCompression,
               bitmapInfoHeader.biSizeImage, bitmapInfoHeader.biXPelsPerMeter, bitmapInfoHeader.biYPelsPerMeter,
               bitmapInfoHeader.biClrUsed, bitmapInfoHeader.biClrImportant);*/

    fprintf_s( filePtrWrite,    "Bitmap Info %s\n"
                                "Width in pixel:       %d\n"
                                "Height in pixel:      %d\n"
                                "Total Pixel:          %d\n"
                                "Bits per pixel:       %d\n",
               &bfType, bitmapInfoHeader.biWidth, bitmapInfoHeader.biHeight,
               bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight, bitmapInfoHeader.biBitCount);


    //write picturedata Array text
    fprintf_s( filePtrWrite,"\nunsigned char PictureData [%d][4] = {\n", (bitsize / 4));

    //read out bitmap data and put it in txt file
    for ( long i = 1; i <= bitsize; i++)
    {
        posIndicator += fread(&bitmapImage, 1, 1, filePtr);
        //detect end of file
        if( feof(filePtr) )
        {
            posIndicator += 1;
            fseek(filePtr, posIndicator, SEEK_SET);
            if( (i % 4) == 1 )
            {
                fprintf_s(filePtrWrite, "{%d, ", 0x1A);
            }
            else if( (i % 4) == 0)
            {
                fprintf_s(filePtrWrite, "%d}, ", 0x1A);
            }
            else fprintf_s(filePtrWrite, "%d, ", 0x1A);
        }
        else if( (i % 4) == 1 )
        {
            fprintf_s(filePtrWrite, "{%d, ", bitmapImage);
        }
        else if( (i % 4) == 0)
        {
            fprintf_s(filePtrWrite, "%d}, ", bitmapImage);
        }
        else fprintf_s(filePtrWrite, "%d, ", bitmapImage);
    }
    fprintf_s( filePtrWrite,"}");

    fclose(filePtrWrite); //close written file
    fclose(filePtr);    //close bmp file

    return 0;
}
