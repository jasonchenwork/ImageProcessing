#include <iostream>
#include <bits/stdc++.h>

using namespace std;


struct BITMAPFILE{
	unsigned char identity[2];        // 2 byte : "BM" is BMP
};
struct BITMAPFILEHEADER { 
  uint32_t  file_size;       // 4 byte : file size
  uint32_t  reserved;            // 4 byte  
  uint32_t    data_offset;     // 4 byte : 
};
struct BITMAPINFOHEADER {
  uint32_t    header_size;      // 4 byte : 
  uint32_t    width;            // 4 byte :  (pixel)
  uint32_t    height;           // 4 byte :  (pixel)
  uint16_t  planes;           // 2 byte :  
  uint16_t bit_per_pixel;    // 2 byte :  
  uint32_t    compression;      // 4 byte :  
  uint32_t    data_size;        // 4 byte :  
  uint32_t    hresolution;      // 4 byte : pixel/m
  uint32_t    vresolution;      // 4 byte : pixel/m
  uint32_t    used_colors;      // 4 byte :  
  uint32_t    important_colors; // 4 byte :  
};
struct PALLETTE {
  char blue;                       // 1 byte :  
  char green;                      // 1 byte :  
  char red;                        // 1 byte : 
  char reserved;                   // 1 byte :  
};


void imagescale(const  unsigned char *imageSource, unsigned char *imageTarget,int Swidth, int Sheight,int Twidth, int Theight)
{
	double zx = (double)Twidth/(double)Swidth;
	double zy=  (double)Theight/(double)Sheight;
	int i, j, m, n;
    double p, q;
    int dR = 0;
	int dG = 0;
	int dB = 0;
	unsigned char R, G, B;         // color of R, G, B
	   for (i = 0; i < Theight; i++)
                {
                    for (j = 0; j < Twidth; j++)
                    {
                        double x, y;
                        y =  (i / zy);
                        x =  (j / zx);

                        if (y > 0)
                        {
                            m = (int)(y);
                        }
                        else
                        {
                            m = (int)(y - 1);
                        }

                        if (x > 0)
                        {
                            n = (int)(x);
                        }
                        else
                        {
                            n = (int)(x - 1);
                        }

                        q = y - m;
                        p = x - n;

                        if (q == 1)
                        {
                            q = 0;
                            m += 1;
                        }
                        if (p == 1)
                        {
                            p = 0;
                            n += 1;

                        }
                        if ((m >= 0) && (m < Sheight - 1) &&  (n >= 0) && (n < Swidth - 1))
                        {
                            dR = ((1.0 - q) * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 2)) + p * (*(imageSource + 3 * (Swidth * (m+1) + n) + 2))) + q * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + (n+1)) + 2)) + p * (*(imageSource + 3 * (Swidth * (m+1) + (n+1)) + 2)) ));
                            dG = ((1.0 - q) * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 1)) + p * (*(imageSource + 3 * (Swidth * (m+1) + n) + 1))) + q * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + (n+1)) + 1)) + p * (*(imageSource + 3 * (Swidth * (m+1) + (n+1)) + 1)) ));
						    dB = ((1.0 - q) * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + n) + 0)) + p * (*(imageSource + 3 * (Swidth * (m+1) + n) + 0))) + q * ((1.0 - p) * (*(imageSource + 3 * (Swidth * m + (n+1)) + 0)) + p * (*(imageSource + 3 * (Swidth * (m+1) + (n+1)) + 0)) ));
						}
                        else
                        {

                           dG = dB = dR = 0;
							

                        }
						 
                        if (dG < 0 )
                        {
                           dG  =0;
                        }
					    if (dR < 0 )
                        {
                           dR  =0;
                        }
						if (dB < 0 )
                        {
                           dB  =0;
                        }
                        if (dG > 255 )
                        {
                            dG   = 255;
                        }
						if (dR > 255 )
                        {
                            dR   = 255;
                        }
						if (dB > 255 )
                        {
                            dB   = 255;
                        }
							 
						     
						*(imageTarget + 3 * (Twidth * i + j) + 2) =(char)dR;
						*(imageTarget + 3 * (Twidth * i + j) + 1) = (char)dG;
						*(imageTarget + 3 * (Twidth * i + j) + 0) =(char)dB;                   
				      

                    }
                } 

}
void openBmp(const char *fname_s, unsigned char **image, unsigned int &width, unsigned int &height, unsigned int &rgb_raw_data_offset)
{
   
	FILE          *fp_s = NULL;    // source file handler
 
   struct BITMAPFILE B_H, *MyBmpFile;
   struct BITMAPFILEHEADER F_H, *MyBmpHeader;
   struct BITMAPINFOHEADER F_Info, *MyBmpInfo;
   struct PALLETTE p_, *MyPallette;

   MyBmpFile=&B_H;
   MyBmpHeader=&F_H;
   MyBmpInfo=&F_Info;

   fp_s = fopen(fname_s, "rb");

   fread(MyBmpFile,sizeof(B_H),1, fp_s );
   printf("Label: %c %c \n",MyBmpFile->identity[0],MyBmpFile->identity[1]);

   fread(MyBmpHeader,sizeof(F_H),1, fp_s );

   
   printf("File size: %d bytes \n",MyBmpHeader->file_size);
   printf("reserved: %d \n",MyBmpHeader->reserved);
   printf("offset: %d \n",MyBmpHeader->data_offset);
   rgb_raw_data_offset=MyBmpHeader->data_offset;

   fread(MyBmpInfo,sizeof(F_Info),1,fp_s);

   printf("header size: %d  byte \n",MyBmpInfo->header_size);
   printf("width: %d \n",MyBmpInfo->width);
   printf("height: %d \n",MyBmpInfo->height);
   printf("planes: %d \n",MyBmpInfo->planes);
   printf("bit_per_pixel: %d pixel \n",MyBmpInfo->bit_per_pixel);
   printf("compress: %d  \n",MyBmpInfo->bit_per_pixel);
   printf("data_size: %d \n",MyBmpInfo->data_size);
   printf("H-Resolution: %d \n",MyBmpInfo->hresolution);
   printf("V-Resolution: %d \n",MyBmpInfo->vresolution);
   printf("Used Color: %d \n",MyBmpInfo->used_colors);
   printf("important colors %d \n",MyBmpInfo->important_colors);


   width=MyBmpInfo->width;
   height=MyBmpInfo->height;

   
   
   fseek(fp_s, MyBmpHeader->data_offset, SEEK_SET);
   *image  = (unsigned char *)malloc((size_t)MyBmpInfo->width * MyBmpInfo->height * 3);
   fread(*image , sizeof(unsigned char), (size_t)(long)width * height * 3, fp_s);

   fclose(fp_s);
}
int saveBmp(const char *fname_s, unsigned char *image, unsigned int width, unsigned int height, unsigned int rgb_raw_data_offset)
{
 

				FILE          *fp_s = NULL;    // source file handler
				unsigned int file_size;           // file size
 
				unsigned char R, G, B;         // color of R, G, B

				unsigned char header[54] = {
				0x42,        // identity : B
				0x4d,        // identity : M
				0, 0, 0, 0,  // file size
				0, 0,        // reserved1
				0, 0,        // reserved2
				54, 0, 0, 0, // RGB data offset
				40, 0, 0, 0, // struct BITMAPINFOHEADER size
				0, 0, 0, 0,  // bmp width
				0, 0, 0, 0,  // bmp height
				1, 0,        // planes
				24, 0,       // bit per pixel
				0, 0, 0, 0,  // compression
				0, 0, 0, 0,  // data size
				0, 0, 0, 0,  // h resolution
				0, 0, 0, 0,  // v resolution 
				0, 0, 0, 0,  // used colors
				0, 0, 0, 0   // important colors
				};
   


			fp_s = fopen(fname_s, "wb");

		    if (fp_s == NULL) {
				printf("fopen fname_t error\n");
				 return -1;
			}


			file_size = width * height * 3 + rgb_raw_data_offset;

			 header[2] = (unsigned char)(file_size & 0x000000ff);
			 header[3] = (file_size >> 8)  & 0x000000ff;
			 header[4] = (file_size >> 16) & 0x000000ff;
			 header[5] = (file_size >> 24) & 0x000000ff;

			 // width
			 header[18] = width & 0x000000ff;
			 header[19] = (width >> 8)  & 0x000000ff;
			 header[20] = (width >> 16) & 0x000000ff;
			 header[21] = (width >> 24) & 0x000000ff;

			  // height
			 header[22] = height &0x000000ff;
			 header[23] = (height >> 8)  & 0x000000ff;
			 header[24] = (height >> 16) & 0x000000ff;
			 header[25] = (height >> 24) & 0x000000ff;

			 // write header
			 fwrite(header, sizeof(unsigned char), rgb_raw_data_offset, fp_s);
			 fwrite(image , sizeof(unsigned char), (size_t)(width * height * 3), fp_s);

			 fclose(fp_s);

			return 0;
}
void testRWbmp()
{
  	unsigned int width,height;
	unsigned int rgb_raw_data_offset; 
    unsigned char *image_s = NULL; // source image array
    unsigned char *image_t = NULL; // target image array  
    openBmp("Lenna.bmp", &image_s, width, height, rgb_raw_data_offset);
    saveBmp("test01.bmp", image_s, width, height, rgb_raw_data_offset);
    free(image_s);
}
int main()
{
    testRWbmp();

    return 0;
}