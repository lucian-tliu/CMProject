from setuptools import setup, Extension
import pybind11
import sys
import os

# 获取项目根目录
project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# 源文件列表
sources = [
    'module/ising_bindings.cpp',
    'src/lattice.cpp',
    'src/isingspins.cpp',
    'src/flip_algorithm.cpp',
]

# 完整路径
source_paths = [os.path.join(project_root, src) for src in sources]

# 包含目录
include_dirs = [
    pybind11.get_include(),
    pybind11.get_include(user=True),
    os.path.join(project_root, 'include'),
    project_root,
]

# 编译器参数
extra_compile_args = ['-std=c++17', '-O3']
if sys.platform == 'linux':
    extra_compile_args.append('-fopenmp')
    extra_link_args = ['-fopenmp']
elif sys.platform == 'darwin':  # macOS
    extra_compile_args.extend(['-Xpreprocessor', '-fopenmp'])
    extra_link_args = ['-lomp']
elif sys.platform == 'win32':   # Windows
    extra_compile_args.append('/openmp')
    extra_link_args = []
else:
    extra_link_args = []

# 扩展模块定义
ext_modules = [
    Extension(
        'ising_model',
        sources=source_paths,
        include_dirs=include_dirs,
        language='c++',
        extra_compile_args=extra_compile_args,
        extra_link_args=extra_link_args,
    ),
]

setup(
    name='ising_model',
    version='0.1.0',
    author='Your Name',
    description='Ising model simulation with Metropolis and Wolff algorithms',
    long_description=open(os.path.join(project_root, 'README.md')).read() if os.path.exists(os.path.join(project_root, 'README.md')) else '',
    ext_modules=ext_modules,
    zip_safe=False,
    python_requires='>=3.6',
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Topic :: Scientific/Engineering :: Physics',
    ],
)