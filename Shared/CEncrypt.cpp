#include "CEncrypt.h"
#include <vector>
using namespace std;
#include "uni.h"

//def this to perform extra data integrity checks
//#define C_EXTRA_DEBUG_CHECKS

#ifdef _ENABLE_MEMORY_DEBUGGING_
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 


CEncrypt::CEncrypt()
{
  //init everything
}

CEncrypt::~CEncrypt()
{
 //clean up
}

unsigned int CEncrypt::encrypt_piece(byte data[], unsigned int i_size, int i_key)
{

#ifdef C_EXTRA_DEBUG_CHECKS
    vector<byte> debugBuff;
    debugBuff.reserve(i_size);
    memcpy(&debugBuff[0], data, i_size);
#endif

    unsigned int i_checksum = 0;

	for (unsigned int i = 0; i < i_size; i++)
   {
	  data[i] = data[i]+2+i_key+i;
	  i_checksum += data[i]+i_key+i;
   }

#ifdef C_EXTRA_DEBUG_CHECKS
 //make sure we can decrypt it accurately
   vector<byte> debugEncrypted;
   debugEncrypted.reserve(i_size);
   memcpy(&debugEncrypted[0], data, i_size);

   //decrypt it
   int decryptChecksum = decrypt_piece(&debugEncrypted[0], i_size, i_key);

   //compare it to the original
   if (memcmp(&debugEncrypted[0], &debugBuff[0], i_size) != 0)
   {
       assert(!"Decryped buffer doesn't match the original");
   }

   assert(decryptChecksum == i_checksum && "Checksums don't match");

#endif

  return i_checksum;
}

unsigned int CEncrypt::decrypt_piece(byte data[], unsigned int i_size, int i_key)
{
   	unsigned int i_checksum = 0;

	for (unsigned int i = 0; i < i_size; i++)
   {
	  i_checksum += data[i]+i_key+i;
	  data[i] = data[i]-(2+i_key+i);
   }

  return i_checksum;
}

bool CEncrypt::encrypt_file(char st_input[], char st_output[], int i_key)
{
	  FILE * fp;
 	  FILE * fp_in; //input file

      const int i_chunk_size = 1024;
	  byte byte_piece[i_chunk_size];
	

	  unsigned int i_checksum = 0;

	  if ( (fp_in=fopen(st_input, "rb")) == NULL)
	 {
	  LogMsg(_T("Error opening %s.  Out of HD space or something?"), st_input);
	  return false;
	 }

      size_t i_read;

	  
	  while ( ((i_read = fread(&byte_piece,sizeof( byte ),i_chunk_size,fp_in)) > 0))
	  {
	    i_checksum += encrypt_piece((unsigned char*) &byte_piece, i_read, i_key);
	  }

	  //finished
	  fclose(fp_in);

	  
	  if ( (fp_in=fopen(st_input, "rb")) == NULL)
	 {
	  LogMsg(_T("Error opening %s.  Out of HD space or something?"), uni(st_input).GetAuto());
	  return false;
	 }

	  if ( (fp=fopen(st_output, "wb")) == NULL)
	 {
	  LogMsg(_T("Error creating %s.  Out of HD space or something?"), uni(st_output).GetAuto());
	  return false;
	 }
	  
	  //write checksum
	  if (fwrite(&i_checksum, sizeof(unsigned int), 1, fp) <= 0)
	  {
	   LogMsg(_T("Error writing"));
	  }
	
	  while ( ((i_read = fread(&byte_piece,sizeof( byte ),i_chunk_size,fp_in)) > 0))
	  {
	   
          encrypt_piece((unsigned char*) &byte_piece, i_read, i_key);
          
          if (fwrite(&byte_piece, sizeof(byte), i_read, fp) <= 0)
          {
              LogMsg(_T("Error writing file.."));
              fclose(fp_in);
              fclose(fp);
              return false;
          }
          
      }
      
      fclose(fp);
	  fclose(fp_in);

 
	//success
	return true;
}

bool CEncrypt::decrypt_file(char st_input[], char st_output[], int i_key)
{
	  FILE * fp;
 	  FILE * fp_in; //input file
      size_t i_read;
      const int i_chunk_size = 1024;
	  byte byte_piece[i_chunk_size];
	
	  unsigned int i_orig_checksum = 0;
	  unsigned int i_checksum = 0;

   if ( (fp_in=fopen(st_input, "rb")) == NULL)
	 {
	  LogMsg(_T("Error opening %s.  Out of HD space or something?"), uni(st_input).GetAuto());
	  return false;
	 }

	  if ( (fp=fopen(st_output, "wb")) == NULL)
	 {
	  LogMsg(_T("Error creating %s.  Out of HD space or something?"), uni(st_output).GetAuto());
	  return false;
	 }
	  



	  //Read checksum
	  if (fread(&i_orig_checksum,sizeof( unsigned int ),1,fp_in) < 1)
	  {
	   LogMsg(_T("Error reading"));
	   return false;
	  }
	
	  while ( ((i_read = fread(&byte_piece,sizeof( byte ),i_chunk_size,fp_in)) > 0))
	  {
	   
	  	i_checksum += decrypt_piece((unsigned char*) &byte_piece, i_read, i_key);
		
	  if (fwrite(&byte_piece, sizeof(byte), i_read, fp) <= 0)
	 {
		LogMsg(_T("Error writing file.."));
		fclose(fp_in);
		fclose(fp);
		return false;
	 }

	  
	  }
	 
	
	  fclose(fp);
	  fclose(fp_in);
	 if (i_checksum != i_orig_checksum) return false; //checksums don't match
 
	//success
	return true;
}

int CEncrypt::reduce(unsigned long ul_reduce)
{
  char st_temp[255];
 sprintf(st_temp, "%u",ul_reduce);
 //shouldn't this have been ul not u?  oh well, can't change it now, would ruin
 //some reg code stuff that's already out there
 //  sprintf(st_temp, "%ul",ul_reduce);

  int i_length = strlen(st_temp);
  int i_sum = 0;
  for (int i=0; i < i_length; i++)
  {
	  i_sum += st_temp[i];
  }

  return i_sum;
}

int CEncrypt::reduce_fixed(unsigned long ul_reduce)
{
  char st_temp[255];
 sprintf(st_temp, "%u",ul_reduce);
 //shouldn't this have been ul not u?  oh well, can't change it now, would ruin
 //some reg code stuff that's already out there
 //  sprintf(st_temp, "%ul",ul_reduce);

  int i_length = strlen(st_temp);
  int i_sum = 0;
  for (int i=0; i < i_length; i++)
  {
	  i_sum += st_temp[i];
  }

  return i_sum;
}


