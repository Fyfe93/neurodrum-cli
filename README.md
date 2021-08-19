# NeuroDrumOnnx
Command-line application using Onnx runtime and Onnx models from the NeuroDrum project

## Instructions
Requires the Juce library as this is a Juce project and uses the Projucer

1. Clone Onnx runtime and build following the instructions in the Onnyx Documentation
https://onnxruntime.ai/docs/how-to/build/inferencing.html

2. create "include" and "lib" directories in NeuroDrumOnnx project root folder

3. For now you must copy header files and corresponding .a library files over to the previously created include and lib directories. To check you have placed the library files and includes in the correct locations open the Projucer file 'NeuroDrum.jucer' and check against the linked library/include locations.

4. Launch the Xcode project from the Projucer and build.

5. The compiled executable will be found in the 'Builds' folder.
