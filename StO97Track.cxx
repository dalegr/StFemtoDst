#include "StO97Track.h"

ClassImp(StO97Track)

//________________
StO97Track::StO97Track() : mId(0), mPdgId(0), mPx(0), mPy(0), mPz(0),
  mMass(0), mXfr(0), mYfr(0), mZfr(0), mTfr(0) {
  /* emtpy */
}

//_________________
StO97Track::StO97Track(const StO97Track &t) {

  //Copy constructor
  mId = t.mId;
  mPdgId = t.mPdgId;
  mPx = t.mPx; mPy = t.mPy; mPz = t.mPz; mMass = t.mMass;
  mXfr = t.mXfr; mYfr = t.mYfr; mZfr = t.mZfr; mTfr = t.mTfr;
}

//________________
Int_t StO97Track::GetPdgId(const TDatabasePDG *pdgDb)
{
  return pdgDb->ConvertIsajetToPdg(mPdgId);
}

//________________
StO97Track &StO97Track::operator=(const StO97Track &t) {

  //Assignment constructor
  if (this != &t) {
    mId = t.mId;
    mPdgId = t.mPdgId;
    mPx = t.mPx;
    mPy = t.mPy;
    mPz = t.mPz;
    mMass = t.mMass;
    mXfr = t.mXfr;
    mYfr = t.mYfr;
    mZfr = t.mZfr;
    mTfr = t.mTfr;
  }
  return *this;
}

//_________________
Int_t StO97Track::GetCharge(const TDatabasePDG *pdgDb, bool isajet) {
  int pdg = isajet ? pdgDb->ConvertIsajetToPdg(mPdgId) : mPdgId;
  TParticlePDG *pdgInfo = pdgDb->GetParticle(pdg);
  if (pdgInfo != NULL) {
    return (Int_t)(pdgInfo->Charge()/3.);
  }
  else {
    std::cout << "[StO97Track WARN]: can't get TPartcilePDG, using mPdgId > 0 ? 1 : -1\n";
    return GetCharge();
  }
}

//_________________
StO97Track::~StO97Track() {
  /* empty */
}

//_________________
void StO97Track::SetId(Int_t id) {
  
  //Set id of the track
  if( ( id < std::numeric_limits<unsigned short>::min() ) ||
      ( id > std::numeric_limits<unsigned short>::max() ) ) {
    mId = std::numeric_limits<unsigned short>::max();
  }
  else {
    mId = (UShort_t)id;
  }
}

//________________
void StO97Track::SetTrack(Int_t id, Int_t pdgId, 
                          Float_t px, Float_t py, Float_t pz, Float_t mass,
                          Float_t xFr, Float_t yFr, Float_t zFr, Float_t tFr) {
  
  //Set all track information
  SetId(id);
  SetPdgId(pdgId);
  mPx = px; mPy = py; mPz = pz; mMass = mass;
  mXfr = xFr; mYfr = yFr; mZfr = zFr; mTfr = tFr;
}

//_________________
Bool_t StO97Track::GetIsSpec() {

  //Check if the particle is a spectator
  Bool_t mIsSpectator = false;
  if( TMath::Abs(mXfr) < 0.0001 &&
      TMath::Abs(mYfr) < 0.0001 &&
      TMath::Abs(mZfr) < 0.0001 &&
      mTfr < 0.0001 ) {
    mIsSpectator = true;
  }
  return mIsSpectator;
}

//_________________
Float_t StO97Track::GetPt() {
  return TMath::Sqrt( mPx*mPx + mPy*mPy );
}

//_________________
Float_t StO97Track::GetPtot() {
  return TMath::Sqrt( mPx*mPx + mPy*mPy + mPz*mPz );
}

//_________________
Float_t StO97Track::GetEnergy() {
  //Calculate energy
  return TMath::Sqrt( mPx*mPx + mPy*mPy + mPz*mPz + mMass*mMass);
}

//_________________
Float_t StO97Track::GetE() {
  return GetEnergy();
}

//_________________
Float_t StO97Track::GetPseudoRapidity() {
  return ( 0.5 * TMath::Log( (GetPtot() + mPz) / (GetPtot() - mPz) ) );
}

//_________________
Float_t StO97Track::GetEta() {
  return GetPseudoRapidity();
}

//_________________
Float_t StO97Track::GetRapidity() {
  return ( 0.5 * TMath::Log( (GetEnergy() + mPz) / (GetEnergy() - mPz) ) );
}

//_________________
Float_t StO97Track::GetY() {
  return GetRapidity();
}
