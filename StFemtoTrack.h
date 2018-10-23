//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//
#ifndef STFEMTOTRACK_HH
#define STFEMTOTRACK_HH

#include "Rtypes.h"
#include "TObject.h"
#include "TVector3.h"
#include "TMath.h"

const Float_t __SCALE__ = 1000.;
const Float_t __INV_SCALE__  = 0.001;
const UShort_t USHORTMAX = 65535;
const Short_t  SHORTMAX = 32767;

//_________________
class StFemtoTrack : public TObject {

 public:
  StFemtoTrack();
  StFemtoTrack(Float_t px, Float_t py, Float_t pz);
  ~StFemtoTrack();

  //Setters
  void SetId(Short_t id)               { mId = (UShort_t)id; }
  void SetNHits(Char_t nHits)          { mNHits = nHits; }   //Should give Char_t
  void SetNHitsPoss(UShort_t nHits)    { mNHitsPoss = (UChar_t)nHits; }
  //void SetNHitsDedx(UShort_t nHits)    { mNHitsDedx = (UChar_t)nHits; }
  void SetNSigmaElectron(Float_t ns);
  void SetNSigmaPion(Float_t ns);
  void SetNSigmaKaon(Float_t ns);
  void SetNSigmaProton(Float_t ns);
  void SetDedx(Float_t dEdx);
  void SetDedx(Double_t dEdx);
  void SetMap0(Int_t map0)              { mMap0 = (UInt_t)map0; }
  void SetMap0(UInt_t map0)             { mMap0 = map0; }
  void SetMap1(Int_t map1)              { mMap1 = (UInt_t)map1; }
  void SetMap1(UInt_t map1)             { mMap1 = map1; }
  void SetPx(Float_t px)                { mMomX = px; }
  void SetPy(Float_t py)                { mMomY = py; }
  void SetPz(Float_t pz)                { mMomZ = pz; }
  void SetP(Float_t px, Float_t py, Float_t pz);
  void SetDCAxGlobal(Float_t x);
  void SetDCAyGlobal(Float_t y);
  void SetDCAzGlobal(Float_t z);
  void SetDCAGlobal(Float_t x, Float_t y, Float_t z);
  void SetGlobalPx(Float_t px)          { mGlobMomX = px; } 
  void SetGlobalPy(Float_t py)          { mGlobMomY = py; } 
  void SetGlobalPz(Float_t pz)          { mGlobMomZ = pz; } 
  void SetGlobalP(Float_t px, Float_t py, Float_t pz);
  void SetBeta(Float_t beta)            { mBeta = beta; }

  //Getters
  Short_t   GetId() const                    { return (Short_t)mId; }
  Short_t   GetType() const;
  Short_t   GetFlag() const                  { return 300; }
  UShort_t  GetNHits() const                 { return (UShort_t)TMath::Abs(mNHits); }
  UShort_t  GetNHitsFit() const;
  UShort_t  GetNHitsPoss() const             { return (UShort_t)mNHitsPoss; }
  //UShort_t  GetNHitsDedx() const             { return (UShort_t)mNHitsDedx; }
  UShort_t  GetNHitsDedx() const             { return (UShort_t)TMath::Abs(mNHits); } //Temporary
  Float_t   GetNSigmaElectron() const        { return (Float_t)mNSigmaElectron*__INV_SCALE__; }
  Float_t   GetNSigmaPion() const            { return (Float_t)mNSigmaPion*__INV_SCALE__; }
  Float_t   GetNSigmaKaon() const            { return (Float_t)mNSigmaKaon*__INV_SCALE__; }
  Float_t   GetNSigmaProton() const          { return (Float_t)mNSigmaProton*__INV_SCALE__; }
  Float_t   GetPidProbElectron() const       { return 0.5; }
  Float_t   GetPidProbPion() const           { return 0.5; }
  Float_t   GetPidProbKaon() const           { return 0.5; }
  Float_t   GetPidProbProton() const         { return 0.5; }
  Double_t  GetDedx() const                  { return (Double_t)mDedx * 1e-9; }
  UInt_t    GetMap0() const                  { return (UInt_t)mMap0; }
  UInt_t    GetMap1() const                  { return (UInt_t)mMap1; }
  Float_t   GetChi2() const                  { return 1.; }
  Short_t   GetCharge() const                { return (mNHits>0) ? +1 : -1; }
  Float_t   GetPx() const                    { return mMomX; }
  Float_t   GetPy() const                    { return mMomY; }
  Float_t   GetPz() const                    { return mMomZ; }
  TVector3  GetP() const;
  Float_t   GetPt() const;
  Float_t   GetPtot() const;
  Float_t   GetEta() const;
  Float_t   GetDCAxy() const                 { return 0.; }
  Float_t   GetDCAx() const                  { return 0.; }
  Float_t   GetDCAy() const                  { return 0.; }
  Float_t   GetDCAz() const                  { return 0.; }
  Float_t   GetDCAxyGlobal() const;
  Float_t   GetDCAxGlobal() const            { return (Float_t)mDCAxGlobal * 0.0001; }
  Float_t   GetDCAyGlobal() const            { return (Float_t)mDCAyGlobal * 0.0001; }
  Float_t   GetDCAzGlobal() const            { return (Float_t)mDCAzGlobal * 0.0001; }
  Float_t   GetDCAGlobal() const;
  Float_t   GetGlobalPx() const              { return mGlobMomX; }
  Float_t   GetGlobalPy() const              { return mGlobMomY; }
  Float_t   GetGlobalPz() const              { return mGlobMomZ; }
  TVector3  GetGlobalP() const;
  Float_t   GetPtGlobal() const;
  Float_t   GetPtotGlobal() const;
  Float_t   GetBeta() const                  { return mBeta; }
  Float_t   GetMassSqr() const;
  Bool_t    GetIsTofTrack(); 

 private:

  UShort_t  mId;
  Char_t    mNHits;           //q * NHits ; nHitsFit=nHits+1 for primaries 
  UChar_t   mNHitsPoss;
  //UChar_t   mNHitsDedx;
  Short_t   mNSigmaElectron;  // ns*1000     !
  Short_t   mNSigmaPion;      // ns*1000     !
  Short_t   mNSigmaKaon;      // ns*1000     !
  Short_t   mNSigmaProton;    // ns*1000     !
  UShort_t  mDedx;            // dEdx*1e9    !
  UInt_t    mMap0;            // TopologyMap data0
  UInt_t    mMap1;            // TopologyMap data1
  Float_t   mMomX;            // Primary Px at pVtx
  Float_t   mMomY;            // Primary Py at pVtx
  Float_t   mMomZ;            // Primary Pz at pVtx
  Short_t   mDCAxGlobal;      // gDCA * 10000 (IMPORTANT: for primaries)
  Short_t   mDCAyGlobal;      // gDCA * 10000 (IMPORTANT: for primaries)
  Short_t   mDCAzGlobal;      // gDCA * 10000 (IMPORTANT: for primaries)
  Float_t   mGlobMomX;        // Px Global at DCA to pVtx
  Float_t   mGlobMomY;        // Py Global at DCA to pVtx
  Float_t   mGlobMomZ;        // Pz Global at DCA to pVtx
  //To get the origin at gDCA (3D): mOrigin = PrimVtxPosition + gDCA

  Float_t   mBeta;            // ToF beta

  ClassDef(StFemtoTrack, 2)
};

#endif
