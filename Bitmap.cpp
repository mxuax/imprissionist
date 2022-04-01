//
// bitmap.cpp
//
// handle MS bitmap I/O. For portability, we don't use the data structure 
// defined in Windows.h. However, there is some strange thing, the size of our 
// structure is different from what it should be though we define it in the 
// same way as MS did. So, there is a hack, we use the hardcoded constant, 14, 
// instead of the sizeof to calculate the size of the structure.  You are not 
// supposed to worry about this part. However, I will appreciate if you find 
// out the reason and let me know. Thanks.
//
#include <FL/fl_ask.H>
#include "bitmap.h"
 
BMP_BITMAPFILEHEADER bmfh; 
BMP_BITMAPINFOHEADER bmih; 

// Bitmap data returned is (R,G,B) tuples in row-major order.
unsigned char* readBMP(char* fname,
	int& width,
	int& height)
{
	FILE* file;
	BMP_DWORD pos;
	if (fname == NULL) {
		return NULL;
	}
	if ((file = fopen(fname, "rb")) == NULL) {
		fl_alert("Can't open bitmap file");
		return NULL;
	}


	//	I am doing fread( &bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, file ) in a safe way. :}
	fread(&(bmfh.bfType), 2, 1, file);
	fread(&(bmfh.bfSize), 4, 1, file);
	fread(&(bmfh.bfReserved1), 2, 1, file);
	fread(&(bmfh.bfReserved2), 2, 1, file);
	fread(&(bmfh.bfOffBits), 4, 1, file);

	pos = bmfh.bfOffBits;

	fread(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, file);

	// error checking
	if (bmfh.bfType != 0x4d42) {	// "BM" actually
		fl_alert("bmfh.bfType!= 0x4d42");
		return NULL;
	}

	int code_type = 0;

	if (bmih.biBitCount == 24 || bmih.biBitCount == 32)
	{
		/*char s = static_cast<char>(bmih.biBitCount);*/

		if (bmih.biBitCount == 24) {
			code_type = 24;
		}
		else {
			code_type = 32;
		}
	}

	else {
		fl_alert("bmih.biBitCount == %d", bmih.biBitCount);
		return NULL;
	}


	/*
		if ( bmih.biCompression != BMP_BI_RGB ) {
			return NULL;
		}
	*/
	fseek(file, pos, SEEK_SET);

	width = bmih.biWidth;
	height = bmih.biHeight;



	if (code_type == 24) {
		int padWidth = width * 3;
		int pad = 0;
		if (padWidth % 4 != 0)
		{
			pad = 4 - (padWidth % 4);
			padWidth += pad;
		}
		int bytes = height * padWidth;

		unsigned char* data = new unsigned char[bytes];

		int result = fread(data, bytes, 1, file);

		if (!result) {
			fl_alert("!result");
			delete[] data;
			return NULL;
		}

		fclose(file);

		// shuffle bitmap data such that it is (R,G,B) tuples in row-major order
		int i, j;
		j = 0;
		unsigned char temp;
		unsigned char* in;
		unsigned char* out;

		in = data;
		out = data;
		for (j = 0; j < height; ++j)
		{
			for (i = 0; i < width; ++i)
			{
				out[1] = in[1];
				temp = in[2];
				out[2] = in[0];
				out[0] = temp;

				in += 3;
				out += 3;
			}
			in += pad;
		}
		return data;
	}
	else {
		int padWidth = width * 4;
		int pad = 0;
		if (padWidth % 4 != 0)
		{
			pad = 4 - (padWidth % 4);
			padWidth += pad;
		}
		int bytes = height * padWidth;

		unsigned char* data = new unsigned char[bytes];

		int result = fread(data, bytes, 1, file);

		if (!result) {
			fl_alert("!result");
			delete[] data;
			return NULL;
		}

		fclose(file);

		// shuffle bitmap data such that it is (R,G,B) tuples in row-major order
		int i, j;
		j = 0;
		//unsigned char temp;

		int tran_bytes = height * width * 3;
		unsigned char* tran = new unsigned char[tran_bytes];
		unsigned char* tran_out = NULL;
		unsigned char* data_init = NULL;
		tran_out = tran;
		data_init = data;
		for (j = 0; j < height; ++j)
		{
			for (i = 0; i < width; ++i)
			{
				tran[1] = data[1];
				tran[0] = data[2];
				tran[2] = data[0];

				data += 4;
				tran += 3;
			}
			data += pad;
		}
		delete[] data_init;
		if (tran_out == NULL) {
			fl_alert("tran == NULL");
			delete[] tran_out;
			return NULL;
		}
		return tran_out;
	}
}
 
void writeBMP(char*				iname,
			  int				width, 
			  int				height, 
			  unsigned char*	data) 
{ 
	int bytes, pad;
	bytes = width * 3;
	pad = (bytes%4) ? 4-(bytes%4) : 0;
	bytes += pad;
	bytes *= height;

	bmfh.bfType = 0x4d42;    // "BM"
	bmfh.bfSize = sizeof(BMP_BITMAPFILEHEADER) + sizeof(BMP_BITMAPINFOHEADER) + bytes;
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = /*hack sizeof(BMP_BITMAPFILEHEADER)=14, sizeof doesn't work?*/ 
					 14 + sizeof(BMP_BITMAPINFOHEADER);

	bmih.biSize = sizeof(BMP_BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BMP_BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biYPelsPerMeter = (int)(100 / 2.54 * 72);
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	FILE *outFile=fopen(iname, "wb"); 

	//	fwrite(&bmfh, sizeof(BMP_BITMAPFILEHEADER), 1, outFile);
	fwrite( &(bmfh.bfType), 2, 1, outFile); 
	fwrite( &(bmfh.bfSize), 4, 1, outFile); 
	fwrite( &(bmfh.bfReserved1), 2, 1, outFile); 
	fwrite( &(bmfh.bfReserved2), 2, 1, outFile); 
	fwrite( &(bmfh.bfOffBits), 4, 1, outFile); 

	fwrite(&bmih, sizeof(BMP_BITMAPINFOHEADER), 1, outFile); 

	bytes /= height;
	unsigned char* scanline = new unsigned char [bytes];
	for ( int j = 0; j < height; ++j )
	{
		memcpy( scanline, data + j*3*width, bytes );
		for ( int i = 0; i < width; ++i )
		{
			unsigned char temp = scanline[i*3];
			scanline[i*3] = scanline[i*3+2];
			scanline[i*3+2] = temp;
		}
		fwrite( scanline, bytes, 1, outFile);
	}

	delete [] scanline;

	fclose(outFile);
} 