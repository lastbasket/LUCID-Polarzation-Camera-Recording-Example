/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2023, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/

#include "stdafx.h"
#include "ArenaApi.h"
#include "SaveApi.h"
#include <iostream>

#define TAB1 "  "
#define TAB2 "    "
#define ERASE_LINE "\t\t\t\t\t"

// Record: Introduction
//    This example introduces the basics of video recording using the save
//    library. This includes preparing, creating, and writing images to the
//    video.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// Memory Limitations
//    This example appends many images to record a video stream and can quickly
//    exceed memory limits when running on a "Win32" platform target. The 32-bit
//    Windows system allocates a total of 2GB of memory for all data types. It is
//    suggested to run this example on an "x64" platform, which has an 8TB memory
//    limit: 2GB of shared data between the stack and static data, and the
//    remainder for dynamic data. If running on a Win32 platform, this example
//    reduces the height and width for each image being saved to the video file.
#define WIDTH 2448  // image width
#define HEIGHT 2048 // image height

// Frames per second
//    This example chooses its FPS setting arbitrarily. If the device is
//    outputting images at a similar frame rate, it may seem as though the image
//    is configured for real-time video, but this is only incidental. 250 images
//    and 25 frames per second will produce a 10 second video.
#define FRAMES_PER_SECOND 10.0

// number of images to grab
#define NUM_IMAGES 50

// File name
//    The relative path and file name to save to. After running the example, a
//    video should exist at the location specified. The image writer chooses the
//    file format by the image's extension. Aside from MPEG-4 (.mp4), videos can
//    be saved as AVI (.avi), MOV (.mov), and raw (.raw) files.
#define FILE_NAME_0 "video_0.mp4"
#define FILE_NAME_45 "video_45.mp4"
#define FILE_NAME_90 "video_90.mp4"
#define FILE_NAME_135 "video_135.mp4"


// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

void usage(char* app)
{
	std::cout << "Usage:\n";
	std::cout << app << " [-w width] [-h height] [-n numImages]\n";
	std::cout << "Where:\n";
	std::cout << "width:     camera image width to configure. Default is " << WIDTH << ".\n";
	std::cout << "height:    camera image height to configure. Default is " << HEIGHT << ".\n";
	std::cout << "numImages: number of images to capture for recording. Default is " << NUM_IMAGES << ".\n";
	std::cout << "fps:       framerate to use for the recording. Default is " << FRAMES_PER_SECOND << ".\n";
	std::cout << std::endl;
}

// sets integer value safely
// (1) ensures increment
// (2) ensures over minimum
// (3) ensures below maximum
// (4) sets value
int64_t SetIntValue(GenApi::INodeMap* pNodeMap, const char* nodeName, int64_t value)
{
	// get node
	GenApi::CIntegerPtr pInteger = pNodeMap->GetNode(nodeName);

	// Ensure increment
	//    If a node has an increment (all integer nodes & some float nodes), only
	//    multiples of the increment can be set.
	value = (((value - pInteger->GetMin()) / pInteger->GetInc()) * pInteger->GetInc()) + pInteger->GetMin();

	// Check min/max values
	//    Values must not be less than the minimum or exceed the maximum value of
	//    a node. If a value does so, simply push it within range.
	if (value < pInteger->GetMin())
	{
		value = pInteger->GetMin();
	}

	if (value > pInteger->GetMax())
	{
		value = pInteger->GetMax();
	}

	// set value
	pInteger->SetValue(value);

	// return value for output
	return value;
}

// (1) ensures over minimum
// (2) ensures below maximum
// (3) sets value
double SetFloatValue(GenApi::INodeMap* pNodeMap, const char* nodeName, double value)
{
	// get node
	GenApi::CFloatPtr pFloat = pNodeMap->GetNode(nodeName);

	// Check min/max values
	//    Values must not be less than the minimum or exceed the maximum value of
	//    a node. If a value does so, simply push it within range.
	if (value < pFloat->GetMin())
	{
		value = pFloat->GetMin();
	}

	if (value > pFloat->GetMax())
	{
		value = pFloat->GetMax();
	}

	// set value
	pFloat->SetValue(value);

	// return value for output
	return value;
}

// demonstrates recording a video
// (1) prepares video parameters
// (2) prepares video recorder
// (3) sets video settings
// (4) opens video
// (5) appends images
// (6) closes video
void RecordVideo(const std::vector<Arena::IImage*>& images, double fps)
{
	// Prepare video parameters
	std::cout << TAB1 << "Prepares video parameters (" << images[0]->GetWidth() << "x" << images[0]->GetHeight() << ", " << fps << " FPS)\n";

	Save::VideoParams params_0(
		images[0]->GetWidth(),
		images[0]->GetHeight(),
		fps);

	Save::VideoParams params_45(
		images[0]->GetWidth(),
		images[0]->GetHeight(),
		fps);

	Save::VideoParams params_90(
		images[0]->GetWidth(),
		images[0]->GetHeight(),
		fps);

	Save::VideoParams params_135(
		images[0]->GetWidth(),
		images[0]->GetHeight(),
		fps);

	// Prepare video recorder
	std::cout << TAB1 << "Prepare video recorder for video 0 " << FILE_NAME_0 << "\n";
	std::cout << TAB1 << "Prepare video recorder for video 45" << FILE_NAME_45 << "\n";
	std::cout << TAB1 << "Prepare video recorder for video 90" << FILE_NAME_90 << "\n";
	std::cout << TAB1 << "Prepare video recorder for video 135" << FILE_NAME_135 << "\n";


	Save::VideoRecorder videoRecorder_0(
		params_0,
		FILE_NAME_0);
	Save::VideoRecorder videoRecorder_45(
		params_45,
		FILE_NAME_45);
	Save::VideoRecorder videoRecorder_90(
		params_90,
		FILE_NAME_90);
	Save::VideoRecorder videoRecorder_135(
		params_135,
		FILE_NAME_135);
	// Set codec, container, and pixel format
	std::cout << TAB1 << "Set codec to H264, container to MPEG-4, and pixel format to BGR8\n";

	videoRecorder_0.SetH264Mp4BGR8();
	videoRecorder_45.SetH264Mp4BGR8();
	videoRecorder_90.SetH264Mp4BGR8();
	videoRecorder_135.SetH264Mp4BGR8();

	// Open video
	std::cout << TAB1 << "Open video\n";

	std::cout << "\nFFMPEG OUTPUT---------------\n\n";
	videoRecorder_0.Open();

	videoRecorder_45.Open();

	videoRecorder_90.Open();

	videoRecorder_135.Open();

	// Append images
	std::cout << TAB2 << "Append images\n";

	for (size_t i = 0; i < images.size(); i++)
	{
		if (i % 25 == 0)
			std::cout << TAB2;
		std::cout << ".";
		if (i % images.size() == images.size() - 1)
			std::cout << "\n";
		else if (i % 25 == 24)
			std::cout << "\r" << ERASE_LINE << "\r";


        const uint32_t srcWidth = static_cast<uint32_t>(images[i]->GetWidth());
        const uint32_t srcHeight = static_cast<uint32_t>(images[i]->GetHeight());
        // Create the buffer for four channels
        uint8_t *dummyBuffer = new uint8_t[srcWidth * srcHeight];

		Arena::IImage *outputImage0 = Arena::ImageFactory::Create(dummyBuffer, srcWidth * srcHeight, srcWidth, srcHeight, PfncFormat::Mono8);
		Arena::IImage *outputImage45 = Arena::ImageFactory::Create(dummyBuffer, srcWidth * srcHeight, srcWidth, srcHeight, PfncFormat::Mono8);
		Arena::IImage *outputImage90 = Arena::ImageFactory::Create(dummyBuffer, srcWidth * srcHeight, srcWidth, srcHeight, PfncFormat::Mono8);
		Arena::IImage *outputImage135 = Arena::ImageFactory::Create(dummyBuffer, srcWidth * srcHeight, srcWidth, srcHeight, PfncFormat::Mono8);

		uint8_t *outputBuffer0 = const_cast<uint8_t*>(outputImage0->GetData());
		uint8_t *outputBuffer45 = const_cast<uint8_t*>(outputImage45->GetData());
		uint8_t *outputBuffer90 = const_cast<uint8_t*>(outputImage90->GetData());
		uint8_t *outputBuffer135 = const_cast<uint8_t*>(outputImage135->GetData());

		uint8_t *inputBufferPtr = const_cast<uint8_t*>(images[i]->GetData());

		size_t buffer_size = images[i]->GetSizeFilled() - 1;

		uint32_t bufferIndex = 0;

		for (uint32_t i = 0; i < buffer_size; i += 4)
		{
			// 0 degree pixels are byte 0
			outputBuffer0[bufferIndex] = inputBufferPtr[i];

			// 45 degree pixels are byte 1
			outputBuffer45[bufferIndex] = inputBufferPtr[i+1];

			// 90 degree pixels are byte 2
			outputBuffer90[bufferIndex] = inputBufferPtr[i+2];

			// 135 degree pixels are byte 3
			outputBuffer135[bufferIndex] = inputBufferPtr[i+3];

			bufferIndex++;
		}

		// videoRecorder_0.AppendImage(images[i]->GetData());
		videoRecorder_0.AppendImage(Arena::ImageFactory::Convert(outputImage0, RGB8)->GetData());
		videoRecorder_45.AppendImage(Arena::ImageFactory::Convert(outputImage45, RGB8)->GetData());
		videoRecorder_90.AppendImage(Arena::ImageFactory::Convert(outputImage90, RGB8)->GetData());
		videoRecorder_135.AppendImage(Arena::ImageFactory::Convert(outputImage135, RGB8)->GetData());

	}

	// Close video
	std::cout << TAB1 << "Close video\n";

	videoRecorder_0.Close();
	videoRecorder_45.Close();
	videoRecorder_90.Close();
	videoRecorder_135.Close();

	std::cout << "\nFFMPEG OUTPUT---------------\n";
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main(int argc, char** argv)
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "\nCpp_Record\n\n";

	// Parse command line args
	int64_t width = WIDTH;
	int64_t height = HEIGHT;
	uint32_t numImages = NUM_IMAGES;
	double fps = FRAMES_PER_SECOND;

	for (int32_t i = 1; i < argc; i++)
	{
		if ((strcmp(argv[i], "-w") == 0) && (i + 1 < argc))
		{
			width = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-h") == 0) && (i + 1 < argc))
		{
			height = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-n") == 0) && (i + 1 < argc))
		{
			numImages = strtol(argv[++i], NULL, 10);
		}
		else if ((strcmp(argv[i], "-fps") == 0) && (i + 1 < argc))
		{
			fps = strtof(argv[++i], NULL);

			if (fps <= 0)
			{
				std::cout << "Framerate must be greater than 0.\n";
				return -1;
			}
		}
		else if (strcmp(argv[i], "--help") == 0)
		{
			usage(argv[0]);
			return 0;
		}
		else
		{
			std::cout << "Invalid argument [" << argv[i] << "]\n";
			usage(argv[0]);
			return -1;
		}
	}

	if (height == 0 || width == 0)
	{
		std::cout << "Invalid with or height specified!\n";
		return -1;
	}

	std::cout << "While the recorder is running, images may be buffered to memory.\n";
	std::cout << "To reduce the chance of problems when running on platforms with lower\n"
			<< "performance and/or lower amounts of memory, this example will use a\n"
			<< "default resolution of " << WIDTH << "x" << HEIGHT << std::endl;
	std::cout << "The default resolution can be overridden with command line arguments.\n"
			<< "Use: " << argv[0] << " --help for more info.\n";

	std::cout << "\nProceed with example? ('y' to continue) ";

	char continueExample = 'a';
	std::cin >> continueExample;

	if (continueExample != 'y')
	{
		std::cout << "\nPress enter to complete\n";

		// clear input
		while (std::cin.get() != '\n')
			continue;

		std::getchar();
		return -1;
	}

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> deviceInfos = pSystem->GetDevices();
		if (deviceInfos.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		else if (numImages == 0)
		{
			std::cout << "\nnumImages should be greater than 0\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		Arena::IDevice* pDevice = pSystem->CreateDevice(deviceInfos[0]);

		// Store initial settings
		GenICam::gcstring acquisitionModeInitial = "";
		bool frameRateEnableInitial = true;
		double frameRateInitial = 0.0;
		int64_t imageWidthInitial = 0;
		int64_t imageHeightInitial = 0;

		// Store acquisition mode
		acquisitionModeInitial = Arena::GetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode");

		// Store frame rate enable
		frameRateEnableInitial = Arena::GetNodeValue<bool>(pDevice->GetNodeMap(), "AcquisitionFrameRateEnable");

		if (frameRateEnableInitial)
		{
			// Store frame rate value
			frameRateInitial = Arena::GetNodeValue<double>(pDevice->GetNodeMap(), "AcquisitionFrameRate");
		}

		// Store image width
		imageWidthInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Width");

		// Store image height
		imageHeightInitial = Arena::GetNodeValue<int64_t>(pDevice->GetNodeMap(), "Height");

		// Set acquisition mode
		Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", "Continuous");

		
		// ['Mono8', 'Mono10', 'Mono10p', 'Mono10Packed', 'Mono12', 
		// 'Mono12p', 'Mono12Packed', 'Mono16', 'PolarizeMono8', 
		// 'PolarizeMono12', 'PolarizeMono12p', 'PolarizeMono12Packed', 
		// 'PolarizeMono16', 'PolarizedAngles_0d_45d_90d_135d_Mono8', 
		// 'PolarizedStokes_S0_S1_S2_S3_Mono8', 'PolarizedDolpAolp_Mono8', 
		// 'PolarizedDolpAolp_Mono12p', 'PolarizedDolp_Mono8', 
		// 'PolarizedDolp_Mono12p', 'PolarizedAolp_Mono8', 'PolarizedAolp_Mono12p']
	

		Arena::SetNodeValue<GenICam::gcstring>(
		pDevice->GetNodeMap(),
		"PixelFormat",
		"PolarizedAngles_0d_45d_90d_135d_Mono8");
		// PolarizedAolp_Mono8

		// Set width and height
		//    Reducing the size of an image reduces the amount of bandwidth
		//    required for each image. The less bandwidth required per image, the
		//    more images can be sent over the same bandwidth.
		width = SetIntValue(pDevice->GetNodeMap(), "Width", width);
		height = SetIntValue(pDevice->GetNodeMap(), "Height", height);

		// Set framerate
		Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "AcquisitionFrameRateEnable", true);

		fps = SetFloatValue(pDevice->GetNodeMap(), "AcquisitionFrameRate", fps);

		std::cout << "Using: \nwidth: " << width
				<< "\nheight: " << height
				<< "\nnumImages: " << numImages
				<< "\nfps: " << fps
				<< std::endl
				<< std::endl;

		std::vector<Arena::IImage*> images;

		// enable stream auto negotiate packet size
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

		// enable stream packet resend
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

		pDevice->StartStream();

		std::cout << "Capturing images\n";
		for (unsigned int i = 0; i < numImages; i++)
		{
			if (i % 25 == 0)
				std::cout << TAB2;
			std::cout << ".";
			if (i % numImages == numImages - 1)
				std::cout << "\n";
			else if (i % 25 == 24)
				std::cout << "\r" << ERASE_LINE << "\r";

			Arena::IImage* pImage = pDevice->GetImage(2000);

			// images.push_back(Arena::ImageFactory::Convert(pImage, RGB8));
			images.push_back(Arena::ImageFactory::Copy(pImage));

            // images.push_back(pImage);

			pDevice->RequeueBuffer(pImage);
		}
		pDevice->StopStream();

		// run example
		std::cout << "Commence example\n\n";
		RecordVideo(images, fps);
		std::cout << "\nExample complete\n";

		// clean up example
		for (size_t i = 0; i < images.size(); i++)
		{
			Arena::ImageFactory::Destroy(images[i]);
		}

		// Restore initial settings

		// Restore width and height
		SetIntValue(pDevice->GetNodeMap(), "Width", imageWidthInitial);
		SetIntValue(pDevice->GetNodeMap(), "Height", imageHeightInitial);

		// Restore acquisition mode
		Arena::SetNodeValue<GenICam::gcstring>(pDevice->GetNodeMap(), "AcquisitionMode", acquisitionModeInitial);

		// Restore framerate
		Arena::SetNodeValue<bool>(pDevice->GetNodeMap(), "AcquisitionFrameRateEnable", frameRateEnableInitial);

		if (frameRateEnableInitial)
		{
			SetFloatValue(pDevice->GetNodeMap(), "AcquisitionFrameRate", frameRateInitial);
		}

		pSystem->DestroyDevice(pDevice);
		Arena::CloseSystem(pSystem);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		exceptionThrown = true;
	}
	catch (std::exception& ex)
	{
		std::cout << "\nStandard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
	}
	catch (...)
	{
		std::cout << "\nUnexpected exception thrown\n";
		exceptionThrown = true;
	}

	std::cout << "Press enter to complete\n";

	// clear input
	while (std::cin.get() != '\n')
		continue;

	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}