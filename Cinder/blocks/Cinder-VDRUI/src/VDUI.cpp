#include "VDUI.h"

using namespace videodromm;

VDUI::VDUI(VDSettingsRef aVDSettings, VDSessionFacadeRef aVDSession, VDUniformsRef aVDUniforms) {
	mVDSettings = aVDSettings;
	mVDSession = aVDSession;
	mVDUniforms = aVDUniforms;
	// Params
	mVDParams = VDParams::create();
	// UITextures
	mUITextures = VDUITextures::create(mVDUniforms, mVDSession);
	// UIFbos
	mUIFbos = VDUIFbos::create(mVDSettings, mVDSession, mVDUniforms);
	// UIAnimation
	mUIAnimation = VDUIAnimation::create(mVDSettings, mVDSession, mVDUniforms);

	// UIWarps
	mUIWarps = VDUIWarps::create(mVDSettings, mVDSession, mVDUniforms);
	// imgui
	mouseGlobal = false;
	//mouseZ = false;
	mIsResizing = true;
	mShowWarps = false;
	mShowFbos = true;
	mShowTextures = false;
	mShowBlend = false;
}

void VDUI::Run(const char* title, unsigned int fps) {
	static int currentWindowRow1 = 1;
	static int currentWindowRow2 = 0;

	ImGuiStyle& style = ImGui::GetStyle();
	//ImGuiStyle& style = ImGui::GetStyle();

	if (mIsResizing) {
		mIsResizing = false;
		// set ui window and io events callbacks 
		// version 903 doesn't need init here
#if CINDER_VERSION == 902
		ImGui::connectWindow(getWindow());
		ImGui::initialize();
#endif

#pragma region style
		// our theme variables
		style.WindowRounding = 8;
		style.WindowPadding = ImVec2(3, 3);
		style.FramePadding = ImVec2(2, 2);
		style.FrameRounding = 6;
		style.ItemSpacing = ImVec2(3, 3);
		style.ItemInnerSpacing = ImVec2(3, 3);
		//style.WindowMinSize = ImVec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight());
		style.WindowMinSize = ImVec2(mVDParams->getUISmallPreviewW(), mVDParams->getUISmallPreviewH());
		style.Alpha = 0.65f;

		style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

#pragma endregion style
	}
#pragma region menu
	/*if (ImGui::BeginMainMenuBar()) {

		if (ImGui::BeginMenu("Options"))
		{
			ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	*/
#pragma endregion menu
	// right panel
	ImGui::SetNextWindowSize(ImVec2(242.0f, mVDParams->getUILargeH()), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(mVDParams->getUIXPosCol3(), mVDParams->getUIYPosRow1()), ImGuiSetCond_Once);

	ImGui::Begin(" Messages", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
	{
		if (ImGui::Button("Clear")) {
			mVDSettings->setMsg("");
			mVDSettings->setErrorMsg("");
			// TODO mVDSettings->mMidiMsg = ""; mVDSession->setmi("");

			mVDSettings->mSocketIOMsg = "";
			mVDSession->setOSCMsg("");
			//mVDSettings->mShaderMsg = "";
		}

		ImGui::TextColored(ImColor(200, 200, 0), "Msg: %s", mVDSettings->getMsg().c_str());
		int ec = mVDSession->getErrorCode();
		if (ec > 0) {
			switch (ec)
			{
			case 1:
				ImGui::TextColored(ImColor(255, 0, 0), "Set Uniform max=0");
				break;
			case 2:
				ImGui::TextColored(ImColor(255, 0, 0), "Set Uniform value out of bounds");
				break;
			case 3:
				ImGui::TextColored(ImColor(255, 0, 0), "Uniform value not set");
				break;
			default:
				ImGui::TextColored(ImColor(0, 255, 0), "Code:%d", ec);
				break;
			}
		}
		//ImGui::TextWrapped("Shader: %s", mVDSettings->mShaderMsg.c_str());
		ImGui::TextWrapped("Midi: %s", mVDSession->getMidiMsg().c_str());
		ImGui::TextWrapped("WS: %s", mVDSession->getWSMsg().c_str());
		ImGui::TextWrapped("OSC: %s", mVDSession->getOSCMsg().c_str());
		ImGui::TextColored(ImColor(255, 0, 0), "Last error: %s", mVDSettings->getErrorMsg().c_str());
	}
	ImGui::End();
	// Center panel
	ImGui::SetNextWindowSize(ImVec2(800.0f, mVDParams->getUILargePreviewH()), ImGuiSetCond_Once);
	ImGui::SetNextWindowPos(ImVec2(mVDParams->getUIXPosCol1(), mVDParams->getUIYPosRow1()), ImGuiSetCond_Once);

	sprintf(buf, " Fps %c %d ###fps", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], fps);
	ImGui::Begin(buf, NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
	{
		// line 1
		ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
		ImGui::Image(mVDSession->buildRenderedMixetteTexture(0), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mixette");
		ImGui::SameLine();
		ImGui::Image(mVDSession->buildPostFboTexture(), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Post");
		ImGui::SameLine();
		ImGui::Image(mVDSession->buildRenderedWarpFboTexture(), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Warp");
		ImGui::SameLine();
		ImGui::Image(mVDSession->buildFxFboTexture(), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Fx");

		ImGui::SameLine();

		// tempo
		static ImVector<float> tempoValues; if (tempoValues.empty()) { tempoValues.resize(40); memset(&tempoValues.front(), 0, tempoValues.size() * sizeof(float)); }
		static int tempoValues_offset = 0;
		static float tRefresh_time = -1.0f + 1.0f / 20.0f;
		if (ImGui::GetTime() > tRefresh_time)
		{
			tRefresh_time = ImGui::GetTime();
			tempoValues[tempoValues_offset] = mVDSession->getUniformValue(mVDUniforms->ITEMPOTIME);
			tempoValues_offset = (tempoValues_offset + 1) % tempoValues.size();
		}

		if (mVDSession->getUniformValue(mVDUniforms->ITEMPOTIME) < 0.1) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::PlotLines("T", &tempoValues.front(), (int)tempoValues.size(), tempoValues_offset, toString(int(mVDSession->getUniformValue(mVDUniforms->IBPM))).c_str(), 0.0f, 0.6f, ImVec2(0, 30));
		if (mVDSession->getUniformValue(mVDUniforms->ITEMPOTIME) < 0.1) ImGui::PopStyleColor();
		ImGui::SameLine();


		// fps
		static ImVector<float> fpsValues; if (fpsValues.empty()) { fpsValues.resize(100); memset(&fpsValues.front(), 0, fpsValues.size() * sizeof(float)); }
		static int fpsValues_offset = 0;
		static float refresh_time = -1.0f + 1.0f / 6.0f;
		if (ImGui::GetTime() > refresh_time)
		{
			refresh_time = ImGui::GetTime();
			fpsValues[fpsValues_offset] = fps;
			fpsValues_offset = (fpsValues_offset + 1) % fpsValues.size();
		}
		if (fps < 24.0) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		sprintf(buf, "%d", fps);
		ImGui::PlotLines("F", &fpsValues.front(), (int)fpsValues.size(), fpsValues_offset, buf, 0.0f, 100.0f, ImVec2(0, 30));
		if (fps < 24.0) ImGui::PopStyleColor();
		// audio
		ImGui::SameLine();
		static ImVector<float> timeValues; if (timeValues.empty()) { timeValues.resize(40); memset(&timeValues.front(), 0, timeValues.size() * sizeof(float)); }
		static int timeValues_offset = 0;
		// audio maxVolume

		if (ImGui::GetTime() > tRefresh_time)
		{
			tRefresh_time = ImGui::GetTime();
			timeValues[timeValues_offset] = mVDSession->getUniformValue(mVDUniforms->IMAXVOLUME);
			timeValues_offset = (timeValues_offset + 1) % timeValues.size();
		}

		ImGui::PlotHistogram("H", mVDSession->getFreqs(), mVDSession->getFFTWindowSize(), 0, NULL, 0.0f, 255.0f, ImVec2(0, 30));
		ImGui::SameLine();
		if (mVDSession->getUniformValue(mVDUniforms->IMAXVOLUME) > 240.0) ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
		ImGui::PlotLines("V", &timeValues.front(), (int)timeValues.size(), timeValues_offset, toString(int(mVDSession->getUniformValue(mVDUniforms->IMAXVOLUME))).c_str(), 0.0f, 255.0f, ImVec2(0, 30));
		if (mVDSession->getUniformValue(mVDUniforms->IMAXVOLUME) > 240.0) ImGui::PopStyleColor();

		ImGui::PushItemWidth(mVDParams->getPreviewFboWidth());
		ImGui::SameLine();

		multx = mVDSession->getUniformValue(mVDUniforms->IAUDIOX); // 40 was 12
		if (ImGui::SliderFloat("AX", &multx, 0.01f, 7.0f)) {
			mVDSession->setUniformValue(mVDUniforms->IAUDIOX, multx);
		}
		ImGui::SameLine();
		// audio preferred
		if (ImGui::Button("Mic In")) {
			mVDSession->toggleUseLineIn();
		}
		// midi preferred
		if (!mVDSession->isMidiSetup()) {
			ImGui::SameLine();
			sprintf(buf, "Midi");
			if (ImGui::Button(buf)) mVDSession->setupMidi();
		}

		int hue = 0;

		// debug
		ctrl = mVDUniforms->IDEBUG;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Debug")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();


		if (ImGui::Button("Warp++")) {
			mVDSession->createWarp();
		}
		hue++;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Warps")) {
			mToggleShowWarps();
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Fbos")) {
			mToggleShowFbos();
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Tex")) {
			mToggleShowTextures();
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Blend")) {
			mToggleShowBlend();
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();
		/*
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.9f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.9f, 0.8f, 0.8f));

		if (ImGui::Button("Auto Layout")) {
			mVDSession->toggleAutoLayout();
		}
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Auto Layout for render window");

		// Auto Layout for render window
		if (mVDSession->isAutoLayout()) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.9f, 1.0f, 0.5f));
		}
		else {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.1f, 0.1f));
			// render window width
			static int rw = mVDSettings->mRenderWidth;
			if (ImGui::SliderInt("rdr w", &rw, 640, 4080))
			{
				//mVDSession->setRenderWidth(rw);
				mVDSettings->mRenderWidth = rw;
			}
			ImGui::SameLine();
			// render window height
			static int rh = mVDSettings->mRenderHeight;
			if (ImGui::SliderInt("rdr h", &rh, 480, 1280))
			{
				//mVDSession->setRenderHeight(rh);
				mVDSettings->mRenderHeight = rh;
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
*/

		ctrl = mVDUniforms->IGLITCH;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Glitch")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ctrl = mVDUniforms->IGREYSCALE;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Greyscale")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ctrl = mVDUniforms->ITOGGLE;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Toggle")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		ctrl = mVDUniforms->IINVERT;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Invert")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();


		// iVignette
		ctrl = mVDUniforms->IVIGNETTE;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Vignette")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		// iClear
		ctrl = mVDUniforms->ICLEAR;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("Clear")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();
		/*
		// iflipv
		ctrl = mVDUniforms->IFLIPV;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("FlipV")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();

		// ifliph
		ctrl = mVDUniforms->IFLIPH;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("FlipH")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		ImGui::SameLine();
		*/
		// post flip		
		ctrl = mVDUniforms->IFLIPPOSTV;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("FlipPostV")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);
		hue++;
		/*
		ImGui::SameLine();
		ctrl = mVDUniforms->IFLIPPOSTH;
		(getFloatValue(ctrl)) ? ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue / 16.0f, 1.0f, 0.5f)) : ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f, 0.1f, 0.1f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue / 16.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue / 16.0f, 0.8f, 0.8f));
		if (ImGui::Button("FlipPostH")) {
			toggleValue(ctrl);
		}
		ImGui::PopStyleColor(3);

		ImGui::Text(" Fp %dx%d F %dx%d", mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight(), mVDParams->getFboWidth(), mVDParams->getFboHeight());
		ImGui::SameLine();*/
		ImGui::Text(" Main %dx%d", mVDSettings->mMainWindowWidth, mVDSettings->mMainWindowHeight);
		ImGui::SameLine();
		// windows
		ImGui::Text(" Render window %dx%d", mVDSettings->mRenderWidth, mVDSettings->mRenderHeight);
		ImGui::SameLine();
		// mouse
		ImGui::Text(" Clic %d", ImGui::GetIO().MouseDown[0]);
		ImGui::SameLine();
		ImGui::Text(" Pos: %.1f,%.1f", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);

		// time
		/*smooth = mVDSession->getUniformValue(mVDUniforms->ISMOOTH);
		if (ImGui::SliderFloat("Smooth", &smooth, 0.001f, 0.02f)) {
			mVDSession->setUniformValue(mVDUniforms->ISMOOTH, smooth);
		}
		ImGui::SameLine();*/
		ImGui::Text(" Time %.2f", mVDSession->getUniformValue(mVDUniforms->ITIME));
		ImGui::SameLine();
		ImGui::Text(" TStart %.2f", mVDSession->getUniformValue(mVDUniforms->ISTART));
		ImGui::SameLine();
		ImGui::Text(" Factor %.2f", mVDSession->getUniformValue(mVDUniforms->ITIMEFACTOR));
		//ImGui::SameLine();
		//ImGui::Text(" Tempo Time %.2f", mVDSession->getUniformValue(mVDUniforms->ITEMPOTIME));
		ImGui::SameLine();
		ImGui::Text(" Delta Time %.2f", mVDSession->getUniformValue(mVDUniforms->IDELTATIME));
		// LiveOSC Obsolete ImGui::Text("Trk %s %.2f", mVDSettings->mTrackName.c_str(), mVDSettings->liveMeter);
		//ImGui::SameLine();
		//			ImGui::Checkbox("Playing", &mVDSettings->mIsPlaying);
		//ImGui::Text(" Tempo %.2f", mVDSession->getUniformValue(mVDUniforms->IBPM));
		ImGui::SameLine();
		ImGui::Text(" Beat %.0f", mVDSession->getUniformValue(mVDUniforms->IBEAT));
		ImGui::SameLine();
		ImGui::Text(" Bar %.0f", mVDSession->getUniformValue(mVDUniforms->IBAR));
		ImGui::SameLine();
		ImGui::Text(" BStart %.0f", mVDSession->getUniformValue(mVDUniforms->IBARSTART));
		ImGui::SameLine();
		ImGui::Text(" Bb %.0f", mVDSession->getUniformValue(mVDUniforms->IBARBEAT));
		ImGui::SameLine();
		ImGui::Text(" %s (%.0f)", mVDSession->getTrackName().c_str(), mVDSession->getUniformValue(mVDUniforms->ITRACK));
		//ImGui::SameLine();
		//sprintf(buf, "%s", mVDSession->getTrackName().c_str());
		//ImGui::Text("Trk %s %.2f", mVDSettings->mTrackName.c_str(), mVDSettings->liveMeter);
		//ImGui::Text(" ", mVDSession->getTrackName());

		// line 3
		for (unsigned int m = 0; m < mVDSession->getModesCount(); m++) {
			if (m > 0) ImGui::SameLine();
			sprintf(buf, "%s##mode", mVDSession->getModeName(m).c_str());
			if (mVDSession->getUniformValue(mVDUniforms->IDISPLAYMODE) == m) {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(m / 16.0f, 1.0f, 0.5f));
			}
			else {
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(m / 16.0f, 0.1f, 0.1f));
			}
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(m / 16.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(m / 16.0f, 0.8f, 0.8f));
			if (ImGui::Button(buf)) mVDSession->setUniformValue(mVDUniforms->IDISPLAYMODE, m);
			sprintf(buf, "Set mode to %s", mVDSession->getModeName(m).c_str());
			if (ImGui::IsItemHovered()) ImGui::SetTooltip(buf);
			ImGui::PopStyleColor(3);
		}

/*
		const float spacing = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

		ImGui::PushID("fbomixes");
		for (unsigned int m = 0; m < mVDSession->getFboShaderListSize(); m++)
		{
			if (m > 0) ImGui::SameLine();
			ctrl = mVDUniforms->IWEIGHT0 + m;
			float iWeight = mVDSession->getUniformValue(ctrl);
			ImGui::PushID(m);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(m / 16.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(m / 16.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(m / 16.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(m / 16.0f, 0.9f, 0.9f));
			ImGui::Image(mVDSession->buildFboRenderedTexture(m), ivec2(mVDParams->getPreviewFboWidth(), mVDParams->getPreviewFboHeight()));

			ImGui::SameLine();
			if (ImGui::VSliderFloat("##v", ImVec2(18, 60), &iWeight, 0.0f, 1.0f, ""))
			{
				setFloatValue(ctrl, iWeight);
			};
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
				ImGui::SetTooltip("%.3f", iWeight);
			ImGui::PopStyleColor(4);
			ImGui::PopID();
		}
		ImGui::PopID();
		ImGui::PopStyleVar();*/

		ImGui::PopItemWidth();
	}
	ImGui::End();

	mUIAnimation->Run("Animation");
	/*switch (currentWindowRow1) {
	case 0:

		break;
	case 1:
		// Animation
		mUIAnimation->Run("Animation");
		break;
	case 2:
		// Mouse
		mUIMouse->Run("Mouse");
		break;
	case 3:
		// Render
		mUIRender->Run("Render");
		// Blend
		mUIBlend->Run("Blend");
		break;
	case 4:
		// Audio
		mUIAudio->Run("Audio");
		break;
	case 5:
		// Color
		mUIColor->Run("Color");
		break;
	case 6:
		// Osc
		mUIOsc->Run("Osc");
		break;
	case 7:
		// SocketIO
		mUISocketIO->Run("SocketIO");
		break;
	case 8:
		// Midi
		mUIMidi->Run("Midi");
		break;
	}
	mVDSession->blendRenderEnable(currentWindowRow1 == 3);*/
	// Warps
	if (mShowWarps) {
		mUIWarps->Run("Warps");
	}
	// Fbos
	if (mShowFbos) {
		mUIFbos->Run("Fbos");
	}
	// textures
	if (mShowTextures) {
		mUITextures->Run("Textures");
	}
	// blendmodes
	/*if (mShowBlend) {
		mUIBlend->Run("BlendModes");
	}*/

	// Shaders
	//mUIShaders->Run("Shaders");


}
