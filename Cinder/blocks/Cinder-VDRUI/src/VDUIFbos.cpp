#include "VDUIFbos.h"

using namespace videodromm;

VDUIFbos::VDUIFbos(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms) {
	mVDSettings = aVDSettings;
	mVDSession = aVDSession;
	mVDUniforms = aVDUniforms;
	// Params
	mVDParams = VDParams::create();
	//globalUniforms = true;
	for (int c = 0; c < 128; c++)
	{
		localValues[c] = mVDSession->getUniformValue(c);
	}
}

void VDUIFbos::Run(const char* title) {

	/*
	** textures
	*/
	/*static int XLeft[64];
	static int YTop[64];
	static int XRight[64];
	static int YBottom[64];
	static bool rnd[64];
	static bool anim[64];*/

#pragma region fbos

	/*
	** fbos
	*/

	for (unsigned int f = 0; f < mVDSession->getFboShaderListSize(); f++) {
		xPos = mVDParams->getUIMargin() + mVDParams->getUIXPosCol1() + ((mVDParams->getUILargePreviewW() + mVDParams->getUIMargin()) * (f));
		yPos = mVDParams->getUIYPosRow2();
		ImGui::SetNextWindowSize(ImVec2(mVDParams->getUILargePreviewW(), mVDParams->getUILargePreviewH() * 1.4), ImGuiSetCond_Once);
		ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiSetCond_Once);
		ImGui::PushStyleColor(ImGuiCol_TitleBg, (ImVec4)ImColor::HSV(f / 16.0f, 0.9f, 0.9f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(f / 16.0f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(f / 16.0f, 0.6f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(f / 16.0f, 0.7f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(f / 16.0f, 0.9f, 0.9f));

		sprintf(buf, " %s##fbolbl%d", mVDSession->getFboName(f).c_str(), f);
		ImGui::Begin(buf, NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
		{
			ImGui::PushID(f);
			ctrl = mVDUniforms->IWEIGHT0 + f;
			float iWeight = mVDSession->getUniformValue(ctrl);

			ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
			if (!mVDSession->isFboValid(f)) {
				ImGui::TextColored(ImColor(255, 0, 0), "err: %s", mVDSession->getFboError(f).c_str());
			}
			else {
				ImGui::TextColored(ImColor(0, 255, 0), "%s", mVDSession->getFboMsg(f).c_str());
			}
			ImGui::SameLine();
			//ImGui::TextColored(ImColor(155, 255, 0), "%d/%dms ", mVDSession->getFboMs(f), mVDSession->getFboMsTotal(f));
			ImGui::TextColored(ImColor(155, 255, 0), "%dms ", mVDSession->getFboMsTotal(f));

			sprintf(buf, "fbo##rdrfbouniform%d", f);
			mShowRenderedTexture ^= ImGui::Button(buf);
			ImGui::SameLine();

			int hue = 0;
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
			sprintf(buf, "tex##rdrtexuniform%d", f);
			//mShowInputTexture ^= ImGui::Button(buf);
			if (ImGui::Button(buf)) {
				//mShowInputTexture = !mShowInputTexture;
				mVDSession->setSelectedFbo(f);
			}
			ImGui::PopStyleColor(3);

			hue++;
			ImGui::SameLine();
			sprintf(buf, "au##audio%d", f);
			if (ImGui::Button(buf)) {
				mVDSession->setFboTextureAudioMode(f);
			}
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Set input texture to audio");

			hue++;
			ImGui::SameLine();
			sprintf(buf, "tn##tn%d", f);
			if (ImGui::Button(buf)) {
				mVDSession->saveThumbnail(f);
			}
			//mVDSession->buildFboRenderedTexture(f) && 
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save thumbnail");

			if (mShowRenderedTexture) ImGui::Image(mVDSession->buildFboRenderedTexture(f), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
			ImGui::SameLine();
			if (ImGui::VSliderFloat("##v", ImVec2(18, 60), &iWeight, 0.0f, 1.0f, ""))
			{
				setValue(ctrl, f, iWeight);
			};

			
			ImGui::TextColored(ImColor(155, 50, 255), "%s", mVDSession->getFboStatus(f).c_str());

#pragma region tex
			/*for (unsigned int t = 0; t < mVDSession->getInputTexturesCount(f); t++) {
				if (t > 0 && (t % 6 != 0)) ImGui::SameLine();
				if (mVDSession->getFboInputTextureIndex(f) == t) {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(t / 7.0f, 1.0f, 1.0f));
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(t / 7.0f, 0.1f, 0.1f));
				}
				sprintf(buf, "%d##fboit%d%d", t, f, t);
				if (ImGui::Button(buf)) mVDSession->setFboInputTexture(f, t);

				sprintf(buf, "Set input texture to %s", mVDSession->getInputTextureName(t).c_str());
				if (ImGui::IsItemHovered()) ImGui::SetTooltip(buf);
				ImGui::PopStyleColor(1);
			}*/


			/*if (mVDSession->isSequence(t) || mVDSession->isMovie(t)) {
				sprintf(buf, "p##s%d", t);
				if (ImGui::Button(buf))
				{
					mVDSession->togglePlayPause(t);
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play/Pause");
			}
			if (mVDSession->isSequence(t)) {
				ImGui::SameLine();
				sprintf(buf, "b##sqs%d", t);
				if (ImGui::Button(buf))
				{
					mVDSession->syncToBeat(t);
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Sync to beat");

				ImGui::SameLine();
				sprintf(buf, "r##rs%d", t);
				if (ImGui::Button(buf))
				{
					mVDSession->reverse(t);
				}
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reverse");

				if (mVDSession->isLoadingFromDisk(t)) {
					ImGui::SameLine();
					sprintf(buf, "l##ts%d", t);
					if (ImGui::Button(buf))
					{
						mVDSession->toggleLoadingFromDisk(t);
					}
					if (ImGui::IsItemHovered()) ImGui::SetTooltip("Pause loading from disk");
				}
				speeds[t] = mVDSession->getSpeed(t);
				sprintf(buf, "speed##spd%d", t);
				if (ImGui::SliderFloat(buf, &speeds[t], 0.0f, 1.0f))
				{
					mVDSession->setSpeed(t, speeds[t]);
				}

				playheadPositions[t] = mVDSession->getPosition(t);
				sprintf(buf, "scrub##srb%d", t);
				if (ImGui::SliderInt(buf, &playheadPositions[t], 0, mVDSession->getMaxFrame(t)))
				{
					mVDSession->setPlayheadPosition(t, playheadPositions[t]);
				}


			}
			else {

			}*/

#pragma endregion tex


			// uniforms
			int channelIndex = 0;
			int texNameEndIndex = 0;
			for (auto u : mVDSession->getUniforms(f)) {
				string uName = u.getName(); // TODO use getIndex?
				ctrl = mVDSession->getUniformIndexForName(uName);

				switch (u.getType()) {
				case GL_BOOL:
					// boolean 35670 GL_BOOL 0x8B56
					(getValue(ctrl, f) > 0.0f) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
					sprintf(buf, "%s##booluniform%d", uName.c_str(), f);
					if (ImGui::Button(buf)) {
						toggleValue(ctrl, f);
					}
					ImGui::PopStyleColor(3);
					hue++;
					break;
				case GL_INT:
					// int 5124 GL_INT 0x1404
					(getValue(ctrl, f)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
					sprintf(buf, "%s##intuniform%d", uName.c_str(), f);
					if (ImGui::Button(buf)) {
						//toggleValue(ctrl, f);
					}
					ImGui::PopStyleColor(3);
					hue++;
					break;
				case GL_SAMPLER_2D:
					// sampler2d 35678 GL_SAMPLER_2D 0x8B5E

					texNameEndIndex = uName.find("tex");
					if (texNameEndIndex != std::string::npos && texNameEndIndex != -1) {
						// hydra fbo
						ImGui::Image(mVDSession->getFboInputTextureListItem(f, channelIndex), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
						channelIndex++;
						ImGui::SameLine();
					}
					//sprintf(buf, "%s", uName.c_str());
					//ImGui::TextColored(ImColor(220, 150, 0), buf);
					break;
				case GL_FLOAT:
					// float 5126 GL_FLOAT 0x1406
					localValues[ctrl] = mVDSession->getUniformValue(ctrl);
					if (ctrl > 0) {
						sprintf(buf, "%s##floatuniform%d", uName.c_str(), f);

						float spd = (ctrl == mVDUniforms->IPIXELX || ctrl == mVDUniforms->IPIXELY) ? 1.0f : 0.001f;

						if (ImGui::DragFloat(buf, &localValues[ctrl], spd, getMinUniformValue(ctrl), getMaxUniformValue(ctrl)))
						{
							setValue(ctrl, f, localValues[ctrl]);
						}
					}
					else {
						// 0 = time or unknown uniform
						if (uName == "iTime" || uName == "uTime" || uName == "time" || uName == "TIME") {
							// don't display
						}
						else {
							location = u.getLocation();
							mUniformValueByLocation[location] = mVDSession->getUniformValueByLocation(f, location);
							sprintf(buf, "%s##floatuniform%d", uName.c_str(), f);
							if (ImGui::DragFloat(buf, &mUniformValueByLocation[location], 0.001f, 0.0001f, 50.0f))
							{
								mVDSession->setUniformValueByLocation(f, location, mUniformValueByLocation[location]);

							}

						}
					}
					break;
				case GL_FLOAT_VEC2:
					// vec2 35664		
					/*if (uName == "RENDERSIZE" || uName == "resolution") {
						float fw = mVDSession->buildFboRenderedTexture(f)->getWidth();
						sprintf(buf, "rw %.0f", fw);						
						ImGui::TextColored(ImColor(100, 100, 100), buf);
						ImGui::SameLine();
						float fh = mVDSession->buildFboRenderedTexture(f)->getHeight();				
						sprintf(buf, "rh %.0f", fh);
						ImGui::TextColored(ImColor(100, 100, 100), buf);					
					}
					else {
						sprintf(buf, "vec2 %s", uName.c_str(), f);
						ImGui::TextColored(ImColor(150, 220, 0), buf);
					}*/
					break;
				case GL_FLOAT_VEC3:
					// vec3 35665
					//sprintf(buf, "vec3 %s", uName.c_str(), f);
					//ImGui::TextColored(ImColor(100, 100, 0), buf);

					break;
				case GL_FLOAT_VEC4:
					// vec4 35666 GL_FLOAT_VEC4
					/*sprintf(buf, "vec4 %s %d", uName.c_str(), u.getType());
					ImGui::TextColored(ImColor(100, 100, 100), buf);
					if (ctrl == mVDUniforms->IMOUSE) {
						mouseX = getValue(mVDUniforms->IMOUSEX, f);
						if (ImGui::SliderFloat("MouseX", &mouseX, 0.0f, mVDParams->getFboWidth(), "%.4f", 3.0f))
						{
							setValue(mVDUniforms->IMOUSEX, mouseX, f);
						}
						mouseY = getValue(mVDUniforms->IMOUSEY, f);
						if (ImGui::SliderFloat("MouseY", &mouseY, 0.0f, mVDParams->getFboHeight(), "%.4f", 0.3f))
						{
							setValue(mVDUniforms->IMOUSEY, mouseY, f);
						}
					}*/
					break;
				default:
					//ciModelViewProjection 35676 GL_FLOAT_MAT4 0x8B5C
					/* gl2.h
						GL_FLOAT_VEC2                     0x8B50
						GL_FLOAT_VEC3                     0x8B51
						GL_FLOAT_VEC4                     0x8B52
					*/
					if (uName != "ciModelViewProjection") {
						sprintf(buf, "! %s %d", uName.c_str(), u.getType());
						ImGui::TextColored(ImColor(255, 0, 0), buf);
					}
					break;
				}

			} //for uniforms

			/*float fw = mVDSession->getFboTextureWidth(f);
			sprintf(buf, "tw %.0f", fw);
			ImGui::TextColored(ImColor(120, 120, 120), buf);
			ImGui::SameLine();
			float fh = mVDSession->getFboTextureHeight(f);
			sprintf(buf, "th %.0f", fh);
			ImGui::TextColored(ImColor(120, 120, 120), buf);*/

			ImGui::PopItemWidth();
			ImGui::PopID();
		}
		ImGui::End();
		ImGui::PopStyleColor(5);
	} // for getFboList

#pragma endregion fbos

}
