R__LOAD_LIBRARY(libStHbtMaker.so)
R__LOAD_LIBRARY(libStFemto.so)

#include <iostream>

using namespace std;



const double pionMass = 0.139570;
const double NHitsFitOverNHitsPoss = 0.52;
const double mSigmaPion[2] = {-2., 2.};
const double mSigmaExclude[2] = {-2., 2.};

#define OUTERR(p, line) if (!p) { cout << "Can't create " #p " at line " << line << endl; return 1; }

void test(const char *inFile = "CuAu200_test.FemtoDst.root", const char *outFile = "out.root")
{
	// Create HBT maker
	StHbtMaker *hbt = new StHbtMaker();
	OUTERR(hbt, __LINE__);
	hbt->setDebug(0);
	StHbtManager *manager = hbt->hbtManager();
	OUTERR(manager, __LINE__);

	// Create FemtoDst HBT reader
	StHbtFemtoDstReader *reader = new StHbtFemtoDstReader("", inFile, "root", -1, false);
	OUTERR(reader, __LINE__);
	manager->setEventReader(reader);

	// Create analysis
        StHbtVertexMultAnalysis *analysis = new StHbtVertexMultAnalysis(/* using default options (see
                                                                           corresponding .h file) */);
	OUTERR(analysis, __LINE__);
	unsigned int why_the_hell_do_you_want_the_constant_lvalue_there_dammit_We_cant_use_rvalue_there = 1e3;
	analysis->setMinSizePartCollection(why_the_hell_do_you_want_the_constant_lvalue_there_dammit_We_cant_use_rvalue_there);
	analysis->setNumEventsToMix(4);

	// Create event and track cuts
	StHbtBasicEventCut *eventCut = new StHbtBasicEventCut();
	StHbtBasicTrackCut *trackCut = new StHbtBasicTrackCut();
	StHbtBasicPairCut  *pairCut  = new StHbtBasicPairCut();

	// Add value to the event
	eventCut->setCheckBadRun(false);
	eventCut->setSphericity(-200., 200.);

	// Add value to tha track
	trackCut->setMass(pionMass);
	trackCut->setNHitsFitOverNHitsPoss(NHitsFitOverNHitsPoss);
	trackCut->setNSigmaPion(mSigmaPion[0], mSigmaPion[1]);
	trackCut->setNSigmaOther(mSigmaExclude[0], mSigmaExclude[1]);
	trackCut->setTnTNSigmaPion(mSigmaPion[0], mSigmaPion[1]);

	// Add cuts to the analysis
	analysis->setEventCut(eventCut);
	analysis->setFirstParticleCut(trackCut);
	analysis->setSecondParticleCut(trackCut);
	analysis->setPairCut(pairCut);

	// Create corr function
	StHbtCorrFctn3DLCMSSym *cf = new StHbtCorrFctn3DLCMSSym("cf", 40 /* qinv bins */, 0.2 /* qinv high */);
	OUTERR(reader, __LINE__);

	//  Add corr function to the analysis and analysis to the manager
	analysis->addCorrFctn(cf);
	manager->addAnalysis(analysis);

	unsigned long nEvents = 10;//reader->GetNEvents();
	cout << "Number of events: " << nEvents << endl;

	hbt->Init();
	while (nEvents--)
	{
		int ret = hbt->Make();
		if (ret)
		{
			cout << "StHbtMaker error code: " << ret << " at line " << __LINE__ << endl;
			break;
		}
	}
	hbt->Finish();

	// Create output file
	TFile *fOut = new TFile(outFile, "RECREATE");
	OUTERR(fOut, __LINE__);

	// Write histograms
	cf->writeOutHistos();
	fOut->Close();
}
