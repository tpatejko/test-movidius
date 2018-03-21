#include <mvnc.h>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <memory>

bool prepareTensor(void*& input, std::string& imageName)
{
  return true;
}

void loadGraphFromFile(std::unique_ptr<char[]>& graphFile, const std::string& graphFileName, unsigned int* graphSize)
{
  std::ifstream ifs;
  ifs.open(graphFileName, std::ifstream::binary);

  if (ifs.good() == false) {
    throw std::string("Error: Unable to open graph file: ") + graphFileName;
  }

  // Get size of file
  ifs.seekg(0, ifs.end);
  *graphSize = ifs.tellg();
  ifs.seekg(0, ifs.beg);


  graphFile.reset(new char[*graphSize]);

  // TODO: readfile

}


int main(int argc, char** argv) {

  void * graphHandle = nullptr;
  const std::string graphFileName("myGoogleNetGraph");
  int exit_code = 0;
  try {
    
    if(argc != 2 ) {
      throw std::string("ERROR: Wrong syntax. Valid syntax:\n \
               test-ncs <name of image to process> \n \
                ");
    }

    std::vector<std::string> ncs_names;
    char tmpncsname[200]; // How to determine max size automatically
    int index = 0;  // Index of device to query for
    mvncStatus ret = MVNC_OK;
    while(ret == MVNC_OK) {
      ret = mvncGetDeviceName(index++,tmpncsname,200); // hardcoded max name size 
      if (ret == MVNC_OK) {
        ncs_names.push_back(tmpncsname); 
        std::cout << "Found NCS: " << tmpncsname << std::endl;
      }
    }

    // If not devies present the exit
    if (ncs_names.size() == 0) {
      throw std::string("Error: No Intel Movidius identified in a system!\n");
    }

    // Using first device
    // TODO: run workload on many devices
    void* dev_handle = 0;
    ret = mvncOpenDevice(ncs_names[0].c_str(), &dev_handle);
    if(ret != MVNC_OK) {
      throw std::string("Error: Could not open NCS device: ") + ncs_names[0];
    }

    // Allocate graph
    unsigned int graphSize = 0;
    std::unique_ptr<char[]> graphFile;
    loadGraphFromFile(graphFile, graphFileName, &graphSize);

    ret = mvncAllocateGraph(dev_handle,&graphHandle,static_cast<void*>(graphFile.get()),graphSize);
    if (ret != MVNC_OK) {
      std::cerr << "Error: Graph allocation on NCS failed!" << std::endl;
      exit(-1);
    }
    
    // Loading tensor, tensor is of a HalfFloat data type 

//    prepareTensor(void*& input, std::string& imageName);
//    ret = mvncLoadTensor(graphHandle, input, inputLength, /* user param???*/) 
//    if (ret != MVNC_OK) {
//      std::cerr << "Error: Loading Tensor failed!" << std::endl;
//    }

  }
  catch (std::string err) {
    exit_code = -1;
  }

  // Cleaning up
//  ret = mvncDeallocateGraph(graphHandle);
//  if (ret != MVNC_OK) {
 //   std::cerr << "Error: Deallocation of Graph failed!" << std::endl;
 // }
  return exit_code;
}
