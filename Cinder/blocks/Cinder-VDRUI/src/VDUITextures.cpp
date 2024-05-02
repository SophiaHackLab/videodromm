#include "VDUITextures.h"

using namespace videodromm;

VDUITextures::VDUITextures(VDUniformsRef aVDUniforms, VDSessionFacadeRef aVDSession) {
	mVDUniforms = aVDUniforms;
	mVDSession = aVDSession;
	// Params
	mVDParams = VDParams::create();
}
VDUITextures::~VDUITextures() {

}

void VDUITextures::Run(const char* title) {

	xPos = mVDParams->getUIMargin() + mVDParams->getUIXPosCol1();
	yPos = mVDParams->getUIYPosRow3();
	int t = 0;
	int validImages = 0;
	for (int ti = 0; ti < mVDSession->getInputTexturesCount(); ti++) {
		if (mVDSession->isValidInputTexture(ti)) {
			t = ti;
			validImages++;
			ImGui::SetNextWindowSize(ImVec2(mVDParams->getUISmallPreviewW(), mVDParams->getPreviewHeight()), ImGuiSetCond_Once);
			ImGui::SetNextWindowPos(ImVec2(xPos, yPos), ImGuiSetCond_Once);
			unsigned int ms = mVDSession->getFboMs(t);
			sprintf(buf, " %s##s%d", mVDSession->getInputTextureName(t).c_str(), t);
			ImGui::Begin(buf, NULL, ImVec2(0, 0), ImGui::GetStyle().Alpha, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse);
			{
				ImGui::PushItemWidth(mVDParams->getUISmallPreviewW());
				ImGui::PushID(t);
				if (mVDSession->getFboInputTexture(t)) {
					ImGui::Image(mVDSession->getFboInputTexture(t), ivec2(mVDParams->getUISmallPreviewW(), mVDParams->getUISmallPreviewH()));
				}

				ImGui::TextColored(ImColor(155, 50, 255), "%d-", t);
				ImGui::SameLine();
				if (ms > 30) {
					ImGui::TextColored(ImColor(255, 0, 0), "%dms", ms);
				}
				else {
					ImGui::TextColored(ImColor(155, 255, 0), "%dms", ms);
				}
				ImGui::PopID();
				ImGui::PopItemWidth();
			}
			ImGui::End();
			xPos += mVDParams->getUISmallPreviewW() + mVDParams->getUIMargin();
			
			if (validImages % 22 == 21)
			{
				xPos = mVDParams->getUIMargin() + mVDParams->getUIXPosCol1();
				yPos -= mVDParams->getPreviewHeight() + mVDParams->getUIMargin();
				if (yPos < mVDParams->getUIYPosRow2() + 200) yPos = mVDParams->getUIYPosRow3();
			}
		}
	}

}
