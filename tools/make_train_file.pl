#!/usr/bin/perl

use strict;
use warnings;

sub create_header {
    print "struct Train\n";
    print "{\n";
    print "    const char *key;\n";
    print "    int val;\n";
    print "};\n";
    print "%%\n";

}

sub main {

    create_header();
    while (my $line = <>) {
        chomp $line;
        $line =~ /(this.)(.*) = {(.*)};/;
        my $class = $2;
        my $body  = $3;
        $body =~ s/(\d),/$1\t/g;
        my @elems = split "\t", $body;
        for my $e (@elems) {
            my ($key, $val) = split ':', $e;
            $key =~ s/"//g;
            print "$class$key\t$val\n";
        }
    }
}

main();




__END__

%{
    #include "command_options.h"
    typedef struct CommandOptionCode CommandOptionCode;
    %}
    struct CommandOption
    {
        const char *Option;
        int OptionCode;
    };
%%
    +helpverbose, CommandOptionCode::HELPVERBOSE
    +password, CommandOptionCode::PASSWORD
    +nocopyright, CommandOptionCode::NOCOPYRIGHT
    +nolog, CommandOptionCode::NOLOG
    +_64bit, CommandOptionCode::_64BIT

    --- word.key ---
    %{
        /*
 * a portion of GNU make keywords
 */

     #include "word.h"
     %}
    struct OPE { char *name; int type; int args; };
%%
    addprefix,      F_ADDPREFIX,    DYNAMIC
    addsuffix,      F_ADDSUFFIX,    DYNAMIC
    basename,       F_BASENAME,     MONAMIC
    dir,            F_DIR,          MONAMIC
    filter,         F_FILTER,       DYNAMIC
    filter-out,     F_FILTEROUT,    DYNAMIC
    findstring,     F_FINDSTRING,   DYNAMIC
    firstword,      F_FIRSTWORD,    MONAMIC
