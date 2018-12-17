//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//
#ifndef STOSCAR97EVENT_H
#define STOSCAR97EVENT_H

#include <TObject.h>
#include <TClonesArray.h>

#include "StO97Track.h"

//_________________
class StO97Event : public TObject {

 public:
  //Constructor
  StO97Event();
  //Copy constructor
  StO97Event(const StO97Event &copy);
  //Assignment operator
  StO97Event &operator=(const StO97Event &event);
  //Destructor
  virtual ~StO97Event();
  //Clear option
  void Clear(Option_t* option= "");

  //Getters
  UInt_t  GetRunId() const       { return mRunId; }
  UInt_t  GetEventNumber() const { return mEventNumber; }
  Int_t   GetNTracks() const     { return (UShort_t)mNTracks; }
  Float_t GetImpactPar() const   { return mImpactPar; }
  Float_t GetEventPlaneRot() const         { return mEventPlaneRot; }
  Float_t GetTransverseSphericity() const  { return (Float_t)mTransverseSphericity * 0.01; }
  Float_t GetTransverseSphericity2() const { return (Float_t)mTransverseSphericity2 * 0.01; }
  TClonesArray  *GetTracks()      { return mTracks; }

  //Setters
  void SetRunId(Int_t id)                { (id>0) ? mRunId=(UInt_t)id : mRunId=(UInt_t)(-1 * id); }
  void SetEventNumber(Int_t num)         { (num>0) ? mEventNumber=(UInt_t)num : mEventNumber=(UInt_t)(-1 * num); }
  void SetImpactPar(Float_t impPar)      { mImpactPar = impPar; }
  void SetEventPlaneRot(Float_t epr)     { mEventPlaneRot = epr; }
  void SetTracks(TClonesArray *tracks)   { mTracks = tracks; }
  void SetTransverseSphericity(Float_t x);
  void SetTransverseSphericity2(Float_t x);
  StO97Track *AddOscar97Track();
  
 private:

  UInt_t  mRunId;                 //Run number: should be a unique number for the file
  UInt_t  mEventNumber;           //Event number
  Float_t mImpactPar;             //Impact parameter (fm/c)
  Float_t mEventPlaneRot;         //Rotation of the event plane (fixed to 0. in UrQMD)
  UShort_t mNTracks;              //Length of recorded StO97Tracks
  UChar_t mTransverseSphericity;  //For primary tracks with pT>0.15 GeV and |eta|<0.5
  UChar_t mTransverseSphericity2; //For primary tracks with pT>0.15 GeV and |eta|<1
  TClonesArray *mTracks;     //Array of StO97Tracks

  ClassDef(StO97Event, 1)
};

#endif 
