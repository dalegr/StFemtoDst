#include "StHbtO97DstReader.h"

ClassImp(StHbtO97DstReader)

const Float_t PYTHIA_RBFIELD = -4.5106000;

//_________________
StHbtO97DstReader::StHbtO97DstReader(const char *dir, const char *fileName,
                                     const char *filter, int maxFiles, bool debug) {
    mDir      = string(dir);
    mFileName = string(fileName);
    mFilter   = filter;
    mMaxFiles = maxFiles;
    mDebug    = debug;

    mTotalTracks = 0.;

    mIsaJet     = 0; // default is PDG codes
    mTree       = 0;
    mTChain     = 0;
    mO97Event   = 0;
    mEventIndex = 0;

    InitRead(mDir, mFileName, mFilter, mMaxFiles);
    mNEvents = (unsigned int)mTChain->GetEntries();
}

//_________________
int StHbtO97DstReader::InitRead(string dir, string fileName,
                                string filter, int maxFiles) {
    mEventIndex = 0;
    mTChain = new TChain("StO97Dst", "StO97Dst");

    std::cout << "[StHbtO97DstReader]: ---=== StHbtO97DstReader initialization ===---" << std::endl;

    //
    // Load PDG database
    //
    pdgDb = new TDatabasePDG;
    if (pdgDb) 
      pdgDb->ReadPDGTable("pdg_table.txt");
    else
      return 0;

    int numFiles = 0;
    if (!fileName.empty()) {

        if (strstr(fileName.c_str(), ".lis") ||
            strstr(fileName.c_str(), ".list")) {
            numFiles = FillChain(mTChain, 
                                 (dir + fileName).c_str(), 
                                 maxFiles);
        }
        else {
            mTChain->Add((dir + fileName).c_str());
            numFiles++;
        }
    } 
    else {
        numFiles = FillChain(mTChain, (char *)dir.c_str(), // not cool
                             filter.c_str(), maxFiles);
    }
    mTChain->SetBranchAddress("StO97Event", &mO97Event);
    mNEvents = (unsigned int)mTChain->GetEntries();
    return numFiles;
}

//_________________
int StHbtO97DstReader::FillChain(TChain *chain, const char *fileName, 
                                 int maxFiles) {

    std::ifstream inStream(fileName);
    if (!inStream.is_open()) {
        std::cout << "[StHbtO97DstReader]: can't open file list: "
            << fileName << std::endl;
        return 0;
    }
    std::cout << "[StHbtO97DstReader]: using file list: "
        << fileName << std::endl;

    int count = 0;
    char buf[0xFF];
    for ( ; inStream.good(); ) {
        inStream.getline(buf, 0xFF);
        TString fileName(buf);
        if (fileName.Contains("root")) {
            chain->Add(buf);
            count++;
            if ((maxFiles > 0) && (count > maxFiles)) break;
        }
    }

    std::cout << "[StHbtO97DstReader]: Added " << count
        << " files to the chain" << std::endl;
    return count;
}

//_________________
int StHbtO97DstReader::FillChain(TChain *chain, char *dir,
                                 const char *filter, int maxFiles) {

    TSystem *gSystem = new TSystem();
    if (!gSystem) {
        std::cout << "[StHbtO97DstReader]: cannot allocate memory for TSystem"
            << std::endl;
        return 0;
    }
    void *pdir = gSystem->OpenDirectory(dir);
    if (!pdir) {
        std::cout << "[StHbtO97DstReader]: can't open directory " 
            << dir << std::endl;
        delete gSystem;
        return 0;
    }

    int count = 0;
    char *fileName;

    while ((fileName = (char *)gSystem->GetDirEntry(dir))) {
        if (strcmp(fileName, ".") == 0 || strcmp(fileName, "..") == 0)
            continue;
        if (strstr(fileName, filter)) {
            char *fullName = gSystem->ConcatFileName(dir, fileName);
            std::cout << "[StHbtO97DstReader]: Adding "
                << fullName << " to the chain" << std::endl;
            chain->Add(fullName);
            delete fullName;
            count++;
            if ((maxFiles > 0) && (count > maxFiles)) break;
        }
    }

    std::cout << "[StHbtO97DstReader]: Added " << count 
        << " files to the chain" << std::endl;
    delete gSystem;
    return count;
}

//_________________
void StHbtO97DstReader::UninitRead() {
    if (mTChain)     delete mTChain;
    if (mO97Event)   delete mO97Event;

    mO97Event = 0;
    mTChain   = 0;
}

//_________________
float StHbtO97DstReader::dedxMean(float mass, float mom) {
    float dedxMean;
    float tpcDedxGain   = 0.174325e-06;
    float tpcDedxOffset = -2.71889; 
    float tpcDedxRise   = 776.626;

    float gamma = TMath::Sqrt(mom*mom/(mass*mass) + 1.);
    float beta  = TMath::Sqrt(1. - 1./(gamma*gamma));
    float rise  = tpcDedxRise*beta*beta*gamma*gamma;
    if (beta > 0)
        dedxMean = tpcDedxGain/(beta*beta)*(0.5*TMath::Log(rise) - beta*beta 
                                            - tpcDedxOffset);
    else
        dedxMean = 1000.;
    return dedxMean;
}

//_________________
StHbtEvent *StHbtO97DstReader::Read() {

    if (mDebug) {
        std::cout << "[StHbtO97DstReader]: cEventIndex = "
            << mEventIndex << " cNEvents = "
            << mNEvents << std::endl;
    }

    if (!mNEvents) {
        std::cout << "[StHbtO97DstReader]: no events to read\n";
        return 0;
    }

    mO97Event->Clear();
    int bytes = mTChain->GetEntry(mEventIndex++);

    if (mNEvents < mEventIndex) {
        std::cout << "[StHbtO97DstReader]: EOF\n";
        return 0;
    }

    if (!bytes) {
        std::cout << "[StHbtO97DstReader]: no event\n";
        return 0;
    }

    StHbtEvent *mHbtEvent = 0;

    if (mO97Event) {
        mHbtEvent = new StHbtEvent();

        int nTracks = mO97Event->GetNTracks();
        int refMultPos = 0;
        int refMultNeg = 0;
        TVector3 vPos( 0., 0., 0. );

        TClonesArray *mO97TrackArr = mO97Event->GetTracks();

        if(mO97TrackArr) {
            mTotalTracks += (float)nTracks;
            if (mDebug) {
                std::cout << "[StHbtO97DstReader]: nTracks = "
                    << nTracks << std::endl;
            }

            for (int i = 0; i < nTracks; i++) {

                // Initialize StO97Track and StHbtTrack
                StO97Track *mO97Track = (StO97Track *)mO97TrackArr->At(i);
                StHbtTrack *mHbtTrack = new StHbtTrack();

                // Set charge, mass and momentum
                char charge = mO97Track->GetCharge(pdgDb, mIsaJet);

                float trkMass = mO97Track->GetMass();
                TVector3 trkMom(mO97Track->GetPx(), 
                                mO97Track->GetPy(), 
                                mO97Track->GetPz());
                // Calculate DCA in cm
                TVector3 origin(mO97Track->GetXfr()*1e-13,
                                mO97Track->GetYfr()*1e-13,
                                mO97Track->GetZfr()*1e-13);
                float dcaXY = origin.Perp();
                float dcaX = origin.x();
                float dcaY = origin.y();
                float dcaZ = origin.z();

                // Calculate reference multiplicity
                if (charge != 0 &&
                    fabs(trkMom.PseudoRapidity()) < 0.5 &&
                    trkMom.Mag() >= 1e-10 &&
                    origin.Mag() < 3.)
                {
                  if (charge > 0)   refMultPos++;
                  else              refMultNeg++;
                }

                // Set nSigma and PID probability
                float trkNSigmaElectron,  trkNSigmaPion;
                float trkNSigmaKaon,      trkNSigmaProton;
                float trkPidProbElectron, trkPidProbPion;
                float trkPidProbKaon,     trkPidProbProton;
                int pdg = mIsaJet ? mO97Track->GetPdgId(pdgDb) : mO97Track->GetPdgId();
                switch (abs(pdg)) {
                  case 211: // pion
                    trkNSigmaElectron = -99.;
                    trkNSigmaPion     = 0.;
                    trkNSigmaKaon     = -99.;
                    trkNSigmaProton   = -99.;
                    trkPidProbElectron = -99.;
                    trkPidProbPion     = 0.;
                    trkPidProbKaon     = -99.;
                    trkPidProbProton   = -99.;
                    break;
                  case 321: // kaon
                    trkNSigmaElectron = -99.;
                    trkNSigmaPion     = -99.;
                    trkNSigmaKaon     = 0.;
                    trkNSigmaProton   = -99.;
                    trkPidProbElectron = -99.;
                    trkPidProbPion     = -99.;
                    trkPidProbKaon     = 0.;
                    trkPidProbProton   = -99.;
                    break;
                  case 2212: // proton
                    trkNSigmaElectron = -99.;
                    trkNSigmaPion     = -99.;
                    trkNSigmaKaon     = -99.;
                    trkNSigmaProton   = 0.;
                    trkPidProbElectron = -99.;
                    trkPidProbPion     = -99.;
                    trkPidProbKaon     = -99.;
                    trkPidProbProton   = 0.;
                    break;
                  default:
                    trkNSigmaElectron = -99.;
                    trkNSigmaPion     = -99.;
                    trkNSigmaKaon     = -99.;
                    trkNSigmaProton   = -99.;
                    trkPidProbElectron = -99.;
                    trkPidProbPion     = -99.;
                    trkPidProbKaon     = -99.;
                    trkPidProbProton   = -99.;
                }

                // Set mO97Track info
                mHbtTrack->setId( mO97Track->GetId() );
                //mHbtTrack->setTrackType( 1 ); // 0 - global, 1 - primary
                //mHbtTrack->setCharge( charge );
                mHbtTrack->setFlag( 400 );

                mHbtTrack->setNHits( charge * 45 );
                //mHbtTrack->setNHitsFit( 45 );
                mHbtTrack->setNHitsPossible( 45 );
                mHbtTrack->setNHitsDedx( 45 );

                mHbtTrack->setNSigmaElectron( trkNSigmaElectron );
                mHbtTrack->setNSigmaPion( trkNSigmaPion );
                mHbtTrack->setNSigmaKaon( trkNSigmaKaon );
                mHbtTrack->setNSigmaProton( trkNSigmaProton );

                mHbtTrack->setPidProbElectron( trkPidProbElectron );
                mHbtTrack->setPidProbPion( trkPidProbPion );
                mHbtTrack->setPidProbKaon( trkPidProbKaon );
                mHbtTrack->setPidProbProton( trkPidProbProton );

                mHbtTrack->setDedx( dedxMean(trkMass, trkMom.Mag()) );


                // Set DCA in cm
                mHbtTrack->setDca( dcaX, dcaY, dcaZ );
                mHbtTrack->setChi2(1.);

                if( (dcaXY*dcaXY + dcaZ*dcaZ) > 2. ) { //Lets assume that if dca>2cm -> global track
                  mHbtTrack->setP( TVector3(0., 0., 0.) );
                }
                else {
                  mHbtTrack->setP( trkMom );
                }
                mHbtTrack->setGlobalP( trkMom );

                mHbtTrack->setTopologyMap( 0, 0. );
                mHbtTrack->setTopologyMap( 1, 0. );

                mHbtTrack->setBeta( trkMom.Mag()/mO97Track->GetEnergy() );
                mHbtTrack->setHiddenInfo(0);

                mHbtTrack->setEmissionPointX( mO97Track->GetXfr() );
                mHbtTrack->setEmissionPointY( mO97Track->GetYfr() );
                mHbtTrack->setEmissionPointZ( mO97Track->GetZfr() );
                mHbtTrack->setEmissionPointT( mO97Track->GetTfr() );

                mHbtTrack->setPdgCode( mO97Track->GetPdgId() );
                mHbtTrack->setBField( PYTHIA_RBFIELD );

                mHbtEvent->trackCollection()->push_back( mHbtTrack );
            } // for (int i = 0; i < nTracks; i++)
        } // if (mO97TrackArr)

        mHbtEvent->setEventNumber( mO97Event->GetEventNumber() );
        mHbtEvent->setRunNumber( 0 );
        mHbtEvent->setZdcAdcEast( 45000 );
        mHbtEvent->setZdcAdcWest( 45000 );
        mHbtEvent->setNumberOfPrimaryTracks( nTracks );
        mHbtEvent->setEventPlaneAngle( 0. );
        mHbtEvent->setEventPlaneResolution( 0. );
        mHbtEvent->setRefMultPos( refMultPos );
        mHbtEvent->setRefMult( refMultPos + refMultNeg );
        mHbtEvent->setBTofTrayMult( refMultPos + refMultNeg );
        mHbtEvent->setPrimaryVertex( vPos );
        mHbtEvent->setBField( PYTHIA_RBFIELD );
        mHbtEvent->setVpdVz( 0. );
        mHbtEvent->setSphericity( mO97Event->GetTransverseSphericity() );
    } // if (mO97Event)

    return mHbtEvent;
}

//_________________
StHbtEvent *StHbtO97DstReader::returnHbtEvent() {

  StHbtEvent *mHbtEvent = Read();
  if (!mHbtEvent) {
    std::cout << "[StHbtO97DstReader]: no hbt event (mHbtEvent == NULL)"
        << std::endl;
    return 0;
  }
  else {
    return mHbtEvent;
  }
}

//_________________
StHbtString StHbtO97DstReader::Report() {
  StHbtString temp = "\nThis is the StHbtO97DstReader\n";
  return temp;
}

//_________________
void StHbtO97DstReader::PrintTotalTracks() {
  std::cout << "[StHbtO97DstReader]: TotalTracks = "
      << mTotalTracks << std::endl;
}

//_________________
int StHbtO97DstReader::GetNEvents() {
  if (mTChain) {
    return mNEvents;
  }
  else {
    return -1;
  }
}
