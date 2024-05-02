echo Install dependencies
echo Cinder-Hap2
git clone https://github.com/videodromm/Cinder-Hap2 ../../Cinder/blocks/Cinder-Hap2
echo Cinder-HTTP
git clone https://github.com/ryanbartley/Cinder-HTTP ../../Cinder/blocks/Cinder-HTTP
echo Cinder-Warping
git clone https://github.com/videodromm/Cinder-Warping ../../Cinder/blocks/Cinder-Warping
echo Cinder-VDR
git clone https://github.com/videodromm/Cinder-VDR ../../Cinder/blocks/Cinder-VDR
echo Cinder-VDRUI
git clone https://github.com/videodromm/Cinder-VDRUI ../../Cinder/blocks/Cinder-VDRUI
echo Cinder-ImGui
git clone https://github.com/simongeilfus/Cinder-ImGui ../../Cinder/blocks/Cinder-ImGui --recursive
echo Cinder-MIDI2
git clone https://github.com/brucelane/Cinder-MIDI2 ../../Cinder/blocks/Cinder-MIDI2
echo Cinder-WebSocketPP
git clone https://github.com/videodromm/Cinder-WebSocketPP ../../Cinder/blocks/Cinder-WebSocketPP
echo Cinder-WMFVideo
git clone https://github.com/Potion/Cinder-WMFVideo ../../Cinder/blocks/Cinder-WMFVideo
echo Cinder-Spout
git clone https://github.com/brucelane/Cinder-Spout ../../Cinder/blocks/Cinder-Spout
echo Cinder-Syphon
git clone https://github.com/videodromm/Cinder-Syphon ../../Cinder/blocks/Cinder-Syphon
echo Cinder-NDI
git clone https://github.com/videodromm/Cinder-NDI ../../Cinder/blocks/Cinder-NDI
pwd
echo Pull changes
git pull
cd ..
echo assets
git clone https://github.com/videodromm/assets
cd assets
git pull
cd ..

echo videodromm-controller-cinder
git clone https://github.com/videodromm/videodromm-controller-cinder
cd videodromm-controller-cinder
pwd
git pull
cd ..

echo videodromm-livecoding-cinder
git clone https://github.com/videodromm/videodromm-livecoding-cinder
cd videodromm-livecoding-cinder
pwd
git pull
cd ..

echo videodromm-visualizer-cinder
git clone https://github.com/videodromm/videodromm-visualizer-cinder
cd videodromm-visualizer-cinder
pwd
git pull
cd ..

echo Required dependencies checked out
cd ../Cinder/blocks/Cinder-Videodromm
git pull
cd ../Cinder-Warping
git pull
cd ../Cinder-Hap2
git pull
cd ../Cinder-WebSocketPP
git pull
cd ../Cinder-Spout
git pull

