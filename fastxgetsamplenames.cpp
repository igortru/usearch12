#include "myutils.h"
#include "objmgr.h"
#include "seqinfo.h"
#include "seqsource.h"
#include "label.h"
#include <set>
#include "progress.h"

void cmd_fastx_get_sample_names()
	{
	const string InputFileName(oget_str(OPT_fastx_get_sample_names)); //src_refactor_opts

	SeqSource &SS = *MakeSeqSource(InputFileName);

	ObjMgr &OM = *ObjMgr::CreateObjMgr();
	SeqInfo *SI = OM.GetSeqInfo();

	FILE *fOut = 0;
	if (ofilled_str(OPT_output)) //src_refactor_opts
		fOut = CreateStdioFile(oget_str(OPT_output)); //src_refactor_opts

	set<string> Samples;
	ProgressStartSS(SS, "Sample names");
	for (;;)
		{
		bool Ok = SS.GetNext(SI);
		if (!Ok)
			break;
		string Sample;
		GetSampleNameFromLabel(SI->m_Label, Sample);
		if (Sample.empty())
			Die("Empty sample name");
		Samples.insert(Sample);
		}
	ProgressDoneSS();

	for (set<string>::const_iterator p = Samples.begin(); p != Samples.end(); ++p)
		fprintf(fOut, "%s\n", (*p).c_str());

	CloseStdioFile(fOut);
	}
