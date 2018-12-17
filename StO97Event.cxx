#include "StO97Event.h"
#include <limits>

ClassImp(StO97Event)

//_________________
StO97Event::StO97Event() : mRunId(0), mEventNumber(0),
  mImpactPar(0.), mEventPlaneRot(0.), mNTracks(0), mTransverseSphericity(0),
  mTransverseSphericity2(0) {

  mTracks = new TClonesArray("StO97Track", 1500);
}

//_________________
StO97Event::StO97Event(const StO97Event &ev) {

  //Copy constructor
  mRunId = ev.mRunId;
  mEventNumber = ev.mEventNumber;
  mImpactPar = ev.mImpactPar;
  mEventPlaneRot = ev.mEventPlaneRot;
  mNTracks = ev.mNTracks;
  mTransverseSphericity = ev.mTransverseSphericity;
  mTransverseSphericity2 = ev.mTransverseSphericity2;
  mTracks = new TClonesArray(*ev.mTracks);
}

//_________________
StO97Event &StO97Event::operator=(const StO97Event &ev) {
  
  if (this != &ev) {
    mRunId = ev.mRunId;
    mEventNumber = ev.mEventNumber;
    mNTracks = ev.mNTracks;
    mImpactPar = ev.mImpactPar;
    mEventPlaneRot = ev.mEventPlaneRot;
    mTransverseSphericity = ev.mTransverseSphericity;
    mTransverseSphericity2 = ev.mTransverseSphericity2;
    mTracks = new TClonesArray(*ev.mTracks); //Is this correct?
  }
  return *this;
}

//_________________
StO97Event::~StO97Event() {
  if (mTracks) {
    delete mTracks;
    mTracks = 0;
  }
}

//_________________
StO97Track *StO97Event::AddOscar97Track() {

  TClonesArray &tracks = *mTracks;
  StO97Track *track = new(tracks[mNTracks++]) StO97Track;
  return track;
}

//_________________
void StO97Event::Clear(Option_t* option) {
  mRunId = 0;
  mEventNumber = 0;
  mNTracks = 0;
  mImpactPar = 0.;
  mEventPlaneRot = 0.;
  mTransverseSphericity = 0;
  mTransverseSphericity2 = 0;
  mTracks->Delete();
}

//_________________
void StO97Event::SetTransverseSphericity(Float_t x) {
  if( x<0 || x>std::numeric_limits<unsigned char>::max() ) {
    mTransverseSphericity = std::numeric_limits<unsigned char>::max();
  }
  else {
    mTransverseSphericity = (UChar_t)(x * 100.);
  }
}

//_________________
void StO97Event::SetTransverseSphericity2(Float_t x) {
  if( x<0 || x>std::numeric_limits<unsigned char>::max() ) {
    mTransverseSphericity2 = std::numeric_limits<unsigned char>::max();
  }
  else {
    mTransverseSphericity2 = (UChar_t)(x * 100.);
  }
}
