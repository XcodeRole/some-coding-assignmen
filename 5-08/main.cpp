#include <iostream>
#include <vector>
#include "Utils.h"                  //  A tool library written by God, a layer of warp for common tools of OpenCL, address:https://github.com/gcielniak/OpenCL-Tutorials/tree/master/include
#include "CImg.h"                   //  A cross-platform image processing library tool

using namespace cimg_library;
using namespace std;

int platform_id = 0;                // Platform serial number: such as Intel CPU/GPU platform, NVIDIA CUDA platform, etc
int device_id = 0;                  // Device id, because in a system, there may be several devices under a platform, for example, I have 2 graphics cards under the NVIDIA platform
string img_filename = "test.pgm";   // image data

void print_help();                  // Helper function declaration
cl_uint histEqual();                // Implementation of histogram equalization based on OpenCL
cl_device_id getdevice(int platform_id, int device_id);     // Get an OpenCL device object by platform id and device id

int main(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "-p") == 0) && (i < (argc - 1))) { platform_id = atoi(argv[++i]); }
		else if ((strcmp(argv[i], "-d") == 0) && (i < (argc - 1))) { device_id = atoi(argv[++i]); }
		else if (strcmp(argv[i], "-l") == 0) { std::cout << ListPlatformsDevices() << std::endl; }
		else if ((strcmp(argv[i], "-f") == 0) && (i < (argc - 1))) { img_filename = argv[++i]; }
		else if (strcmp(argv[i], "-h") == 0) { print_help(); return 0; }
	}

	histEqual();
	return 0;
}

void print_help() {
	std::cerr << "Application usage:" << std::endl;
	std::cerr << "  -p : select platform " << std::endl;
	std::cerr << "  -d : select device" << std::endl;
	std::cerr << "  -l : list all platforms and devices" << std::endl;
	std::cerr << "  -f : input image file (default: test.pgm)" << std::endl;
	std::cerr << "  -h : print this message" << std::endl;
}

cl_uint histEqual()
{
	cl_int err = 0;
	cl_device_id dev = getdevice(platform_id, device_id);
	size_t max_item_per_group = 0;
	// Get the maximum value of work items in a single work group
	clGetDeviceInfo(dev, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_item_per_group), &max_item_per_group, NULL);

	// Read data from the image data file, note that the type used here is unsigned char
	// Convert the image directly to grayscale
	CImg<unsigned char> img_input_spectrum(img_filename.c_str());
	int h = img_input_spectrum.height();
	int w = img_input_spectrum.width();

	int size_per_item = 32;                                                // Set to process 32 pixels per work item                                    
	int global_work_item_size = ceil(w*h / (float)size_per_item);
	global_work_item_size = ceil(global_work_item_size / 256.0f) * 256;    // Set the size of the global work item

	CImg<unsigned char> img_input(w, h, 1, 1, 0);                          // Initialize a grayscale image
	cimg_forXY(img_input_spectrum, x, y)                                   // Set the grayscale value of a grayscale image
	{
		img_input(x, y) = img_input_spectrum(x, y);
	}

	//std::cout<< img_input.height() <<", "<< img_input.width() <<"," << img_input.size() << std::endl;
	//std::cout << img_input.depth() << std::endl;
	//std::cout << img_input.spectrum() << std::endl;
	CImgDisplay disp_input(img_input, "input");                            // display image

	cimg::exception_mode(0);                                               // Set CImg's exception handling mode
	try {

		cl::Context context = GetContext(platform_id, device_id);          // Get context based on platform and device
		std::cout << "Runing on " << GetPlatformName(platform_id) << ", " << GetDeviceName(platform_id, device_id) << std::endl;
		cl::CommandQueue queue(context);                                   // Create queue
		cl::Program::Sources sources;
		AddSources(sources, "img_hist_equal.cl");                          // Get the source code
		cl::Program program(context, sources);                             // create program

		try {
			program.build();                                               // compile program
		}
		catch (const cl::Error& err) {                                     // Compile exception handling
			std::cout << "Build Status: " << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Options:\t" << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			std::cout << "Build Log:\t " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(context.getInfo<CL_CONTEXT_DEVICES>()[0]) << std::endl;
			throw err;
		}

		// Declare the device memory for storing the input image, and the processing result (output) also exists here
		cl::Buffer dev_img_input(context, CL_MEM_READ_ONLY, sizeof(unsigned char)*w*h);   
		cl::Buffer hist(context, CL_MEM_READ_WRITE, sizeof(int) * 256);     //Global histogram allocation memory
		cl::Buffer hist_eq(context, CL_MEM_READ_WRITE, sizeof(int) * 256);  //Local histogram allocation space, local memory is similar to CUDA's shared memory
		queue.enqueueWriteBuffer(dev_img_input, CL_TRUE, 0, w*h * sizeof(unsigned char), &img_input.data()[0]);   // Data is copied from host to OpenCL device (here GPU)

        
		// Step-1 Calculate the histogram of a grayscale image
		cl::Kernel kernel1 = cl::Kernel(program, "imgToHist");              // Load the kernel function with the specified name in the program
		err = kernel1.setArg(0, dev_img_input);
		err = kernel1.setArg(1, hist);
		err = kernel1.setArg(2, sizeof(int) * 256, NULL);                   // Local memory (shared memory) does not need to be passed by value
		err = kernel1.setArg(3, size_per_item);
		kernel1.setArg(4, w*h);
		err = queue.enqueueNDRangeKernel(kernel1, cl::NullRange, cl::NDRange(global_work_item_size), cl::NDRange(256));

		// Step-2 Equalize the histogram, that is, through the histogram equalization algorithm, change the original gray value of each pixel into the equalized gray value
		cl::Kernel kernel2 = cl::Kernel(program, "histEq");
		kernel2.setArg(0, hist);
		kernel2.setArg(1, hist_eq);
		kernel2.setArg(2, w*h);
		err = queue.enqueueNDRangeKernel(kernel2, cl::NullRange, cl::NDRange(256), cl::NDRange(256));

		// Step-3 Obtain the output image through the equalized histogram
		cl::Kernel kernel3 = cl::Kernel(program, "histEqToImg");
		kernel3.setArg(0, dev_img_input);
		kernel3.setArg(1, hist_eq);
		err = queue.enqueueNDRangeKernel(kernel3, cl::NullRange, cl::NDRange(img_input.size()), cl::NullRange);

		// Data transmission and conversion to facilitate the generation of image formats
		vector<unsigned char> output_buffer(img_input.size());
		err = queue.enqueueReadBuffer(dev_img_input, CL_TRUE, 0, w*h * sizeof(unsigned char), &output_buffer.data()[0]);

		CImg<unsigned char> output_img(output_buffer.data(), w, h, 1, 1);
		CImgDisplay disp_output(output_img, "output");

		while (!disp_input.is_closed() && !disp_output.is_closed() && !disp_input.is_keyESC() && !disp_output.is_keyESC()) {
			disp_input.wait(5000);                                       // Image display window dwell time
			disp_output.wait(5000);
		}
		return 0;
	}
	catch (const cl::Error& err) {
		std::cerr << "ERROR: " << err.what() << ", " << getErrorString(err.err()) << std::endl;
		return -1;
	}
	catch (CImgException& err) {
		std::cerr << "ERROR: " << err.what() << std::endl;
		return -1;
	}
}


cl_device_id getdevice(int platform_id, int device_id)
{
	cl_platform_id *platforms;
	cl_device_id *devices;
	cl_uint platform_size, device_size;
	int err;
	err = clGetPlatformIDs(0, NULL, &platform_size);
	platforms = (cl_platform_id*)malloc(sizeof(platform_id)*platform_size);
	err = clGetPlatformIDs(1, platforms, NULL);
	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, 0, NULL, &device_size);
	devices = (cl_device_id*)malloc(sizeof(cl_device_id)*device_size);
	err = clGetDeviceIDs(platforms[platform_id], CL_DEVICE_TYPE_ALL, sizeof(cl_device_id)*device_size, devices, NULL);
	cl_device_id dev(devices[device_id]); // = devices[device_id];
	free(platforms); free(devices);

	return dev;
}