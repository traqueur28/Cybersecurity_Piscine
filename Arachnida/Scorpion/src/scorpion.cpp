#include "../inc/scorpion.hpp"

static void getJPEG(std::ifstream &file) {
    (void)file;

    std::string line;
    
    while (std::getline(file, line)) {
        size_t tmp = line.find('\xFF');
        if (tmp != std::string::npos)
        {
            std::cout << "GG" << std::endl;
            if (line[tmp + 1] == '\x51')
            std::cout << "GOT" << std::endl;

        }
        else
            std::cout << "NO GG" << std::endl;

    }
}  

static bool isBMP(const std::string &filename) {
    unsigned char signature[] = {0x42, 0x4D};

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return false;
    }

    char buffer[2] = "\0";
    file.read(buffer, 2);

    // Check if 2 first bits match with PNG signature
    for (int j = 0; j < 2; j++) {
        if (static_cast<unsigned char>(buffer[j]) != signature[j]) {
            std::cout << "The file " << filename << " isn't valid BMP." << std::endl;
            file.close();
            return false;
        }
    }

    
    std::cout << "The file " << filename << " is valid BMP." << std::endl;
    file.close();
    return true;
}

static bool isGIF(const std::string &filename) {
    unsigned char signature[] = {0x47, 0x49, 0x46};

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return false;
    }

    char buffer[3] = "\0";
    file.read(buffer, 3);

    // Check if 3 first bits match with PNG signature
    for (int j = 0; j < 3; j++) {
        if (static_cast<unsigned char>(buffer[j]) != signature[j]) {
            std::cout << "The file " << filename << " isn't valid GIF." << std::endl;
            return false;
        }
    }

    
    std::cout << "The file " << filename << " is valid GIF." << std::endl;
    return true;
}

static bool isPNG(const std::string &filename) {
    unsigned char signature[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return false;
    }

    char buffer[8] = "\0";
    file.read(buffer, 8);

    // Check if 8 first bits match with PNG signature
    for (int j = 0; j < 8; j++) {
        if (static_cast<unsigned char>(buffer[j]) != signature[j]) {
            std::cout << "The file " << filename << " isn't valid PNG." << std::endl;
            return false;
        }
    }

    
    std::cout << "The file " << filename << " is valid PNG." << std::endl;
    return true;
}

static bool isJPEG(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
        return false;
    }

    char buffer[2] = "\0";
    file.read(buffer, 2);

    // Check if 2 first bits match with JPEG signature
    if (static_cast<unsigned char>(buffer[0]) == 0xFFu && static_cast<unsigned char>(buffer[1]) == 0xD8u) {
        std::cout << "The file " << filename << " is valid JPEG." << std::endl;
        getJPEG(file);
        return true;
    } else {
        std::cout << "The file " << filename << " isn't valid JPEG." << std::endl;
        return false;
    }
}

int main(int argc, char **argv)
{

    // • .jpg/jpeg  OK
    // • .png
    // • .gif
    // • .bmp
    // parsing extension OU/ET parse file type ?
    for (int i = 1; i < argc; i++)
    {
    
        if (isJPEG(argv[i]))
            ;
        else if (isPNG(argv[i]))
        {
            ;
        }
        else if (isGIF(argv[i]))
        {
            ;
        }
        else if (isBMP(argv[i]))
        {
            ;
        }
        else
            std::cout << "Unknown format." << std::endl;


    }
    
    // open file with good reader

    return (0);
}