# Attempt to load a config.make file.
# If none is found, project defaults in config.project.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# make sure the the OF_ROOT location is defined
ifndef OF_ROOT
    OF_ROOT=../../..
endif

# call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk

test: test.o
	clang++ testbin/test.o -lgtest -lgtest_main -o testbin/test

test.o: src/test_collision.cpp
	clang++ -c src/test_collision.cpp -I/home/psamtani/development/openFrameworks_0.8.1/addons/ofxOpenCv/src/ -o testbin/test.o

