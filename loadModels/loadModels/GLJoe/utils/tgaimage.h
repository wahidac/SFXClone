/**
 * @file GLJoe/utils/tgaimage.h
 * 
 * @brief Load a TGA image file
 * 
 * @author Joachim Valente <joachim.valente@gmail.com>
 * @date 6/13/2013
 */

#ifndef DEF_GLJOE_TGAIMAGE_H
#define DEF_GLJOE_TGAIMAGE_H

namespace GLJoe
{

class TgaImage
{
protected:
	int _width;
	int _height;
	unsigned char _byteCount;
	unsigned char* _data;

public:

	TgaImage() :
		_width(0), _height(0), _byteCount(0), _data(NULL)
	{}
	
	~TgaImage()
	{
		delete[] _data;
	}
	
	bool loadTGA(const char *filename)
	{
		FILE *file;
		unsigned char type[4];
		unsigned char info[6];

		file = fopen(filename, "rb");

		if (!file)
			return false;

		if (fread (&type, sizeof(char), 3, file) != 3)
			Warning("Problem while loading TGA file");
		fseek (file, 12, SEEK_SET);
		if (fread (&info, sizeof(char), 6, file) != 6)
			Warning("Problem while loading TGA file");
			

		// Image type either 2 (color) or 3 (greyscale)
		if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
		{
			fclose(file);
			return false;
		}

		_width = info[0] + info[1] * 256;
		_height = info[2] + info[3] * 256;
		_byteCount = info[4] / 8;

		if (_byteCount != 3 && _byteCount != 4)
		{
			fclose(file);
			return false;
		}

		size_t imageSize = _width * _height * _byteCount;

		// Allocate memory for image _data
		_data = new unsigned char[imageSize];

		// Read in image _data
		if (fread(_data, sizeof(unsigned char), imageSize, file) != imageSize)
			Warning("Problem while loading TGA file");

		// Close file
		fclose(file);

		return true;
	}
	
	int width() const
	{
		return _width;
	}
	
	int height() const
	{
		return _height;
	}
	
	unsigned char byteCount() const
	{
		return _byteCount;
	}
	
	unsigned char* data() const
	{
		return _data;
	}
};


} // namespace GLJoe

#endif // DEF_GLJOE_TGAIMAGE_H

