CXXFLAGS=`root-config --libs --cflags --glibs`

all: recoefficiencytnp globalefficiencytnp idipefficiencytnp triggermatchtnp isolationtnp isolationwithouttrigger isolationzqtprojection isolationzqt isolationdeltaphistar idtnp iptnp errorfunction isolationwithouttriggererrorfunction isolationwithouttriggererrorfunctiontnp unfoldingmatrixreco unfoldingmatrixglobal unfoldingmatrixrest spectracomparison zptcomparison zqtprojectioncomparison

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

isolationwithouttrigger: Functions.o libFunctions.so isolationwithouttrigger.C
	g++ $(CXXFLAGS) -O3 isolationwithouttrigger.C -L. -lFunctions -o isolationwithouttrigger

isolationzqtprojection: Functions.o libFunctions.so isolationzqtprojection.C
	g++ $(CXXFLAGS) -O3 isolationzqtprojection.C -L. -lFunctions -o isolationzqtprojection

isolationzqt: Functions.o libFunctions.so isolationzqt.C
	g++ $(CXXFLAGS) -O3 isolationzqt.C -L. -lFunctions -o isolationzqt

isolationdeltaphistar: Functions.o libFunctions.so isolationdeltaphistar.C
	g++ $(CXXFLAGS) -O3 isolationdeltaphistar.C -L. -lFunctions -o isolationdeltaphistar

isolationwithouttriggererrorfunction: Functions.o libFunctions.so isolationwithouttriggererrorfunction.C
	g++ $(CXXFLAGS) -O3 isolationwithouttriggererrorfunction.C -L. -lFunctions -o isolationwithouttriggererrorfunction

isolationwithouttriggererrorfunctiontnp: Functions.o libFunctions.so isolationwithouttriggererrorfunctiontnp.C
	g++ $(CXXFLAGS) -O3 isolationwithouttriggererrorfunctiontnp.C -L. -lFunctions -o isolationwithouttriggererrorfunctiontnp

unfoldingmatrixreco: Functions.o libFunctions.so unfoldingmatrixreco.C
	g++ $(CXXFLAGS) -O3 unfoldingmatrixreco.C -L. -lFunctions -o unfoldingmatrixreco

unfoldingmatrixglobal: Functions.o libFunctions.so unfoldingmatrixglobal.C
	g++ $(CXXFLAGS) -O3 unfoldingmatrixglobal.C -L. -lFunctions -o unfoldingmatrixglobal

unfoldingmatrixrest: Functions.o libFunctions.so unfoldingmatrixrest.C
	g++ $(CXXFLAGS) -O3 unfoldingmatrixrest.C -L. -lFunctions -o unfoldingmatrixrest

idtnp: Functions.o libFunctions.so idtnp.C
	g++ $(CXXFLAGS) -O3 idtnp.C -L. -lFunctions -o idtnp

iptnp: Functions.o libFunctions.so iptnp.C
	g++ $(CXXFLAGS) -O3 iptnp.C -L. -lFunctions -o iptnp

errorfunction: Functions.o libFunctions.so errorfunction.C
	g++ $(CXXFLAGS) -O3 errorfunction.C -L. -lFunctions -o errorfunction

spectracomparison: Functions.o libFunctions.so spectracomparison.C
	g++ $(CXXFLAGS) -O3 spectracomparison.C -L. -lFunctions -o spectracomparison

zptcomparison: Functions.o libFunctions.so zptcomparison.C
	g++ $(CXXFLAGS) -O3 zptcomparison.C -L. -lFunctions -o zptcomparison

zqtprojectioncomparison: Functions.o libFunctions.so zqtprojectioncomparison.C
	g++ $(CXXFLAGS) -O3 zqtprojectioncomparison.C -L. -lFunctions -o zqtprojectioncomparison

Functions.o: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -c -fpic Functionstnp.C -o Functions.o

libFunctions.so: Functionstnp.C Functionstnp.h
	g++ $(CXXFLAGS) -O3 -shared -fpic -o libFunctions.so Functionstnp.C

