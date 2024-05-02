#include "CinderNDISender.h"

#include "cinder/Log.h"
#include "cinder/Surface.h"

#include <Processing.NDI.Send.h>

CinderNDISender::CinderNDISender( const std::string name )
	: mName{ name }, mNdiSender{ nullptr }, mFramerateNumerator{ 60000 }, mFramerateDenominator{ 1001 }
{
	if( ! NDIlib_is_supported_CPU() ) {
		CI_LOG_E( "Failed to initialize NDI because of unsupported CPU!" );
	}

	if( ! NDIlib_initialize() ) {
		CI_LOG_E( "Failed to initialize NDI!" );
	}

	NDIlib_send_create_t NDI_send_create_desc = { mName.c_str(), nullptr, true, false };
	mNdiSender = NDIlib_send_create( &NDI_send_create_desc );
}

CinderNDISender::~CinderNDISender()
{
	if( mNdiSender ) {
		NDIlib_send_destroy( mNdiSender );
	}
	NDIlib_destroy();
}

void CinderNDISender::setFramerate( int numerator, int denominator )
{
	mFramerateNumerator = numerator;
	mFramerateDenominator = denominator;
}


void CinderNDISender::sendSurface( ci::Surface& surface )
{
	sendSurface( surface, NDIlib_send_timecode_synthesize );
}

void CinderNDISender::sendSurface( ci::Surface& surface, long long timecode, bool async )
{
	if( NDIlib_send_get_no_connections( mNdiSender, 0 ) ) {

		NDIlib_metadata_frame_t metadata_desc;
		/*if( NDIlib_send_capture( mNdiSender, &metadata_desc, 0 ) ) {
			CI_LOG_I( "Received meta-data : " << metadata_desc.p_data );
		}*/


		NDIlib_video_frame_v2_t NDI_video_frame;
		NDI_video_frame.xres = (unsigned int)( surface.getWidth() );
		NDI_video_frame.yres = (unsigned int)( surface.getHeight() );
		NDI_video_frame.FourCC = NDIlib_FourCC_type_BGRX;
		NDI_video_frame.p_data = (uint8_t*)(surface.getData());
		NDI_video_frame.frame_rate_N = mFramerateNumerator;
		NDI_video_frame.frame_rate_D = mFramerateDenominator;
		
		if( async ) {
			NDIlib_send_send_video_async_v2( mNdiSender, &NDI_video_frame );
		}
		else {
			NDIlib_send_send_video_v2( mNdiSender, &NDI_video_frame );
		}
		//if( timecode % 25 == 0 )
		//	CI_LOG_I( ( NDI_tally.on_program ? "PGM " : "" ) << " " << ( NDI_tally.on_preview ? "PVW " : "" ) );
	}
	else {
		//CI_LOG_I( "No connection, not sending frames." );
	}
}

void CinderNDISender::sendSurfaceForceSync()
{
	NDIlib_send_send_video_async( mNdiSender, NULL );
}

void CinderNDISender::sendMetadata( const ci::XmlTree& metadataString )
{
	sendMetadata( metadataString, NDIlib_send_timecode_synthesize );
}

void CinderNDISender::sendMetadata( const ci::XmlTree& xmlTree, long long timecode )
{
	auto str = ci::toString( xmlTree );

	if( NDIlib_send_get_no_connections( mNdiSender, 0 ) ) {
		const NDIlib_metadata_frame_t NDI_metadata = {
			(int)(str.size()),
			timecode,
			const_cast<CHAR*>(str.c_str())
		};
		NDIlib_send_send_metadata( mNdiSender, &NDI_metadata );
	}
}
