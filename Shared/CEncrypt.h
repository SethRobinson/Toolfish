//CEncrypt, able to encrypt and decrypt files using a key
//By Seth A. Robinson 2001
#pragma once

#include "all.h"
#include "stdio.h"

class CEncrypt
{
  public:

      CEncrypt();
      ~CEncrypt();

      bool encrypt_file(char st_input[], char st_output[], int i_key);
      unsigned int encrypt_piece(byte data[], unsigned int i_size, int i_key);
      unsigned int decrypt_piece(byte data[], unsigned int i_size, int i_key);
      bool decrypt_file(char st_input[], char st_output[], int i_key);
      int reduce(unsigned long ul_reduce);
      int reduce_fixed(unsigned long ul_reduce);
};
