#include "../inc/spider.hpp"

void    ft_error_parsing()
{
    std::cout << "Error: argument [-r] [-l [N]] [-p [PATH]] URL" << std::endl;
    exit(0);
}

void    ft_error_path(){
    std::cout << "Error: unvalid path or no access" << std::endl; 
    exit(0);
}

void    ft_error_curl(){
    std::cout << "Error: error from curl" << std::endl; 
    exit(0);
}

int is_valid_flag(int &i, int argc, char **argv, t_data &data)
{
    std::string flags[] = {"-r", "-l", "-p"};

    for (int j = 0; j < 3; j++){
        if (std::string(argv[i]) == flags[j]){
            switch (j){
                case 0: //-r
                    if (!data.depth)
                        data.depth = 1;
                    return 0;

                case 1: //-l
                    // if next arg isn't URL and valid number > 0
                    // change depth and i
                    // else default depth = 5
                    int tmp;
                    if (i + 1 < argc - 1 && (tmp = atoi(argv[i + 1])) > 0)
                    {
                        data.depth = tmp;
                        i++;
                    }
                    else
                        data.depth = 5;
                    return 0;

                case 2: //-p
                    // if next arg isn't URL next argv is path
                    // else default path
                    if (i + 1 < argc - 1)
                    {
                        i++;
                        data.path = argv[i];
                        if (data.path.size() - 1 != '/')
                            data.path.push_back('/');
                    }
                    else
                        data.path = "./data/";
                    return 0;
            }
        }
    }

    return -1;
}

int ft_parsing(int argc, char **argv, t_data &data)
{
    (void)argv;
    int i = 1;

    while (i < argc)
    {
        if (i == argc - 1)
            data.url = argv[i];
        else if (-1 == is_valid_flag(i, argc, argv, data))
        {
            ft_error_parsing();
        }


        i++;
    }
    return 0;
}

int ft_get_img(t_data data, std::string file)
{
    std::ifstream ifs(file.c_str());
    
    if (!ifs)
    {
        std::cout << "warning: file can't be open." << std::endl;
        return -1;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        // Processing each line
        line.erase(0, 9);
        line.erase(line.end() - 1);

        // check if full or partial path
        if (line.compare(0, 4, "http"))
            line.insert(0, data.url);
            //line.insert(0, "/").insert(0, data.url);
            

        // std::cout << "Ligne lue :" << line << std::endl;
        std::string n_file = line;

        // Replace '/' with ' '
        for (size_t i = 0; i < n_file.size(); i++)
        {
            if (n_file[i] == '/')
                n_file[i] = '_';
        }

        // Replace space with %20
        for (size_t i = 0; i < line.size(); i++)
        {
            if (line[i] == ' ')
                line.replace(i, 1, "%20");
        }

        std::string cmd_tmp = "curl -s -o \"" + data.path + n_file + "\" \"" + line + "\"";
        std::cout << cmd_tmp << std::endl;
        system(cmd_tmp.c_str());
    }
    
    return 0;
}

bool    ft_check_ishostname(std::string &url)
{
    size_t  res = url.find('/', url.find_first_not_of('/', url.find('/', 0)));

    if (res == std::string::npos || res == url.size() - 1)
    {
        if (res != std::string::npos)
            std::cout << "type http:///qweasd.com/"<< std::endl;
        else
        {
            url.push_back('/');
            std::cout << "type http:///qweasd.com"<< std::endl;
        }
        return true;
    }
    if (url.find_first_not_of('/', res) == std::string::npos)
    {
        std::cout << "type http:///qweasd.com////" << std::endl;
        return true;
    }

    std::cout << "type http://qweqwe.com///asd" << std::endl;

    return false;
}

int ft_extract(t_data data, int depth)
{
    // std::cout << "---------" << std::endl;
    // std::cout << "data.url: " << data.url << std::endl;
    // std::cout << "data.path: " << data.path << std::endl;
    // std::cout << "depth: " << depth << std::endl;

    // check if hostname
    // -> yes : just concat
    // -> no : del last /xxxxx.x (after last '/') for url_child and get_img
    bool ishostname = ft_check_ishostname(data.url);
    (void)ishostname;

    // Replace space with %20
    for (size_t i = 0; i < data.url.size(); i++)
    {
        if (data.url[i] == ' ')
            data.url.replace(i, 1, "%20");
    }

    std::string url = "curl -s -o " + data.path + "/page \"" + data.url + "\"";
    std::string page = data.path + "/page";
    std::string file_img = data.path + "/img";
    std::string file_href = data.path + "/href" + std::to_string(depth);

    // std::cout << "url = " << url << std::endl;
    //      get data
    if (system(url.c_str()))
        ft_error_curl();

    //      grep all link and img with correct suffix
    std::string cmd_href = "grep -o \'a href=\"[^\"]*\"\' " + page + " | sort | uniq > " + file_href;
    std::string cmd_imge = "grep -o \'img src=\"[^\"]*\\.\\(jpg\\|jpeg\\|png\\|gif\\|bmp\\)\"\' " + page + " | sort | uniq > " + file_img;

    // std::cout << cmd_imge << std::endl;

    if (system(cmd_href.c_str()))
        ft_error_curl();
    system(cmd_imge.c_str());

    std::cout << "Pre:" << data.url << std::endl;


    // not hostname del last part
    if (!ishostname)
        data.url.erase(data.url.find_last_of('/') + 1);

    std::cout << "Aft:" << data.url << std::endl;

    // DL img
    // jpg/jpeg/pnj/gif/bmp
    ft_get_img(data, file_img);

    // Command delete store file to clear ang get only img
    std::string del_cmd = "rm -f " + file_href + " " + file_img + " " + page;

    // check depth, leave or go deeper
    if (depth >= data.depth)
    {
        system(del_cmd.c_str());
        return 0;
    }

    // Go deeper
    // get next link
    std::ifstream ifs(file_href);
    
    if (!ifs)
    {
        std::cout << "error: href file can't be open." << std::endl;
        return 0;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        // Processing each line
        line.erase(0, 8).erase(line.end() - 1);

        // std::cout << line << std::endl;

        if (line.compare(0, 4, "http"))
            line.insert(0, data.url);
        // std::cout << line << std::endl;


        t_data  data_child = data;
        data_child.url = line;
        
        ft_extract(data_child, depth + 1);
    }


    // fin fichier data suppr
    system(del_cmd.c_str());
    return 0;
}

int main(int argc, char **argv)
{
    // Init data struct
    t_data  data;

    data.url = "";
    data.path = "./data/";
    data.depth = 0;

    // ------

    if (argc < 2)
        ft_error_parsing();
    else
    {
        // parse our arguments
        ft_parsing(argc, argv, data);

        std::cout << "data.url: " << data.url << std::endl;
        std::cout << "data.path: " << data.path << std::endl;
        std::cout << "data.depth: " << data.depth << std::endl;

        // create path
        std::string path = "mkdir -p " + data.path;

        if (system(path.c_str()))
            ft_error_path();

        // start
        ft_extract(data, 0);
    }

    return 0;
}