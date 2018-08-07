#!/usr/bin/env python
#-*-coding:utf-8-*-

#============================================================================
#copy from :https://bitbucket.org/renficiaud/cmake-pip/src/cce460f9d6066885a10d341c0ef92b599866e025/cmake_pip/cmake_extension.py?at=master&fileviewer=file-view-default

"""Main extension definition for the CMake extensions

This file defines the main extension definition for the CMake extensions. The CMake extension
is embodied by the class :py:class:`.ExtensionCMake`. The function :py:func:`.build_cmake`
is the builder associated to this extensions.
"""

from distutils.command.build_ext import build_ext as _build_ext
from distutils.extension import Extension as _Extension
from distutils.core import Command as _Command, setup as _setup

from distutils import log

import subprocess
import os
import sys


def _is_64_build():
    return sys.maxsize > 2 ** 32


class ExtensionCMake(_Extension):
    """Defines a CMake extension.

    A CMake extension is a definition of an extension where the entire build is delegated
    to CMake. For the extension to work, a CMake file (`CMakeLists.txt`) should be accessible either directly
    or by GIT.

      * `name` : name of the package
      * `cmake_target` the target used to generate the extension. If empty, the default
        `ALL` target will be used.
      * `cmake_install`: an install command in the cmake sources installs the target
        in a specific location. Specify `cmake_install_component` if the install is
        for a particular component
      * `cmake_install_component` the component used for installation. See documentation.
        Implied `cmake_install`.
      * `cmake_file` the location of the cmake file or the cmake file itself
      * `cmake_options` cmake options
      * `cmake_src_layout` indicates that the layout is a regular one
      * `cmake_external_project` an external cmake project to download
      * `cmake_builder` the optional builder for the platform. Defaults to `Makefiles` on
        Linux/OSX and VS 2012 on Win32. The `Win64` part should be omitted as
        it is automatically deduced by the current python executable/interpreter.

    Overrides of the regular distutils commands
    """

    def __init__(self,
                 name,
                 cmake_file,
                 cmake_target=None,
                 cmake_locate_extensions=None,
                 cmake_options=None,
                 cmake_package=None,
                 cmake_src_layout=None,
                 cmake_external_project=None,
                 cmake_install=None,
                 cmake_install_component=None,
                 cmake_builder=None,
                 *args, **kwargs):
        # self.cmake_generated_module = kwargs.get('name', args[0])
        self.cmake_file = cmake_file
        self.cmake_options = cmake_options
        self.cmake_target = cmake_target
        self.cmake_locate_extensions = cmake_locate_extensions if cmake_locate_extensions is not None else False
        if cmake_src_layout:
            assert cmake_src_layout.lower() in ['in_tree', 'out_tree']
            self.cmake_src_layout = cmake_src_layout.lower()
        else:
            self.cmake_src_layout = 'in_tree'

        self.cmake_external_project = cmake_external_project
        self.cmake_install_component = cmake_install_component
        self.cmake_install = cmake_install
        self.cmake_builder = cmake_builder

        _Extension.__init__(self, name, sources=[cmake_file],
                            *args, **kwargs)

        #self.cmake_regular_layout = kwargs.get('cmake_regular_layout')


class build_cmake(_Command):
    """Distutil builder associated to the CMake extension


    """

    description = "Builds cmake extensions"
    user_options = [  #('boostroot=', None, 'specifies the boost directory'),  # option with = because it takes an argument
                    ('cmake-additional-options=', None, 'additional cmake options known at runtime'),
                    ]

    def initialize_options(self):
        self.cmake_additional_options = None
        self.build_temp = None
        self.build_lib = None
        self.build_platlib = None
        self.install_dir = None

        self.cached_cmake_configure = {}

    def finalize_options(self):
        self.set_undefined_options('build', ('build_temp', 'build_temp'),
                                            ('build_lib', 'build_lib'),
                                            ('build_platlib', 'build_platlib'))
        self.set_undefined_options('install', ('install_lib', 'install_dir'))
        pass

    def set_extensions(self, ext_list):

        for ext in ext_list:
            if(not isinstance(ext, ExtensionCMake)):
                raise RuntimeError('Only ExtensionCMake allowed')

        self.extension_list = ext_list

    def cmake_configure(self, ext, options):
        """Configuring CMake.

        This is the main function for configuring CMake on all platforms. It takes
        """

        # cmake location
        cmake_path = os.path.dirname(ext.cmake_file)  # location of the CMakeLists.txt yayi_src_files_location

        # is 64 bits interpreter? we build for the current interpreter
        is_64bits = _is_64_build()

        # common to linux/osx
        if sys.platform in ["linux2"]:
            #  indicates the location where the libraries will be installed after the setup install command
            options += ['-DCMAKE_INSTALL_RPATH=$ORIGIN/.']

        # for OSX
        if sys.platform == "darwin":
            # indicates that the directory part will be replaced by @rpath, related to CMAKE_INSTALL_RPATH
            options += ['-DCMAKE_MACOSX_RPATH=ON', ]
            pass

        # set up the builder
        if not ext.cmake_builder:
            builder = []
            # if sys.platform == 'win32':
            #     builder = ['-G', 'Visual Studio 12' + ' Win64' if is_64bits else '']
            # else:
            #     builder = []
        else:
            builder = [ext.cmake_builder]
            # if sys.platform == 'win32':
            #     builder = [ext.cmake_builder + ' Win64' if is_64bits else '']
            #     pass
            # else:
            #     builder = [ext.cmake_builder]

        # indicating the location of python as well, in case we are in a virtual environment
        # this python executable should be inherited
        options += ['-DPYTHON_EXECUTABLE=%s' % sys.executable]

        # stabilizing the options
        options.sort()

        cmd = ['cmake'] + builder + options + [os.path.abspath((os.path.join(os.path.abspath(os.curdir), cmake_path)))]

        import hashlib
        t = hashlib.sha1()
        for i in options:
            t.update(i)

        digest = t.hexdigest()

        if digest in self.cached_cmake_configure:
            self.cached_cmake_configure[digest] += [ext]
            return

        self.cached_cmake_configure[digest] = [ext]

        # build location
        build_location = os.path.join(self.cmake_build_location, digest)
        if not os.path.exists(build_location):
            os.makedirs(build_location)

        print 'curdir', os.path.abspath(os.curdir)
        print 'current dir', os.listdir(os.curdir)

        log.info('#' * 40)
        log.info('#[CMAKE-PIP] CMake configuration')
        log.info('#[CMAKE-PIP]\n\t- command is\n\t%s\n\t- running in path\n\t%s', ' '.join(cmd), build_location)
        config_proc = subprocess.Popen(cmd,
                                       cwd=build_location,
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE)
        config_proc.wait()
        if(config_proc.returncode != 0):
            log.error('#[CMAKE-PIP] config cmake returned an error code %d', config_proc.returncode)
            log.error('#[CMAKE-PIP] stopping the build')
            log.error('#[CMAKE-PIP] STDERR')
            log.error('\n'.join(config_proc.stderr.readlines()))
            log.error('#[CMAKE-PIP] STDOUT')
            log.error('\n'.join(config_proc.stdout.readlines()))
            raise Exception('Error produced by cmake_configure')

        log.info('#' * 40)
        log.info('# CMake configuration COMPLETE')

        pass

    def cmake_configure_and_build(self, ext):

        options = ext.cmake_options
        if options is None:
            options = []

        # global options
        if self.cmake_additional_options is not None:
            options += [self.cmake_additional_options]

        # change of layout should go there

        self.cmake_configure(ext, options)

        # now the version should be available
        if self.distribution.metadata.version is None:
            self.distribution.metadata.version = '0.1a'

        # todo
        # check that the python library is compatible with the version of boost we have
        # which might be not the case for virtualenv like with exotic python +/-
        # boost. In any case, boost-python and python should be coherent.

        try:
            from multiprocessing import cpu_count
            cpu_count_ = cpu_count()
        except Exception, _:
            cpu_count_ = 1

        log.info('#' * 40)
        log.info('# CMake BUILD')

        # print 'curdir', os.path.abspath(os.curdir)
        # print 'current dir', os.listdir(os.curdir)
        # print 'build dir', os.listdir(self.cmake_build_location)

        cmake_cmd = ['cmake', '--build', '.']

        # release on win32
        if sys.platform == "win32":
            cmake_cmd += ['--config', 'Release']

        # additional options: cpu for multithreaded builds
        additional_options = []
        if not ext.cmake_builder:
            # with default builders, we can pass those options
            if sys.platform == "win32":
                additional_options += ['--', '/m:%d' % cpu_count_, '/v:m']
            else:
                additional_options += ['--', '-j%d' % cpu_count_]

        # build location
        build_location = self.get_extension_build_location(ext)

        # target
        if ext.cmake_target is not None:
            cmake_cmd += ['--target', ext.cmake_target]

        # todo flush the cmake output into a file
        build_proc = subprocess.Popen(cmake_cmd + additional_options, cwd=build_location)
        build_proc.wait()

        if(build_proc.returncode != 0):
            log.error('#[CMAKE-PIP] config cmake returned an error code %d', build_proc.returncode)
            log.error('#[CMAKE-PIP] stopping the build')
            log.error('#[CMAKE-PIP] STDERR')
            log.error('\n'.join(build_proc.stderr.readlines()))
            log.error('#[CMAKE-PIP] STDOUT')
            log.error('\n'.join(build_proc.stdout.readlines()))
            raise Exception('Error produced by cmake_configure_and_build')

        # install if this is part of the procedure
        # TODO !

        if ext.cmake_install or ext.cmake_install_component:
            # run the installer with build/lib/ (without the hash) as prefix
            # TODO
            pass

        elif ext.cmake_locate_extensions:
            import imp
            extensions = [i[0] for i in imp.get_suffixes() if i[2] == imp.C_EXTENSION]
            extensions.append(".py")
            build_ext = self.get_finalized_command('build_ext')

            for dirpath, dirnames, filenames in os.walk(build_location):

                target_files = [os.path.join(dirpath, i) for i in filenames if os.path.splitext(i)[1] in extensions]
                #copy so 
                for i in target_files:
                    if os.path.splitext(i)[1] == ".py":
                        destination = os.path.join(self.build_platlib,os.path.basename(i))
                        build_ext.mkpath(os.path.dirname(destination))
                        self.copy_file(i, destination)
                    else:     
                        fullname = build_ext.get_ext_fullname(ext.name)
                        filename = build_ext.get_ext_filename(fullname)

                        # if os.path.basename(filename).lower() != '${ALL}':
                        #     if os.path.basename(i) != os.path.basename(filename):
                        #         # this is not the current target, we skip
                        #         continue

                        # import ipdb; ipdb.set_trace()

                        destination = os.path.join(self.build_platlib, filename)               
                        build_ext.mkpath(os.path.dirname(destination))
                        self.copy_file(i, destination)

             



        log.info('#' * 40)
        log.info('[CMAKE-PIP] build_cmake ok')

        if 0:
            # the installation procedure is copying yayi files into the python package tree, so it should be part of the
            # build itself
            log.info('#' * 40)
            log.info('[YAYI] build_cmake -- install the python package component')

            cmake_cmd = ['cmake', '--build', '.', '--target', 'PythonPackageSetup']
            if sys.platform == "win32":
                cmake_cmd += ['--config', 'Release']  # important otherwise the other version might get installed
                cmake_cmd += ['--', '/v:m']

            build_proc = subprocess.Popen(cmake_cmd, cwd=self.cmake_build_location)
            build_proc.wait()

            # consider, for installing one component only:
            # ${CMAKE_COMMAND} -DCOMPONENT=python_package_install_intermediate -P ${CMAKE_BINARY_DIR}/cmake_install.cmake

            if(build_proc.returncode != 0):
                log.error('[YAYI] build_cmake install returned an error code %d', build_proc.returncode)
                log.error('[YAYI] stopping the commands')
                raise Exception('Error produced by cmake "install" command, see logs for more information')

            log.info('#' * 40)
            log.info('[CMAKE-PIP] build_cmake -- install ok')

        pass  # class build_cmake

    def run(self):

        self.cmake_build_location = os.path.abspath(self.build_temp)
        self.cmake_install_location = os.path.abspath(self.build_lib)
        self.cmake_install_prefix = os.path.abspath(self.install_dir)
        self.cmake_platlib = os.path.abspath(self.build_platlib)

        log.info('#' * 40)
        log.info('[CMAKE-PIP] build_cmake\n'
                 '\tinside directory %s\n'
                 '\tcreating temporary installation to directory %s\n'
                 '\tcmake_platlib: %s\n'
                 '\tPREFIX to directory %s',
                 self.cmake_build_location,
                 self.cmake_install_location,
                 self.cmake_platlib,
                 self.cmake_install_prefix)

        if self.extension_list:
            for ext in self.extension_list:
                log.info('[CMAKE-PIP] extension %s\n'
                         '\tcmake located %s\n'
                         '\tcmake option %s\n',
                         ext.name,
                         ext.cmake_file if ext.cmake_file else 'ERRR',
                         ext.cmake_options)  # if ext.cmake_options else 'default options')
        else:
            log.info('[CMAKE-PIP] NO EXTENSION')

        # performs the configuration and building for all extensions
        for ext in self.extension_list:
            self.cmake_configure_and_build(ext)

        log.info('[CMAKE-PIP] build_cmake -- install ok')

        pass  # class build_cmake

    def get_extension_build_location(self, ext):
        # build location
        for k, v in self.cached_cmake_configure.items():
            if ext in v:
                return os.path.join(self.cmake_build_location, k)
        else:
            assert(False), 'Cannot find the configuration of the current extension'

    def get_outputs(self):
        """Returns the list of files generated by this specific build command"""
        global cmake_install_location

        # Apparently needed while undocumented. Note: should be able to run it in dry-run, which is now impossible
        log.warn('#' * 40 + " I am in get_outputs, build directory is " + self.build_temp)

        if self.extension_list:

            for ext in self.extension_list:
                log.info('[CMAKE-PIP] extension %s\n'
                         '\tcmake located %s\n'
                         '\tcmake option %s\n',
                         ext.name,
                         ext.cmake_file if ext.cmake_file else 'ERRR',
                         ext.cmake_options)  # if ext.cmake_options else 'default options')
        else:
            log.info('[CMAKE-PIP] NO EXTENSION')

        # here we retrieve the information from cmake itself.
        # todo: abstract the file that is read: this is the component that is installed, the python script
        # here should not know about that
        list_installed_files = []

        if self.extension_list:
            for ext in self.extension_list:

                build_location = self.get_extension_build_location(ext)

                if ext.cmake_install or ext.cmake_install_component:
                    # if the targets goes into an install, we read the file generated by
                    # cmake to retrieve the content of this component.
                    # in case of a component, we read that component file

                    if ext.cmake_install_component:
                        # "install_manifest_%s.txt" % component : the file generated by cmake
                        component_file = 'install_manifest_%s.txt' % ext.cmake_install_component
                    else:
                        component_file = 'install_manifest.txt'

                    with open(os.path.join(build_location, component_file), 'r') as f:
                        for l in f.readlines():
                            list_installed_files.append(os.path.relpath(l.strip(), self.build_lib))

                elif ext.cmake_locate_extensions:
                    # if the component is not specified, and this one is specified instead,
                    # we look for all compatible shared libraries generated under
                    # the build location

                    import imp
                    extensions = [i[0] for i in imp.get_suffixes() if i[2] == imp.C_EXTENSION]
                    selected_files = []
                    for dirpath, dirnames, filenames in os.walk(self.build_platlib):

                        selected_files += [os.path.join(dirpath, i) for i in filenames if os.path.splitext(i)[1] in extensions]

                    list_installed_files += selected_files

        # import ipdb; ipdb.set_trace();

        log.warn("returning %s", '\n\t'.join(list_installed_files))
        return list_installed_files

    # not needed as 'create_package_layout' is called from the run
    # sub_commands = Command.sub_commands #[('create_package_layout', None)] +


class build_ext(_build_ext):
    sub_commands = [('build_cmake', None)] + _build_ext.sub_commands

    def build_extensions(self):
        """Pass the CMake extensions to the appropriate builder"""

        log.info('#' * 40)
        log.info('[CMAKE-PIP] build_ext')

        ext_list = [ext for ext in self.extensions if isinstance(ext, ExtensionCMake)]

        if ext_list:
            log.debug("CMake extension found")
            for ext in ext_list:
                log.debug("- %s", ext.name)

        build_cmake = self.get_finalized_command('build_cmake', 1)
        build_cmake.set_extensions(ext_list)

        return _build_ext.build_extensions(self)

    def build_extension(self, ext):
        log.debug("build_extension for extension %s", ext.name)
        if(not isinstance(ext, ExtensionCMake)):
            return _build_ext.build_extension(self, ext)

        log.debug("build_extension for CMake %s", ext.name)

        # the 1 at the end construct the object always, even if not specified on
        # the command line.
        build_cmake = self.get_finalized_command('build_cmake', 1)

        print 'is debug?', self.debug

        # for each of the cmake extensions, configure this one a bit
        # - name of the target
        # - build location
        # - install location, maybe run the tests

        # right now, doing this only stuff
        build_cmake.run()

        # now maybe extend the self.libraries

        pass

    def get_outputs(self):

        # get all the outputs for the other commands
        pruned_extension = [i for i in self.extensions if not isinstance(i, ExtensionCMake)]
        original = self.extensions
        self.extensions = pruned_extension

        r = _build_ext.get_outputs(self)

        self.extensions = original

        # get the outputs of the cmake commands
        build_cmd = self.get_finalized_command('build_cmake')
        build_files = build_cmd.get_outputs()
        build_dir = getattr(build_cmd, 'build_platlib')

        return r + build_files


def setup(*args, **kwargs):
    if 'cmdclass' in kwargs:
        pass
    else:
        kwargs['cmdclass'] = {'build_ext': build_ext,
                              'build_cmake': build_cmake}
    return _setup(*args, **kwargs)


#============================================================================
ext1 = ExtensionCMake('_sxtwl',
                      'CMakeLists.txt',
                      cmake_options=["-DSXTWL_WRAPPER_PYTHON=1"],
                      cmake_locate_extensions=True
                     )


setup(name='sxtwl',
      version='1.0.0',
      description='一个非常好用的天文历库',
      author='yuangu',
      author_email='lifulinghan@aol.com',
      url='https://github.com/yuangu/sxtwl_cpp',
      license='GPL-v3.0',
      ext_modules=[ext1],

)