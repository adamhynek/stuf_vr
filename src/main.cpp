#include "common/IDebugLog.h"  // IDebugLog
#include "skse64_common/skse_version.h"  // RUNTIME_VERSION
#include "skse64/PluginAPI.h"  // SKSEInterface, PluginInfo
#include "skse64/GameData.h"
#include "skse64_common/SafeWrite.h"

#include <ShlObj.h>  // CSIDL_MYDOCUMENTS

#include "version.h"


auto drawEitherHandLoc = RelocAddr<uintptr_t>(0x730A53);
auto drawRightHandLoc = RelocAddr<uintptr_t>(0x732FBF);
auto drawLeftHandLoc = RelocAddr<uintptr_t>(0x732B4F);
auto drawBothHandsLoc = RelocAddr<uintptr_t>(0x72FEA0);


void DoAsmEdits()
{
	{
		UInt64 nops = 0x9090909090909090;
		SafeWriteBuf(drawEitherHandLoc.GetUIntPtr(), &nops, 6);
		_MESSAGE("nop'd out first weapon draw for either hand");
	}

	{
		UInt8 ret = 0xC3;

		SafeWrite8(drawRightHandLoc.GetUIntPtr(), ret);
		_MESSAGE("ret'd right hand weapon draw");

		SafeWrite8(drawLeftHandLoc.GetUIntPtr(), ret);
		_MESSAGE("ret'd left hand weapon draw");

		SafeWrite8(drawBothHandsLoc.GetUIntPtr(), ret);
		_MESSAGE("ret'd both hand dual-cast weapon draw");
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface* skse, PluginInfo* info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim VR\\SKSE\\stuf_vr.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_DebugMessage);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("STUF VR v%s", STUF_VERSION_VERSTRING);

		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "STUF VR";
		info->version = STUF_VERSION_MAJOR;

		if (skse->isEditor) {
			_FATALERROR("[FATAL ERROR] Loaded in editor, marking as incompatible!\n");
			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VR_VERSION_1_4_15) {
			_FATALERROR("[FATAL ERROR] Unsupported runtime version %08X!\n", skse->runtimeVersion);
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)
	{	// Called by SKSE to load this plugin
		_MESSAGE("STUF VR loaded");

		DoAsmEdits();

		return true;
	}
};
