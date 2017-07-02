TEMPLATE = subdirs
# Needed to ensure that things are built right, which you have to do yourself :(
CONFIG += ordered

# All the projects in your application are sub-projects of your solution
SUBDIRS = virtualreality \
          vr-window

vr-window.depends = virtualreality
