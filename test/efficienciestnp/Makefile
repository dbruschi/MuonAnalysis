CXXFLAGS=`root-config --libs --cflags --glibs`

recoefficiencytnp: Functions.o libFunctions.so recoefficiencytnp.C
	g++ $(CXXFLAGS) -O3 recoefficiencytnp.C -L. -lFunctions -o recoefficiencytnp 

globalefficiencytnp: Functions.o libFunctions.so globalefficiencytnp.C
	g++ $(CXXFLAGS) -O3 globalefficiencytnp.C -L. -lFunctions -o globalefficiencytnp 

Functions.o: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -c -fpic Functionstnp.C -o Functions.o

libFunctions.so: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -shared -fpic -o libFunctions.so Functionstnp.C

