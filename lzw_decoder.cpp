#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <unordered_map>

#include <chrono> // measuring execution time

/**
 * Implementationg of the Lempel-Ziv-Welch algorithm decoder
 * 
 * Based on Welch's 1984 paper, the algorithm decodes 12-bit
 * codes from the binary files and uses them to decode the
 * message based on a 256 entry dictionary. The dictionary
 * has a limit of 4096 and it resets to 256 entries after
 * it is filled up by the algorithm.
 * 
 * Time complexity: 
 */
class LzwDecoder {
private:
   
   /* Member and class variables variables */
   std::string file_path_;

   const int s_dictionary_init_size {256};
   const int s_dictionary_max_size  {4096};

   /* Bit manipulation functionality */
   std::unordered_map<int, std::string> initialize_dictionary() {
      std::unordered_map<int, std::string> dict;
      for (int i = 0 ; i < s_dictionary_init_size ; ++i) {
         dict[i] = std::string(1, (char) i);
      }

      return dict;
   }

   unsigned int handle_first_12_bits(unsigned int first, unsigned int mid, unsigned int mask) {
      first <<= 4;
      mid >>= 4;
      mid = mid & mask;
      first = first | mid;

      return first;
   }

   unsigned int handle_last_12_bits(unsigned int mid, unsigned int last, unsigned int mask) {
      mid = mid & mask;
      mid <<= 8;
      mid = mid | last;

      return mid;
   }

   unsigned int handle_remaining_bytes(unsigned int penultimate, unsigned int ultimate) {
      return (penultimate << 8) | ultimate;
   }

   /**
    * Building the list of 12 bit codes out of the compressed file.
    * 
    * Read file contents and parse 2x12 bit codes at a time from
    * 3 bytes using bit operations.
    * 
    * @return vector containing the parsed 12 bit codes
    */
   std::vector<unsigned int> construct_code_list() {
      
      std::vector<unsigned int> codes;

      // Open file and read contents
      std::ifstream is (file_path_, std::ifstream::binary);
      is.seekg (0, is.end);
      int length = is.tellg();
      is.seekg (0, is.beg);
      char * buffer = new char [length];
      is.read(buffer,length);
      is.close();

      // Flag for marking if there are two remaining bytes at the end
      bool b_finished_parsing = true;
      
      // Interpreting 3 bytes at a time; 3 bytes = 2x12-bit codes
      int i = 0;
      int mask = (1 << 4) - 1; // 0000 1111 mask for the last 4 bits
      while (i < length) {

         if (i + 3 > length) {
            b_finished_parsing = false;
            break;
         }

         // Getting the bytes as unsigned integer values (important to be unsigned)
         uint32_t first = (unsigned char) buffer[i];
         uint32_t mid = (unsigned char) buffer[i + 1];
         uint32_t second = (unsigned char) buffer[i + 2];

         // Handling first code
         first = handle_first_12_bits(first, mid, mask);

         // Handling second code
         second = handle_last_12_bits(mid, second, mask);

         codes.push_back(first);
         codes.push_back(second);

         i += 3;
      }

      // Handle the remaining two bytes if there are any
      if (!b_finished_parsing) {
         unsigned int remaining_byte_1 = buffer[length - 2];
         unsigned int remaining_byte_2 = buffer[length - 1];

         codes.push_back(handle_remaining_bytes(remaining_byte_1, remaining_byte_2));
      }
      
      // Clear buffer memory
      delete[] buffer;

      return codes;
   }

public:

   LzwDecoder(const std::string & _file_path) 
      : file_path_(_file_path) {}


   void setFilePath(std::string _new_file_path) {
      file_path_ = _new_file_path;
   }

   /**
    * Main decode function
    * 
    * Uses member variable file path to decode the compressed files.
    * 
    * @return string containing the decoded message
    */
   std::string decode() {
      // Build code list and decoding dictionary
      std::vector<unsigned int> codes = construct_code_list();
      std::unordered_map<int, std::string> dc = initialize_dictionary();

      // Create output buffer and initialize dictionary index
      std::string output = "";
      int index = s_dictionary_init_size;

      // Handle the first code
      std::string output_chunk = dc[codes[0]];
      output.append(output_chunk);
      dc[index] = output_chunk;

      int i = 1;

      // Decode input
      while (i < codes.size()) {
         while (index < s_dictionary_max_size) {
            output_chunk = dc[codes[i]];

            // If the code corresponds to the last entry in the dictionary
            if (codes[i] == index) {
               output_chunk += output_chunk[0];
            }

            output.append(output_chunk);
            dc[index] = dc[index] + output_chunk[0];
            ++index;
            dc[index] = output_chunk;

            ++i;

            // Exceding codes size breaks the inner loop
            if (i >= codes.size()) {
               break;
            }
         }


         // Exceeding codes size breaks the outer loop
         if (i >= codes.size()) {
            break;
         }

         // If this point is reached, reinitialize dictionary
         dc = initialize_dictionary();
         index = s_dictionary_init_size;

         output_chunk = dc[codes[i]];
         output.append(output_chunk);
         dc[index] = output_chunk;
      }

      return output;
   }
};

int main() {

   // Uncomment print statements to see the output

   LzwDecoder decoder("LzwInputData/compressedfile1.z");
   auto then = std::chrono::high_resolution_clock::now();
   decoder.decode();
   auto now = std::chrono::high_resolution_clock::now();
   auto execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
   std::cout << "Execution time - compressedfile1.z: " << execution_time << " milliseconds ... \n" ;
   // std::cout << decoder.decode() << std::endl;

   decoder.setFilePath("LzwInputData/compressedfile2.z");
   then = std::chrono::high_resolution_clock::now();
   decoder.decode();
   now = std::chrono::high_resolution_clock::now();
   execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
   std::cout << "Execution time - compressedfile2.z: " << execution_time << " milliseconds ... \n" ;
   // std::cout << decoder.decode() << std::endl;
   
   decoder.setFilePath("LzwInputData/compressedfile3.z");
   then = std::chrono::high_resolution_clock::now();
   decoder.decode();
   now = std::chrono::high_resolution_clock::now();
   execution_time = std::chrono::duration_cast<std::chrono:: milliseconds>(now - then).count();
   std::cout << "Execution time - compressedfile3.z: " << execution_time << " milliseconds ... \n" ;
   // std::cout << decoder.decode() << std::endl;
   
   decoder.setFilePath("LzwInputData/compressedfile4.z");
   then = std::chrono::high_resolution_clock::now();
   decoder.decode();
   now = std::chrono::high_resolution_clock::now();
   execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count();
   std::cout << "Execution time - compressedfile4.z: " << execution_time << " milliseconds ... \n" ;
   // std::cout << decoder.decode() << std::endl;
}