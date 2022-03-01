CXXFLAGS=`root-config --libs --cflags --glibs`

all: recoefficiencytnp globalefficiencytnp idipefficiencytnp triggermatchtnp isolationtnp

recoefficiencytnp: Functions.o libFunctions.so recoefficiencytnp.C
	g++ $(CXXFLAGS) -O3 recoefficiencytnp.C -L. -lFunctions -o recoefficiencytnp 

globalefficiencytnp: Functions.o libFunctions.so globalefficiencytnp.C
	g++ $(CXXFLAGS) -O3 globalefficiencytnp.C -L. -lFunctions -o globalefficiencytnp 

idipefficiencytnp: Functions.o libFunctions.so idipefficiencytnp.C
	g++ $(CXXFLAGS) -O3 idipefficiencytnp.C -L. -lFunctions -o idipefficiencytnp 

triggermatchtnp: Functions.o libFunctions.so triggermatchtnp.C
	g++ $(CXXFLAGS) -O3 triggermatchtnp.C -L. -lFunctions -o triggermatchtnp 

isolationtnp: Functions.o libFunctions.so isolationtnp.C
	g++ $(CXXFLAGS) -O3 isolationtnp.C -L. -lFunctions -o isolationtnp 

Functions.o: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -c -fpic Functionstnp.C -o Functions.o

libFunctions.so: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -shared -fpic -o libFunctions.so Functionstnp.C

