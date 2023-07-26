cd shaders
glslc.exe ColorBlend.frag -o ColorBlend.spv
glslc.exe Default.frag -o Default.spv
glslc.exe PosUvNormToUv.vert -o PosUvNormToUv.spv
glslc.exe PosUvNormToUvFragpos.vert -o PosUvNormToUvFragpos.spv
glslc.exe PosUvNormToUvFragposFragnorm.vert -o PosUvNormToUvFragposFragnorm.spv
glslc.exe PosUvToUv.vert -o PosUvToUv.spv
glslc.exe PosUvToUvFragpos.vert -o PosUvToUvFragpos.spv

pause