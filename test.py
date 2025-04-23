#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""This module tests if the custom resolver works.


>>> export TF_DEBUG=AR_RESOLVER_INIT
>>> export PXR_PLUGINPATH_NAME=$PWD/build/install/resources
>>> python test.py


"""

# IMPORT THIRD-PARTY LIBRARIES
from pxr import Ar


def main():
    """Run the main execution of the current script."""

    resolver = Ar.GetResolver()

    uri = "asset://project/char_cat/geo"

    ## print("This should still print an empty string", resolver.Resolve("Makefile"))
    ## print("This should print /bar", resolver.Resolve("foo:apple"), "<<<<")






if __name__ == "__main__":
    main()
