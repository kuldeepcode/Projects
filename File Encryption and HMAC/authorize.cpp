
#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;
#include <iomanip>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
#include <sstream>
#include <string>
using std::string;
#include <cstring>
#include <vector>
#include <iterator>

#include <cstdlib>
using std::exit;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include"files.h"
using CryptoPP::FileSink;
using CryptoPP::FileSource;

#include "aes.h"
using CryptoPP::AES;

#include "sha.h"
using CryptoPP::SHA256;

#include "ccm.h"
using CryptoPP::CTR_Mode;

#include "assert.h"
using namespace std;


int main(int argc, char* argv[])
{
  AutoSeededRandomPool prng;
  byte fkey[SHA256::DIGESTSIZE];
  string key_from_file, plain;
  byte key[AES::MAX_KEYLENGTH]; // 32 bytes
  string filename=argv[2];
  string share_fkey;
  FileSource(argv[1], true,
             new HexDecoder(
                            new StringSink(key_from_file))
             );
  //removing directory paths if any
  filename = filename.substr(filename.find_last_of("/")+1,filename.length());

  byte iv[AES::BLOCKSIZE]; // 16 bytes
  iv[0] = 0;
  //prng.GenerateBlock(iv, sizeof(iv));
  string temp = key_from_file+filename;
  byte digest_input[temp.length()];
  for (int i=0;i<=temp.length();i++)
    digest_input[i]=temp[i];

  SHA256().CalculateDigest(fkey, digest_input, sizeof(digest_input));


  StringSource(fkey, sizeof(fkey),true,
               new HexEncoder(
                              new StringSink(share_fkey))
               );

  cout<<"fkey to share : "<<share_fkey<<endl;

  string new_filename = filename.substr(filename.find_last_of(".")+1,filename.length()) + '.' + filename.substr(0,filename.find_last_of("."));
  byte efile[SHA256::DIGESTSIZE];
  string encoded_efile;
  byte filename_plain_input[new_filename.length()];
  for (int i=0;i<=new_filename.length();i++)
    filename_plain_input[i]=(byte)new_filename[i];

  SHA256().CalculateDigest(efile, filename_plain_input, sizeof(filename_plain_input));
  StringSource(efile, sizeof(efile), true,
               new HexEncoder(
                              new StringSink(encoded_efile)
                              ) // HexEncoder
               ); // StringSource
  cout<<"the filename on cloud server : "<<encoded_efile<<endl<<endl;
  return 0;
}
