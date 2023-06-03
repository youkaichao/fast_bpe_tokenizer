import glob
import setuptools
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "fast_bpe_tokenizer._Tokenizer",
        ["src/base64.cpp", "src/bind.cpp"],
        include_dirs=["src/"],
        cxx_std=11,
        extra_compile_args=["-O3"],
    ),
]

setuptools.setup(
    name="fast_bpe_tokenizer",
    version="0.0.1",
    author="Kaichao You",
    author_email="youkaichao@gmail.com",
    description="A fast BPE tokenizer",
    url="https://github.com/youkaichao/fast_bpe_tokenizer",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    license="MIT",
    packages=["fast_bpe_tokenizer"],
    classifiers=[
        "Programming Language :: C++ :: 11",
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
)