#include "dsglPng.hpp"

namespace DSGL {
	namespace Images {
		Png::Png(const char * file) {
			Read(file);
		}

		Png::~Png() {
			delete[] this->pngStruct.IHDR.Data;
			delete[] this->rawData;
		}

		void Png::Read(const char * file) {
			FILE * inputImage = fopen(file,"rb");

			if (inputImage == NULL) {
				fclose(inputImage);
				throw Exception(DSGL_IMAGES_CANNOT_OPEN_IMAGE_FILE, DSGL_IMAGES_MSG_CANNOT_OPEN_IMAGE_FILE, file);
			}

			try {
				for (int i=0;i<8;i++) {
					this->pngStruct.Signature[i] = (unsigned char) SafeFGetC(inputImage);
				}

				this->pngStruct.IHDR.DataLength =  SafeFGetC(inputImage) << 24;
				this->pngStruct.IHDR.DataLength += SafeFGetC(inputImage) << 16;
				this->pngStruct.IHDR.DataLength += SafeFGetC(inputImage) << 8;
				this->pngStruct.IHDR.DataLength += SafeFGetC(inputImage);

				this->pngStruct.IHDR.Type[0] = (unsigned char) SafeFGetC(inputImage);
				this->pngStruct.IHDR.Type[1] = (unsigned char) SafeFGetC(inputImage);
				this->pngStruct.IHDR.Type[2] = (unsigned char) SafeFGetC(inputImage);
				this->pngStruct.IHDR.Type[3] = (unsigned char) SafeFGetC(inputImage);
				this->pngStruct.IHDR.Type[4] = 0;

				this->pngStruct.IHDR.Data = new unsigned char[this->pngStruct.IHDR.DataLength];
	
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Width = (unsigned int) (SafeFGetC(inputImage) << 24);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Width += (unsigned int) (SafeFGetC(inputImage) << 16);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Width += (unsigned int) (SafeFGetC(inputImage) << 8);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Width += (unsigned int) SafeFGetC(inputImage);

       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Height = (unsigned int) (SafeFGetC(inputImage) << 24);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Height += (unsigned int) (SafeFGetC(inputImage) << 16);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Height += (unsigned int) (SafeFGetC(inputImage) << 8);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Height += (unsigned int) SafeFGetC(inputImage);
				
				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->BitDepth = (unsigned char) SafeFGetC(inputImage);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->ColorType = (unsigned char) SafeFGetC(inputImage);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Compression = (unsigned char) SafeFGetC(inputImage);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Filter = (unsigned char) SafeFGetC(inputImage);
       				((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Interlace = (unsigned char) SafeFGetC(inputImage);

       				this->pngStruct.IHDR.Crc = (unsigned char) SafeFGetC(inputImage) << 24;
       				this->pngStruct.IHDR.Crc += (unsigned char) SafeFGetC(inputImage) << 16;
       				this->pngStruct.IHDR.Crc += (unsigned char) SafeFGetC(inputImage) << 8;
       				this->pngStruct.IHDR.Crc += (unsigned char) SafeFGetC(inputImage);
			}

			catch(DSGL::Exception e) {
				fclose(inputImage);
				if (e.code == DSGL_IMAGES_INPUT_IMAGE_CORRUPTED) {
					throw DSGL::Exception(DSGL_IMAGES_INPUT_IMAGE_CORRUPTED, DSGL_IMAGES_MSG_INPUT_IMAGE_CORRUPTED, file);
				}
			}
			
			fclose(inputImage);

			if ( ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->BitDepth != 8) {
				throw Exception(DSGL_IMAGES_UNSUPPORTED_BIT_DEPTH, DSGL_IMAGES_MSG_UNSUPPORTED_BIT_DEPTH);
			}
			
			png_image image;
			memset(&image, 0, (sizeof image));
			image.version = PNG_IMAGE_VERSION;

			if (png_image_begin_read_from_file(&image, file) != 0 ) {

				switch ( ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->ColorType) {
					case 2:
						image.format = PNG_FORMAT_RGB;
						break;
					case 6:
						image.format = PNG_FORMAT_RGBA;
						break;
						
				  	default:
						throw Exception(DSGL_IMAGES_UNSUPPORTED_COLOR_TYPE, DSGL_IMAGES_MSG_UNSUPPORTED_COLOR_TYPE);
				}
				this->rawData = new png_bytep[PNG_IMAGE_SIZE(image)];
				if (this->rawData != NULL && png_image_finish_read(&image, NULL, this->rawData, 0, NULL) !=0) {
					this->rawDataSize = PNG_IMAGE_SIZE(image);
                		}
        		}
		}

		GLenum Png::GetInternalFormat() {
			switch ( ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->ColorType) {
				case 2:
					return GL_RGB8;
				default:
					return GL_RGBA8;
			}
		}
		GLenum Png::GetFormat() {
			switch ( ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->ColorType) {
				case 2:
					return GL_RGB;
				default:
					return GL_RGBA;
			}
		}

		GLenum Png::GetType() {
			return GL_UNSIGNED_BYTE;
		}

		unsigned int Png::Width() {
			return ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Width;		
		}

		unsigned int Png::Height() {
			return ((IHDRCHUNK *) (this->pngStruct.IHDR.Data))->Height;
		}

		int Png::SafeFGetC(FILE * file) {
			if (feof(file)) {
				throw DSGL::Exception(DSGL_IMAGES_INPUT_IMAGE_CORRUPTED, DSGL_IMAGES_MSG_INPUT_IMAGE_CORRUPTED);
			}
			return fgetc(file);
		}
	}
}

