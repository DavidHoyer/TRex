#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char with;
    unsigned char height;
    unsigned char BitsPP;
    unsigned char dataStart;
} headerData;

int main()
{
    unsigned char Data [20];
    unsigned char tempData;

    headerData myPicture;

    FILE *filepointer_read;
    FILE *filepointer_write;

    filepointer_read = fopen("T_Rex3.bmp", "r");
    filepointer_write = fopen("T_Rex3.txt","w");

    if ((filepointer_read == NULL) || (filepointer_write == NULL)) {
        printf("one of the files could not open");
        fclose(filepointer_read);
        fclose(filepointer_write);
    }
    else
    {
        printf("files opend succsessfuly!\n");
    }

    //set filepointer and read dataStart
    fseek(filepointer_read, 10, SEEK_SET);
    fscanf(filepointer_read, "%c", &myPicture.dataStart);

    //set filepointer and read with
    fseek(filepointer_read, 18, SEEK_SET);
    fscanf(filepointer_read, "%c", &myPicture.with);

    //set filepointer and read length
    fseek(filepointer_read, 22, SEEK_SET);
    fscanf(filepointer_read, "%c", &myPicture.height);

    //set filepointer and read length
    fseek(filepointer_read, 28, SEEK_SET);
    fscanf(filepointer_read, "%c", &myPicture.BitsPP);

    fprintf(filepointer_write, "BM\n");

    fprintf(filepointer_write, "DataStart: %d\n", myPicture.dataStart);
    fprintf(filepointer_write, "With: %d\n", myPicture.with);
    fprintf(filepointer_write, "Length: %d\n\n", myPicture.height);
    fprintf(filepointer_write, "Bits/Pixel: %d\n\n", myPicture.BitsPP);

    fprintf(filepointer_write, "PictureData [%i][4] = {\n", myPicture.with*myPicture.height);

    fseek(filepointer_read, myPicture.dataStart, SEEK_SET);

    /*read and write picture color data
            first loop to go from row to row
            second loop to go from pixel to pixel in a row
    */

    for(int i = 0; i < (myPicture.height*myPicture.with); i++)
    {
        if(i != 0 && i%5 == 0)                                 //write only 10 pixels per line
            fprintf(filepointer_write,"\n");
                                                   //add the "{" at the beginning of a col
        fprintf(filepointer_write,"{");

        for(int a = 0; a < 4; a++)
        {
            fscanf(filepointer_read, "%c", &tempData);      //read color value

            if(a < 3)
                fprintf(filepointer_write, "%d,", tempData);
            else
                fprintf(filepointer_write, "%d", tempData);
        }

        fprintf(filepointer_write,"},");
    }

    /*int a = 0;
    for (int i =0; i < myPicture.height; i++)
    {
        fprintf(filepointer_write,"\t");
        for (int u =0; u< (myPicture.with*4); u++)
        {
            if(a == 0)
                fprintf(filepointer_write,"{");

            fscanf(filepointer_read, "%c", &tempData);          //read color value
            fprintf(filepointer_write, "%d,", tempData);        //write color value as integer in file

            a++;
            if(a == 4){
                a = 0;
                fprintf(filepointer_write,"} ");
            }
        }
        fprintf(filepointer_write,"\n");                        //making a new line after a row completly read
    }*/


    fprintf(filepointer_write, "\n};");                           //close array

    //close files
    fclose(filepointer_read);
    fclose(filepointer_write);

    return 0;
}

