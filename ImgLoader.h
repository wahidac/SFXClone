#ifndef IMGLOADER_H
#define IMGLOADER_H

#include<GLJoe.h>
#include<stdio.h>

#include "lodepng.h"

class ImgLoader
{
public:
	// Use the lodepng library to read in a PNG image as RGBA values
	void loadImage(const char* filename, GLubyte* imagebuffer, unsigned width, unsigned height)
	{
		std::vector<unsigned char> raw_image; //the raw pixels

		unsigned error = lodepng::decode(raw_image, width, height, filename);

		//if there's an error, display it
		if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				int vec_position = (height * i + j) * 4;
				int buf_position = (height * i + j) * 3;
				imagebuffer[buf_position++] = raw_image[vec_position++];
				imagebuffer[buf_position++] = raw_image[vec_position++];
				imagebuffer[buf_position++] = raw_image[vec_position++];
			}
		}
	}
};

#endif