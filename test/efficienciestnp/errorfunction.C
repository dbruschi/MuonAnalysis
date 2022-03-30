#include "TFile.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TF1.h"

void errorfunction(const char* name) {
	TFile* file=TFile::Open(name);
	TFile* output=new TFile("errorfunction.root","RECREATE");
	output->cd();
	TH2D* histo=(TH2D*)file->Get("histo3");
	TH1D* normchi2=new TH1D("normchi2","",100,0,10);
	TH1D* etabins=new TH1D("etabins","",1,0,1);
	TH1D* maxetabin=new TH1D("maxetabin","",1,0,1);
	TH1D* minetabin=new TH1D("minetabin","",1,0,1);
	etabins->SetBinContent(1,histo->GetNbinsX());
	maxetabin->SetBinContent(1,histo->GetXaxis()->GetBinCenter(histo->GetNbinsX())+histo->GetXaxis()->GetBinWidth(histo->GetNbinsX())/2);
	minetabin->SetBinContent(1,histo->GetXaxis()->GetBinCenter(1)-histo->GetXaxis()->GetBinWidth(1)/2);
	TH2D* pulls=new TH2D("pulls","",histo->GetNbinsX(),histo->GetXaxis()->GetBinCenter(1)-histo->GetXaxis()->GetBinWidth(1)/2,histo->GetXaxis()->GetBinCenter(histo->GetNbinsX())+histo->GetXaxis()->GetBinWidth(histo->GetNbinsX())/2,histo->GetNbinsY(),histo->GetYaxis()->GetBinCenter(1)-histo->GetYaxis()->GetBinWidth(1)/2,histo->GetYaxis()->GetBinCenter(histo->GetNbinsY())+histo->GetYaxis()->GetBinWidth(histo->GetNbinsY())/2);
	for (unsigned int i=1; i!=histo->GetNbinsX()+1; i++) {
		histo->GetXaxis()->SetRange(i,i);
		TH1D* histox=(TH1D*)histo->ProjectionY();
		std::string histoname("histo");
		histoname+=std::to_string(i);
		histox->SetName(histoname.c_str());
		std::string errorfunctionname("errorfunction");
		errorfunctionname+=std::to_string(i);
		std::string errorfunctiontitle("errorfunction");
		errorfunctiontitle+=" "+std::to_string(histo->GetXaxis()->GetBinCenter(i)-histo->GetXaxis()->GetBinWidth(i)/2);
		errorfunctiontitle+=" "+std::to_string(histo->GetXaxis()->GetBinCenter(i)+histo->GetXaxis()->GetBinWidth(i)/2);
		std::string histotitle("histo");
		histotitle+=" "+std::to_string(histo->GetXaxis()->GetBinCenter(i)-histo->GetXaxis()->GetBinWidth(i)/2);
		histotitle+=" "+std::to_string(histo->GetXaxis()->GetBinCenter(i)+histo->GetXaxis()->GetBinWidth(i)/2);
		histox->SetTitle(histotitle.c_str());
		TF1* fit=new TF1(errorfunctionname.c_str(),"[0]*ROOT::Math::erf((x-[1])/[2])+[3]",25.,60.);
		fit->SetTitle(errorfunctiontitle.c_str());
		fit->SetParameter(0,0.9);
		fit->SetParameter(1,30.);
		fit->SetParameter(2,5.);
		fit->SetParameter(3,0.);
		histox->Fit(errorfunctionname.c_str());
		normchi2->Fill(fit->GetChisquare()/fit->GetNDF());
		fit->Write();
		histo->GetXaxis()->SetRange(1,histo->GetNbinsX());
		for (unsigned int j=1; j!=histo->GetNbinsY()+1; j++) {
			pulls->SetBinContent(i,j,(fit->Eval(histo->GetYaxis()->GetBinCenter(j))-histo->GetBinContent(i,j))/histo->GetBinError(i,j));
		}
	}
	output->Write();
	output->Close();
}

int main(int argc, char **argv) {
	errorfunction(argv[1]);
	return 0;
}
