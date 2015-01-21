# SConscript - build project under Linux

SOURCE = "main.cpp";
TARGET = "project"

SHADERS = Glob( "*.frag" ) + Glob( "*.vert" );
TEXTURES = Glob( "*.ppm" ) + Glob( "*.jpg" ) + Glob( "*.png" );

Import( "env" );
Import( "libGLUTIL" );
Import( "libLinmath" );

from SCript.Stages import config, build, install;

dataFiles = SHADERS + TEXTURES;

@build
def build_lab():
	obj = [env.Object(src) for src in SOURCE.split()];

	lib = [libGLUTIL, libLinmath];
	prg = env.Program( target = TARGET, source = obj + lib );
	
	# The following line ensures that files are moved to the build dir
	dat = [env.File(data) for data in dataFiles];

	return prg; 

@install
def install_files():
	dataPath = "#share/" + TARGET;

	iprog = env.Install( "#bin", TARGET );
	idata = [env.Install( dataPath, data ) for data in dataFiles];

	return [iprog] + idata;

# EOF vim:syntax=python:foldmethod=marker:ts=4:noexpandtab
