import io
import os
import sys
import re
from shutil import rmtree
import textwrap
import shlex
import subprocess

from setuptools import find_packages, setup, Command, Extension
from setuptools.command.build_ext import build_ext
from distutils.errors import CompileError, DistutilsError, DistutilsPlatformError, LinkError, DistutilsSetupError
from distutils import log as distutils_logger
from distutils.version import LooseVersion
import traceback
from click.decorators import option
from pexpect import expect

fps_lib = Extension('fps.wrapper.c_lib', [])

# Package meta-data.
NAME = 'fps'
DESCRIPTION = ''
URL = ''
EMAIL = 'xyao@cs.hku.hk'
AUTHOR = 'xyao, HKU'
REQUIRES_PYTHON = '>=2.7.0'
VERSION = None

# What packages are required for this module to be executed?
REQUIRED = [
    # 'cffi>=1.4.0',
]

# What packages are optional?
EXTRAS = {
    # 'fancy feature': ['django'],
}

here = os.path.abspath(os.path.dirname(__file__))

# Import the README and use it as the long-description.
# Note: this will only work if 'README.md' is present in your MANIFEST.in file!
try:
    with io.open(os.path.join(here, '../README.md'), encoding='utf-8') as f:
        long_description = '\n' + f.read()
except OSError:
    long_description = DESCRIPTION

# Load the package's __version__.py module as a dictionary.
about = {}
if not VERSION:
    with open(os.path.join(here, NAME, '__version__.py')) as f:
        exec(f.read(), about)
else:
    about['__version__'] = VERSION

def is_build_action():
    if len(sys.argv) <= 1:
        return False

    if sys.argv[1].startswith('build'):
        return True

    if sys.argv[1].startswith('bdist'):
        return True

    if sys.argv[1].startswith('install'):
        return True
    
# Start to build c libs
# ------------------------------------------------
def test_compile(build_ext, name, code, libraries=None, include_dirs=None, library_dirs=None,
                 macros=None, extra_compile_preargs=None, extra_link_preargs=None):
    test_compile_dir = os.path.join(build_ext.build_temp, 'test_compile')
    if not os.path.exists(test_compile_dir):
        os.makedirs(test_compile_dir)

    source_file = os.path.join(test_compile_dir, '%s.cc' % name)
    with open(source_file, 'w') as f:
        f.write(code)

    compiler = build_ext.compiler
    [object_file] = compiler.object_filenames([source_file])
    shared_object_file = compiler.shared_object_filename(
        name, output_dir=test_compile_dir)

    compiler.compile([source_file], extra_preargs=extra_compile_preargs,
                     include_dirs=include_dirs, macros=macros)
    compiler.link_shared_object(
        [object_file], shared_object_file, libraries=libraries, library_dirs=library_dirs,
        extra_preargs=extra_link_preargs)

    return shared_object_file    
    
def get_cpp_flags(build_ext):
    last_err = None
    default_flags = ['-std=c++11', '-fPIC', '-O2', '-Wall']
    avx_flags = ['-mf16c', '-mavx']
    flags_to_try = []
    if sys.platform == 'darwin':
        # Darwin most likely will have Clang, which has libc++.
        flags_to_try = [default_flags + ['-stdlib=libc++'] + avx_flags,
                        default_flags + avx_flags,
                        default_flags + ['-stdlib=libc++'],
                        default_flags]
    else:
        flags_to_try = [default_flags + avx_flags,
                        default_flags + ['-stdlib=libc++'] + avx_flags,
                        default_flags,
                        default_flags + ['-stdlib=libc++']]
    for cpp_flags in flags_to_try:
        try:
            test_compile(build_ext, 'test_cpp_flags', extra_compile_preargs=cpp_flags,
                         code=textwrap.dedent('''\
                    #include <unordered_map>
                    void test() {
                    }
                    '''))

            return cpp_flags
        except (CompileError, LinkError):
            last_err = 'Unable to determine C++ compilation flags (see error above).'
        except Exception:
            last_err = 'Unable to determine C++ compilation flags.  ' \
                       'Last error:\n\n%s' % traceback.format_exc()

    raise DistutilsPlatformError(last_err)

def get_link_flags(build_ext):
    last_err = None
    libtool_flags = []
    ld_flags = []
    flags_to_try = []
    if sys.platform == 'darwin':
        flags_to_try = [libtool_flags, ld_flags]
    else:
        flags_to_try = [ld_flags, libtool_flags]
    for link_flags in flags_to_try:
        try:
            test_compile(build_ext, 'test_link_flags', extra_link_preargs=link_flags,
                         code=textwrap.dedent('''\
                    void test() {
                    }
                    '''))

            return link_flags
        except (CompileError, LinkError):
            last_err = 'Unable to determine C++ link flags (see error above).'
        except Exception:
            last_err = 'Unable to determine C++ link flags.  ' \
                       'Last error:\n\n%s' % traceback.format_exc()

    raise DistutilsPlatformError(last_err)

def get_wrapper_options(build_ext):
    cpp_flags = get_cpp_flags(build_ext)
    link_flags = get_link_flags(build_ext)

    MACROS = [('EIGEN_MPL2_ONLY', 1)]
    INCLUDES = ['../include']
    SOURCES = ['fps/wrapper/scheduler.cc',
               'fps/wrapper/server.cc',
               'fps/wrapper/tools.cc',
               'fps/wrapper/worker.cc']
    if "BYTEPS_USE_MPI" in os.environ and os.environ["BYTEPS_USE_MPI"] == "1":
        mpi_flags = get_mpi_flags()
        COMPILE_FLAGS = cpp_flags + \
            shlex.split(mpi_flags) + ["-DBYTEPS_USE_MPI"]
        LINK_FLAGS = link_flags + shlex.split(mpi_flags)
    else:
        COMPILE_FLAGS = cpp_flags
        LINK_FLAGS = link_flags
    LIBRARY_DIRS = []
    LIBRARIES = []

    # RDMA and NUMA libs

    # ps-lite
    EXTRA_OBJECTS = ['../build/libps.a',
                     '../deps/lib/libprotobuf-lite.a',
                     '../deps/lib/libzmq.a']

    return dict(MACROS=MACROS,
                INCLUDES=INCLUDES,
                SOURCES=SOURCES,
                COMPILE_FLAGS=COMPILE_FLAGS,
                LINK_FLAGS=LINK_FLAGS,
                LIBRARY_DIRS=LIBRARY_DIRS,
                LIBRARIES=LIBRARIES,
                EXTRA_OBJECTS=EXTRA_OBJECTS)

def build_wrapper_extension(build_ext, options):

    # We assume we have CUDA
    '''
    cuda_include_dirs, cuda_lib_dirs = get_cuda_dirs(
        build_ext, options['COMPILE_FLAGS'])
    options['MACROS'] += [('HAVE_CUDA', '1')]
    options['INCLUDES'] += cuda_include_dirs
    options['LIBRARY_DIRS'] += cuda_lib_dirs
    options['LIBRARIES'] += ['cudart']
    tensorflow_lib.define_macros = options['MACROS']
    tensorflow_lib.include_dirs = options['INCLUDES']
    tensorflow_lib.sources = options['SOURCES'] + \
        ['byteps/tensorflow/ops.cc']
    tensorflow_lib.extra_compile_args = options['COMPILE_FLAGS'] + \
        tf_compile_flags
    tensorflow_lib.extra_link_args = options['LINK_FLAGS'] + tf_link_flags
    tensorflow_lib.library_dirs = options['LIBRARY_DIRS']
    tensorflow_lib.libraries = options['LIBRARIES']
    tensorflow_lib.extra_objects = options['EXTRA_OBJECTS']
    '''
    # fps_lib.define_macros = options['MACROS']
    print(options)
    fps_lib.include_dirs = options['INCLUDES']
    fps_lib.sources = options['SOURCES']
    fps_lib.extra_compile_args = options['COMPILE_FLAGS']
    fps_lib.extra_link_args = options['LINK_FLAGS']
    # fps_lib.library_dirs = options['LIBRARY_DIRS']
    # fps_lib.libraries = options['LIBRARIES']
    fps_lib.extra_objects = options['EXTRA_OBJECTS']
    build_ext.build_extension(fps_lib)

# run the customize_compiler

class custom_build_ext(build_ext):

    def build_extensions(self):
        if not os.path.exists("../build/libps.a") or \
           not os.path.exists("../deps/lib"):
            make_option = ""
            if os.environ.get('CI', 'false') == 'false':
                make_option += "-j "
            '''
            if int(os.environ.get('BYTEPS_USE_RDMA', 0)):
                make_option += "USE_RDMA=1 "
            '''

            make_process = subprocess.Popen('make ' + make_option,
                                            cwd='../',
                                            stdout=sys.stdout,
                                            stderr=sys.stderr,
                                            shell=True)
            make_process.communicate()
            if make_process.returncode:
                raise DistutilsSetupError('An ERROR occured while running the '
                                          'Makefile for the ps-lite library. '
                                          'Exit code: {0}'.format(make_process.returncode))

        options = get_wrapper_options(self)
        # print(options)
        built_plugins = []
        
        try:
            build_wrapper_extension(self, options)
            built_plugins.append(True)
        except:
            built_plugins.append(False)
            raise

        if not built_plugins:
            raise DistutilsError(
                'fps, TensorFlow, MXNet, PyTorch plugins were excluded from build. Aborting.')
        if not any(built_plugins):
            raise DistutilsError(
                'None of fps, TensorFlow, MXNet, PyTorch plugins were built. See errors above.')    
'''
        # If PyTorch is installed, it must be imported before others, otherwise
        # we may get an error: dlopen: cannot load any more object with static TLS
        if not int(os.environ.get('BYTEPS_WITHOUT_PYTORCH', 0)):
            dummy_import_torch()

        if not int(os.environ.get('BYTEPS_WITHOUT_TENSORFLOW', 0)):
            try:
                build_tf_extension(self, options)
                built_plugins.append(True)
            except:
                if not int(os.environ.get('BYTEPS_WITH_TENSORFLOW', 0)):
                    print('INFO: Unable to build TensorFlow plugin, will skip it.\n\n'
                          '%s' % traceback.format_exc())
                    built_plugins.append(False)
                else:
                    raise
        if not int(os.environ.get('BYTEPS_WITHOUT_MXNET', 0)):
            try:
                build_mx_extension(self, options)
                built_plugins.append(True)
                print('INFO: MXNet extension is built successfully.')
            except:
                if not int(os.environ.get('BYTEPS_WITH_MXNET', 0)):
                    print('INFO: Unable to build MXNet plugin, will skip it.\n\n'
                          '%s' % traceback.format_exc())
                    built_plugins.append(False)
                else:
                    raise
        if not int(os.environ.get('BYTEPS_WITHOUT_PYTORCH', 0)):
            try:
                torch_version = check_torch_version()
                build_torch_extension(self, options, torch_version)
                built_plugins.append(True)
                print('INFO: PyTorch extension is built successfully.')
            except:
                if not int(os.environ.get('BYTEPS_WITH_PYTORCH', 0)):
                    print('INFO: Unable to build PyTorch plugin, will skip it.\n\n'
                          '%s' % traceback.format_exc())
                    built_plugins.append(False)
                else:
                    raise
'''

# Where the magic happens:
setup(
    name=NAME,
    version=about['__version__'],
    description=DESCRIPTION,
    long_description=long_description,
    long_description_content_type='text/markdown',
    author=AUTHOR,
    author_email=EMAIL,
    python_requires=REQUIRES_PYTHON,
    url=URL,
    packages=find_packages(exclude=('tests',)),
    install_requires=REQUIRED,
    extras_require=EXTRAS,
    include_package_data=True,
    license='MIT',
    classifiers=[
        # Trove classifiers
        # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy'
    ],
    ext_modules=[fps_lib],
    # $ setup.py publish support.
    cmdclass={
        'build_ext': custom_build_ext
    },
    # cffi is required for PyTorch
    # If cffi is specified in setup_requires, it will need libffi to be installed on the machine,
    # which is undesirable.  Luckily, `install` action will install cffi before executing build,
    # so it's only necessary for `build*` or `bdist*` actions.
    setup_requires=[],
)











