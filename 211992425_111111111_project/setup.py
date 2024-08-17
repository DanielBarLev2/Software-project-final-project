from setuptools import Extension, setup

# Define the extension module
module = Extension("mysymnmf", sources=['symnmfmodule.c'])

# Set up the package
setup(
    name='mysymnmf',            # Name of the package
    version='1.0',              # Version of the package
    description='Python wrapper for custom C extension',  # Short description
    ext_modules=[module]        # List of extension modules to include in the package
)

