package OpenSSL::safe::installdata;

use strict;
use warnings;
use Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(
    @PREFIX
    @libdir
    @BINDIR @BINDIR_REL_PREFIX
    @LIBDIR @LIBDIR_REL_PREFIX
    @INCLUDEDIR @INCLUDEDIR_REL_PREFIX
    @APPLINKDIR @APPLINKDIR_REL_PREFIX
    @ENGINESDIR @ENGINESDIR_REL_LIBDIR
    @MODULESDIR @MODULESDIR_REL_LIBDIR
    @PKGCONFIGDIR @PKGCONFIGDIR_REL_LIBDIR
    @CMAKECONFIGDIR @CMAKECONFIGDIR_REL_LIBDIR
    $VERSION @LDLIBS
);

our @PREFIX                     = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl' );
our @libdir                     = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl' );
our @BINDIR                     = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/apps' );
our @BINDIR_REL_PREFIX          = ( 'apps' );
our @LIBDIR                     = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl' );
our @LIBDIR_REL_PREFIX          = ( '' );
our @INCLUDEDIR                 = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/include', '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/include' );
our @INCLUDEDIR_REL_PREFIX      = ( 'include', './include' );
our @APPLINKDIR                 = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/ms' );
our @APPLINKDIR_REL_PREFIX      = ( 'ms' );
our @ENGINESDIR                 = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/engines' );
our @ENGINESDIR_REL_LIBDIR      = ( 'engines' );
our @MODULESDIR                 = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl/providers' );
our @MODULESDIR_REL_LIBDIR      = ( 'providers' );
our @PKGCONFIGDIR               = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl' );
our @PKGCONFIGDIR_REL_LIBDIR    = ( '.' );
our @CMAKECONFIGDIR             = ( '/home/xiao/project/f1c100s/SDK/app/3rd_lib/openssl' );
our @CMAKECONFIGDIR_REL_LIBDIR  = ( '.' );
our $VERSION                    = '3.5.0-dev';
our @LDLIBS                     =
    # Unix and Windows use space separation, VMS uses comma separation
    $^O eq 'VMS'
    ? split(/ *, */, '-ldl -pthread -latomic ')
    : split(/ +/, '-ldl -pthread -latomic ');

1;
