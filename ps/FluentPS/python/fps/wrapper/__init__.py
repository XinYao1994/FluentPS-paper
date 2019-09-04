import ctypes
import os
import sysconfig
from asn1crypto._ffi import null

def get_ext_suffix():
    """Determine library extension for various versions of Python."""
    ext_suffix = sysconfig.get_config_var('EXT_SUFFIX')
    if ext_suffix:
        return ext_suffix

    ext_suffix = sysconfig.get_config_var('SO')
    if ext_suffix:
        return ext_suffix

    return '.so'

def get_extension_full_path(pkg_path, *args):
    assert len(args) >= 1
    dir_path = os.path.join(os.path.dirname(pkg_path), *args[:-1])
    full_path = os.path.join(dir_path, args[-1] + get_ext_suffix())
    return full_path

class tools(object):
    """Wrapper class for the basic API."""

    def __init__(self, pkg_path, *args):
        full_path = get_extension_full_path(pkg_path, *args)
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)
        
    def __init__(self):
        full_path = get_extension_full_path(__file__, 'c_lib')
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)

    def NumWorkers(self):
        return self.C_LIB_CTYPES.NumWorkers()
    
    def NumServers(self):
        return self.C_LIB_CTYPES.NumServers()

    def IsWorker(self):
        return self.C_LIB_CTYPES.IsWorker()
    
    def IsServer(self):
        return self.C_LIB_CTYPES.IsServer()
    
    def IsScheduler(self):
        return self.C_LIB_CTYPES.IsScheduler()
    
    def Test_array_1(self, in_data):
        self.C_LIB_CTYPES.Test_array_1.argtypes = [ctypes.py_object]
        self.C_LIB_CTYPES.Test_array_1.restype = ctypes.py_object
        return self.C_LIB_CTYPES.Test_array_1(in_data)
    
    def Test_array_2(self):
        self.C_LIB_CTYPES.Test_array_2.restype = ctypes.py_object
        return self.C_LIB_CTYPES.Test_array_2()
    
    def Test_array_3(self):
        self.C_LIB_CTYPES.Test_array_3.restype = ctypes.py_object
        return self.C_LIB_CTYPES.Test_array_3()
    
    
class scheduler(object):
    """Wrapper class for the basic API."""

    def __init__(self, pkg_path, *args):
        full_path = get_extension_full_path(pkg_path, *args)
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)

    def __init__(self):
        full_path = get_extension_full_path(__file__, 'c_lib')
        # print(full_path) fps-0.1.0-py3.6-linux-x86_64.egg/fps/wrapper/c_lib.cpython-36m-x86_64-linux-gnu.so
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)
        
    def Init(self, custom_id, timeout=5):
        self.C_LIB_CTYPES.Init(custom_id, timeout)
    
    def Finalize(self, custom_id):
        self.C_LIB_CTYPES.Finalize(custom_id)
    
class server(object):
    """Wrapper class for the basic API."""

    def __init__(self, pkg_path, *args):
        full_path = get_extension_full_path(pkg_path, *args)
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)

    def __init__(self):
        full_path = get_extension_full_path(__file__, 'c_lib')
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)
        
    def Init(self, custom_id, timeout=5):
        self.C_LIB_CTYPES.Init_s(custom_id, timeout)
    
    def Finalize(self, custom_id):
        self.C_LIB_CTYPES.Finalize_s(custom_id)
    
    def create(self, app_id):
        self.C_LIB_CTYPES.create_s(app_id)
        
    def start(self, staleness=0, pro=1.0):
        self.C_LIB_CTYPES.start_s.argtypes = [ctypes.c_int, ctypes.c_double]
        self.C_LIB_CTYPES.start_s(staleness, pro)
    
class worker(object):
    """Wrapper class for the basic API."""

    def __init__(self, pkg_path, *args):
        full_path = get_extension_full_path(pkg_path, *args)
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)

    def __init__(self):
        full_path = get_extension_full_path(__file__, 'c_lib')
        self.C_LIB_CTYPES = ctypes.CDLL(full_path, mode=ctypes.RTLD_GLOBAL)
        
    def Init(self, custom_id, timeout=5):
        self.C_LIB_CTYPES.Init_w(custom_id, timeout)
    
    def Finalize(self, custom_id):
        self.C_LIB_CTYPES.Finalize_w(custom_id)
    
    def create(self, app_id, custom_id):
        self.C_LIB_CTYPES.create_w(app_id, custom_id)
        
    def start(self):
        self.C_LIB_CTYPES.start_w()
        
    # int push(const std::vector<ps::Key>& keys, const std::vector<Val>& vals);
    def push(self, keys, vals, lens=None):
        self.C_LIB_CTYPES.push.argtypes = [ctypes.py_object, ctypes.py_object, ctypes.py_object]
        return self.C_LIB_CTYPES.push(keys, vals, lens)
    
    # int pull(const std::vector<ps::Key>& keys, std::vector<Val>* vals);
    def pull(self, keys, lens=None):
        self.C_LIB_CTYPES.pull.argtypes = [ctypes.py_object, ctypes.py_object]
        return self.C_LIB_CTYPES.pull(keys, lens)
        
    def wait(self, timestamp):
        self.C_LIB_CTYPES.wait(timestamp)
       
    ''' int spush(const std::vector<ps::Key>& keys, const std::vector<float>& vals, int staleness); ''' 
    def spush(self, keys, vals, staleness, lens=None):
        self.C_LIB_CTYPES.spush.argtypes = [ctypes.py_object, ctypes.py_object, ctypes.c_int, ctypes.py_object]
        return self.C_LIB_CTYPES.spush(keys, vals, staleness, lens)
    
    ''' int spull(const std::vector<ps::Key>& keys, std::vector<Val>* vals, int staleness) '''
    def spull(self, keys, staleness, lens=None):
        self.C_LIB_CTYPES.spull.argtypes = [ctypes.py_object, ctypes.c_int, ctypes.py_object]
        return self.C_LIB_CTYPES.spull(keys, staleness, lens)
    
    def read(self):
        self.C_LIB_CTYPES.read_vals.restype = ctypes.py_object
        return self.C_LIB_CTYPES.read_vals()



