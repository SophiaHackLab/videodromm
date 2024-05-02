#pragma once

#include "cinder/app/App.h"
#include "cinder/Rand.h"

// UserInterface
#include "CinderImGui.h"

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
	// stores the pointer to the VDConsole instance
	typedef std::shared_ptr<class VDUITextures> VDUITexturesRef;

	class VDUITextures
	{
	public:
		VDUITextures(VDUniformsRef aVDUniforms, VDSessionFacadeRef aVDSession);
		static VDUITexturesRef	create(VDUniformsRef aVDUniforms, VDSessionFacadeRef aVDSession)
		{
			return shared_ptr<VDUITextures>(new VDUITextures(aVDUniforms, aVDSession));
		}
		~VDUITextures();
		void    Run(const char* title);
	private:
		// Params
		VDParamsRef					mVDParams;
		// Uniforms
		VDUniformsRef				mVDUniforms;
		// Session
		VDSessionFacadeRef			mVDSession;
		// imgui
		char						buf[64];
		int							xPos, yPos;
		int							xStep = 1;
		int							yStep = 1;
		// sequence
		int							playheadPositions[12];
		float						speeds[12];
	};
}