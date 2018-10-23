#include "StHbtFemtoDstReader.h"

ClassImp(StHbtFemtoDstReader)

//_________________
StHbtFemtoDstReader::StHbtFemtoDstReader(const char *aDir, 
					 const char *aFileName,
					 const char *aFilter,
					 int aMaxFiles,
					 bool aDebug) {
    mDir      = string(aDir);
    mFileName = string(aFileName);
    mFilter   = aFilter;
    mMaxFiles = aMaxFiles;
    mDebug    = aDebug;

    mTotalTracks = 0.;

    mTree       = 0;
    mTChain     = 0;
    mFemtoEvent = 0;
    mEventIndex = 0;
    mTrig       = 0;

    mSphFlag = false;
    mSphLo   = 0.0;
    mSphHi   = 1.0;

    InitRead(mDir, mFileName, mFilter, mMaxFiles);
    mNEvents = (unsigned int)mTChain->GetEntries();
}

//_________________
int StHbtFemtoDstReader::FillChain(TChain *aChain, char *aDir,
                                   const char *aFilter, int aMaxFiles) {

    TSystem *gSystem = new TSystem();
    if (!gSystem) {
        std::cout << "StHbtFemtoDstReader[ERROR]: cannot allocate memory for TSystem\n";
        return 0;
    }
    void *lDir = gSystem->OpenDirectory(aDir);
    if (!lDir) {
        std::cout << "StHbtFemtoDstReader[ERROR]: can't open directory " 
            << aDir << std::endl;
        delete gSystem;
        return 0;
    }

    int mCount = 0;
    char *lFileName;

    while ((lFileName = (char *)gSystem->GetDirEntry(aDir))) {
        if (strcmp(lFileName, ".") == 0 || strcmp(lFileName, "..") == 0)
            continue;
        if (strstr(lFileName, aFilter)) {
            char *lFullName = gSystem->ConcatFileName(aDir, lFileName);
            std::cout << "StHbtFemtoDstReader[INFO]: Adding "
                << lFullName << " to the chain" << std::endl;
            aChain->Add(lFullName);
            delete lFullName;
            mCount++;
            if ((aMaxFiles > 0) && (mCount > aMaxFiles)) break;
        }
    }

    std::cout << "StHbtFemtoDstReader[INFO]: Added " << mCount 
        << " files to the chain" << std::endl;
    delete gSystem;
    return mCount;
}

//_________________
int StHbtFemtoDstReader::FillChain(TChain *aChain, const char *aFileName,
                                   int aMaxFiles) {

    std::ifstream lInStream(aFileName);

    if (!lInStream.is_open()) {
        std::cout << "StHbtFemtoDstReader[ERROR]: can't open file list: "
            << aFileName << std::endl;
        return 0;
    }
    std::cout << "StHbtFemtoDstReader[INFO]: using file list: "
        << aFileName << std::endl;

    int mCount = 0;
    char buf[0xFF];
    for ( ; lInStream.good(); ) {
        lInStream.getline(buf, 0xFF);
        TString lFileName(buf);
        if (lFileName.Contains("root")) {
            aChain->Add(buf);
            mCount++;
            if ((aMaxFiles > 0) && (mCount > aMaxFiles)) break;
        }
    }

    std::cout << "StHbtFemtoDstReader[INFO]: Added " << mCount
        << " files to the chain" << std::endl;
    return mCount;
}

//_________________
int StHbtFemtoDstReader::InitRead(string aDir, string aFileName,
                                  string aFilter, int aMaxFiles) {

    mEventIndex = 0;
    //  mFemtoEvent = new StFemtoEvent();
    mTChain = new TChain("StFemtoDst", "StFemtoDst");

    std::cout << "StHbtFemtoDstReader[INFO]: Call InitRead" << std::endl;

    int lNumFiles = 0;
    if (!aFileName.empty()) {
        if (strstr(aFileName.c_str(), ".lis") ||
            strstr(aFileName.c_str(), ".list")) {
            lNumFiles = FillChain( mTChain, 
                                  (aDir + aFileName).c_str(), 
                                  aMaxFiles );
        }
        else {
            mTChain->Add((aDir + aFileName).c_str());
            lNumFiles++;
        }
    } else {
        lNumFiles = FillChain( mTChain, (char *)aDir.c_str(), // not cool
                              aFilter.c_str(), aMaxFiles );
    }
    mTChain->SetBranchAddress("StFemtoEvent", &mFemtoEvent);
    mNEvents = (unsigned int)mTChain->GetEntries();
    return lNumFiles;
}

//_________________
void StHbtFemtoDstReader::UninitRead() {
    //  if (mFemtoEvent) delete mFemtoEvent;
    if (mTChain) delete mTChain;
    mFemtoEvent = 0;
    mTChain   = 0;
}

//_________________
void StHbtFemtoDstReader::PrintTotalTracks() {

    std::cout << "StHbtFemtoDstReader[INFO]: mTotalTracks = "
        << mTotalTracks << std::endl;
}

//_________________
int StHbtFemtoDstReader::GetNEvents() {

    if (mTChain) {
        return mNEvents;
    }
    else {
        return -1;
    }
}

//_________________
StHbtEvent *StHbtFemtoDstReader::Read() {


    if(mDebug) {
        std::cout << "StHbtFemtoDstReader[DEBUG]: mEventIndex = "
            << mEventIndex << " mNEvents = "
            << mNEvents << std::endl;
    }

    if(!mNEvents) {
        std::cout << "StHbtFemtoDstReader[INFO]: no events to read\n";
        return 0;
    }

    mFemtoEvent->Clear();
    int lBytes = mTChain->GetEntry(mEventIndex++);

    if(mNEvents < mEventIndex) {
        std::cout << "StHbtFemtoDstReader[INFO]: EOF\n";
        return 0;
    }

    if(!lBytes) {
        std::cout << "StHbtFemtoDstReader[INFO]: no event\n";
        return 0;
    }

    StHbtEvent *mHbtEvent = NULL;

    if(mFemtoEvent) {

        if (mSphFlag)
        {
            float sph = mFemtoEvent->GetSphericity();
            if (sph <= mSphLo || sph > mSphHi)   return 0;
        }

        mHbtEvent = new StHbtEvent();

        //Check trigger Id
        if (!mTrigIdCol.empty()) {
            bool lTrigPass = false;
            unsigned int lTrigId = mFemtoEvent->GetTriggerId();
            if (((lTrigId & mTrig) == 0) || ((lTrigId & (!mTrig)) != 0))
                return mHbtEvent;
        }

        float mMagField = mFemtoEvent->GetMagneticField();
        TVector3 v(mFemtoEvent->GetVertexPositionX(),
                   mFemtoEvent->GetVertexPositionY(),
                   mFemtoEvent->GetVertexPositionZ());

        if(mDebug) {
            std::cout << Form("Vertex: x=%4.2f y=%4.2f z=%4.2f ",v.x(),v.y(),v.z()) << std::endl;
        }

        mHbtEvent->setEventNumber( mFemtoEvent->GetEventId() );
        mHbtEvent->setRunNumber( mFemtoEvent->GetRunId() );
        mHbtEvent->setZdcAdcEast( mFemtoEvent->GetZDCe() );
        mHbtEvent->setZdcAdcWest( mFemtoEvent->GetZDCw() );
        mHbtEvent->setRefMultPos( mFemtoEvent->GetRefMultPos() );
        mHbtEvent->setRefMult( mFemtoEvent->GetRefMult() );
        mHbtEvent->setPrimaryVertex( v );
        mHbtEvent->setMagneticField( mMagField );
        mHbtEvent->setVpdVz( mFemtoEvent->GetVpdVz() );
        mHbtEvent->setCent16( mFemtoEvent->GetCent16() );
        mHbtEvent->setRefMultCorr( mFemtoEvent->GetRefMultCorr() );
        mHbtEvent->setSphericity( mFemtoEvent->GetSphericity() );
        mHbtEvent->setBTofTrayMult( mFemtoEvent->GetNumberOfBTofHit() );
        mHbtEvent->setEventPlaneAngle( mFemtoEvent->GetRPgeneral() );

        TClonesArray *mFemtoTrackArr = mFemtoEvent->GetFemtoTracks();

        if (mFemtoTrackArr) {
            int mNFemtoTracks = mFemtoTrackArr->GetEntries();
            mTotalTracks += (float)mNFemtoTracks;
            if (mDebug) {
                std::cout << "StHbtFemtoDstReader[DEBUG]: mNFemtoTracks = "
                    << mNFemtoTracks << std::endl;
            }

            for (int i = 0; i < mNFemtoTracks; i++) {

                StFemtoTrack *mFemtoTrack = (StFemtoTrack *)mFemtoTrackArr->At(i);
                StHbtTrack *mHbtTrack = new StHbtTrack;
                Short_t charge = mFemtoTrack->GetCharge();

                mHbtTrack->setId( mFemtoTrack->GetId() );
                //mHbtTrack->setTrackType(mFemtoTrack->GetType());
                mHbtTrack->setFlag( mFemtoTrack->GetFlag() );

                mHbtTrack->setNHits( charge * mFemtoTrack->GetNHits() );
                //mHbtTrack->setNHitsFit( mFemtoTrack->GetNHitsFit() );
                mHbtTrack->setNHitsPossible( mFemtoTrack->GetNHitsPoss() );
                mHbtTrack->setNHitsDedx( mFemtoTrack->GetNHitsDedx() );

                mHbtTrack->setNSigmaElectron(mFemtoTrack->GetNSigmaElectron());
                mHbtTrack->setNSigmaPion(mFemtoTrack->GetNSigmaPion());
                mHbtTrack->setNSigmaKaon(mFemtoTrack->GetNSigmaKaon());
                mHbtTrack->setNSigmaProton(mFemtoTrack->GetNSigmaProton());

                mHbtTrack->setPidProbElectron(mFemtoTrack->GetPidProbElectron());
                mHbtTrack->setPidProbPion(mFemtoTrack->GetPidProbPion());
                mHbtTrack->setPidProbKaon(mFemtoTrack->GetPidProbKaon());
                mHbtTrack->setPidProbProton(mFemtoTrack->GetPidProbProton());

                mHbtTrack->setDedx( mFemtoTrack->GetDedx() );
                mHbtTrack->setChi2( mFemtoTrack->GetChi2() );

                mHbtTrack->setDca(mFemtoTrack->GetDCAxGlobal(),
                                  mFemtoTrack->GetDCAyGlobal(),
                                  mFemtoTrack->GetDCAzGlobal());

                //Primary track information
                TVector3 mPrimMomentum( mFemtoTrack->GetPx(),
                                             mFemtoTrack->GetPy(),
                                             mFemtoTrack->GetPz() );
                TVector3 mPrimOrigin( mFemtoEvent->GetVertexPositionX(),
                                           mFemtoEvent->GetVertexPositionY(),
                                           mFemtoEvent->GetVertexPositionZ() );

                //Global track information
                TVector3 mGlobMomentum(mFemtoTrack->GetGlobalPx(),
                                       mFemtoTrack->GetGlobalPy(),
                                       mFemtoTrack->GetGlobalPz());
                TVector3 mGlobOrigin( (mFemtoTrack->GetDCAxGlobal() + 
                                       mFemtoEvent->GetVertexPositionX()),
                                     (mFemtoTrack->GetDCAyGlobal() + 
                                      mFemtoEvent->GetVertexPositionY()),
                                     (mFemtoTrack->GetDCAzGlobal() + 
                                      mFemtoEvent->GetVertexPositionZ()) );

                //Set mP. Primary tracks have non-zero mP, globals have mP(0.,0.,0.)
                if( mFemtoTrack->GetType() == 0) { // Global tracks
                  mHbtTrack->setP( TVector3(0., 0., 0.) );
                }
                else { //Primary tracks
                  mHbtTrack->setP( mPrimMomentum );
                }
                //set momentum of the global track
                mHbtTrack->setGlobalP( mGlobMomentum );

                if(mDebug) {
                  std::cout << Form("Track parameters: px=%3.2f py=%3.2f pz=%3.2f q=%1d nhits=%2d",
                                    mPrimMomentum.x(),mPrimMomentum.y(),mPrimMomentum.z(),
                                    charge, mFemtoTrack->GetNHits() ) << std::endl;
                }

                mHbtTrack->setTopologyMap( 0, mFemtoTrack->GetMap0() );
                mHbtTrack->setTopologyMap( 1, mFemtoTrack->GetMap1() );

                mHbtTrack->setBeta( mFemtoTrack->GetBeta() );
                mHbtTrack->setHiddenInfo( 0 );

                mHbtEvent->trackCollection()->push_back( mHbtTrack );
            } //for (int i = 0; i < mNFemtoTracks; i++)
        } //if (mFemtoTrackArr)
    } //if(mFemtoEvent)
    else {
      // FemtoDstMaker returned NULL, so we are not OK
      mReaderStatus = 1;
    }

    return mHbtEvent;
}

//_________________
StHbtEvent *StHbtFemtoDstReader::returnHbtEvent() {

  StHbtEvent *mHbtEvent = Read();
  if (!mHbtEvent) {
    std::cout << "StHbtFemtoDstReader[WARN]: no hbt event (mHbtEvent == NULL)\n";
    return 0;
  }
  else {
    return mHbtEvent;
  }
}

//_________________
StHbtString StHbtFemtoDstReader::report() {

  StHbtString temp = "\nThis is the StHbtFemtoDstReader\n";
  return temp;
}


void StHbtFemtoDstReader::SetTrigger(unsigned int trig, char mode) {
  unsigned int trigColSize = mTrigIdCol.size();
  for (unsigned int iTrig = 0; iTrig < trigColSize; iTrig++) {
    if (mTrigIdCol[iTrig] == trig) {
      unsigned int mask = 1 << iTrig;
      if (!mode)
        mTrig = mTrig & (!mask);
      else
        mTrig = mTrig | mask;
      break;
    }
  }
  //
  // Output trigger mask
  //
  char bitMask[33] = {0};
  for (unsigned int iBit = 0; iBit < 32; iBit++) {
    bitMask[31 - iBit] = '0' + ((mTrig & (1 << iBit)) >> iBit);
  }

  std::cout << "StHbtFemtoDstReader[INFO]: Trigger mask = "
      << mTrig << " bit = "
      << bitMask << std::endl;
}

//_________________
void StHbtFemtoDstReader::AddTrigId(unsigned int aTrigId) {

  mTrigIdCol.push_back(aTrigId);
}

//_________________
void StHbtFemtoDstReader::DoDebug(bool aDebug) {
  mDebug = aDebug;
}
