/*
hist          total histogram
local_hist    Local histograms within each working group
data_per_item Number of pixels processed per work item
all_byte_size Image size, byte level
*/
#pragma OPENCL EXTENSION cl_khr_global_uchar_base_atomics:enable      // Enable atomic operations (basic atomic operations on 32-bit integers in local memory)
#pragma OPENCL EXTENSION cl_khr_local_uchar_base_atomics:enable       // This is still very important, otherwise it will prompt functions that do not support atomic operations
#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics:enable
#pragma OPENCL EXTENSION cl_khr_local_int32_base_atomics:enable

__kernel void imgToHist(__global const uchar* imgMat, __global int* hist, __local int* local_hist, uint data_per_item, uint all_byte_size)
{
	int l_idx = get_local_id(0);
	int g_idx = get_global_id(0);
	local_hist[l_idx] = 0;                     // Local histogram data initialization, this requires a workgroup size of 256
	barrier(CLK_LOCAL_MEM_FENCE);              // Local synchronization, i.e. waiting until the shared (local) memory is updated

	int item_offset = g_idx * data_per_item;   // Pixel position offset processed by each work item
	for (unsigned int i = item_offset; i < item_offset + data_per_item && i < all_byte_size; i++) {
		atomic_inc(local_hist + imgMat[i]);
		//atomic_add(local_hist+imgMat[i], 1);
	}

	// Global synchronization, waiting for all threads to execute here
	barrier(CLK_GLOBAL_MEM_FENCE);            

	// Merge the local histograms in shared memory in each workgroup into a global histogram
	if (l_idx < 256) {                        
		atomic_add(hist + l_idx, local_hist[l_idx]);
	}
}

// 
Histogram equalization, because the problem itself is not very parallel, so the execution efficiency of this function is consistent with serial, or even lower
/*
   hist     Global histogram
   hist_eq  Histogram after equalization
   imgSize  number of image pixels
*/
__kernel void histEq(__global const int* hist, __global int* hist_eq, const int imgSize)
{
	int l_idx = get_local_id(0);
	//if (l_idx >= 256) return;
	for (int i = 0; i <= l_idx; i++)
		hist_eq[l_idx] += hist[i];
	hist_eq[l_idx] = hist_eq[l_idx] * 255 / imgSize;
}

// Apply an equalized histogram to an image
__kernel void histEqToImg(__global uchar* imgMat, __global int* hist_eq)
{
	int g_idx = get_global_id(0);
	imgMat[g_idx] = hist_eq[imgMat[g_idx]];
}