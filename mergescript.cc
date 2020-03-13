#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"

using namespace std;

void mergescript(int channelMask, const char* detector, const char* files)
{
  cout << "Initialisieren" << endl;

  TFile* inputFile = new TFile("nELBE_beam_intTrg_0021.root", "READ");
  TTree* inputTree = (TTree*) inputFile->Get(detector);

  TFile* outputFile = new TFile("output.root", "RECREATE");
  TTree* outputTree = new TTree(detector, "Detector Data");

  string filesString(files);
  cout << filesString << endl;

  unsigned char channel;
  unsigned short energy;
  unsigned int timestamp;
  unsigned short flags;
  unsigned short timestampExtended;
  float time;
  unsigned int nSamples;
  unsigned char traceTypes;
  short trace1_nSamples;
  unsigned char dTrace1_nSamples;
  unsigned char dTrace2_nSamples;  


  inputTree->SetBranchAddress("channel", &channel);
  inputTree->SetBranchAddress("energy", &energy);
  inputTree->SetBranchAddress("timestamp", &timestamp);
  inputTree->SetBranchAddress("flags", &flags);
  inputTree->SetBranchAddress("timestampExtended", &timestampExtended);
  inputTree->SetBranchAddress("time", &time);
  inputTree->SetBranchAddress("nSamples", &nSamples);
  inputTree->SetBranchAddress("traceTypes", &traceTypes);
  inputTree->SetBranchAddress("trace1[nSamples]", &traceTypes);
  inputTree->SetBranchAddress("dTrace1[nSamples]", &dTrace1_nSamples);
  inputTree->SetBranchAddress("dTrace2[nSamples]", &dTrace2_nSamples);

  outputTree->Branch("channel", &channel, "channel/b");
  outputTree->Branch("energy", &energy, "energy/s");
  outputTree->Branch("timestamp", &timestamp, "timestamp/i");
  outputTree->Branch("flags", &flags, "flags/s");
  outputTree->Branch("timestampExtended", &timestampExtended, "timestampExtended/s");
  outputTree->Branch("time", &time, "time/F");
  outputTree->Branch("nSamples", &nSamples, "nSamples/i");
  outputTree->Branch("traceTypes", &traceTypes, "traceTypes/b");
  outputTree->Branch("trace1[nSamples]", &traceTypes, "trace1[nSamples]/S");
  outputTree->Branch("dTrace1[nSamples]", &dTrace1_nSamples, "dTrace1[nSamples]/b");
  outputTree->Branch("dTrace2[nSamples]", &dTrace2_nSamples, "dTrace2[nSamples]/b");

  long long entryCount = inputTree->GetEntries();

  cout << "Filtern" << endl;
  for(long long i = 0; i < entryCount; i++){
    inputTree->GetEntry(i);
    if (channelMask & (1 << channel)){
      outputTree->Fill();
    }
  }

  outputFile->Write();

  cout << "Fertig!" << endl;
}