//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//
#ifndef StHbtO97DstReader_hh
#define StHbtO97DstReader_hh

#include "StHbtMaker/StHbtEventReader.h"
#include "StHbtMaker/StHbtEvent.h"
#include "StHbtMaker/StHbtTrack.h"
#include "StHbtMaker/StHbtEnumeration.h"

#include "StO97Event.h"
#include "TSystem.h"
#include "TChain.h"
#include <string>
#include <fstream>
#include <stdlib.h>
#include "TDatabasePDG.h"
#include "TVector3.h"

class TTree;
class TFile;

//_________________
class StHbtO97DstReader : public StHbtEventReader {
private:
  string mDir;
  string mFileName;
  string mFilter;

  TDatabasePDG *pdgDb;  
  bool          mDebug;
  bool          mIsaJet;      // default is the PDG codes
  unsigned int  mEventIndex;
  unsigned int  mNEvents;
  TChain       *mTChain;
  TTree        *mTree;
  StO97Event   *mO97Event;
  float         mTotalTracks;

  int mMaxFiles;

  int InitRead(string dir, string fileName,
               string filter, int maxFiles);
  int FillChain(TChain *chain, const char *fileName, 
                int maxFiles);
  int FillChain(TChain *chain, char *dir,
                const char *filter, int maxFiles);
  void UninitRead();
  float dedxMean(float mass, float mom);
  StHbtEvent *Read();
public:
  StHbtO97DstReader(const char *dir, const char *fileName,
                    const char *filter = ".", int maxFiles = 1e9, bool debug = false);
  StHbtEvent *returnHbtEvent();
  void PrintTotalTracks();
  StHbtString Report();
  int GetNEvents();
  void SetIsaJetMode(bool mode) { mIsaJet = mode; }

  ClassDef(StHbtO97DstReader, 2)
};

#endif
