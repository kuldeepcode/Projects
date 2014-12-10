
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

#include "hmac.h"
using CryptoPP::HMAC;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::HashFilter;
using CryptoPP::HashVerificationFilter;

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

#include "secblock.h"
using CryptoPP::SecByteBlock;


int main(int argc, char* argv[])
{
  AutoSeededRandomPool prng;
  byte fkey[SHA256::DIGESTSIZE];
  string key_from_file, plain;
  byte key[AES::MAX_KEYLENGTH]; // 32 bytes
  string filename=argv[2];
  FileSource(argv[1], true,
             new HexDecoder(
                            new StringSink(key_from_file))
             );
  FileSource(argv[2], true, new StringSink(plain));

  //removing directory paths if any
  filename = filename.substr(filename.find_last_of("/")+1,filename.length());

  byte iv[AES::BLOCKSIZE]; // 16 bytes
  iv[0] = 0;
  //prng.GenerateBlock(iv, sizeof(iv));
  string temp = key_from_file+filename;
  byte digest_input[temp.length()];
  for (int i=0;i<=temp.length();i++)
    digest_input[i]=(byte)temp[i];

  SHA256().CalculateDigest(fkey, digest_input, sizeof(digest_input));

  //print key
  cout<<"key from file: "<<key_from_file<<endl;

  string cipher, encoded, recovered;

  /*
    This section implements hashbased message authentication code
    this will generate a MAC that is hexencoded and appended to the encrypted file
  */

  SecByteBlock mac_key(16);
  for (int i=0;i<sizeof(mac_key);i++){
    mac_key[i]=1;
  }
  string mac;

  try
    {
      HMAC< SHA256 > hmac(mac_key, mac_key.size());

      StringSource(plain, true,
                   new HashFilter(hmac,
                                  new StringSink(mac)
                                  ) // HashFilter
                   ); // StringSource
    }
  catch(const CryptoPP::Exception& e)
    {
      cerr << e.what() << endl;
      exit(1);
    }

  // print MAC
  encoded.clear();
  StringSource(mac, true,
               new HexEncoder(
                              new StringSink(encoded)
                              )
               );
  string encoded_mac=encoded;

  /*
    This step changes the file name before hashing it to create a new encrypted filename
    It splits the format of the file and appends before filename
    ex) example.txt becomes--> txt.example
  */
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
  /*
    This stem encrypts the plaintext using AES CTR mode
  */

  try
    {
      CTR_Mode< AES >::Encryption e;
      e.SetKeyWithIV(fkey, sizeof(fkey), iv);

      // The StreamTransformationFilter adds padding
      //  as required. ECB and CBC Mode must be padded
      //  to the block size of the cipher.
      StringSource(plain, true,
                   new StreamTransformationFilter(e,
                                                  new StringSink(cipher)
                                                  )
                   );
    }
  catch(const CryptoPP::Exception& e)
    {
      cerr << e.what() << endl;
      exit(1);
    }
  string encoded_cipher;
  //print
  encoded.clear();
  StringSource(cipher, true,
               new HexEncoder(
                              new StringSink(encoded)
                              )
               );
  encoded_cipher = encoded_mac + "\n" + encoded;

  ofstream file;
  file.open(encoded_efile);
  file<<encoded_cipher;
  file.close();
  cout<<"The encrypted filename is : "<<encoded_efile<<endl<<endl;
  return 0;
}
