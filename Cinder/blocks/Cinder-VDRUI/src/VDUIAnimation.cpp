#include "VDUIAnimation.h"

using namespace videodromm;

VDUIAnimation::VDUIAnimation(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms) {
	mVDSettings = aVDSettings;
	mVDSession = aVDSession;
	mVDUniforms = aVDUniforms;
	// Params
	mVDParams = VDParams::create();
	// zoom
	minZoom = getMinUniformValue(12);
	maxZoom = getMaxUniformValue(12);
	for (int c = 0; c < 128; c++)
	{
		localValues[c] = mVDSession->getUniformValue(c);
	}
	// contour
	minContour = getMinUniformValue(mVDUniforms->ICONTOUR);
	maxContour = getMaxUniformValue(mVDUniforms->ICONTOUR);
	iResolutionX = (int)mVDSession->getUniformValue(mVDUniforms->IRESOLUTIONX);
	iResolutionY = (int)mVDSession->getUniformValue(mVDUniforms->IRESOLUTIONY);
	iOutW = (int)mVDSession->getUniformValue(mVDUniforms->IOUTW);
	iOutH = (int)mVDSession->getUniformValue(mVDUniforms->IOUTH);
	iBarBeat = (int)mVDSession->getUniformValue(mVDUniforms->IBARBEAT);
	iBlendmode = 0.0f;
}
VDUIAnimation::~VDUIAnimation() {

}

void VDUIAnimation::Run(const char* title) {
	ImGui::SetNextWindowSize(ImVec2(mVDParams->getUILargeW(), mVDParams->getUILargeH() * 3.1), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(mVDParams->getUIMargin(), mVDParams->getUIYPosRow1()), ImGuiSetCond_Once);
	int hue = 0;
	ImGui::Begin(" Animation", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
		if (ImGui::CollapsingHeader("Color", NULL, true, true))
		{
			ImGui::PushItemWidth(200.0f);
			// foreground color
			color[0] = getFloatValue(mVDUniforms->ICOLORX);
			color[1] = getFloatValue(mVDUniforms->ICOLORY);
			color[2] = getFloatValue(mVDUniforms->ICOLORZ);
			color[3] = getFloatValue(mVDUniforms->IALPHA);
			ImGui::ColorEdit4("f", color);
			for (int i = 0; i < 4; i++)
			{
				if (getFloatValue(i + mVDUniforms->ICOLORX) != color[i])
				{
					setFloatValue(i + mVDUniforms->ICOLORX, color[i]);
				}
			}

			// background color
			backcolor[0] = getFloatValue(mVDUniforms->IBACKGROUNDCOLORX);
			backcolor[1] = getFloatValue(mVDUniforms->IBACKGROUNDCOLORY);
			backcolor[2] = getFloatValue(mVDUniforms->IBACKGROUNDCOLORZ);
			ImGui::ColorEdit3("g", backcolor);
			for (int i = 0; i < 3; i++)
			{
				if (getFloatValue(i + mVDUniforms->IBACKGROUNDCOLORX) != backcolor[i])
				{
					setFloatValue(i + mVDUniforms->IBACKGROUNDCOLORX, backcolor[i]);
				}

			}
			ImGui::PopItemWidth();
			ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
		}

		if (ImGui::CollapsingHeader("Animation", NULL, true, true))
		{
			if (ImGui::Button("Reset")) {
				mVDSession->resetAnim();
			}
			for (size_t iUniform = 5; iUniform < 29; iUniform++)
			{
				int hue = 0;

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_TIME) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.3f));
				}
				sprintf_s(buf, "a##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) {
					mVDSession->setAnim(iUniform, mVDSettings->ANIM_TIME);
				}
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_AUTO) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.25f));
				}
				sprintf_s(buf, "f##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_AUTO); }
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_BASS) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.25f));
				}
				sprintf_s(buf, "b##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_BASS); }
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_MID) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.25f));
				}
				sprintf_s(buf, "m##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_MID); }
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_SMOOTH) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.25f));
				}
				sprintf_s(buf, "s##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_SMOOTH); }
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				if (mVDSession->getAnim(iUniform) == mVDSettings->ANIM_NONE) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(13.0f / 16.0f, 1.0f, 0.5f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.25f));
				}
				sprintf_s(buf, "x##%s", mVDSession->getUniformName(iUniform).c_str());
				if (ImGui::Button(buf)) {
					mVDSession->setAnim(iUniform, mVDSettings->ANIM_NONE);
					mVDSession->setUniformValue(iUniform, mVDSession->getDefaultUniformValue(iUniform));
				}
				ImGui::PopStyleColor(1);
				hue++;
				ImGui::SameLine();

				localValues[iUniform] = mVDSession->getUniformValue(iUniform);
				sprintf_s(buf, "%d %s", (int)iUniform, mVDSession->getUniformName(iUniform).c_str());
				if (iUniform > 22 || iUniform == 14 || iUniform == 8 ) {
					if (iUniform == 25) {
						// iZoom
						if (ImGui::SliderFloat(buf, &localValues[iUniform], 0.0f, 1.49f))
						{
							setFloatValue(iUniform, localValues[iUniform]);
						}
					}
					else {
						if (iUniform == 23 || iUniform == 24) {
							// iRenderXY
							if (ImGui::SliderFloat(buf, &localValues[iUniform], -1.0f, 1.0f))
							{
								setFloatValue(iUniform, localValues[iUniform]);
							}
						}
						else {

							if (ImGui::SliderFloat(buf, &localValues[iUniform], 0.0f, 40.0f)) // 20211108 TODO PB with getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
							{
								setFloatValue(iUniform, localValues[iUniform]);
							}
						}
					}
				}
				else {
					if (ImGui::SliderFloat(buf, &localValues[iUniform], 0.00f, 1.0f)) // 20211108 TODO PB with getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
					{
						setFloatValue(iUniform, localValues[iUniform]);
					}
				}
			}

			// badTv
			/*ctrl = mVDUniforms->IBADTV;
			if (ImGui::Button("a##badtv")) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_TIME); }
			ImGui::SameLine();
			if (ImGui::Button("f##badtv")) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_AUTO); }
			ImGui::SameLine();
			if (ImGui::Button("x##badtv")) { mVDSession->setAnim(iUniform, mVDSettings->ANIM_NONE); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::DragFloat("badTv", &localValues[ctrl], 0.01f, getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}
			// iPixelX
			ctrl = mVDUniforms->IPIXELX;
			if (ImGui::Button("x##iPixelX")) { mVDSession->setAnim(ctrl, mVDSettings->ANIM_NONE); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("iPixelX/min/max", &localValues[ctrl], getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}
			// iPixelY
			ctrl = mVDUniforms->IPIXELY;
			if (ImGui::Button("x##iPixelY")) { mVDSession->setAnim(ctrl, mVDSettings->ANIM_NONE); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("iPixelY/min/max", &localValues[ctrl], getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}*/
			// iBarBeat
			ctrl = mVDUniforms->IBARBEAT;
			if (ImGui::Button("x##iBarBeat")) { iBarBeat = 1; setFloatValue(ctrl, 1); }
			ImGui::SameLine();
			if (ImGui::SliderInt("iBarBeat", &iBarBeat, 1, 50))
			{
				setFloatValue(ctrl, iBarBeat);
			}
			/*sprintf_s(buf, "XorY");
			mVDSettings->iXorY ^= ImGui::Button(buf);*/

			// steps
			/*ctrl = mVDUniforms->ISTEPS;
			if (ImGui::Button("x##steps")) { localValues[ctrl] = 16.0f; setFloatValue(ctrl, localValues[ctrl]); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("steps", &localValues[ctrl], 1.0f, 128.0f))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}
			// pixelate
			ctrl = mVDUniforms->IPIXELATE;
			if (ImGui::Button("x##pixelate")) { localValues[ctrl] = 1.0f; setFloatValue(ctrl, localValues[ctrl]); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("pixelate", &localValues[ctrl], 0.01f, 1.0f))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}
			// trixels
			ctrl = mVDUniforms->ITRIXELS;
			if (ImGui::Button("x##trixels")) { localValues[ctrl] = 0.0f; setFloatValue(ctrl, localValues[ctrl]); }
			ImGui::SameLine();
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("trixels", &localValues[ctrl], 0.00f, 1.0f))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}*/
		} // Uniforms
		if (ImGui::CollapsingHeader("Params", NULL, true, false))
		{

			for (size_t iUniform = 54; iUniform < 60; iUniform++)
			{
				int hue = 0;

				localValues[iUniform] = mVDSession->getUniformValue(iUniform);
				sprintf_s(buf, "%d %s", (int)iUniform, mVDSession->getUniformName(iUniform).c_str());

				if (ImGui::SliderFloat(buf, &localValues[iUniform], 0.00f, 10.0f))
				{
					setFloatValue(iUniform, localValues[iUniform]);
				}
				ImGui::SameLine();
				sprintf_s(buf, "%d", (int)iUniform);
				if (ImGui::InputFloat(buf, &localValues[iUniform], 0.00f, 3.0f))
				{
					setFloatValue(iUniform, localValues[iUniform]);
				}
			}


		} // Params
		// Audio
		if (ImGui::CollapsingHeader("Audio", NULL, true, false))
		{
			ImGui::PushItemWidth(mVDParams->getPreviewFboWidth() * 2);
			// mic
			(mVDSession->getUseLineIn()) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(4.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(4.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(4.0f, 0.8f, 0.8f));
			if (ImGui::Button("Mic In")) {
				mVDSession->toggleUseLineIn();
			}
			ImGui::PopStyleColor(3);
			hue++;
			ImGui::SameLine();
			// monitor
			(mVDSession->getUseWaveMonitor()) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.3f, 0.3f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f, 0.8f, 0.8f));
			if (ImGui::Button("Wave")) {
				mVDSession->toggleUseWaveMonitor();
			}
			ImGui::PopStyleColor(3);
			hue++;
			ImGui::SameLine();
			// wave
			(mVDSession->isAudioBuffered()) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.2f, 0.2f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f, 0.8f, 0.8f));
			if (ImGui::Button("Buf")) {
				mVDSession->toggleAudioBuffered();
			}
			ImGui::PopStyleColor(3);
			hue++;
			ImGui::SameLine();

			// random
			(mVDSession->getUseRandom()) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(3.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.3f, 0.3f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(3.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(3.0f, 0.8f, 0.8f));
			if (ImGui::Button("Rnd")) {
				mVDSession->toggleUseRandom();
			}
			ImGui::PopStyleColor(3);
			hue++;
			ImGui::SameLine();

			// TODO 20200221 ImGui::Text("Position %d", mVDSession->getPosition(0));
			ImGui::TextColored(ImColor(0, 255, 0), "Max %.1f", mVDSession->getUniformValue(mVDUniforms->IMAXVOLUME));

			static int iFreq0 = mVDSession->getFreqIndex(0);
			sprintf_s(buf, "f0 %4.2f##f0", mVDSession->getFreq(0));
			if (ImGui::SliderInt(buf, &iFreq0, 0, mVDSession->getFFTWindowSize()))
			{
				mVDSession->setFreqIndex(0, iFreq0);
			}
			static int iFreq1 = mVDSession->getFreqIndex(1);
			sprintf_s(buf, "f1 %4.2f##f1", mVDSession->getFreq(1));
			if (ImGui::SliderInt(buf, &iFreq1, 0, mVDSession->getFFTWindowSize()))
			{
				mVDSession->setFreqIndex(1, iFreq1);
			}

			static int iFreq2 = mVDSession->getFreqIndex(2);
			sprintf_s(buf, "f2 %4.2f##f2", mVDSession->getFreq(2));
			if (ImGui::SliderInt(buf, &iFreq2, 0, mVDSession->getFFTWindowSize()))
			{
				mVDSession->setFreqIndex(2, iFreq2);
			}

			static int iFreq3 = mVDSession->getFreqIndex(3);
			sprintf_s(buf, "f3 %4.2f##f3", mVDSession->getFreq(3));
			if (ImGui::SliderInt(buf, &iFreq3, 0, mVDSession->getFFTWindowSize()))
			{
				mVDSession->setFreqIndex(3, iFreq3);
			}
			ImGui::PopItemWidth();
		} // Audio

		if (ImGui::CollapsingHeader("Midi", NULL, true))
		{
			sprintf_s(buf, "Enable");
			if (ImGui::Button(buf)) mVDSession->setupMidi();
			if (mVDSession->isMidiSetup()) {
				ImGui::TextColored(ImColor(0, 255, 0), "%s", "Midi enabled");
				ImGui::Text(">%s", mVDSession->getMidiMsg().c_str());
				if (ImGui::CollapsingHeader("MidiIn", "20", true, true))
				{
					ImGui::Columns(2, "datain", true);
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Connect"); ImGui::NextColumn();
					ImGui::Separator();
					for (int i = 0; i < mVDSession->getMidiInPortsCount(); i++)
					{
						if (mVDSession->getMidiInPortName(i) != "Ableton Push 2 1") {
							ImGui::Text(mVDSession->getMidiInPortName(i).c_str()); ImGui::NextColumn();

							if (mVDSession->isMidiInConnected(i))
							{
								sprintf_s(buf, "Disconnect %d", i);
							}
							else
							{
								sprintf_s(buf, "Connect %d", i);
							}

							if (ImGui::Button(buf))
							{
								if (mVDSession->isMidiInConnected(i))
								{
									mVDSession->closeMidiInPort(i);
								}
								else
								{
									mVDSession->openMidiInPort(i);
								}
							}
							ImGui::NextColumn();
							ImGui::Separator();
						}
					}
					ImGui::Columns(1);
				}
				// Midi out
				if (ImGui::CollapsingHeader("MidiOut", "20", true, true))
				{
					ImGui::Columns(2, "dataout", true);
					ImGui::Text("Name"); ImGui::NextColumn();
					ImGui::Text("Connect"); ImGui::NextColumn();
					ImGui::Separator();
					for (int i = 0; i < mVDSession->getMidiOutPortsCount(); i++)
					{
						ImGui::Text(mVDSession->getMidiOutPortName(i).c_str()); ImGui::NextColumn();

						if (mVDSession->isMidiOutConnected(i))
						{
							sprintf_s(buf, "Disconnect  %d", i);
						}
						else
						{
							sprintf_s(buf, "Connect  %d", i);
						}

						if (ImGui::Button(buf))
						{
							if (mVDSession->isMidiOutConnected(i))
							{
								mVDSession->closeMidiOutPort(i);
							}
							else
							{
								mVDSession->openMidiOutPort(i);
							}
						}
						ImGui::NextColumn();
						ImGui::Separator();
					}
					ImGui::Columns(1);
				}
			}
		}

		// Tempo
		if (ImGui::CollapsingHeader("Tempo", NULL, true, false))
		{
			//if (ImGui::Button("x##startx")) { mVDSettings->iStart = 0.0f; }
			//ImGui::SameLine();
			//ImGui::SliderFloat("start", &mVDSettings->iStart, 0.01f, 1.0f, "%.4f");

			if (ImGui::Button("0##istartx")) {
				setFloatValue(mVDUniforms->ITIME, (float)getElapsedSeconds());
				setFloatValue(mVDUniforms->ISTART, mVDSession->getUniformValue(mVDUniforms->ITIME));
				setFloatValue(mVDUniforms->IBARSTART, mVDSession->getUniformValue(mVDUniforms->IBAR));
			}
			ImGui::SameLine();
			ImGui::TextColored(ImColor(0, 255, 0), "Start %.1f", mVDSession->getUniformValue(mVDUniforms->ISTART));
			/* 20211018 replaced by iSpeed
			if (ImGui::Button("x##spdx")) { mVDSettings->iSpeedMultiplier = 1.0f; }
			ImGui::SameLine();
			ImGui::SliderFloat("speed x", &mVDSettings->iSpeedMultiplier, 0.01f, 1.0f, "%.4f");//, 2.01f

			static int tf = 5;
			if (ImGui::Button("x##tfx")) { tf = 5; mVDSession->setTimeFactor(5); }
			ImGui::SameLine();
			if (ImGui::SliderInt("time x", &tf, 0, 9)) mVDSession->setTimeFactor(tf);

			ImGui::SliderFloat("iTimeFactor", &mVDSettings->iTimeFactor, 0.01f, 1.0f, "%.4f");

			// iTimeFactor KO 0.0 on 1st touch
			ctrl = mVDUniforms->ITIMEFACTOR;
			localValues[ctrl] = mVDSession->getUniformValue(ctrl);
			if (ImGui::SliderFloat("timeFactor", &localValues[ctrl], getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, localValues[ctrl]);
			}*/

			//ImGui::Text("Elapsed %.2f", mVDSession->getUniformValue(mVDUniforms->IELAPSED));
			// duration			
			//ImGui::SameLine();
			//ImGui::Text("duration", &mVDSettings->iBarDuration);
			//ImGui::SameLine();
			//ImGui::Text("phase %.2f ", mVDSession->getUniformValue(mVDUniforms->IPHASE));
			// BUG taptempo
			if (ImGui::Button("Tap toggle")) { toggleSpinalTap(); }
			if (spinalTap) {
				static float tempo = mVDSession->getUniformValue(mVDUniforms->IBPM);
				//ImGui::Text("Tempo %.2f ", tempo);
				if (ImGui::DragFloat("Tempo", &tempo, 0.01f, 0.01f, 200.0f, "%.2f"))
				{
					setFloatValue(mVDUniforms->IBPM, tempo);
				}
				if (ImGui::SliderFloat("TempoS", &tempo, 0.01f, 200.0f, "%.01f")) {
					setFloatValue(mVDUniforms->IBPM, tempo);
				}
			}
			else {
				if (ImGui::Button("Tap tempo")) { mVDSession->tapTempo(); }
			}

			if (mVDSession->getUseTimeWithTempo()) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 1.0f, 0.5f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.1f, 0.1f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));
			if (ImGui::Button("Time tempo")) { mVDSession->toggleUseTimeWithTempo(); }
			ImGui::PopStyleColor(3);


			//ImGui::SameLine();
			ImGui::PopItemWidth();

		}

		// OSC
		if (ImGui::CollapsingHeader("OSC", true))
		{
			static char host[128] = "127.0.0.1"; // #define IP_LOCALHOST 127.0.0.1
			// validate in loading, not here 
			std::copy(mVDSettings->mOSCDestinationHost.begin(), (mVDSettings->mOSCDestinationHost.size() >= 128 ? mVDSettings->mOSCDestinationHost.begin() + 128 : mVDSettings->mOSCDestinationHost.end()), host);
			static int senderPort = mVDSettings->mOSCDestinationPort;
			ImGui::InputText("destination host", host, IM_ARRAYSIZE(host));
			if (ImGui::InputInt("destination port", &senderPort)) mVDSettings->mOSCDestinationPort = senderPort;
			if (mVDSession->isOscSenderConnected()) {
				ImGui::Text("Osc sender connected %d", mVDSettings->mOSCDestinationPort);
			}
			else {
				if (ImGui::Button("Sender connect"))
				{
					mVDSession->addOSCObserver(mVDSettings->mOSCDestinationHost, mVDSettings->mOSCDestinationPort);
				}
			}

			static int receiverPort = mVDSession->getOSCReceiverPort();
			if (ImGui::InputInt("receiver port", &receiverPort)) mVDSession->setOSCReceiverPort(receiverPort);
			if (mVDSession->isOscReceiverConnected()) {
				ImGui::Text("Osc receiver connected %d", mVDSession->getOSCReceiverPort());
				ImGui::Text(">%s", mVDSession->getOSCMsg().c_str());
			}
			else {
				if (ImGui::Button("Receiver connect"))
				{
					mVDSession->setupOSCReceiver();
				}
			}

			/* TODO if useful ImGui::Text("Sending to 2nd host %s", mVDSettings->mOSCDestinationHost2.c_str());
			ImGui::SameLine();
			ImGui::Text(" on port %d", mVDSettings->mOSCDestinationPort2);*/
		}

		// Websocket
		if (ImGui::CollapsingHeader("Websocket", "8088", true, false))
		{
			//static char host[128] = "127.0.0.1"; // #define IP_LOCALHOST 127.0.0.1

			static int clientPort = mVDSession->getWSClientPort();
			if (ImGui::InputInt("client port", &clientPort)) mVDSession->setWSClientPort(clientPort);

			if (ImGui::Button("Connect"))
			{
				mVDSession->wsConnect();//  setupWSClient();
			}
			ImGui::SameLine();
			if (ImGui::Button("Ping")) { mVDSession->wsPing(); }
			ImGui::Text(">%s", mVDSession->getWSMsg().c_str());
		}

		// Render
		if (ImGui::CollapsingHeader("Render", false))
		{

			ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
			// multiple extra screens resolution
			ImGui::TextColored(ImColor(178, 0, 240), "rw %d", mVDSettings->mRenderWidth);
			ImGui::SameLine();
			ImGui::TextColored(ImColor(178, 0, 240), " rh %d", mVDSettings->mRenderHeight);
			// output resolution
			ctrl = mVDUniforms->IOUTW;
			if (ImGui::Button("x##ioutw")) { iOutW = 1280; setFloatValue(ctrl, 1280); }
			ImGui::SameLine();
			if (ImGui::SliderInt("iOutW", &iOutW, 320, 5000))
			{
				setFloatValue(ctrl, iOutW);
			}
			ctrl = mVDUniforms->IOUTH;
			if (ImGui::Button("x##iouth")) { iOutH = 720; setFloatValue(ctrl, iOutH); }
			ImGui::SameLine();
			if (ImGui::SliderInt("iOutH", &iOutH, 480, 2000))
			{
				setFloatValue(ctrl, iOutH);
			}
			// iResolution
			ctrl = mVDUniforms->IRESOLUTIONX;
			if (ImGui::Button("x##IRESOLUTIONX")) { iResolutionX = 1280; setFloatValue(ctrl, 1280); }
			ImGui::SameLine();
			if (ImGui::SliderInt("iResolutionX", &iResolutionX, (int)getMinUniformValue(ctrl), (int)getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, (float)iResolutionX);
			}
			ctrl = mVDUniforms->IRESOLUTIONY;
			if (ImGui::Button("x##IRESOLUTIONY")) { iResolutionY = 720; setFloatValue(ctrl, 720); }
			ImGui::SameLine();
			if (ImGui::SliderInt("iResolutionY", &iResolutionY, (int)getMinUniformValue(ctrl), (int)getMaxUniformValue(ctrl)))
			{
				setFloatValue(ctrl, (float)iResolutionY);
			}

			hue++;
			// mRenderXY
			/*ctrl = mVDUniforms->IRENDERXYX;
			if (ImGui::Button("x##IRENDERX")) { setFloatValue(ctrl, 0.0); }
			ImGui::SameLine();
			static float mx = mVDSession->getUniformValue(mVDUniforms->IRENDERXYX);
			if (ImGui::SliderFloat("RenderX", &mx, -1.0f, 1.0f))
			{
				setFloatValue(ctrl, mx);
			}

			ctrl = mVDUniforms->IRENDERXYY;
			if (ImGui::Button("x##IRENDERY")) { setFloatValue(ctrl, 0.0); }
			ImGui::SameLine();
			static float my = mVDSession->getUniformValue(mVDUniforms->IRENDERXYX);
			if (ImGui::SliderFloat("RenderY", &my, -1.0f, 1.0f))
			{
				setFloatValue(ctrl, my);
			}
			*/
			// mRenderXY
			static float texMultW = mVDSettings->mTexMult.x;
			if (ImGui::SliderFloat("texWx", &texMultW, 0.2f, 4.0f))
			{
				mVDSettings->mTexMult.x = texMultW;
			}
			ImGui::SameLine();
			static float texMultH = mVDSettings->mTexMult.y;
			if (ImGui::SliderFloat("texHx", &texMultH, 0.2f, 4.0f))
			{
				mVDSettings->mTexMult.x = texMultH;
			}

			// rect mxLeft
			static int mxLeft = mVDSettings->mxLeft;
			if (ImGui::SliderInt("mxL", &mxLeft, 0, 2280))
			{
				mVDSettings->mxLeft = mxLeft;
			}
			ImGui::SameLine();
			// rect myLeft
			static int myLeft = mVDSettings->myLeft;
			if (ImGui::SliderInt("myL", &myLeft, 0, 2280))
			{
				mVDSettings->myLeft = myLeft;
			}
			// rect mxRight
			static int mxRight = mVDSettings->mxRight;
			if (ImGui::SliderInt("mxR", &mxRight, 0, 2280))
			{
				mVDSettings->mxRight = mxRight;
			}
			ImGui::SameLine();
			// rect myRight
			static int myRight = mVDSettings->myRight;
			if (ImGui::SliderInt("myR", &myRight, 0, 2280))
			{
				mVDSettings->myRight = myRight;
			}

			const char* blendModes[] = { "mix", "multiply", "colorBurn", "linearBurn", "darkerColor", "lighten", "screen", "colorDodge", "linearDodge", "lighterColor", "overlay", "softLight", "hardLight", "vividLight", "linearLight", "pinLight", "hardMix", "difference", "exclusion", "subtract", "divide", "hue", "color", "saturation", "luminosity", "darken", "left", "right" };

			ctrl = mVDUniforms->IBLENDMODE;
			iBlendmode = (int)mVDSession->getUniformValue(ctrl);
			sprintf_s(buf, "Blendmode %s##ibl", blendModes[iBlendmode]);
			if (ImGui::SliderInt(buf, &iBlendmode, 0, 25))
			{
				setFloatValue(ctrl, (float)iBlendmode);
			}
			// debug
			/*ctrl = mVDUniforms->IDEBUG;
			(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
			if (ImGui::Button("debug")) {
				toggleValue(ctrl);
			}
			ImGui::PopStyleColor(3);
			hue++;

			ctrl = mVDUniforms->IVAMOUNT;
			iVAmount = mVDSession->getUniformValue(ctrl);
			if (ImGui::DragFloat("Amount", &iVAmount, 0.001f, 0.0f, 1.0f))
			{
				setFloatValue(ctrl, iVAmount);
			}
			ctrl = mVDUniforms->IVFALLOFF;
			iVFallOff = mVDSession->getUniformValue(ctrl);
			if (ImGui::DragFloat("FallOff", &iVFallOff, 0.001f, 0.0f, 0.99f))
			{
				setFloatValue(ctrl, iVFallOff);
			}

			// iContour
			ctrl = mVDUniforms->ICONTOUR;
			if (ImGui::Button("a##contour")) { mVDSession->setAnim(ctrl, mVDSettings->ANIM_TIME); }
			ImGui::SameLine();
			if (ImGui::Button("t##contour")) { mVDSession->setAnim(ctrl, mVDSettings->ANIM_AUTO); }
			ImGui::SameLine();
			if (ImGui::Button("x##contour")) { mVDSession->setAnim(ctrl, mVDSettings->ANIM_NONE); }
			ImGui::SameLine();
			contour = mVDSession->getUniformValue(ctrl);
			if (ImGui::DragFloat("contour", &contour, 0.001f, minContour, maxContour))
			{
				setFloatValue(ctrl, contour);
			}
			ImGui::DragFloat("mincr", &minContour, 0.001f, getMinUniformValue(ctrl), getMaxUniformValue(ctrl));
			ImGui::SameLine();
			ImGui::DragFloat("maxcr", &maxContour, 0.001f, getMinUniformValue(ctrl), getMaxUniformValue(ctrl));
			*/

			/*if (ImGui::Button("Create Window")) {
				mVDSession->createWindow();
			}
			ImGui::SameLine();
			if (ImGui::Button("Delete Window")) {
				mVDSession->deleteWindow();
			}*/


			//ImGui::SameLine();
			//ImGui::Text("solo %d", mVDSession->getSolo());

			ImGui::PopItemWidth();
		}

	}
	ImGui::End();
}
