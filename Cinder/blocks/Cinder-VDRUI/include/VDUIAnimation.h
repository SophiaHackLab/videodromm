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
	// stores the pointer to the VDUIAnimation instance
	typedef std::shared_ptr<class VDUIAnimation> VDUIAnimationRef;
	typedef std::shared_ptr<class VDUIOsc> VDUIOscRef;
#define IM_ARRAYSIZE(_ARR)			((int)(sizeof(_ARR)/sizeof(*_ARR)))
	class VDUIAnimation
	{
	public:
		VDUIAnimation(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms);
		static VDUIAnimationRef	create(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms)
		{
			return shared_ptr<VDUIAnimation>(new VDUIAnimation(aVDSettings, aVDSession, aVDUniforms));
		}
		~VDUIAnimation();
		void    Run(const char* title);
	private:
		// Params
		VDParamsRef						mVDParams;
		// Settings
		VDSettingsRef					mVDSettings;
		// Session
		VDSessionFacadeRef				mVDSession;
		// Uniforms
		VDUniformsRef					mVDUniforms;

		// imgui
		float							getFloatValue(unsigned int aCtrl) {
			return mVDSession->getUniformValue(aCtrl);
		};
		void							setFloatValue(unsigned int aCtrl, float aValue) {
			mVDSession->setUniformValue(aCtrl, aValue);
		};
		void							toggleValue(unsigned int aCtrl) {
			mVDSession->toggleValue(aCtrl);
		};

		float							getMinUniformValue(unsigned int aIndex) {
			return mVDSession->getMinUniformValue(aIndex); // TODO ERROR UI 0 for value
		};
		float							getMaxUniformValue(unsigned int aIndex) {
			return mVDSession->getMaxUniformValue(aIndex); // TODO ERROR UI 0 for value
		};

		unsigned int					ctrl;
		map<int, float>					localValues;
		float							f = 0.0f;
		char							buf[64];
		// zoom
		float							minZoom;
		float							maxZoom;
		// tempo
		void							toggleSpinalTap() { spinalTap = !spinalTap; };
		bool							spinalTap = false;

		// render
		float							contour, iVAmount, iVFallOff;
		int								iBlendmode;
		int								iResolutionX, iResolutionY;
		int								iOutW, iOutH, iBarBeat;
		// contour
		float							minContour;
		float							maxContour;
		// color
		float							color[4];
		float							backcolor[4];
	};
}