ofxPXCU - openFrameworks addon for the Perceptual Computing SDK
--------
<b>A wrapper for the Intel Perceptual Computing SDK's simplified utility interface (PXCUPipeline)<br/>
Download the [Perceptual Computing SDK](http://software.intel.com/en-us/vcsource/tools/perceptual-computing-sdk)<br/>
Download [openFrameworks](http://openframeworks.cc)</b><br/>

This wrapper aims to provide access to all the features provided by the Perceptual Computing SDK's simple utility interface.<br/>
Currently implemented features include:
* RGB Capture (VGA and WXGA resolutions)
* Depth Capture
* IR Capture
* Label/Blob Capture
 
Support for the following features is currently being implemented and will be available soon <i>(by the end of Jan '13 -[sG](mailto:seth.gibson@intel.com))</i>:
* Hand and Fingertip Tracking
* Face Location and Landmark Detection
* Simple Gesture Recognition

<h3>Installation</h3>
* Clone the repo into your <b>openframeworks_root\addons</b> folder
* The VS and C::B project files for the examples all live in the example's root:
  * example-capture\example-capture.workspace
  * example-capture\example-capture.sln

<h3>Examples</h3>
* example-capture: Grabbing RGB, Depth, Label/Blob, and IR images. <i>(Set the startup project to example-capture -[sG](mailto:seth.gibson@intel.com))</i>
* More examples coming soon...

Questions, Comments, Interface design/Feature suggestions? [Please Email Me!](mailto:seth.gibson@intel.com)
