#include <cstdio>
#include <chrono>
#include <Processing.NDI.Advanced.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#ifdef _WIN32
#ifdef _WIN64
#pragma comment(lib, "Processing.NDI.Lib.Advanced.x64.lib")
#else // _WIN64
#pragma comment(lib, "Processing.NDI.Lib.Advanced.x86.lib")
#endif // _WIN64
#endif // _WIN32
// This function creates and std::ofstream to write a txt file with the NDIlib_source_t structure information
void saveSourcesToTxtFile(const std::unordered_map<std::string, NDIlib_source_t>& sources_map, const std::string& filename)
{
	std::ofstream outfile(filename);
	if (!outfile)
	{
		std::cerr << "Error: file can be written" << std::endl;
	}

	for (const auto& source : sources_map)
	{
		outfile << "Nombre NDI: " << source.first << "\n" <<
			"Adress: " << source.second.p_ip_address << "\n" <<
			"URL: " << source.second.p_url_address << "\n\n";
	}

	outfile.close();
}
int main(int argc, char* argv[])
{
	// Not required, but "correct" (see the SDK documentation).
	if (!NDIlib_initialize())
		return 0;

	// We are going to create an NDI finder that locates sources on the network.
	NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
	if (!pNDI_find)
		return 0;

	// Run for one minute
	using namespace std::chrono;
	for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);)
	{
		// Not required, but "correct" (see the SDK documentation).
		if (!NDIlib_initialize())
			return 0;

		// We are going to create an NDI finder that locates sources on the network.
		NDIlib_find_instance_t pNDI_find = NDIlib_find_create_v2();
		if (!pNDI_find)
			return 0;

		std::unordered_map<std::string, NDIlib_source_t> sources_map; // unordered map to store sources found
		const std::string filename = "ndi_sources.txt"; // filename for storing a list of sources in a txt file 


		// Run for one minute
		using namespace std::chrono;
		for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(1);) {
			// Wait up till 5 seconds to check for new sources to be added or removed
			if (!NDIlib_find_wait_for_sources(pNDI_find, 5000 /* milliseconds */)) {
				std::cout << "No change to the sources found" << std::endl;
				//printf("No change to the sources found.\n");
				continue;
			}

			// Get the updated list of sources
			uint32_t no_sources = 0;
			const NDIlib_source_t* p_sources = NDIlib_find_get_current_sources(pNDI_find, &no_sources);

			// Display all the sources.
			std::cout << "Network sources (" << no_sources << " found)." << std::endl;
			//printf("Network sources (%u found).\n", no_sources);
			for (uint32_t i = 0; i < no_sources; i++)
			{
				std::cout << i + 1 << ". " << p_sources[i].p_ndi_name << std::endl;
				//printf("%u. %s\n", i + 1, p_sources[i].p_ndi_name);
				std::string ndi_name = p_sources[i].p_ndi_name;
				sources_map[ndi_name] = p_sources[i];
			}

			saveSourcesToTxtFile(sources_map, filename);
		}
	}
	// Destroy the NDI finder
	NDIlib_find_destroy(pNDI_find);

	// Finished
	NDIlib_destroy();

	// Success. We are done
	return 0;
}
