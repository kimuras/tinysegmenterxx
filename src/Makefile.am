pkginclude_HEADERS = tinysegmenterxx.hpp tinysegmenterxx_train.hpp
bin_PROGRAMS = segmenterxx segmenterxxtest
segmenterxx_CPPFLAGS = -Wall
segmenterxx_LDFLAGS  = -L/usr/local/lib
segmenterxx_SOURCES  = segmenterxx.cpp

check_PROGRAMS = segmenterxxtest
segmenterxxtest_CPPFLAGS = -Wall
segmenterxxtest_SOURCES  = segmenterxxtest.cpp

TESTS = segmenterxxtest
