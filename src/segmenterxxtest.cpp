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

#include <iostream>
#include <ctime>
#include <unistd.h>

namespace {
  class Initilizer {
  public:
    Initilizer()
    {
      std::srand(std::time(NULL));
    };
    ~Initilizer(){};
  };
  Initilizer init;
}

namespace tinysegmenterxx {

  namespace test {

    const unsigned int CHAR_MAX_SIZ = 500;

    int createRandomInt(int min, int max) {
      int rv = min + (int)(std::rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
      return rv;
    }

    std::string createRandomKatakana(int cnum = 5)
    {
      uint16_t ucsary[cnum];
      for(int i = 0; i < cnum; i++){
        int rv = createRandomInt(12449, 12543);
        ucsary[i] = rv;
      }
      const uint16_t* ucsaryptr = ucsary;
      char utfary[cnum * 4];
      tinysegmenterxx::util::ucstoutf(ucsaryptr, cnum, utfary);
      std::string rv = utfary;
      return rv;
    }

    std::string createRandomKana(int cnum = 5)
    {
      uint16_t ucsary[cnum];
      for(int i = 0; i < cnum; i++){
        int rv = createRandomInt(12353, 12447);
        if(rv == 12439 || rv == 12440) rv -= 2;
        ucsary[i] = rv;
      }
      const uint16_t* ucsaryptr = ucsary;
      char utfary[cnum * 4];
      tinysegmenterxx::util::ucstoutf(ucsaryptr, cnum, utfary);
      std::string rv = utfary;
      return rv;
    }

    std::string createRandomKanji(int cnum = 5)
    {
      uint16_t ucsary[cnum];
      for(int i = 0; i < cnum; i++){
        int rv = createRandomInt(19968, 40959);
        ucsary[i] = rv;
      }
      const uint16_t* ucsaryptr = ucsary;
      char utfary[cnum * 4];
      tinysegmenterxx::util::ucstoutf(ucsaryptr, cnum, utfary);
      std::string rv = utfary;
      return rv;
    }

    std::string createRandomAlphabet(int cnum = 5)
    {
      uint16_t ucsary[cnum];
      for(int i = 0; i < cnum; i++){
        int rv = createRandomInt(65, 122);
        ucsary[i] = rv;
      }
      const uint16_t* ucsaryptr = ucsary;
      char utfary[cnum * 4];
      tinysegmenterxx::util::ucstoutf(ucsaryptr, cnum, utfary);
      std::string rv = utfary;
      return rv;
    }

    std::string createRandomNumber(int cnum = 5)
    {
      uint16_t ucsary[cnum];
      for(int i = 0; i < cnum; i++){
        int rv = createRandomInt(48, 57);
        ucsary[i] = rv;
      }
      const uint16_t* ucsaryptr = ucsary;
      char utfary[cnum * 4];
      tinysegmenterxx::util::ucstoutf(ucsaryptr, cnum, utfary);
      std::string rv = utfary;
      return rv;
    }

    std::string createRandomString(int cnum = 5)
    {
      std::string rv;
      int stringSiz = 0;
      while(1){
        int charNum = createRandomInt(1, 10);
        if(stringSiz + charNum >= cnum){
          charNum = cnum - stringSiz;
        }
        stringSiz += charNum;
        int tp = createRandomInt(1, 5);
        switch(tp) {
        case 1: rv.append(createRandomKatakana(charNum));
          break;
        case 2: rv.append(createRandomKana(charNum));
          break;
        case 3: rv.append(createRandomKanji(charNum));
          break;
        case 4: rv.append(createRandomAlphabet(charNum));
          break;
        case 5: rv.append(createRandomNumber(charNum));
          break;
        }
        if(stringSiz >= cnum) break;
      }
      return rv;
    }

    bool testKatakana()
    {
      std::cerr << "Start Katakana TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string katakana = createRandomKatakana(i);
        tinysegmenterxx::Segmentes segs;
        sg.segment(katakana, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testKatakana()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }

    bool testKana()
    {
      std::cerr << "Start Kana TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string kana = createRandomKana(i);
        tinysegmenterxx::Segmentes segs;
        sg.segment(kana, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testKana()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }

    bool testKanji()
    {
      std::cerr << "Start Kanji TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string kanji = createRandomKanji(i);
        tinysegmenterxx::Segmentes segs;
        for(unsigned int j = 0; j < segs.size(); j++){
          std::cerr << segs[j] << std::endl;
        }
        sg.segment(kanji, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testKanji()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }

    bool testAlphabet()
    {
      std::cerr << "Start Alphabet TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string alphabet = createRandomAlphabet(i);
        tinysegmenterxx::Segmentes segs;
        for(unsigned int j = 0; j < segs.size(); j++){
          std::cerr << segs[j] << std::endl;
        }
        sg.segment(alphabet, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testAlphabet()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }

    bool testNumber()
    {
      std::cerr << "Start Number TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string number = createRandomNumber(i);
        tinysegmenterxx::Segmentes segs;
        for(unsigned int j = 0; j < segs.size(); j++){
          std::cerr << segs[j] << std::endl;
        }
        sg.segment(number, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testNumber()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }

    bool testRandomString()
    {
      std::cerr << "Start Random String TEST" << std::endl;
      tinysegmenterxx::Segmenter sg;
      for(unsigned int i = 1; i < CHAR_MAX_SIZ; i++){
        std::string random = createRandomString(i);
        tinysegmenterxx::Segmentes segs;
        for(unsigned int j = 0; j < segs.size(); j++){
          std::cerr << segs[j] << std::endl;
        }
        sg.segment(random, segs);
        if(segs.size() < 1){
          std::cerr << "test error: testRandomString()" << std::endl;
          return false;
        }
        if(i % 100 == 0){
          std::cerr << "[" << i << "/" << CHAR_MAX_SIZ << "]" << std::endl;
        } else if(i % 10 == 0){
          std::cerr << ".";
        }
      }
      std::cerr << std::endl;
      return true;
    }
  }
}

int main()
{
  bool ok = true;
  if(!tinysegmenterxx::test::testKatakana()) ok = false;
  if(!tinysegmenterxx::test::testKatakana()) ok = false;
  if(!tinysegmenterxx::test::testKana()) ok = false;
  if(!tinysegmenterxx::test::testKanji()) ok = false;
  if(!tinysegmenterxx::test::testAlphabet()) ok = false;
  if(!tinysegmenterxx::test::testNumber()) ok = false;
  if(!tinysegmenterxx::test::testRandomString()) ok = false;

  return !ok;

}
