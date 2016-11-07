#include "dsglImages.hpp"

#include <vector>
#include <png.h>

namespace DSGL {
	namespace Images {
		typedef struct _PngSignature {
			unsigned char Signature[8];	/* Identifier (always 89504E470D0A1A0Ah) */
		} PNGSIGNATURE;

		typedef struct _PngChunk {
			unsigned int        DataLength;	/* Size of Data field in bytes */
			unsigned char       Type[5];	/* Code identifying the type of chunk */
			unsigned char *     Data;	/* The actual data stored by the chunk */
			unsigned int        Crc;	/* CRC-32 value of the Type and Data fields */
		} PNGCHUNK;

		typedef struct _IHDRChunk {
			unsigned int  Width;		/* Width of image in pixels */
			unsigned int  Height;		/* Height of image in pixels */
			unsigned char BitDepth;		/* Bits per pixel or per sample */
			unsigned char ColorType;	/* Color interpretation indicator */
			unsigned char Compression;	/* Compression type indicator */
			unsigned char Filter;		/* Filter type indicator */
			unsigned char Interlace;	/* Type of interlacing scheme used */
		} IHDRCHUNK;

		typedef struct _PNG {
			unsigned char Signature[8];	/* Identifier (always 89504E470D0A1A0Ah) */
			PNGCHUNK        IHDR;
			PNGCHUNK        PLTE;
			PNGCHUNK        IDAT;
			PNGCHUNK        IEND;
		} PNG;

		class Png {
			public:
				Png(const char * file);
				~Png();

				void Read(const char * file);
				void Write(const char * file);

				GLenum GetInternalFormat();
				GLenum GetFormat();
				GLenum GetType();
				unsigned int Width();
				unsigned int Height();

				PNG pngStruct;
				png_bytep * rawData;
				unsigned int rawDataSize;

			private:
				int SafeFGetC(FILE * file);
		};	
	}
}
