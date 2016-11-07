#include "dsglPng.hpp"

namespace DSGL {
	namespace Images {
		Png::Png(const char * file) {
			Read(file);
		}

		Png::~Png() {
		}

		void Png::Read(const char * file) {
			FILE * inputImage = fopen(file,"rb");

			if (inputImage == NULL) {
				fclose(inputImage);
				throw Exception(DSGL_IMAGES_CANNOT_OPEN_IMAGE_FILE, DSGL_IMAGES_MSG__CANNOT_OPEN_IMAGE_FILE, file);
			}

			try {
				for (int i=0;i<8;i++) {
					this->pngStruct->Signature[i] = (unsigned char) fgetc(inputImage);
				}

				this->pngStruct->IHDR.DataLength = (unsigned char) fgetc(inputImage) << 24;
				this->pngStruct->IHDR.DataLength += (unsigned char) fgetc(inputImage) << 16;
				this->pngStruct->IHDR.DataLength += (unsigned char) fgetc(inputImage) << 8;
				this->pngStruct->IHDR.DataLength += (unsigned char) fgetc(inputImage);

				this->pngStruct->IHDR.Type[0] = (unsigned char) fgetc(inputImage);
				this->pngStruct->IHDR.Type[1] = (unsigned char) fgetc(inputImage);
				this->pngStruct->IHDR.Type[2] = (unsigned char) fgetc(inputImage);
				this->pngStruct->IHDR.Type[3] = (unsigned char) fgetc(inputImage);
				this->pngStruct->IHDR.Type[4] = 0;

				this->pngStruct->IHDR.Data = (unsigned char *) malloc(sizeof( unsigned char ) * pngStruct->IHDR.DataLength);
	
				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Width = (unsigned char) fgetc(inputImage) << 24;
				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Width += (unsigned char) fgetc(inputImage) << 16;
				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Width += (unsigned char) fgetc(inputImage) << 8;
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Width += (unsigned char) fgetc(inputImage);

       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Height = (unsigned char) fgetc(inputImage) << 24;
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Height += (unsigned char) fgetc(inputImage) << 16;
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Height += (unsigned char) fgetc(inputImage) << 8;
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Height += (unsigned char) fgetc(inputImage);

       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->BitDepth = (unsigned char) fgetc(inputImage);
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->ColorType = (unsigned char) fgetc(inputImage);
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Compression = (unsigned char) fgetc(inputImage);
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Filter = (unsigned char) fgetc(inputImage);
       				((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Interlace = (unsigned char) fgetc(inputImage);

       				this->pngStruct->IHDR.Crc = (unsigned char) fgetc(inputImage) << 24;
       				this->pngStruct->IHDR.Crc += (unsigned char) fgetc(inputImage) << 16;
       				this->pngStruct->IHDR.Crc += (unsigned char) fgetc(inputImage) << 8;
       				this->pngStruct->IHDR.Crc += (unsigned char) fgetc(inputImage);
			}
			catch(DSGL::Exception e) {
				fclose(inputImage);
				if (e.code == DSGL_IMAGES_INPUT_IMAGE_CORRUPTED) {
					throw DSGL::Exception(DSGL_IMAGES_INPUT_IMAGE_CORRUPTED, DSGL_IMAGES_MSG_INPUT_IMAGE_CORRUPTED, file);
				}
			}

			fclose(inputImage);
		}

		unsigned int Png::Width() {
			return ((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Width;		
		}

		unsigned int Png::Height() {
			((IHDRCHUNK *) (this->pngStruct->IHDR.Data))->Height
		}

		unsigned char Png::SafeFGetC(FILE * file) {
			if (feof(file)) {
				throw DSGL::Exception(DSGL_IMAGES_INPUT_IMAGE_CORRUPTED, DSGL_IMAGES_MSG_INPUT_IMAGE_CORRUPTED);
			}
			return (unsigned char) fgetc(file);
		}
	}
}

