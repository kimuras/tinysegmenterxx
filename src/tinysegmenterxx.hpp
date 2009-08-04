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

#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <stdint.h>
#include "tinysegmenterxx_train.hpp"

namespace tinysegmenterxx {

  typedef std::vector<std::string> Segmentes;

  const unsigned int SEGMENT_NUM       = 65536;
  const unsigned int INPUT_MAX_BUF_SIZ = 65536;
  const unsigned int GETSCORE_BUF_SIZ  = 65536;
  const int DEFAULT_SCORE              = -312;

  const char* UP1__ = "UP1__";
  const char* UP2__ = "UP2__";
  const char* UP3__ = "UP3__";
  const char* BP1__ = "BP1__";
  const char* BP2__ = "BP2__";
  const char* UW1__ = "UW1__";
  const char* UW2__ = "UW2__";
  const char* UW3__ = "UW3__";
  const char* UW4__ = "UW4__";
  const char* UW5__ = "UW5__";
  const char* UW6__ = "UW6__";
  const char* BW1__ = "BW1__";
  const char* BW2__ = "BW2__";
  const char* BW3__ = "BW3__";
  const char* TW1__ = "TW1__";
  const char* TW2__ = "TW2__";
  const char* TW3__ = "TW3__";
  const char* TW4__ = "TW4__";
  const char* UC1__ = "UC1__";
  const char* UC2__ = "UC2__";
  const char* UC3__ = "UC3__";
  const char* UC4__ = "UC4__";
  const char* UC5__ = "UC5__";
  const char* UC6__ = "UC6__";
  const char* BC1__ = "BC1__";
  const char* BC2__ = "BC2__";
  const char* BC3__ = "BC3__";
  const char* TC1__ = "TC1__";
  const char* TC2__ = "TC2__";
  const char* TC3__ = "TC3__";
  const char* TC4__ = "TC4__";
  const char* TC5__ = "TC5__";
  const char* UQ1__ = "UQ1__";
  const char* UQ2__ = "UQ2__";
  const char* UQ3__ = "UQ3__";
  const char* BQ1__ = "BQ1__";
  const char* BQ2__ = "BQ2__";
  const char* BQ3__ = "BQ3__";
  const char* BQ4__ = "BQ4__";
  const char* TQ1__ = "TQ1__";
  const char* TQ2__ = "TQ2__";
  const char* TQ3__ = "TQ3__";
  const char* TQ4__ = "TQ4__";
  const char* B__   = "B";
  const char* B3__  = "B3";
  const char* B2__  = "B2";
  const char* B1__  = "B1";
  const char* E3__  = "E3";
  const char* E2__  = "E2";
  const char* E1__  = "E1";
  const char* O__   = "O";
  const char* U__   = "U";
  const char* A__   = "A";
  const char* N__   = "N";
  const char* I__   = "I";
  const char* K__   = "K";
  const char* M__   = "M";
  const char* H__   = "H";

  namespace util {

    const char* getCharClass(uint16_t c)
    {
      const char* rv = O__;
      if (c <= 0x007F ) {
        // ASCII
        if(c >= 'a' && c <= 'z'){
          rv = A__;
        } else if(c >= 'A' && c <= 'Z'){
          rv = A__;
        } else if(c >= '0' && c <= '9'){
          rv = N__;
        }
      } else if (c >= 0x3040 && c <= 0x309F) {
        // Kana
        rv = I__;
      } else if (c >= 0x30A0 && c <= 0x30FF) {
        // Katakana
        rv = K__;
      } else if (c >= 0x4E00 && c <= 0x9FFF) {
        // Kanji
        if(c == 0x4E00 || c == 0x4E8C || c == 0x4E09 || c == 0x56DB ||
           c == 0x4E94 || c == 0x516D || c == 0x4E03 || c == 0x516B ||
           c == 0x4E5D || c == 0x5341 || c == 0x767E || c == 0x5343 ||
           c == 0x4E07 || c == 0x5104 || c == 0x5146){
          rv = M__;
        }
        rv = H__;
      } else {
        rv = O__;
      }
      return rv;
    }


    void utftoucs(const char *str, uint16_t *ary, int *np)
    {
      const unsigned char *rp = (unsigned char *)str;
      unsigned int wi = 0;
      while(*rp != '\0'){
        int c = *(unsigned char *)rp;
        if(c < 0x80){
          ary[wi++] = c;
        } else if(c < 0xe0){
          if(rp[1] >= 0x80){
            ary[wi++] = ((rp[0] & 0x1f) << 6) | (rp[1] & 0x3f);
            rp++;
          }
        } else if(c < 0xf0){
          if(rp[1] >= 0x80 && rp[2] >= 0x80){
            ary[wi++] = ((rp[0] & 0xf) << 12) | ((rp[1] & 0x3f) << 6) | (rp[2] & 0x3f);
            rp += 2;
          }
        }
        rp++;
      }
      *np = wi;
    }


    int ucstoutf(const uint16_t *ary, int num, char *str)
    {
      unsigned char *wp = (unsigned char *)str;
      for(int i = 0; i < num; i++){
        unsigned int c = ary[i];
        if(c < 0x80){
          *(wp++) = c;
        } else if(c < 0x800){
          *(wp++) = 0xc0 | (c >> 6);
          *(wp++) = 0x80 | (c & 0x3f);
        } else {
          *(wp++) = 0xe0 | (c >> 12);
          *(wp++) = 0x80 | ((c & 0xfff) >> 6);
          *(wp++) = 0x80 | (c & 0x3f);
        }
      }
      *wp = '\0';
      return (char *)wp - str;
    }

  }


  class Segmenter {

  public:
    Segmenter() : train(){}

    ~Segmenter(){}

    void segment(std::string& input, Segmentes& result)
    {
      char seg[SEGMENT_NUM][4];
      char ctype[SEGMENT_NUM][4];

      unsigned int sidx = 0;
      std::strcpy(seg[sidx++], B3__);
      std::strcpy(seg[sidx++], B2__);
      std::strcpy(seg[sidx++], B1__);

      unsigned int cidx = 0;
      std::strcpy(ctype[cidx++], O__);
      std::strcpy(ctype[cidx++], O__);
      std::strcpy(ctype[cidx++], O__);

      unsigned int inputSiz = input.size();

      int stackSiz = inputSiz + 1;
      uint16_t* ary = NULL;
      if(inputSiz + 1 >= INPUT_MAX_BUF_SIZ){
        stackSiz = 0;
        ary = new uint16_t[inputSiz];
      }
      uint16_t stack[stackSiz + 1];
      if(!ary) ary = stack;
      int anum;
      util::utftoucs(input.c_str(), ary, &anum);
      ary[anum] = 0x0000;
      for(int i = 0; i < anum; ++i){
        uint16_t stack2[1];
        stack2[0] = ary[i];
        const uint16_t* ary2 = stack2;
        const char* cTypeBuf = util::getCharClass(stack2[0]);
        std::strcpy(ctype[cidx++], cTypeBuf);
        util::ucstoutf(ary2, 1, seg[sidx++]);
        if(sidx >= SEGMENT_NUM - 6) break;
      }
      if(ary != stack) delete[] ary;
      std::strcpy(seg[sidx++], E1__);
      std::strcpy(seg[sidx++], E2__);
      std::strcpy(seg[sidx++], E3__);
      std::strcpy(ctype[cidx++], O__);
      std::strcpy(ctype[cidx++], O__);
      std::strcpy(ctype[cidx++], O__);
      std::string word = seg[3];
      const char* p1 = U__;
      const char* p2 = U__;
      const char* p3 = U__;

      for(unsigned int i = 4; i < sidx - 3; i++){
        int score = DEFAULT_SCORE;
        const char* w1 = seg[i-3];
        const char* w2 = seg[i-2];
        const char* w3 = seg[i-1];
        const char* w4 = seg[i];
        const char* w5 = seg[i+1];
        const char* w6 = seg[i+2];
        const char* c1 = ctype[i-3];
        const char* c2 = ctype[i-2];
        const char* c3 = ctype[i-1];
        const char* c4 = ctype[i];
        const char* c5 = ctype[i+1];
        const char* c6 = ctype[i+2];
#ifdef DEBUG
        std::cout << w1 << "|\t|" << w2 << "|\t|" << w3 << "|\t|" << w4
                  << "|\t|" << w5 << "|\t|" << w6 << std::endl;
        std::cout << c1 << "|\t|" << c2 << "|\t|" << c3 << "|\t|" << c4
                  << "|\t|" << c5 << "|\t|" << c6 << std::endl;
#endif

        score += getScore(2, UP1__, p1);
        score += getScore(2, UP2__, p2);
        score += getScore(2, UP3__, p3);
        score += getScore(3, BP1__, p1, p2);
        score += getScore(3, BP2__, p2, p3);
        score += getScore(2, UW1__, w1);
        score += getScore(2, UW2__, w2);
        score += getScore(2, UW3__, w3);
        score += getScore(2, UW4__, w4);
        score += getScore(2, UW5__, w5);
        score += getScore(2, UW6__, w6);
        score += getScore(3, BW1__, w2, w3);
        score += getScore(3, BW2__, w3, w4);
        score += getScore(3, BW3__, w4, w5);
        //        std::cout << w1 + w2 + w3 << std::endl;
        score += getScore(4, TW1__, w1, w2, w3);
        score += getScore(4, TW2__, w2, w3, w4);
        score += getScore(4, TW3__, w3, w4, w5);
        score += getScore(4, TW4__, w4, w5, w6);
        score += getScore(2, UC1__, c1);
        score += getScore(2, UC2__, c2);
        score += getScore(2, UC3__, c3);
        score += getScore(2, UC4__, c4);
        score += getScore(2, UC5__, c5);
        score += getScore(2, UC6__, c6);
        score += getScore(3, BC1__, c2, c3);
        score += getScore(3, BC2__, c3, c4);
        score += getScore(3, BC3__, c4, c5);
        score += getScore(4, TC1__, c1, c2, c3);
        score += getScore(4, TC2__, c2, c3, c4);
        score += getScore(4, TC3__, c3, c4, c5);
        score += getScore(4, TC4__, c4, c5, c6);
        score += getScore(3, UQ1__, p1, c1);
        score += getScore(3, UQ2__, p2, c2);
        //score += getScore( UQ1(stack,  p1 + c1); // ayashii
        score += getScore(3, UQ3__, p3, c3);
        score += getScore(4, BQ1__, p2, c2, c3);
        score += getScore(4, BQ2__, p2, c3, c4);
        score += getScore(4, BQ3__, p3, c2, c3);
        score += getScore(4, BQ4__, p3, c3, c4);
        score += getScore(5, TQ1__, p2, c1, c2, c3);
        score += getScore(5, TQ2__, p2, c2, c3, c4);
        score += getScore(5, TQ3__, p3, c1, c2, c3);
        score += getScore(5, TQ4__, p3, c2, c3, c4);

        const char* p = O__;
        if(score > 0){
          result.push_back(word);
          word = "";
          p    = B__;
        }
        p1 = p2;
        p2 = p3;
        p3 = p;
        word.append(seg[i]);
      }
      result.push_back(word);

#ifdef DEBUG
      std::cout << ">>>>>>>>>>>>" << std::endl;
      for(unsigned int i = 0; i < cidx; i++){
        std::cout << "[" << i << "]" << ctype[i] << "\t"<< seg[i]<< std::endl;
      }
      std::cout << "<<<<<<<<<<<" << std::endl;
#endif
    }

  private:
    TrainHash train;

    int getScore(int num, ...)
    {
      char stack[GETSCORE_BUF_SIZ];
      char *wp = stack;
      std::va_list ap;
      va_start(ap, num);
      unsigned int cnt = 0;
      for(int i = 0; i < num; i++){
        const char* str = va_arg(ap, const char*);
        while(*str != '\0'){
          *(wp++) = *(str++);
          cnt++;
          if(cnt >= GETSCORE_BUF_SIZ - 1) break;
        }
      }
      va_end(ap);
      *wp = '\0';

      const struct Train* rv =
        train.in_word_set(stack, wp - stack);

      if(rv) return rv->val;

      return 0;
    }
  };
}
