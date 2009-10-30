# SConstruct

e = Environment(CXXFLAGS = '-g -Wall')
e.ParseConfig('allegro-config --cflags --libs')
e.Program('bin/island-adventure',
	Glob('./src/*.cpp'),
	CPPPATH = Split("""
	.
	./include/
	"""),
	LIBS = e['LIBS'] + Split("""
	png
	"""))
