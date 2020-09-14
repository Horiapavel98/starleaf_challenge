import time 

from os import stat                                 # Importing just the necessary functionality from os
from bitstring import ConstBitStream

class LZWDecoder:
    """
    Lempel-Ziv-Welch algorithm decoder
    """

    def __init__(self, file_path):
        """ Initialize compressed file path and dictionary limits """
        self.file_path = file_path
        
        self.dictionary_init_size = 256
        self.max_dictionary_size = 4096


    def __construct_code_list(self, file_path):
        """
        Read 12 bits at a time from the file with the provided path

        Parameters:

            file_path (str): path to the compressed file

        """
        code_list = list()

        _bytes_stream = ConstBitStream(bytes=open(file_path, 'rb').read())
        # Use os.stat function to get the size in bytes of the compressed file
        _size_in_bytes = stat(file_path).st_size
        _size_in_12bit_chunks = int(_size_in_bytes / 3) * 2

        for i in range(_size_in_12bit_chunks):
            code_list.append(_bytes_stream.read('uint:12'))
        
        if _size_in_bytes % 3 != 0:
            code_list.append(_bytes_stream.read('uint:16'))

        return code_list

    def __initialize_dictionary(self):
        """ Initialize the default (256 entries) dictionary for decoding."""
        return {i: str(chr(i)) for i in range(self.dictionary_init_size)}
    
    def update_file_path(self, new_file_path):
        """
        Update the path pointing to the compressed file

        Parameters:

            new_file_path (str): new path pointing to the compress file

        """
        self.file_path = new_file_path

    def decode(self):
        """ Decode compressed file and return output as a string. """
        # Get 12 bit codes and initialize dictionary
        codes = self.__construct_code_list(self.file_path)
        dc = self.__initialize_dictionary()
        
        # Create output buffer and initialize index of last available position in dictionary
        output = list()
        index = list(dc)[-1] + 1

        # Handle first code
        output_chunk = dc[codes[0]]
        output.append(output_chunk)
        dc[index] = output_chunk

        i = 1

        while i < len(codes):
            while index < self.max_dictionary_size:
                code = codes[i]
                output_chunk = dc[code]
                
                if code == index:
                    output_chunk = output_chunk + output_chunk[0]
                
                output.append(output_chunk)
                dc[index] = dc[index] + output_chunk[0]
                index = index + 1
                dc[index] = output_chunk

                i = i + 1

                if i >= len(codes):
                    break

            if i >= len(codes):
                break

            # If this point is reached, a new dictionary needs to be initiated
            dc = self.__initialize_dictionary()
            index = list(dc)[-1] + 1
            
            # Handle the next code and then proceed
            output_chunk = dc[codes[i]]
            output.append(output_chunk)
            dc[index] = output_chunk

        return str().join(output)



if __name__ == "__main__":

    # Uncomment the print statements to see the decoded output

    decoder = LZWDecoder('LzwInputData/compressedfile1.z')
    start_time = time.time()
    decoder.decode()
    print("Execution time - compressedfile1.z: " + str((time.time() - start_time)) + " seconds...")
    # print(decoder.decode())

    decoder.update_file_path('LzwInputData/compressedfile2.z')
    start_time = time.time()
    decoder.decode()
    print("Execution time - compressedfile2.z: " + str((time.time() - start_time)) + " seconds...")
    # print(decoder.decode())

    decoder.update_file_path('LzwInputData/compressedfile3.z')
    start_time = time.time()
    decoder.decode()
    print("Execution time - compressedfile3.z: " + str((time.time() - start_time)) + " seconds...")
    # print(decoder.decode())

    decoder.update_file_path('LzwInputData/compressedfile4.z')
    start_time = time.time()
    decoder.decode()
    print("Execution time - compressedfile4.z: " + str((time.time() - start_time)) + " seconds...")
    # print(decoder.decode())
