//----------------------------------------------------------------------------//
//						- Final Project -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		API for handling with NBD device Type.
//					
//	AUTHOR 			Liad Raz
//	DATE			12 Apr 2022
//	VERSION     	v0.1
// 
//----------------------------------------------------------------------------//

#ifndef NBDDRIVER_COMM_HPP
#define NBDDRIVER_COMM_HPP

#include <string>               // std::string
#include <memory>               // std::shared_ptr

#include "idriver_comm.hpp"     // IDriver
#include "driver_data.hpp"      // DriverData



class NBDDriver: public IDriver
{
public:
	//
	// Special Members Constructor
    explicit NBDDriver(const std::string& pathToDevice_, size_t storageSize_);
    explicit NBDDriver(const std::string& pathToDevice_, size_t numBlocks, size_t blockSize_);
    ~NBDDriver() noexcept override;  

    // Cctor. copy= are blocked from use
    NBDDriver(const NBDDriver& other_) = delete;
    NBDDriver& operator= (const NBDDriver& other_) = delete;

    //
	// Main Functionality
    std::shared_ptr<DriverData> ReceiveRequest() override;
    void SendReply(std::shared_ptr<const DriverData> data_) override;
    void Disconnect() override;

    //
	// Extra Functionality
    int GetFD() const override;


private:
    int m_sockFds[2];   // for both NBD and Driver
    size_t m_storageSize;

    enum SocketEnd {DriverSoc, NBDSoc};


    // NBD device open file-descriptor nested class 
    // NOTE     Initiated and destroyed with RAII.
    class NBDOpenFile
    {
    public:
        NBDOpenFile(const std::string& pathToDevice_);
        ~NBDOpenFile();
        NBDOpenFile(const NBDOpenFile& other_) = delete;
        NBDOpenFile& operator=(const NBDOpenFile& other_) = delete;

        int m_fd;        
    };
    
    NBDOpenFile m_nbd; // NBD device file descriptor.


    //
    // Utils Functions
    void InitDriverSocketsFD();
    void ConnectNBDDevice();
    size_t CalcStorageSize(size_t numBlocks, size_t blockSize_);
};



//
// Exception Error Handling
struct NBDDriverError: public IDriverError
{
    NBDDriverError(const std::string& str_ = "NBD Error");
};


#endif // NBDDRIVER_COMM_HPP