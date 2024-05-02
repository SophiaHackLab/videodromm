#pragma once

#include "cinder/app/App.h"

// UserInterface
#include "CinderImGui.h"
// Settings
#include "VDSettings.h"
// Session
#include "VDSessionFacade.h"
// Uniforms
#include "VDUniforms.h"
// Params
#include "VDParams.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace videodromm
{
	// stores the pointer to the VDUIWarps instance
	typedef std::shared_ptr<class VDUIWarps> VDUIWarpsRef;

	class VDUIWarps
	{
	public:
		VDUIWarps(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms);
		static VDUIWarpsRef	create(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms)
		{
			return shared_ptr<VDUIWarps>(new VDUIWarps(aVDSettings, aVDSession, aVDUniforms));
		}
		void    Run(const char* title);
	private:
		// Params
		VDParamsRef					mVDParams;
		// Settings
		VDSettingsRef				mVDSettings;
		// Session
		VDSessionFacadeRef			mVDSession;
		// Uniforms
		VDUniformsRef				mVDUniforms;
		// imgui
		char						buf[64];
		int							xPos, yPos;
		bool						mUseMixette = false;
		void setValue(unsigned int aCtrl, float aValue) {
			mVDSession->setUniformValue(aCtrl, aValue);
		}
		int							ctrl;
	};
}