#!/usr/bin/env perl
# vim: ts=4 sw=4

use strict;
use v5.16;

if (scalar @ARGV == 0) {
	@ARGV[0] = '.';
}
elsif (scalar @ARGV != 1) {
	die 'too many args';
}

chdir @ARGV[0];

my $fh;
my $status;
my $r;

my $pid;
my $normally_up;
my $want;
my $paused;

if (-e 'down') {
	$normally_up = 0;
}
else {
	$normally_up = 1;
}

#
# skip supervise/ok
#

open($fh, '<:raw', 'supervise/status') or die "open failed for supervise/status: $!";

$r = read($fh, $status, 20);
if ($r != 20) {
	if (not defined $r) {
		die "read failed for supervise/status: $!"
	}
	else {
		die "supervise/status: bad file format";
	}
}

say "   raw: ", unpack('H36', $status);

my @list = unpack('H16 H8 L< C A', $status);

say "  time: @list[0]";
my $tai = hex(@list[0]) - (1 << 62);
say "   TAI: ", $tai;
say "  UNIX: ", $tai - 10;
say "  curr: ", `date +%s`;

#say "   pid: ", @list[2]+0;
say "   pid: @list[2]";
#say "paused: ", @list[3]+0;
say "paused: @list[3]";
#say "  want: ", @list[4]+0;
say "  want: @list[4]";

close $fh;
