/*
 * Copyright (C) Shunya KIMURA <brmtrain@gmail.com>
 * Use and distribution of this program is licensed under the
 * BSD license. See the COPYING file for full text.
 *
 * Original version TinySegmenter was written by Taku Kudo <taku@chasen.org>
 * The license is below.
 *   TinySegmenter 0.1 -- Super compact Japanese tokenizer in Javascript
 *   (c) 2008 Taku Kudo <taku@chasen.org>
 *   TinySegmenter is freely distributable under the terms of a new BSD licence.
 *   For details, see http://chasen.org/~taku/software/TinySegmenter/LICENCE.txt
 *   http://www.chasen.org/~taku/software/TinySegmenter/
 *
 */


#include <fcntl.h> //stat stat_buf
#include "tinysegmenterxx.hpp"

const unsigned int MAX_BUF_SIZ = 65536;

void printUsage(std::string& fileName)
{
  std::cerr << std::endl;
  std::cerr << fileName << " : the utility of tinysegmenter(Japanese setence segmenter)." << std::endl;
  std::cerr << "  " << fileName <<  " [options] filepath" << std::endl;
  std::cerr << "  " << fileName <<  " < filepath" << std::endl;
  std::cerr << "    -s, --separator=[tab|zero|return(default)]" << std::endl;
  std::cerr << "    -h, --help" << std::endl;
  std::cerr << "    -v, --version" << std::endl;
  exit(0);
}

void procArgs(int argc, char** argv, std::string& separator, std::string& inputPath)
{
  std::string fileName = argv[0];
  for(int i = 1; i < argc; i++){
    std::string argBuf = argv[i];
    if(argBuf == "-s"){
      separator = argv[++i];
    } else if((argBuf.find("--separator=")) != std::string::npos){
      unsigned int idx = argBuf.find("=");
      separator = argv[i] + idx + 1;
    } else if(argBuf == "--help" || argBuf == "-h"){
      printUsage(fileName);
    } else if(argBuf == "--version" || argBuf == "-v"){
      std::string body;
      tinysegmenterxx::util::getVersion(body);
      std::cerr << body << std::endl;
      exit(EXIT_SUCCESS);
    } else {
      if(i == argc - 1){
        inputPath = argBuf;
        break;
      }
      printUsage(fileName);
    }
  }
  if(inputPath.size() < 1){
    printUsage(fileName);
  }

  if(separator.size() < 1){
    separator = '\n';
  } else if(separator == "zero"){
    separator = '\0';
  } else if(separator == "tab"){
    separator = '\t';
  } else if(separator == "return"){
    separator = '\n';
  } else {
    std::cerr << "unknown option: -s " << separator << std::endl;
    std::cerr << "-s needs [zero|tab]" << std::endl;
    exit(1);
  }
}

int main(int argc, char** argv)
{
  std::string fileName = argv[0];
  tinysegmenterxx::Segmenter sg;
  if(argc > 1){
    std::string inputPath;
    std::string separator;
    procArgs(argc, argv, separator, inputPath);
    struct stat stat_buf;
    size_t fileSiz = 0;
    if(stat(inputPath.c_str(), &stat_buf) == 0){
      fileSiz = stat_buf.st_size;
    }
    char* buf = NULL;
    int bufSiz = fileSiz;
    if(fileSiz >= MAX_BUF_SIZ){
      bufSiz = 0;
      buf = new char[fileSiz + 1];
    }
    char inputBuf[bufSiz + 1];
    if(!buf) buf = inputBuf;
    std::ifstream ifs;
    ifs.open(inputPath.c_str(), std::ios::in);
    if(!ifs){
      std::cerr << "cant open file:" << inputPath << std::endl;
      exit(1);
    }
    ifs.read(buf, fileSiz);
    buf[fileSiz] = '\0';
    std::string line = buf;
    tinysegmenterxx::Segmentes segs;
    sg.segment(line, segs);
    for(unsigned int i = 0; i < segs.size(); i++){
      std::cout << segs[i] << separator;
    }
    if(buf != inputBuf) delete[] buf;
  } else {
    std::string buf;
    while(std::cin){
      std::string line;
      std::cin >> line;
      buf.append(line);
      buf.append("\n");
    }
    tinysegmenterxx::Segmentes segs;
    sg.segment(buf, segs);
    for(unsigned int i = 0; i < segs.size(); i++){
      std::cout << segs[i] << std::endl;
    }
  }
  return 0;
}
