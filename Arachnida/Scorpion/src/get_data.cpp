#include "../inc/scorpion.hpp"
#include <exif.h>

static int extract_EXIF(std::ifstream &file) {
    char    c1 = '\0';
    char    c2 = '\0';
    size_t  res = 1;

    (void)file;

    // 010d : string : name
    // 010e : string : decrip

    // 0132	: string : ModifyDate
    // 9003	: string : DateTimeOriginal

    // get type
    if (file)
        file.read(&c1, 1);
    while (file) {
        file.read(&c2, 1);
        res++;
        // if ( (c1 == '\x01' && (c2 == '\x0d' || c2 == '\x0e' || c2 == '\x32'))
        //     || c1 == '\x90' && c2 == '\x03') { // string

        // }

        c1 = c2;
    }

    return res;
}


static void getJPEG(const std::string &filename, std::ifstream &file) {
    size_t  size = 2;
    char    buff_0 = '\0';
    char    buff_1 = '\0';

    // find EXIL
    
    file.read(&buff_0, 1);
    while (file) {
        file.read(&buff_1, 1);
        size++;
        if (buff_0 == '\xff' && buff_1 == '\xe1') {
            std::cout << "GOT EXIF";
            size += extract_EXIF(file);
            break;
        }
        buff_0 = buff_1;
    }

    // After EXIL find or not

    // Open file with fopen
    FILE *file_bis = fopen(filename.c_str(), "rb");
    if (!file_bis) {
        std::cerr << "Error when open file." << std::endl;
        return;
    }
    // Get some meta data
    struct jpeg_decompress_struct cinfo;
    (void)cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, file_bis);
    jpeg_read_header(&cinfo, TRUE);

    std::cout << "size : " << size << " octets " << std::endl;
    std::cout << "width  : " << cinfo.image_width << " pixels" << std::endl;
    std::cout << "height : " << cinfo.image_height << " pixels" << std::endl;

    std::cout << "components : " << cinfo.num_components << " color(s)" << std::endl;

    std::cout << "densityX: " << cinfo.X_density << " px" << std::endl;
    std::cout << "densityY: " << cinfo.Y_density << " px" << std::endl;



    // Autres opérations liées à libjpeg ici

    // Si une exception survient dans cette partie, elle sera attrapée ci-dessous



    std::cout << "end\n";
} 


    void    get_data(const std::string &filename, std::ifstream &file, int type) {
    (void)filename;
    (void)file;
    (void)type;

    switch (type)
    {
    case 0:
        getJPEG(filename, file);
        break;
    
    case 1:
        getJPEG(filename, file);    // PNG
        break;
    
    case 2:
        getJPEG(filename, file);    // GIF
        break;
    
    case 3:
        getJPEG(filename, file);    // BMP
        break;
    
    default:
        break;
    }

}
