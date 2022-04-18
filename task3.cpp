#include <bits/stdc++.h>
#include "bmplib.cpp"

using namespace std;

// functions' prototypes declarations
char displayMenu();
void load();
void save();
void cpyToImage(unsigned char image[][SIZE][RGB]);
void applyFilter(int choice);
void blackWhite();
void invert();
void merge();
void flip(char direction);
void rotate(string degree);
void Darken_and_Lighten_Image(char);
void shrink_image(string size);
void blur();
void shuffle();
void cutImage();
void enlarge();


// image that will be process
unsigned char image[SIZE][SIZE][RGB];

// start of program
int main()
{
    // load image into 2d array
    load();

    while (true)
    {
        applyFilter(displayMenu()); // pass choice of user to applyfilter
    }
}

// create menu for user
char displayMenu()
{
    cout << "Please select a filter to apply or 0 to exit: \n";
    cout << "1- Black & white Filter\n";
    cout << "2- Invert Filter\n";
    cout << "3- Merge Filter\n";
    cout << "4- Flip Image\n";
    cout << "5- Darken and Lighten Image\n";
    cout << "6- Rotate Image\n";
    cout << "8- Enlarge Image\n";
    cout << "9- Shrink Image\n";
    cout << "b- Shuffle Image\n";
    cout << "c- Blur Image\n";
    cout << "s- Save the image to a file\n";
    cout << "0- Exit\n";

    // filter to apply
    char choice;
    cout << ">> ";
    cin >> choice;

    return tolower(choice);
}


// load image to 2d array in memory
void load()
{
    fstream file;
    char fileName[100];

    // loop until user enter valid image
    do
    {
        cout << "please enter file name of the image to process: ";
        cin >> fileName;

        strcat(fileName, ".bmp");
        // see if the image exists or not
        file.open(fileName);
    } while (!file);

    readRGBBMP(fileName, image);
}



void save()
{
    char file[100];
    cout << "Please enter target file name: ";
    cin >> file;
    cout << '\n';
    strcat(file, ".bmp");
    writeRGBBMP(file, image);
}

// copy image after filter to global image
void cpyToImage(unsigned char filterImage[][SIZE][RGB])
{
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for(int k = 0; k < RGB; k++)
                image[i][j][k] = filterImage[i][j][k];

}

// call filter
void applyFilter(int choice)
{
    switch (choice)
    {
        case '0':
            cout << "GOODBYE :)\n";
            exit(0);
            break;

        case '1':
            blackWhite();
            break;

        case '2':
        {
            invert();
            break;
        }
        case '3':
            merge();
            break;

        case '4':
            char direction;
            cout << "Flip (H)orizontally or (V)ertically: "; // take direction from user
            cin >> direction;
            flip(direction);
            break;

        case '5':
            char contrast;
            cout << "Contrast (D)arken or (L)ighten: ";
            cin >> contrast;
            while(contrast != 'D' && contrast != 'd' && contrast != 'L' && contrast != 'l')
                {
                    cout << "Choose only D or L: ";
                    cin >> contrast;
                }
            Darken_and_Lighten_Image(contrast);
            break;

        case '6':
        {
            string degree;
            cout << "Rotate (90), (180) or (270) degrees?: ";
            cin >> degree;
            while(degree != "90" && degree != "180" && degree != "270")
            {
                cout << "Choose only 90, 180 or 270: ";
                cin >> degree;
            }
            rotate(degree);
            break;
        }
        // case '8':
        //     enlarge();
        //     break;
        
        case '9':
        {
            string size;
            cout << "Shrink to (1/2), (1/3) or (1/4)? ";
            cin >> size;
            while(size != "1/2" && size != "1/3" && size != "1/4")
            {
                cout << "Choose (1/2), (1/3) or (1/4) only: ";
                cin >> size;
            }
            shrink_image(size);
            break;
        }
        // case 'b':
        //     shuffle();
        //     break;
        
        case 'c':
            blur();
            break;

        case 's':
            save();
            break;

        default:
        {
            cout << "Not available Filter" << endl;
            break;
            // filter to apply
            char choice;
            cout << ">> ";
            cin >> choice;
        }

    }
}


void blackWhite()
{

    // calcualte average
    long long avg = 0;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for(int k = 0; k < RGB; k++)
            {
                avg += image[i][j][k];
            }
        }
    }

    avg /= 256 * 256 * 3;

    unsigned char blackWhiteImage[SIZE][SIZE][RGB];

    // convert to black and white image
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for(int k = 0; k < RGB; k++)
            {

                if (image[i][j][k] > avg)
                {
                    blackWhiteImage[i][j][k] = 255;
                }
                else
                {
                    blackWhiteImage[i][j][k] = 0;
                }
            }
        }
    }

    cpyToImage(blackWhiteImage);
}


void invert()
{
    unsigned char invertImage[SIZE][SIZE][RGB];

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for(int k = 0; k < RGB; k++)
            {
                invertImage[i][j][k] = 255 - image[i][j][k];
            }
        }
    }

    cpyToImage(invertImage);
}


// Merge two images
void merge()
{
    fstream file;
    char fileName2[100];
    unsigned char image2[SIZE][SIZE][RGB];

    do
    {
        cout << "Please enter name of image file to merge with: ";
        cin >> fileName2;

        strcat(fileName2, ".bmp");
        // see if the image exists or not
        file.open(fileName2);
    } while (!file);

    readRGBBMP(fileName2, image2);

    unsigned char merged_image[SIZE][SIZE][RGB];

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            for(int k = 0; k < 3; k++)
                merged_image[i][j][k] = (image[i][j][k] + image2[i][j][k]) / 2;
        
    cpyToImage(merged_image);
}

void flip(char direction)
{

    unsigned char flipImage[SIZE][SIZE][RGB];

    // flip vertical
    if (direction == 'V' || direction == 'v')
    {
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                for(int k = 0; k < 3; k++)
                {
                    flipImage[i][j][k] = image[255 - i][j][k];
                }
            }
        }
    }
    else if (direction == 'H' || direction == 'h')
    { // flip horizontal
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                for(int k = 0; k < 3; k++)
                {
                    flipImage[i][j][k] = image[i][255 - j][k];
                }
            }
        }
    }

    // apply filter to image
    cpyToImage(flipImage);
}

// Change the contrast of the image
void Darken_and_Lighten_Image(char contrast)
{
    unsigned char adjusted_image[SIZE][SIZE][RGB]{}; // dorl means darken or ligthen

    if(contrast == 'D' || contrast == 'd') // darken the image
    {
        for(int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for(int k = 0; k < 3; k++)
                    adjusted_image[i][j][k] = 0.5 * image[i][j][k]; 
    }


    else if(contrast == 'L' || contrast == 'l')  // ligthen the image
    {
        for(int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for(int k = 0; k < 3; k++)
                    adjusted_image[i][j][k] = pow(image[i][j][k],0.8)+150; 
    }

    cpyToImage(adjusted_image);
}

// rotate image by specific degree
void rotate(string degree)
{

    unsigned char rotatedImage[SIZE][SIZE][RGB];

    // rotate by 90
    if (degree == "90")
    {

        // transpose image matrix
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                for(int k = 0; k < 3; k++)
                    rotatedImage[i][j][k] = image[j][i][k];

    }
    else if (degree == "180")
    { // rotate by 180

        flip('v');
        flip('h');
        return;
    }
    else if (degree == "270")
    {
        rotate("90");
        flip('h');
        return;
    }

    cpyToImage(rotatedImage);
}


void shrink_image(string size){
    unsigned char shrinked_image[SIZE][SIZE][RGB];

    long long avg{};


    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            for(int k = 0; k < 3; k++)
                shrinked_image[i][j][k] = 255;



    if(size == "1/2")
    {
        long avg{};

        for(int i = 0, x = 0; i < SIZE / 2; i++)
        {
        

            for(int j = 0, y = 0; j < SIZE / 2; j++)
            {
                for(int k = 0; k < 3; k++)
                {
                    avg = (image[x][y][k] + image[x][y+1][k] + image[x+1][y][k]  + image[x+1][y+1][k]) / 4;
                    shrinked_image[i][j][k] = avg;
                }
                    y+=2;
            }
            x+=2;
        }
    }
    
    else if(size == "1/3")
    {
        long long avg{};

        for(int i = 0, x = 0; i < SIZE / 3; i++)
        {
        
            for(int j = 0, y = 0; j < SIZE / 3; j++)
            {
                for(int k = 0; k < 3; k++)
                {
                    avg = (image[x-1][y-1][k] + image[x-1][y][k] + image[x-1][y+1][k] + image[x][y-1][k] +  image[x][y][k] +  image[x][y+1][k] + image[x+1][y-1][k] +  image[x+1][y][k] + image[x+1][y+1][k]) / 9;
                    shrinked_image[i][j][k] = avg;
                }

                y+=3;
            }
            x+=3;
        }
    }

    else if(size == "1/4")
    {
        for(int i = 0, x = 0; i < SIZE / 4; i++)
        {
            for(int j = 0, y = 0; j < SIZE / 4; j++)
            {
                for(int k = 0; k < 3; k++)
                {
                    avg = (image[x][y][k] + image[x][y+1][k] + image[x][y+2][k] + image[x][y+3][k] + image[x+1][y][k] + image[x+1][y+1][k] + image[x+1][y+2][k] + image[x+1][y+3][k] + image[x+2][y][k] + image[x+2][y+1][k] + image[x+2][y+2][k] + image[x+2][y+3][k] + image[x+3][y][k] + image[x+3][y+1][k] + image[x+3][y][k] + image[x+3][y+3][k]) / 16;
                    shrinked_image[i][j][k] = avg;
                }
                y+=4;
            }
            x+=4;
        }
    }

    cpyToImage(shrinked_image);
}

void blur()
{
    unsigned char blurred_image[SIZE][SIZE][RGB]{};

    //move kernel
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0; j < SIZE; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                int avg = 0;

                // claculate avg for kernel
                for(int l = i; l < i + 7; l++)
                    for(int m = j; m < j + 7; m++)
                        avg += image[l][m][k];

                avg /= 49;

                // update center of kernel in blurred image
                blurred_image[i][j][k] = avg;
            }
        }
    }

    
    cpyToImage(blurred_image);

}