#include "StFemtoTrack.h"
#include "TMath.h"

ClassImp(StFemtoTrack)

//_________________
StFemtoTrack::StFemtoTrack() {

  mId = -999;
  mNSigmaElectron = -999;
  mNSigmaPion = -999;
  mNSigmaKaon = -999;
  mNSigmaProton = -999;
  mDedx = 0;
  mMomX = 0.;
  mMomY = 0.;
  mMomZ = 0.;
  mBeta = -999.;
}

//_________________
StFemtoTrack::StFemtoTrack(Float_t px, Float_t py, Float_t pz) {
  mId = -999;
  mNSigmaElectron = -999;
  mNSigmaPion = -999;
  mNSigmaKaon = -999;
  mNSigmaProton = -999;
  mDedx = 0;
  mMomX = px;
  mMomY = py;
  mMomZ = pz;
  mBeta = -999.;
}

//_________________
StFemtoTrack::~StFemtoTrack() {
  /* nothing to do*/
}

//_________________
Short_t StFemtoTrack::GetType() const {

  if( (mMomX <= 1e-5) &&
      (mMomY <= 1e-5) &&
      (mMomZ <= 1e-5) ) {
    return 0;            //Global
  }
  else {
    return 1;            //Primary
  }
}

//_________________
Bool_t StFemtoTrack::GetIsTofTrack() {

  if(mBeta < -1.) {
    return false;
  }
  else {
    return true;
  }
}

//_________________
void StFemtoTrack::SetP(Float_t px, Float_t py, Float_t pz) {

  mMomX = px;
  mMomY = py;
  mMomZ = pz;
}

//_________________
TVector3 StFemtoTrack::GetP() const {

  TVector3 p(mMomX, mMomY, mMomZ);
  return p;
}

//_________________
Float_t StFemtoTrack::GetPt() const {

  return TMath::Sqrt(mMomX*mMomX + mMomY*mMomY);
}

//_________________
Float_t StFemtoTrack::GetPtot() const {

  return TMath::Sqrt(mMomX*mMomX + mMomY*mMomY + mMomZ*mMomZ);
}

//_________________
Float_t StFemtoTrack::GetEta() const {

  float ptot = TMath::Sqrt(mMomX*mMomX + mMomY*mMomY + mMomZ*mMomZ);
  float eta = 0.5*TMath::Log( (ptot + mMomZ) / (ptot - mMomZ) );
  return eta;
}

//_________________
void StFemtoTrack::SetGlobalP(Float_t px, Float_t py, Float_t pz) {

  mGlobMomX = px;
  mGlobMomY = py;
  mGlobMomZ = pz;
}

//_________________
UShort_t StFemtoTrack::GetNHitsFit() const {
  
  if(StFemtoTrack::GetType() == 1) {
    return (TMath::Abs(mNHits)+1);
  }
  else {
    return TMath::Abs(mNHits);
  }
}

//_________________
TVector3 StFemtoTrack::GetGlobalP() const {
  
  TVector3 p( mGlobMomX, mGlobMomY, mGlobMomZ );
  return p;
}

//_________________
Float_t StFemtoTrack::GetPtGlobal() const {

  return TMath::Sqrt(mGlobMomX*mGlobMomX + mGlobMomY*mGlobMomY);
}

//_________________
Float_t StFemtoTrack::GetPtotGlobal() const {
  return TMath::Sqrt(mGlobMomX*mGlobMomX + mGlobMomY*mGlobMomY + mGlobMomZ*mGlobMomZ);
}

//_________________
Float_t StFemtoTrack::GetMassSqr() const {

  float mSqr = -999.;
  if(mBeta > 0.) {
    float ptot = TMath::Sqrt(mMomX*mMomX + mMomY*mMomY + mMomZ*mMomZ);
    mSqr = ptot*ptot * (1./(mBeta*mBeta) - 1.);
  }

  return mSqr;
}

//_________________
void StFemtoTrack::SetNSigmaElectron(Float_t ns) {

  if(TMath::Abs(ns*1000) >= SHORTMAX) {
    if(ns>0) {
      mNSigmaElectron = SHORTMAX;
    }
    else {
      mNSigmaElectron = -SHORTMAX;
    }
  }
  else {
    mNSigmaElectron = (Short_t)(ns*1000);
  }
}

//_________________
void StFemtoTrack::SetNSigmaPion(Float_t ns) {

  if(TMath::Abs(ns*1000) >= SHORTMAX) {
    if(ns>0) {
      mNSigmaPion = SHORTMAX;
    }
    else {
      mNSigmaPion = -SHORTMAX;
    }
  }
  else {
    mNSigmaPion = (Short_t)(ns*1000);
  }
}

//_________________
void StFemtoTrack::SetNSigmaKaon(Float_t ns) {

  if(TMath::Abs(ns*1000) >= SHORTMAX) {
    if(ns>0) {
      mNSigmaKaon = SHORTMAX;
    }
    else {
      mNSigmaKaon = -SHORTMAX;
    }
  }
  else {
    mNSigmaKaon = (Short_t)(ns*1000);
  }
}

//_________________
void StFemtoTrack::SetNSigmaProton(Float_t ns) {

  if(TMath::Abs(ns*1000) >= SHORTMAX) {
    if(ns>0) {
      mNSigmaProton = SHORTMAX;
    }
    else {
      mNSigmaProton = -SHORTMAX;
    }
  }
  else {
    mNSigmaProton = (Short_t)(ns*1000);
  }
}

//_________________
void StFemtoTrack::SetDCAxGlobal(Float_t x) {

  if(TMath::Abs(x*10000) >= SHORTMAX) {
    if(x>0) {
      mDCAxGlobal = SHORTMAX;
    }
    else {
      mDCAxGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAxGlobal = (Short_t)(x * 10000);
  }
}

//_________________
void StFemtoTrack::SetDCAyGlobal(Float_t y) {

  if(TMath::Abs(y*10000) >= SHORTMAX) {
    if(y>0) {
      mDCAyGlobal = SHORTMAX;
    }
    else {
      mDCAyGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAyGlobal = (Short_t)(y * 10000);
  }
}

//_________________
void StFemtoTrack::SetDCAzGlobal(Float_t z) {

  if(TMath::Abs(z*10000) >= SHORTMAX) {
    if(z>0) {
      mDCAzGlobal = SHORTMAX;
    }
    else {
      mDCAzGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAzGlobal = (Short_t)(z * 10000);
  }
}

//_________________
void StFemtoTrack::SetDCAGlobal(Float_t x, Float_t y, Float_t z) {

  if(TMath::Abs(x*10000) >= SHORTMAX) {
    if(x>0) {
      mDCAxGlobal = SHORTMAX;
    }
    else {
      mDCAxGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAxGlobal = (Short_t)(x * 10000);
  }

  if(TMath::Abs(y*10000) >= SHORTMAX) {
    if(y>0) {
      mDCAyGlobal = SHORTMAX;
    }
    else {
      mDCAyGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAyGlobal = (Short_t)(y * 10000);
  }

  if(TMath::Abs(z*10000) >= SHORTMAX) {
    if(z>0) {
      mDCAzGlobal = SHORTMAX;
    }
    else {
      mDCAzGlobal = -SHORTMAX;
    }
  }
  else {
    mDCAzGlobal = (Short_t)(z * 10000);
  }
}

//_________________
void StFemtoTrack::SetDedx(Float_t dEdx) {

  if(dEdx >= USHORTMAX) {
    mDedx = USHORTMAX;
  }
  else {
    mDedx = (UShort_t)(dEdx * 1e9);
  }
}

//_________________
void StFemtoTrack::SetDedx(Double_t dEdx) {

  if(dEdx >= USHORTMAX) {
    mDedx = USHORTMAX;
  }
  else {
    mDedx = (UShort_t)(dEdx * 1e9);
  }
}

//_________________
Float_t StFemtoTrack::GetDCAxyGlobal() const {

  Float_t x = (Float_t)mDCAxGlobal * 0.0001;
  Float_t y = (Float_t)mDCAyGlobal * 0.0001;
  return TMath::Sqrt(x*x + y*y);
}

//_________________
Float_t StFemtoTrack::GetDCAGlobal() const {

  Float_t x = (Float_t)mDCAxGlobal * 0.0001;
  Float_t y = (Float_t)mDCAyGlobal * 0.0001;
  Float_t z = (Float_t)mDCAzGlobal * 0.0001;
  return TMath::Sqrt(x*x + y*y + z*z);
}
