#include "huffman_tree.h"

int main(int argc, char *argv[]) {
    const char *options = "ed";
    char option = getopt(argc, argv, options);

    if (option == '?') {
        return 1;
    }
    if (getopt(argc, argv, options) != -1) {
        printf("Only one option allowed.\n");
        return 2;
    }
    if (argc != 4) {
        printf("Usage: ./main <inputFile> -<option> <outputFile>\n");
        return 3;
    }

    std::string inputFileName = argv[optind];
    std::string outputFileName = argv[optind + 1];
    if (option == 'e') {
        HuffmanTree huffmanEncoding(inputFileName);
        huffmanEncoding.encode(inputFileName, outputFileName);
        print(inputFileName, outputFileName);
    } else if (option == 'd') {
        HuffmanTree huffmanDecoding;
        huffmanDecoding.decode(inputFileName, outputFileName);
    } else {
        printf("Usage: ./main <inputFile> -<option> <outputFile>\n");
    }
    return 0;
}