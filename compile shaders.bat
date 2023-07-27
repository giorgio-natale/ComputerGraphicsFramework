cd shaders
glslc.exe ColorBlend.frag -o ColorBlend.spv
glslc.exe Default.frag -o Default.spv
glslc.exe PosUvNormToUv.vert -o PosUvNormToUv.spv
glslc.exe PosUvNormToUvFragpos.vert -o PosUvNormToUvFragpos.spv
glslc.exe PosUvNormToUvFragposFragnorm.vert -o PosUvNormToUvFragposFragnorm.spv
glslc.exe PosUvToUv.vert -o PosUvToUv.spv
glslc.exe PosUvToUvFragpos.vert -o PosUvToUvFragpos.spv
glslc.exe SimplePhong.frag -o SimplePhong.spv

glslc.exe PosUvNormTanToUv.vert -o PosUvNormTanToUv.spv
glslc.exe PosUvNormTanToUvFragpos.vert -o PosUvNormTanToUvFragpos.spv
glslc.exe PosUvNormTanToUvFragposFragnorm.vert -o PosUvNormTanToUvFragposFragnorm.spv
glslc.exe PosUvNormTanToUvFragposFragnormFragtan.vert -o PosUvNormTanToUvFragposFragnormFragtan.spv

glslc.exe GGXTest.frag -o GGXTest.spv




pause