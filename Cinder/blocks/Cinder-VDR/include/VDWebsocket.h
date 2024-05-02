#pragma once
#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/Json.h"

// Mediator
#include "VDMediator.h"
// WebSockets
#include "WebSocketClient.h"

using namespace ci;
using namespace ci::app;
//using namespace std;
using namespace asio;
//using namespace asio::ip; 

namespace videodromm
{
	class VDMediatorObservable;
	typedef std::shared_ptr<VDMediatorObservable> VDMediatorObservableRef;

	// stores the pointer to the VDWebsocket instance
	typedef std::shared_ptr<class VDWebsocket> VDWebsocketRef;
	class VDWebsocket {
	public:
		VDWebsocket();
		static VDWebsocketRef	create()
		{
			return std::shared_ptr<VDWebsocket>(new VDWebsocket());
		}
		void						setupWSClient(VDMediatorObservableRef aVDMediator, const std::string& aWSHost, int aWSPort);

		void						update();
		// messages
		void						sendJSON(const std::string& params);
		void						updateParams(int iarg0, float farg1);
		// WebSockets
		//void						wsWriteBinary(const void *data, int size);
		void						wsWrite(const std::string& msg);
		void						wsConnect();
		void						wsPing();
		// change a control value and update network clients
		void						changeFloatValue(unsigned int aControl, float aValue, bool forceSend = false, bool toggle = false, bool increase = false, bool decrease = false);
		void						changeIntValue(unsigned int aControl, int aValue);
		void						changeBoolValue(unsigned int aControl, bool aValue);
		void						toggleAuto(unsigned int aIndex);
		void						toggleValue(unsigned int aIndex);
		void						toggleTempo(unsigned int aIndex);
		void						resetAutoAnimation(unsigned int aIndex);
		void						changeShaderIndex(unsigned int aWarpIndex, unsigned int aWarpShaderIndex, unsigned int aSlot);
		void						changeWarpFboIndex(unsigned int aWarpIndex, unsigned int aWarpFboIndex, unsigned int aSlot); //aSlot 0 = A, 1 = B,...
		void                        changeFragmentShader(const std::string& aFragmentShaderText);
		// received shaders
		//bool						hasReceivedShader() { return shaderReceived; };
		//std::string					getReceivedShader();
		bool						hasReceivedUniforms() { return shaderUniforms; };
		std::string					getReceivedUniforms();
		// received stream
		std::string	*				getBase64Image();
		bool						hasReceivedStream() { return streamReceived; };
		void						setWSMsg(const std::string& aMsg);
		std::string					getWSMsg();
		/*VDUniformObserverRef	setUniformValue(int aIndex, float aValue) {
			if (aIndex != 20) { // mVDUniforms->IFPS
				wsWrite("{\"params\" :[{ \"name\":" + toString(aIndex) + ",\"value\":" + toString(aValue) + "}]}");
			}
			return shared_from_this();
		}*/
	private:
		//Mediator
		VDMediatorObservableRef		mVDMediator;
		// lights4events
		void						colorWrite();
		// WebSockets
		void						parseMessage(std::string msg);
		// Web socket client
		bool						clientConnected;
		void						wsClientConnect();
		void						wsClientDisconnect();
		int							receivedType;
		int							receivedWarpIndex;
		int							receivedFboIndex;
		int							receivedShaderIndex;
		int							receivedSlot;

		WebSocketClient				mClient;
		double						mPingTime;
		std::string					mWebSocketsMsg;
		std::string					mWSHost;
		int							mWSPort;
		// received shaders
		//bool						shaderReceived; // TODO remove
		//std::string					receivedFragString; // TODO remove
		bool						shaderUniforms;
		std::string					receivedUniformsString;

		std::string					mBase64String;
		// received stream
		bool						streamReceived;
	};
}

