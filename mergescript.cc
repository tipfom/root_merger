#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TCanvas.h"

using namespace std;

void _merge(int channelMask, const char* detector, string file, TTree* outputTree) {
  cout << "Verarbeite " << file << "... ";

  char fileArray[1024];
  strcpy(fileArray, file.c_str());

  TFile* inputFile = new TFile(fileArray, "READ");
  TTree* inputTree = (TTree*) inputFile->Get(detector);

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

  cout << "Filtern ...";
  for(long long i = 0; i < entryCount; i++){
    inputTree->GetEntry(i);
    if (channelMask & (1 << channel)){
      outputTree->Fill();
    }
  }
  cout << "Fertig!" << endl;

}

void mergescript(int channelMask, const char* detector, const char* files)
{
  cout << "Initialisieren" << endl;
  TFile* outputFile = new TFile("output.root", "RECREATE");
  TTree* outputTree = new TTree(detector, "Detector Data");

  string filesString(files);
  string delimiter = ",";
  size_t position = 0;
  string token;
  while((position = filesString.find(delimiter)) != string::npos){
    token = filesString.substr(0, position);
    _merge(channelMask, detector, token, outputTree);
    filesString.erase(0, position + delimiter.length());
  }
  _merge(channelMask, detector, filesString, outputTree);
  outputFile->Write();
  cout << "Gespeichert!" << endl;
}
