# - Find PHP
# This module finds if PHP is installed and determines where the include files
# and libraries are.
#
# Note, unlike the FindPHP4 module, this module uses the php-config script to
# determine information about the installed PHP configuration.  For Linux
# distributions, this script is normally installed as part of some php-dev or
# php-devel package. See http://php.net/manual/en/install.pecl.php-config.php
# for php-config documentation.
#
# This code sets the following variables:
#  PHP_CONFIG_DIR             = directory containing PHP configuration files
#  PHP_CONFIG_EXECUTABLE      = full path to the php-config binary
#  PHP_EXECUTABLE             = full path to the php binary
#  PHP_EXTENSIONS_DIR         = directory containing PHP extensions
#  PHP_EXTENSIONS_INCLUDE_DIR = directory containing PHP extension headers
#  PHP_INCLUDE_DIRS           = include directives for PHP development
#  PHP_LIBS                   = libraries necessary
#  PHP_VERSION_NUMBER         = PHP version number in PHP's "vernum" format eg 50303
#  PHP_VERSION_MAJOR          = PHP major version number eg 5
#  PHP_VERSION_MINOR          = PHP minor version number eg 3
#  PHP_VERSION_PATCH          = PHP patch version number eg 3
#  PHP_VERSION_STRING         = PHP version string eg 5.3.3-1ubuntu9.3
#  PHP_API                    = PHP version api string eg 20131226
#  PHP_FOUND                  = set to TRUE if all of the above has been found.
#

#=============================================================================
# Copyright 2016 Jerry Jacobs
# Copyright 2011-2012 Paul Colby
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file LICENSE.md for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

set(FINDPHP_DEBUG No)

find_program(PHP_CONFIG_EXECUTABLE NAMES php-config7 php-config5 php-config4 php-config)

if (PHP_CONFIG_EXECUTABLE)
	# PHP_CONFIG_DIR
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --configure-options
		#COMMAND sed -ne "s/^.*--with-config-file-scan-dir=\\([^ ]*\\).*/\\1/p"
			OUTPUT_VARIABLE PHP_CONFIG_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# PHP_EXECUTABLE
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --php-binary
			OUTPUT_VARIABLE PHP_EXECUTABLE
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# PHP_EXTENSIONS_DIR
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --extension-dir
			OUTPUT_VARIABLE PHP_EXTENSIONS_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# PHP_EXTENSIONS_INCLUDE_DIR
	execute_process(COMMAND
		${PHP_CONFIG_EXECUTABLE} --include-dir
		OUTPUT_VARIABLE PHP_EXTENSIONS_INCLUDE_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	# PHP_INCLUDE_DIRS
	execute_process(COMMAND
		${PHP_CONFIG_EXECUTABLE} --includes
		OUTPUT_VARIABLE _PHP_INCLUDE_DIRS
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	# Create CMake list from compiler include paths string
	string(REPLACE "-I" ";" PHP_INCLUDE_DIRS ${_PHP_INCLUDE_DIRS})

	# PHP_LIBS
	execute_process(COMMAND
		${PHP_CONFIG_EXECUTABLE} --libs
		OUTPUT_VARIABLE PHP_LIBS
		OUTPUT_STRIP_TRAILING_WHITESPACE)


	# PHP_VERSION_NUMBER
	execute_process(COMMAND
		${PHP_CONFIG_EXECUTABLE} --vernum
		OUTPUT_VARIABLE PHP_VERSION_NUMBER
		OUTPUT_STRIP_TRAILING_WHITESPACE)

	# PHP_VERSION_MAJOR
	# TODO sed should be replaced by CMake function(s)
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --vernum
			OUTPUT_STRIP_TRAILING_WHITESPACE
		COMMAND sed -ne "s/....$//p"
			OUTPUT_VARIABLE PHP_VERSION_MAJOR
	)

	# PHP_VERSION_MINOR
	# TODO sed should be replaced by CMake function(s)
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --vernum
			OUTPUT_STRIP_TRAILING_WHITESPACE
		COMMAND sed -ne "s/..$//p"
		COMMAND sed -ne "s/^.0\\?//p"
			OUTPUT_VARIABLE PHP_VERSION_MINOR
	)

	# PHP_VERSION_PATCH
	# TODO sed should be replaced by CMake function(s)
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --vernum
			OUTPUT_STRIP_TRAILING_WHITESPACE
#		COMMAND sed -ne "s/^...0\\t\?//p"
			OUTPUT_VARIABLE PHP_VERSION_PATCH
	)

	# PHP_VERSION_STRING
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} --version
			OUTPUT_VARIABLE PHP_VERSION_STRING
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	# PHP_API
	execute_process(
		COMMAND
			${PHP_CONFIG_EXECUTABLE} php-config --phpapi
			OUTPUT_VARIABLE PHP_API
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif(PHP_CONFIG_EXECUTABLE)

mark_as_advanced(
	PHP_CONFIG_DIR
	PHP_CONFIG_EXECUTABLE
	PHP_EXECUTABLE
	PHP_EXTENSIONS_DIR
	PHP_EXTENSIONS_INCLUDE_DIR
	PHP_INCLUDE_DIRS
	PHP_LIBS
	PHP_VERSION_MAJOR
	PHP_VERSION_MINOR
	PHP_VERSION_PATCH
	PHP_VERSION_STRING
	PHP_API
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	php
	DEFAULT_MSG
	PHP_VERSION_STRING
	PHP_CONFIG_DIR
	PHP_CONFIG_EXECUTABLE
	PHP_EXECUTABLE
	PHP_EXTENSIONS_DIR
	PHP_EXTENSIONS_INCLUDE_DIR
	PHP_INCLUDE_DIRS
	PHP_LIBS
	PHP_VERSION_NUMBER
	PHP_VERSION_NUMBER
	PHP_VERSION_MAJOR
	PHP_VERSION_MINOR
	PHP_VERSION_PATCH
	PHP_VERSION_STRING
)

# Some handy dev output. Is there a way to enable these in some debug mode?
if (${FINDPHP_DEBUG})
	message("PHP_FOUND                  = ${PHP_FOUND}")
	message("PHP_CONFIG_DIR             = ${PHP_CONFIG_DIR}")
	message("PHP_CONFIG_EXECUTABLE      = ${PHP_CONFIG_EXECUTABLE}")
	message("PHP_EXECUTABLE             = ${PHP_EXECUTABLE}")
	message("PHP_EXTENSIONS_DIR         = ${PHP_EXTENSIONS_DIR}")
	message("PHP_EXTENSIONS_INCLUDE_DIR = ${PHP_EXTENSIONS_INCLUDE_DIR}")
	message("PHP_INCLUDE_DIRS           = ${PHP_INCLUDE_DIRS}")
	message("PHP_LIBS                   = ${PHP_LIBS}")
	message("PHP_VERSION_NUMBER         = ${PHP_VERSION_NUMBER}")
	message("PHP_VERSION_MAJOR          = ${PHP_VERSION_MAJOR}")
	message("PHP_VERSION_MINOR          = ${PHP_VERSION_MINOR}")
	message("PHP_VERSION_PATCH          = ${PHP_VERSION_PATCH}")
	message("PHP_VERSION_STRING         = ${PHP_VERSION_STRING}")
	message("PHP_API                    = ${PHP_API}")
endif()
