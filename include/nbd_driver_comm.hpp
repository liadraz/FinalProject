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

#include "idriver_comm.hpp"     // IDriverComm
#include "driver_data.hpp"      // DriverData



class NBDDriverComm: public IDriverComm
{
public:
	//
	// Special Members Constructor
    explicit NBDDriverComm(const std::string& pathToDevice_, size_t storageSize_);
    explicit NBDDriverComm(const std::string& pathToDevice_, size_t numBlocks, size_t blockSize_);
    ~NBDDriverComm() noexcept override;  

    // Cctor. copy= are blocked from use
    NBDDriverComm(const NBDDriverComm& other_) = delete;
    NBDDriverComm& operator= (const NBDDriverComm& other_) = delete;

    //
	// Main Functionality
    std::shared_ptr<DriverData> ReceiveRequest() override;
    void SendReply(std::shared_ptr<const DriverData> data_) override;
    void Disconnect() override;

    //
	// Extra Functionality
    int GetFD() const override;

    //read on std::thread! //
    //thread should be not detach //

private:
    int m_sockFds[2];    // Server and Client
    int m_nbdFd;        // NBD device file descriptor.

    enum SocketEnd {DriverSoc, NBDSoc};
    
    //
    // Utils Functions
    void InitDriverSocketsFD();
    size_t CalcStorageSizeIMP(size_t num_blocks_, size_t block_size_);
    void ConnectNBDDevice(const std::string& device_path_, size_t storage_size_)
};



#endif // NBDDRIVER_COMM_HPP