from setuptools import Extension, setup

module = Extension("sym", sources=['symnmfmodule.c'])
setup(name='sym',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])