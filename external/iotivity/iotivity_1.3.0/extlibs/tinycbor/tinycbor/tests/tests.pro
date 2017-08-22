TEMPLATE = subdirs
SUBDIRS = parser encoder cpp tojson
msvc: SUBDIRS -= tojson
