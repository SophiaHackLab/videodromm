#pragma once

#include "cinder/app/App.h"

// UserInterface
#include "CinderImGui.h"
// Settings
#include "VDSettings.h"
// Uniforms
#include "VDUniforms.h"
// Session
#include "VDSessionFacade.h"
// UIFbos
#include "VDUIFbos.h"
// UITextures
#include "VDUITextures.h"
// UIBlend
//#include "VDUIBlend.h"
// Animation
#include "VDUIAnimation.h"
// Warps
#include "VDUIWarps.h"
// Params
#include "VDParams.h"

using namespace ci;
using namespace ci::app;
using namespace std;

namespace videodromm
{
	// stores the pointer to the VDConsole instance
	typedef std::shared_ptr<class VDUI> VDUIRef;

	class VDUI
	{
	public:
		VDUI(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSessionFacade, VDUniformsRef aVDUniforms);
		static VDUIRef	create(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSessionFacade, VDUniformsRef aVDUniforms)
		{
			return shared_ptr<VDUI>(new VDUI(aVDSettings, aVDSessionFacade, aVDUniforms));
		}

		void Run(const char* title, unsigned int fps);
		void resize() {
			mIsResizing = true;
			// disconnect ui window and io events callbacks
			//ImGui::disconnectWindow(getWindow());
		}
		bool	isReady() { return !mIsResizing; };
	private:
		// Params
		VDParamsRef					mVDParams;
		// Settings
		VDSettingsRef				mVDSettings;
		// Session
		VDSessionFacadeRef			mVDSession;
		// Uniforms
		VDUniformsRef				mVDUniforms;
		// UIFbos
		VDUIFbosRef					mUIFbos;
		bool						showUIFbos;
		bool						mShowFbos;
		bool						mShowTextures;

		bool						mShowBlend;		
		// 		VDUIBlendRef				mUIBlend;

		// UITextures
		VDUITexturesRef				mUITextures;
		// UIAnimation
		VDUIAnimationRef			mUIAnimation;
		bool						showUIAnimation;
		
		// UIWarps
		VDUIWarpsRef				mUIWarps;
		bool						showUIWarps;
		bool						mShowWarps;

		// imgui
		char						buf[64];
		bool						mIsResizing;
		float						color[4];
		float						backcolor[4];
		float						multx;
		float						smooth;
		bool						mouseGlobal;
		int							ctrl;
		float						contour, iVAmount, iVFallOff, iWeight0, iWeight1, iWeight2, iWeight3, iWeight4, iWeight5, iWeight6, iWeight7;

		void toggleValue(unsigned int aCtrl) {
			mVDSession->toggleValue(aCtrl);
		}
		void mToggleShowWarps() {
			mShowWarps = !mShowWarps;
		}
		void mToggleShowFbos() {
			mShowFbos = !mShowFbos;
		}
		void mToggleShowTextures() {
			mShowTextures = !mShowTextures;
		}
		void mToggleShowBlend() {
			mShowBlend = !mShowBlend;
		}
		/*void setFloatValue(unsigned int aCtrl, float aValue) {
			mVDSession->setUniformValue(aCtrl, aValue);
		}*/
		float getMinUniformValue(unsigned int aIndex) {
			return mVDSession->getMinUniformValue(aIndex);
		}
		float getMaxUniformValue(unsigned int aIndex) {
			return mVDSession->getMaxUniformValue(aIndex);
		}
		float							getFloatValue(unsigned int aCtrl) {
			return mVDSession->getUniformValue(aCtrl);
		};
	};
}