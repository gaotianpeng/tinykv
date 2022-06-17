#ifndef STORAGE_TINYKV_INCLUDE_DB_H_
#define STORAGE_TINYKV_INCLUDE_DB_H_

#include <iostream>
#include <string>

class DB {
public:
   DB()  {
       std::cout << "DB++++++++++++++++++++++++++++";
   }
   ~DB()  {
       std::cout << "DB-----------------------------";
   }
};

#endif  // STORAGE_TINYKV_INCLUDE_DB_H_