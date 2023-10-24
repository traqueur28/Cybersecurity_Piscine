#include "../inc/scorpion.hpp"

static void getExif(const std::string &filename) {
    // Ouvrir l'image
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(filename.c_str());

    if (image.get() == 0) {
        std::cerr << "Impossible d'ouvrir l'image." << std::endl;
        return ;
    }

    try {
        // Lire les métadonnées/EXIF
        image->readMetadata();

        // Accéder aux métadonnées EXIF
        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty()) {
            std::cerr << "Aucune métadonnée EXIF trouvée dans l'image." << std::endl;
        } else {
            // Parcourir et afficher les métadonnées EXIF
            for (Exiv2::ExifData::const_iterator it = exifData.begin(); it != exifData.end(); ++it) {
                std::cout << it->key() << " : " << it->value().toString() << std::endl;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return ;
    }

    return ;
}

static void getJPEG(const std::string &filename) {
    FILE    *file = fopen(filename.c_str(), "rb");

    if (file == nullptr) {
        std::cerr << "Error: Failed to open the file." << std::endl;
        return;
    }

    try {
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;

        // Initialize the JPEG decompression object
        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);

        // Specify the source file
        jpeg_stdio_src(&cinfo, file);

        // Read the JPEG header
        jpeg_read_header(&cinfo, TRUE);

        // Get the image dimensions
        std::cout << "width: " << cinfo.image_width << std::endl;
        std::cout << "height: " << cinfo.image_height << std::endl;

        // Clean up and close the file
        jpeg_destroy_decompress(&cinfo);
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    fclose(file);
}


static void getPNG(const std::string &filename) {
    FILE    *file = fopen(filename.c_str(), "rb");

    if (file == nullptr) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier." << std::endl;
        return ;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        std::cerr << "Erreur : Impossible de créer la structure de lecture PNG." << std::endl;
        fclose(file);
        return ;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        std::cerr << "Erreur : Impossible de créer la structure d'informations PNG." << std::endl;
        png_destroy_read_struct(&png, nullptr, nullptr);
        fclose(file);
        return ;
    }

    if (setjmp(png_jmpbuf(png))) {
        std::cerr << "Erreur : Erreur lors de la lecture du fichier PNG." << std::endl;
        png_destroy_read_struct(&png, &info, nullptr);
        fclose(file);
        return ;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);

    std::cout << "Largeur : " << width << std::endl;
    std::cout << "Hauteur : " << height << std::endl;

    png_destroy_read_struct(&png, &info, nullptr);
    fclose(file);
}


static void getGIF(const std::string &filename) {
    GifFileType* gif = DGifOpenFileName(filename.c_str(), nullptr);

    if (gif == nullptr) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier GIF." << std::endl;
        return ;
    }

    if (DGifSlurp(gif) != GIF_OK) {
        std::cerr << "Erreur : Impossible de lire le fichier GIF." << std::endl;
        DGifCloseFile(gif, nullptr);
        return ;
    }

    int width = gif->SWidth;
    int height = gif->SHeight;

    std::cout << "Largeur : " << width << std::endl;
    std::cout << "Hauteur : " << height << std::endl;

    DGifCloseFile(gif, nullptr);

    return ;
}

#pragma pack(push, 1) // Désactive l'alignement de mémoire pour les structures BMP

struct BMPHeader {
    char signature[2];
    std::int32_t fileSize;
    std::int16_t reserved1;
    std::int16_t reserved2;
    std::int32_t dataOffset;
    std::int32_t infoHeaderSize;
    std::int32_t imageWidth;
    std::int32_t imageHeight;
    std::int16_t planes;
    std::int16_t bitsPerPixel;
};

#pragma pack(pop) // Rétablit l'alignement de mémoire par défaut

static void getBMP(const std::string &filename) {
    std::ifstream bmpFile(filename.c_str(), std::ios::binary);

    if (!bmpFile.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier BMP." << std::endl;
        return ;
    }

    BMPHeader header;
    bmpFile.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        std::cerr << "Erreur : Le fichier n'est pas au format BMP." << std::endl;
        bmpFile.close();
        return ;
    }

    int width = header.imageWidth;
    int height = header.imageHeight;

    std::cout << "Largeur : " << width << std::endl;
    std::cout << "Hauteur : " << height << std::endl;

    bmpFile.close();
}



void    get_data(const std::string &filename, std::ifstream &file, int type) {
    (void)filename;
    (void)file;
    (void)type;

    switch (type)
    {
    case 0:
        getJPEG(filename);
        break;
    
    case 1:
        getPNG(filename);    // PNG
        break;
    
    case 2:
        getGIF(filename);    // GIF
        break;
    
    case 3:
        getBMP(filename);    // BMP
        break;
    
    default:
        break;
    }

    getExif(filename);

}
