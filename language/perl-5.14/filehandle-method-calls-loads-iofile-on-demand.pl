#!perl -w
#use IO::Handle;
use 5.14.0;

STDOUT->autoflush(1); # loads IO::File and succeeds
my @words = qw/this is a test. ignore me/;
for my $word (@words) {
    print $word . " ";
    sleep 1;
}
print "\n";
