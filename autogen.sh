#!/bin/sh

set +x
autoreconf --force --install -I config -I m4
rm -rf autom4te.cache

