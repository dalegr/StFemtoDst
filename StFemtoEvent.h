//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//
#ifndef STFEMTOEVENT_HH
#define STFEMTOEVENT_HH

#include "Rtypes.h"
#include "TObject.h"
#include "StFemtoTrack.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "math.h"


//_________________
class StFemtoEvent : public TObject {

 public:

  StFemtoEvent();
  ~StFemtoEvent();

  //Setters
  void SetEventId(Int_t id)            { mEventId = id; }
  void SetRunId(Int_t id)              { mRunId = id; }
  void SetCollisionType(Bool_t type)   { mCollisionType = type;}
  void SetRefMult(Int_t mult);
  void SetRefMult2(Int_t mult);
  void SetRefMultCorr(Float_t rf);
  void SetRefMultCorrWeight(Float_t w);
  void SetCent16(Int_t c)              { mCent16 = (Char_t)c; }
  void SetRefMultPos(Int_t m)          { mRefMultPos = (UShort_t)m; }
  void SetRefMult2Pos(Int_t m)         { mRefMult2Pos = (UShort_t)m; }
  void SetNumberOfBTofHit(Int_t m)   { if(m<=0) {mNumberOfBTofHit=0;} else {mNumberOfBTofHit = (UShort_t)m;} }
  void SetNumberOfBTofHit(UInt_t m)  { if(m<=0) {mNumberOfBTofHit=0;} else {mNumberOfBTofHit = (UShort_t)m;} }
  void SetNumberOfPrimaryTracks(Int_t m) { mNumberOfPrimaryTracks = (UShort_t)m; }
  void SetNumberOfGlobalTracks(Int_t m)  { mNumberOfGlobalTracks = (UShort_t)m; }
  void SetMagneticField(Float_t f);
  void SetPrimaryVertexPosition(Float_t x, Float_t y, Float_t z);
  void SetPrimaryVertexX(Float_t x)   { mVertexPositionX = x; }
  void SetPrimaryVertexY(Float_t y)   { mVertexPositionY = y; }
  void SetPrimaryVertexZ(Float_t z)   { mVertexPositionZ = z; }
  void SetVpdVz(Float_t vz)           { mVpdVz = vz; } 
  void SetTriggerId(UInt_t id)        { mTriggerId = id; }
  void SetPrimaryVertexRanking(Float_t);
  void SetSphericity(Float_t sph)     { mSphericity = (char)round(sph*100.); }
  void SetRPeast(Float_t rp)              { mRPeast = rp; }
  void SetRPwest(Float_t rp)              { mRPwest = rp; }  
  void SetRPgeneral(Float_t rp)              { mRPgeneral = rp; }
  StFemtoTrack* AddFemtoTrack();
  void Clear(Option_t* option = "C");

  //Getters
  Int_t    GetEventId() const               { return mEventId; }
  Int_t    GetRunId() const                 { return mRunId; }
  Bool_t   GetCollisionType() const         { return mCollisionType; }
  UInt_t   GetRefMult() const               { return (UInt_t)mRefMult; }
  UInt_t   GetRefMult2() const              { return (UInt_t)mRefMult2;}
  Float_t  GetRefMultCorr() const           { return (Float_t)mRefMultCorr * 0.1; }
  Float_t  GetRefMultCorrWeight() const     { return (Float_t)mRefMultCorrWeight * 0.0001; }
  UShort_t GetCent9() const;
  UShort_t GetCent16() const                { return (UShort_t)mCent16; }
  UShort_t GetRefMultPos() const            { return mRefMultPos; }
  UShort_t GetRefMultNeg() const            { return (mRefMult - mRefMultPos); }
  UShort_t GetRefMult2Pos() const           { return mRefMult2Pos; }
  UShort_t GetRefMult2Neg() const           { return (mRefMult2 - mRefMult2Pos); }
  Float_t  GetZDCe() const                  { return (Float_t)30000; }
  Float_t  GetZDCw() const                  { return (Float_t)30000; }
  UShort_t GetNumberOfBTofHit() const       { return mNumberOfBTofHit; }
  UShort_t GetNumberOfPrimaryTracks() const { return mNumberOfPrimaryTracks; }
  UShort_t GetNumberOfGlobalTracks() const  { return mNumberOfGlobalTracks; }
  Float_t  GetMagneticField() const         { return (Float_t)mMagField*0.001; }
  TVector3 GetPrimaryVertexPosition() const;
  Float_t  GetVertexPositionX() const       { return mVertexPositionX; }
  Float_t  GetVertexPositionY() const       { return mVertexPositionY; }
  Float_t  GetVertexPositionZ() const       { return mVertexPositionZ; }
  Float_t  GetVertexPositionR() const;
  Float_t  GetVpdVz() const                 { return mVpdVz; }
  UInt_t   GetTriggerId() const             { return mTriggerId; }
  Float_t  GetPrimaryVertexRanking() const  { return (Float_t)mPrimaryVertexRanking; }
  TClonesArray *GetFemtoTracks() const      { return mFemtoTracks; }
  Int_t    GetNFemtoTracks() const          { return (Int_t)mNFemtoTracks; }
  Float_t  GetSphericity() const            { return ((float)mSphericity)/100.; }
  Float_t  GetRPeast() const                    { return mRPeast; }
  Float_t  GetRPwest() const                    { return mRPwest; }
  Float_t  GetRPgeneral() const                    { return mRPgeneral; }

 private:

  Int_t    mEventId;
  Int_t    mRunId;
  Char_t   mSphericity;          //transverse sphericity
  Float_t  mRPeast, mRPwest, mRPgeneral;     // reaction plane
  Bool_t   mCollisionType;       //0-pp, 1-AA(from those where RefMultCorr is defined)
  UShort_t mRefMult;             //mRefMult
  UShort_t mRefMultCorr;         //mRefMultCorr * 10
  UShort_t mRefMultCorrWeight;   //mRefMultCorrWight * 10000
  UShort_t mRefMult2;            //refMult for |eta| < 1.0
  Char_t   mCent16;
  UShort_t mRefMultPos;            //n positive for refMult
  UShort_t mRefMult2Pos;           //n positive for refMult2
  UShort_t mNumberOfPrimaryTracks;
  UShort_t mNumberOfGlobalTracks;
  UShort_t mNumberOfBTofHit;   
  Short_t  mMagField;               //Field*1000
  Float_t  mVertexPositionX;        //VtxX
  Float_t  mVertexPositionY;        //VtxY
  Float_t  mVertexPositionZ;        //VtxZ
  Float_t  mVpdVz;                  //VzVpd
  UInt_t   mTriggerId;
  Char_t   mPrimaryVertexRanking;  //-9: bad; 1: <1; 2: <100; 3: <10^9; 4: > 10^9
  
  UShort_t mNFemtoTracks;
  TClonesArray *mFemtoTracks;

  ClassDef(StFemtoEvent, 102)
};

#endif
