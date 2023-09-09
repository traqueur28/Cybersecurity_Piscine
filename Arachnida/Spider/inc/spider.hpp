#ifndef SPIDER_HPP
#define SPIDER_HPP

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>

typedef struct   s_data
{
    std::string url;
    std::string path;
    int depth;
}   t_data;

#endif