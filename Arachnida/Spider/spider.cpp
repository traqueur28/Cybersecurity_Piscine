#include <iostream>
#include <fstream>
#include <cstdlib>


int main(int argc, char **argv)
{
    std::cout << argc << ": " << std::endl;
    for (int i = 0; i < argc; i++)
        std::cout << *(argv + i) << std::endl;

    std::string url;
    std::string path = "./data/";
    // unsigned int    depth = 5;

    // 2 = [URL]
    // 3 = -r [URL]
    // 4 = -p [PATH] [URL]
    // 5 = -r -l [N] [URL]
    // 5 = -r -p [PATH] -l [N] [URL]

    if (argc < 2 || argc > 5)
        std::cout << "Error: argc" << std::endl;
    else
    {
        int res = system("curl -o data https://profile.intra.42.fr/users/auth/keycloak_student");
        std::cout << "res = " << res << std::endl;

        // get data
        // lire data
        // si lien ouvrir dans data+1
        //      retour depart
        // si image curl -o image_X [URL_IMG]
        // fin fichier data suppr
    }

    return 0;
}
