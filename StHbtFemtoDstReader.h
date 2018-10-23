//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//
#ifndef StHbtFemtoDstReader_hh
#define StHbtFemtoDstReader_hh

#include "StHbtMaker/StHbtEventReader.h"
#include "StHbtMaker/StHbtEvent.h"
#include "StHbtMaker/StHbtTrack.h"
#include "StHbtMaker/StHbtEnumeration.h"

#include "StFemtoEvent.h"
#include "TSystem.h"
#include "TChain.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include "TString.h"

class TTree;
class TFile;

//_________________
class StHbtFemtoDstReader : public StHbtEventReader {
 private:
  string mDir;
  string mFileName;
  string mFilter;

  bool          mDebug;
  unsigned int  mEventIndex;
  unsigned int  mNEvents;
  unsigned int  mTrig;
  TChain       *mTChain;
  TTree        *mTree;
  StFemtoEvent *mFemtoEvent;
  float         mTotalTracks;
  float         mSphLo, mSphHi;
  bool          mSphFlag;

  std::vector<unsigned int> mTrigIdCol;
  
  int mMaxFiles;
  

  int InitRead(string aDir, string aFileName,
	       string aFilter, int aMaxFiles);
  int FillChain(TChain *aChain, const char *aFileName, 
		int aMaxFiles);
  int FillChain(TChain *aChain, char *aDir,
		const char *aFilter, int aMaxFiles);
  void UninitRead();
  StHbtEvent *Read();
 public:
  StHbtFemtoDstReader(const char *aDirName, const char *aFileName,
		      const char *aFilter = ".", int aMaxFiles = 1e9, bool aDebug = false);
  StHbtEvent *returnHbtEvent();
  void AddTrigId(unsigned int aTrigId);
  void EnaSphericityCut() { mSphFlag = true; }
  void DisSphericityCut() { mSphFlag = false; }
  void SetSphericityCut(float lo, float hi) { mSphLo = lo; mSphHi = hi; }
  void PrintTotalTracks();
  void DoDebug(bool aDebug = false);
  StHbtString report();
  int GetNEvents();
  void SetTrigger(unsigned int trig, char mode);

  ClassDef(StHbtFemtoDstReader, 2)
};

#endif
