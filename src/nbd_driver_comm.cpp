//----------------------------------------------------------------------------//
//						- Final Project -
//----------------------------------------------------------------------------//
//
//	DESCRIPTION		Implementation  nbd_driver device
//					
//	AUTHOR 			Liad Raz
//	DATE			12 Apr 2022
//	VERSION     	v0.1
// 
//----------------------------------------------------------------------------//

#include <sys/socket.h>         // socketpair
#include <cassert>              // assert
#include <fcntl.h>              // open
#include <sys/ioctl.h>          // ioctl
#include <linux/nbd.h>          // NBD_SET_BLKSIZE
#include <string.h>             // strerror

#include "nbd_driver_comm.hpp"     // 


//----------------------------------------------------------------------------//
//	NBDDriver Definitions
//----------------------------------------------------------------------------//

// Create sockets and init NBDDriver class members
NBDDriver::NBDDriver
    (const std::string& pathToDevice_, size_t storageSize_):
    m_storageSize(storageSize_),
    m_nbd(pathToDevice_)
{
    assert(0 < storageSize_);

    InitDriverSocketsFD();
    ConnectNBDDevice();
}


NBDDriver::NBDDriver
    (const std::string& pathToDevice_, size_t numBlocks, size_t blockSize_):
    m_storageSize(CalcStorageSize(numBlocks, blockSize_)),
    m_nbd(pathToDevice_)
{
    assert(0 < numBlocks);
    assert(0 < blockSize_);

    InitDriverSocketsFD();
    ConnectNBDDevice();
}


//
// Utils Functions
//----------------------------------------------------------------------------//
void NBDDriver::InitDriverSocketsFD()
{
    int socketPairFD[2];
    int error = 0;

    // Creates a pair of bio-directional sockets which are connected to each other;
    error = socketpair(AF_UNIX, SOCK_STREAM, 0, socketPairFD);
    if (-1 == error)
    {
        fprintf(stderr, "Trying to create socketpair failed. [%s]\n", strerror(errno));
        throw NBDDriverError();
    }

    m_sockFds[DriverSoc] = socketPairFD[0];      // Server - Driver Master App
    m_sockFds[NBDSoc] = socketPairFD[1];         // Client - NBD device
}


void NBDDriver::ConnectNBDDevice()
{
    // Set nbd Storage size
    ioctl(m_nbdFd, NBD_SET_BLKSIZE, storage_size_);
    
    // Reset the setup_task. Incase an app exits before running BD_DO_IT but after adding sockets we can
    // end up not being allowed to do a new nbd device.
    ioctl(m_nbdFd, NBD_CLEAR_SOCK);

    // Connect NBD device to the NBD socket descriptor
    ioctl(m_nbdFd, NBD_SET_SOCK, m_sockFds[NBDSoc]);
}


size_t NBDDriver::CalcStorageSize(size_t numBlocks, size_t blockSize_)
{
    return numBlocks * blockSize_;
}


NBDDriver::~NBDDriver()
{}



// Member functions - Main Functionality
//----------------------------------------------------------------------------//
std::shared_ptr<DriverData> NBDDriver::ReceiveRequest()
{

}

void NBDDriver::SendReply(std::shared_ptr<const DriverData> data_)
{

}

void NBDDriver::Disconnect()
{

}


int NBDDriver::GetFD() const
{
    return m_sockFds[NBDSoc];
}



//----------------------------------------------------------------------------//
//	NBDOpenFile Nested Class Definitions
//----------------------------------------------------------------------------//

NBDDriver::NBDOpenFile::NBDOpenFile(const std::string& pathToDevice_)
{
    // Get fd for the NBD actual device. Permission Read/Write.
    m_fd = open(pathToDevice_.c_str(), O_RDWR);

    if (-1 == m_fd)
    {
        fprintf(stderr, "Open path to device %s Failed. [%s]\n", pathToDevice_, strerror(errno));

        throw NBDDriverError();
    }
}