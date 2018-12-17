//
//  The version of the class should be changed every time
//  when any changes in the codes are done
//  Grigory Nigmatkulov: 2016/12/15
//

#ifndef STOSCAR97TRACK_H
#define STOSCAR97TRACK_H

#include <TObject.h>
#include <limits>
#include <iostream>
#include <TMath.h>
#include "TParticlePDG.h"
#include "TDatabasePDG.h"


//_________________
class StO97Track : public TObject {

 public:

  //Constructor
  StO97Track();
  //Copy constructor
  StO97Track(const StO97Track &copy);
  //Assignmaent operator
  StO97Track &operator=(const StO97Track &orig);
  //Destructor
  virtual ~StO97Track();

  //Getters
  Int_t   GetId() const     { return (Int_t)mId; }
  Int_t   GetPdgId() const  { return mPdgId; }
  Int_t   GetPdgId(const TDatabasePDG *pdgDb);  
  Int_t   GetCharge() const { return (mPdgId>0) ? 1 : -1; }
  Int_t   GetCharge(const TDatabasePDG *pdgDb, bool isajet = false);
  Float_t GetPx() const     { return mPx; }
  Float_t GetPy() const     { return mPy; }
  Float_t GetPz() const     { return mPz; }
  Float_t GetE();
  Float_t GetEnergy();
  Float_t GetPt();
  Float_t GetPtot();
  Float_t GetEta();
  Float_t GetPseudoRapidity();
  Float_t GetRapidity();
  Float_t GetY();
  Float_t GetMass() const    { return mMass; }
  Float_t GetMassSqr() const { return mMass*mMass; }
  Float_t GetXfr() const     { return mXfr; }
  Float_t GetYfr() const     { return mYfr; }
  Float_t GetZfr() const     { return mZfr; }
  Float_t GetTfr() const     { return mTfr; }
  Bool_t  GetIsSpec();

  //Setters
  void SetId(Int_t id);
  void SetPdgId(Int_t pdgId) { mPdgId = pdgId; }
  void SetPx(Float_t px)     { mPx = px; }
  void SetPy(Float_t py)     { mPy = py; }
  void SetPz(Float_t pz)     { mPz = pz;}
  void SetMass(Float_t mass) { mMass = mass; }
  void SetXfr(Float_t xFr)   { mXfr = xFr; }
  void SetYfr(Float_t yFr)   { mYfr = yFr; }
  void SetZfr(Float_t zFr)   { mZfr = zFr; }
  void SetTfr(Float_t tFr)   { mTfr = tFr; }

  void SetTrack(Int_t id, Int_t pdgId, 
		Float_t px, Float_t py, Float_t pz, Float_t mass,
		Float_t xFr, Float_t yFr, Float_t zFr, Float_t tFr);

  //Print track parameters
  virtual void Print(Option_t *option="") const {
      std::cout << "Track params: "
          << "id: " << mId << " pdgId: " << mPdgId
          << " px: " << mPx << " py: " << mPy << " pz: " << mPz
          << " mass: " << mMass << " xFr: " << mXfr << " yFr: " << mYfr
          << " zFr: " << mZfr << "tFr: " << mTfr << std::endl;
  }

 private:

  UShort_t mId;     // Number of the particle
  Int_t    mPdgId;  // PDG code
  Float_t  mPx;     // Px (GeV/c)
  Float_t  mPy;     // Py (GeV/c)
  Float_t  mPz;     // Pz (GeV/c)
  Float_t  mMass;   // Particle mass (GeV/c^2)
  Float_t  mXfr;    // x at freeze-out (fm)
  Float_t  mYfr;    // y at freeze-out (fm)
  Float_t  mZfr;    // z at freeze-out (fm)
  Float_t  mTfr;    // Freeze-out time (fm/c)

  ClassDef(StO97Track, 2)
};


#endif
