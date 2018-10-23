#include "StFemtoEvent.h"
#include "TMath.h"

ClassImp(StFemtoEvent)

//_________________
StFemtoEvent::StFemtoEvent() {

  mFemtoTracks = new TClonesArray("StFemtoTrack", 1500);
  mNFemtoTracks = 0;
}

//_________________
StFemtoEvent::~StFemtoEvent() {
  /* nothing */
}

//_________________
void StFemtoEvent::Clear(Option_t *option) {

  mFemtoTracks->Clear(option);
  mNFemtoTracks = 0;
}

//_________________
StFemtoTrack* StFemtoEvent::AddFemtoTrack() {

  TClonesArray &tracks = *mFemtoTracks;
  StFemtoTrack *track = new(tracks[mNFemtoTracks++]) StFemtoTrack;
  return track;
}

//_________________
void StFemtoEvent::SetPrimaryVertexPosition(Float_t x, Float_t y, Float_t z) { 

  mVertexPositionX = x;
  mVertexPositionY = y;
  mVertexPositionZ = z;
}

//_________________
TVector3 StFemtoEvent::GetPrimaryVertexPosition() const { 

  TVector3 p(mVertexPositionX, mVertexPositionY, mVertexPositionZ);
  return p; 
}

//_________________
Float_t StFemtoEvent::GetVertexPositionR() const {

  Float_t r = TMath::Sqrt(mVertexPositionX*mVertexPositionX + 
			  mVertexPositionY*mVertexPositionY);
  return r;
}

//_________________
void StFemtoEvent::SetPrimaryVertexRanking(Float_t rank) {

  if(rank <= 0.) { 
    mPrimaryVertexRanking = -9;
  }
  else if(rank <= 1.) {
    mPrimaryVertexRanking = 1;
  }
  else if(rank <= 100) {
    mPrimaryVertexRanking = 2;
  }
  else if(rank <= 1e9) {
    mPrimaryVertexRanking = 3;
  }
  else {
    mPrimaryVertexRanking = 4;
  }
}

//_________________
void StFemtoEvent::SetRefMult(Int_t mult) {

  if(mult<0 || mult>60000) {
    mRefMult = USHORTMAX;
  }
  else {
    mRefMult = (UShort_t)mult;
  }
}

//_________________
void StFemtoEvent::SetRefMult2(Int_t mult) {

  if(mult<0 || mult>60000) {
    mRefMult2 = USHORTMAX;
  }
  else {
    mRefMult2 = (UShort_t)mult;
  }
}

//_________________
void StFemtoEvent::SetRefMultCorr(Float_t rf) {

  if(rf<0. || rf >6000) {
    mRefMultCorr = USHORTMAX;
  }
  else {
    mRefMultCorr = (UShort_t)(rf * 10);
  }
}

//_________________
void StFemtoEvent::SetRefMultCorrWeight(Float_t w) {

  if(w<0. || w>6.) {
    mRefMultCorrWeight = USHORTMAX;
  }
  else {
    mRefMultCorrWeight = (UShort_t)(w * 10000);
  }
}

//_________________
void StFemtoEvent::SetMagneticField(Float_t f) {

  if(TMath::Abs(f*1000) > SHORTMAX) {
    if(f>0) {
      mMagField = SHORTMAX;
    }
    else {
      mMagField = -SHORTMAX;
    }
  }
  else {
    mMagField = (Short_t)(f*1000);
  }
}

//_________________
UShort_t StFemtoEvent::GetCent9() const {

  if(mCent16 == 15) {  
    return 8;  //0-5%      
  }
  else if(mCent16 == 14) {
    return 7;  //5-10%
  }
  else if( (mCent16 == 13) || (mCent16 == 12) ) {
    return 6;  //10-20%
  }
  else if( (mCent16 == 11) || (mCent16 == 10) ) {
    return 5;  //20-30%
  }
  else if( (mCent16 == 9) || (mCent16 == 8) ) {
    return 4;  //30-40%
  }
  else if( (mCent16 == 7) || (mCent16 == 6) ) {
    return 3;  //40-50%
  }
  else if( (mCent16 == 5) || (mCent16 == 4) ) {
    return 2;  //50-60%
  }
  else if( (mCent16 == 3) || (mCent16 == 2) ) {
    return 1;  //60-70%
  }
  else if( (mCent16 == 1) || (mCent16 == 0) ) {
    return 0;  //70-80%
  }
  else {
    return 9999;
  }
}

/*
//_________________
void StFemtoEvent::SetZDCe(Float_t z) {
  
  if(z >= USHORTMAX) {
    mZDCe = USHORTMAX;
  }
  else {
    mZDCe = (UShort_t)z;
  }
}

//_________________
void StFemtoEvent::SetZDCw(Float_t z) {
  
  if(z >= USHORTMAX) {
    mZDCw = USHORTMAX;
  }
  else {
    mZDCw = (UShort_t)z;
  }
}
*/
