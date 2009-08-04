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



#include "tinysegmenterxx.hpp"

void printUsage(std::string& fileName)
{
  std::cerr << fileName << " : the utility to extract nouns" << std::endl;
  std::cerr << "  usage: " << fileName << " file " << std::endl;
  return;
}

int main(int argc, char** argv)
{
  std::string fileName = argv[0];
  if(argc < 2) printUsage(fileName);
  std::string farg = argv[1];
  if(farg == "-h" || farg == "--help") printUsage(fileName);
  tinysegmenterxx::Segmenter sg;
  std::ifstream ifs(farg.c_str());
  if(!ifs){
    std::cerr << "file open error: " << farg << std::endl;
    return false;
  }
  std::string line;
  while(std::getline(ifs, line)){
    tinysegmenterxx::Segmentes segs;
    sg.segment(line, segs);
    for(unsigned int i = 0; i < segs.size(); i++){
      std::cout << segs[i] << std::endl;
    }
  }
}
