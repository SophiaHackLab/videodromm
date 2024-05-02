#include "CinderNDIReceiver.h"
#include <cstdio>
#include <chrono>

#include "cinder/Log.h"
#include "cinder/Surface.h"
#include <Processing.NDI.Recv.h>

CinderNDIReceiver::CinderNDIReceiver() : mNdiSources{ nullptr } {
	if( ! NDIlib_is_supported_CPU() ) {
		CI_LOG_E( "Failed to initialize NDI because of unsupported CPU!" );
	}

	if( ! NDIlib_initialize() ) {
		CI_LOG_E( "Failed to initialize NDI!" );
	}

	mNdiFinder = NDIlib_find_create_v2();
	if( !mNdiFinder ) {
		CI_LOG_E( "Failed to create NDI finder!" );
	}
	
	mCurrentIndex = -1;
	mNdiInitialized = true;
	mReady = false;
	mConnecting = false;
	mQuitSourceFindingThread = false;
}

CinderNDIReceiver::~CinderNDIReceiver()
{
	if (mNdiInitialized) {
		if (mNdiFinder) {
			NDIlib_find_destroy(mNdiFinder);
		}
		if (mNdiReceiver) {
			NDIlib_recv_destroy(mNdiReceiver);
		}

		NDIlib_destroy();
		mNdiInitialized = false;
	}

	if (mSourceFindingThread) {
		mQuitSourceFindingThread = true;
		mSourceFindingThread->join();
	}
}

void CinderNDIReceiver::setup(std::string name) {
	// only the first instance will be verbose
	static bool firstInstance = true;
	mVerbose = firstInstance;
	if (firstInstance) {
		firstInstance = false;
	}

	mPreferredSenderName = name;
	if (shouldWaitForPreferredSender()) {
		CI_LOG_I("Started NDI input stream for sender with name " << mPreferredSenderName);
	}

	mSourceFindingThread = std::unique_ptr<std::thread>(new std::thread(&CinderNDIReceiver::threadedSourceFind, this));
}

int CinderNDIReceiver::getIndexForSender(std::string name) {
	if (name.empty()) return -1;

	if (mNDIsenderNames.size() > 0) {
		for (int i = 0; i<(int)mNDIsenderNames.size(); i++) {
			std::string test = mNDIsenderNames.at(i);
			if (test.find(name) != std::string::npos) {
				return i;
			}
		}
	}
	return -1;
}

bool CinderNDIReceiver::getIsNewFrame() {
	bool newFrame = mNewFrame;
	mNewFrame = false;
	return newFrame;
}

bool CinderNDIReceiver::shouldWaitForPreferredSender() {
	return !mPreferredSenderName.empty();
}

void CinderNDIReceiver::initConnection(int index) {
	mReady = false;
	mConnecting = true;

	if (index != mCurrentIndex) {
		mCurrentIndex = index;
	}
	if( mNdiSources ) {

		if (mNdiReceiver) {
			NDIlib_recv_destroy(mNdiReceiver);
		}

		NDIlib_recv_create_v3_t NDI_recv_create_desc;
		NDI_recv_create_desc.source_to_connect_to = mNdiSources[index];
		NDI_recv_create_desc.color_format = NDIlib_recv_color_format_BGRX_BGRA;
		NDI_recv_create_desc.bandwidth = NDIlib_recv_bandwidth_highest;
		NDI_recv_create_desc.allow_video_fields = true;

		mNdiReceiver = NDIlib_recv_create_v3(&NDI_recv_create_desc);
		if(!mNdiReceiver) {
			CI_LOG_E("Failed to create NDI receiver!");
			mConnecting = false;
			return;
		}
		else {
			CI_LOG_I("Connected to sender at index #" << std::to_string(index) << " OK");
		}

		const NDIlib_tally_t tally_state = { true, false };
		NDIlib_recv_set_tally( mNdiReceiver, &tally_state);

		mReady = true;
		mConnecting = false;
	}
}

void CinderNDIReceiver::threadedSourceFind() {
	while (!mQuitSourceFindingThread) {
		if (mConnecting) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		// check if sources have changed (blocking for maximum of 1 second)
		if (!NDIlib_find_wait_for_sources(mNdiFinder, 1000)) {
			continue;
		}
		else {
			if(mVerbose) CI_LOG_I("NDI sources changed");
		}

		// get sources list
		uint32_t nSources = 0;
		mNdiSources = NDIlib_find_get_current_sources(mNdiFinder, &nSources);

		if (mReady) {	// we are connected to a source so we can sleep for a while
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}
		
		if (nSources == 0) {
			if (mVerbose) CI_LOG_I("No NDI sources found - looking for " << (shouldWaitForPreferredSender() ? mPreferredSenderName : "any stream"));
			continue;
		}

		// cache names
		mNDIsenderNames.clear();
		for (uint32_t i = 0; i < nSources; i++) {
			std::string name = mNdiSources[i].p_ndi_name;
			mNDIsenderNames.push_back(mNdiSources[i].p_ndi_name);
		}

		if (mVerbose) CI_LOG_I("Found NDI sources:");
		for (auto name : mNDIsenderNames) {
			if (mVerbose) CI_LOG_I("\t" << name);
		}

		// if we have a preferred sender name, try to find it
		if (shouldWaitForPreferredSender()) {
			for (uint32_t i = 0; i < nSources; i++) {
				if (mNDIsenderNames[i].find(mPreferredSenderName) != std::string::npos) {
					CI_LOG_I("Found preferred NDI source '" << mPreferredSenderName << "', full source name: '" << mNDIsenderNames[i] << "'");
					initConnection(i);
					continue;
				}
			}

			if (!mReady) {
				CI_LOG_I("Did not find preferred NDI source '" << mPreferredSenderName << "'. Found " << nSources << " other sources");
			}
		}
		else {
			CI_LOG_I("No NDI source preference, connecting to '" << mNDIsenderNames[0] << "'");

			initConnection(0);
		}
	}
}

bool CinderNDIReceiver::isReady() {
	return mReady;
}


void CinderNDIReceiver::update()
{
	if (!mNdiInitialized) {
		return;
	}

	if (!mReady) {
		return;
	}

	NDIlib_video_frame_t video_frame;
	NDIlib_metadata_frame_t metadata_frame;

	switch( NDIlib_recv_capture( mNdiReceiver, &video_frame, NULL, &metadata_frame, 0 ) ) {
		// No data
		case NDIlib_frame_type_none:
		{
			//CI_LOG_I( "No data received. " );
			break;
		}

		// Video data
		case NDIlib_frame_type_video:
		{
			//CI_LOG_I( "Video data received with width: " << video_frame.xres << " and height: " << video_frame.yres );
			auto surface = ci::Surface::create( video_frame.p_data, video_frame.xres, video_frame.yres, video_frame.line_stride_in_bytes, ci::SurfaceChannelOrder::BGRA );
			mVideoTexture.first = ci::gl::Texture::create( *surface );
			mVideoTexture.first->setTopDown( false );
			mVideoTexture.second = video_frame.timecode;
			NDIlib_recv_free_video( mNdiReceiver, &video_frame );
			break;
		}

		// Meta data
		case NDIlib_frame_type_metadata:
		{
			//CI_LOG_I( "Meta data received." );
			mMetadata.first = metadata_frame.p_data;
			mMetadata.second = metadata_frame.timecode;
			NDIlib_recv_free_metadata( mNdiReceiver, &metadata_frame );
			break;
		}

		default:
			break;
	}
}

int CinderNDIReceiver::getCurrentSenderIndex() {
	return mCurrentIndex;
}

int CinderNDIReceiver::getNumberOfSendersFound() {
	return mNDIsenderNames.size();
}

void CinderNDIReceiver::switchSource(int index) {
	initConnection(index);
}

std::string CinderNDIReceiver::getCurrentSenderName() {
	if (mNDIsenderNames.size() == 0) {
		return "none";
	} else {
		return mNDIsenderNames[mCurrentIndex];
	}
}


std::pair<std::string, long long> CinderNDIReceiver::getMetadata()
{
	return mMetadata;
}

std::pair<ci::gl::Texture2dRef, long long> CinderNDIReceiver::getVideoTexture()
{
	return mVideoTexture;
}
